#pragma once

namespace ls
{
    namespace mem
    {
        // growing pool allocator that can only allocate (deallocate) objects of type T
        // incompatibile with standard containers
        // allocation returns pointer to the uninitialized storage
        // therefore it should be called from some overload of new
        // so the new does initialization after receiving pointer to uninitialized storage
        // similarily, deallocation does not call destructor,
        // because it is expected to be done by operator delete earlier
        template <class T>
        class HomogeneousMemoryPool
        {
            // TODO: consider freeing unused pools

            template <class U>
            static constexpr U pow2(const U exp)
            {
                return U(1) << exp;
            }

            // returns index of the most significant set bit counting from the lsb side, from 0
            // returns -1 if no bits are set
            static constexpr int msbi(size_t value)
            {
                int result = -1;
                while (value)
                {
                    ++result;
                    value >>= 1;
                }
                return result;
            }

            // thin interface layer for memory pool, fully managed from outside
            class Pool
            {
                using UninitializedStorageType = std::aligned_storage_t<sizeof(T), alignof(T)>;

            public:
                Pool() :
                    m_cells(nullptr),
                    m_size(0)
                {

                }

                Pool(const size_t size) :
                    m_cells(std::make_unique<UninitializedStorageType[]>(size)),
                    m_size(size)
                {

                }

                Pool(const Pool&) = delete;
                Pool(Pool&&) = default;
                Pool& operator=(const Pool&) = delete;
                Pool& operator=(Pool&&) = default;

                void* requestMemory(const size_t i)
                {
                    return reinterpret_cast<T*>(m_cells.get()) + i;
                }
                void abandonMemory(const size_t i)
                {
                }
                bool contains(const void* const ptr) const
                {
                    return reinterpret_cast<const T*>(ptr) >= reinterpret_cast<const T*>(m_cells.get())
                        && reinterpret_cast<const T*>(ptr) < reinterpret_cast<const T*>(m_cells.get()) + m_size;
                }

                // requires: contains(ptr) == true
                size_t pointerToIndex(const void* const ptr) const
                {
                    return reinterpret_cast<const T*>(ptr) - reinterpret_cast<const T*>(m_cells.get());
                }

                size_t size() const
                {
                    return m_size;
                }

            private:
                std::unique_ptr<UninitializedStorageType[]> m_cells;
                size_t m_size;

                void* indexToPointer(const size_t i)
                {
                    return reinterpret_cast<const T*>(m_cells.get()) + i;
                }
            };


        public:
            struct PoolLocation
            {
                size_t poolId;
                size_t offset;
            };

            // first two pools are of size 2^initialPoolSizeLog2
            // next ones are following a geometric series with q=2
            static constexpr size_t initialPoolSizeLog2 = 3u;

            // empirically chosen to be the best without using too much additional memory
            static constexpr float cachedFreeCellsRatio = 0.15f;
            static constexpr size_t minCachedFreeCells = 8u;

            HomogeneousMemoryPool() :
                m_pools(),
                m_isUsed(pow2(initialPoolSizeLog2), false),
                m_searchStart(0)
            {
                m_pools.emplace_back(pow2(initialPoolSizeLog2));

                updateCacheSize();
            }

            HomogeneousMemoryPool(const HomogeneousMemoryPool<T>&) = delete;
            HomogeneousMemoryPool(HomogeneousMemoryPool<T>&&) = default;
            HomogeneousMemoryPool<T>& operator=(const HomogeneousMemoryPool<T>&) = delete;
            HomogeneousMemoryPool<T>& operator=(HomogeneousMemoryPool<T>&&) = default;

            // allocates a new T in the Pool
            // creates new Pool with double size if no space for allocation
            void* requestMemory()
            {
                const size_t freeCellIndex = findFreeCell();
                PoolLocation responsibleSubject = indexToLocalPoolIndex(freeCellIndex);

                updateOnInserted(freeCellIndex);

                return m_pools[responsibleSubject.poolId].requestMemory(responsibleSubject.offset);
            }

            void abandonMemory(void* const ptr)
            {
                PoolLocation responsibleSubject = pointerToLocalPoolIndex(ptr);
                m_pools[responsibleSubject.poolId].abandonMemory(responsibleSubject.offset);

                const size_t poolStart = poolInitialIndex(responsibleSubject.poolId);

                updateOnErased(poolStart + responsibleSubject.offset);
            }

            // requires that the ptr was allocated by this object
            size_t pointerToIndex(const void* const ptr) const
            {
                const size_t poolId = findPool(ptr);
                const Pool& pool = m_pools[poolId];
                const size_t offset = pool.pointerToIndex(ptr);
                const size_t poolStart = poolInitialIndex(poolId);

                return poolStart + offset;
            }

            // requires that the ptr was allocated by this object
            PoolLocation pointerToLocalPoolIndex(const void* const ptr) const
            {
                const size_t poolId = findPool(ptr);
                const Pool& pool = m_pools[poolId];
                const size_t offset = pool.pointerToIndex(ptr);

                return { poolId, offset };
            }

            // requires: i < m_isUsed.size()
            PoolLocation indexToLocalPoolIndex(const size_t i)
            {
                const size_t poolId = indexToPool(i);
                const size_t poolStart = poolInitialIndex(poolId);

                return { poolId, i - poolStart };
            }

        private:
            std::vector<Pool> m_pools;
            std::vector<uint_least8_t> m_isUsed;
            std::vector<size_t> m_availableCellsCache;
            size_t m_currentMaxCachedCells;
            size_t m_searchStart;

            // returns index of a free cell taken from cache
            // allocates new Pool if needed
            size_t findFreeCell()
            {
                if (m_availableCellsCache.empty())
                {
                    const size_t numCells = m_isUsed.size();
                    const size_t numCellsToCache = m_currentMaxCachedCells / 2 + 1; // leave some space in case some get deallocated later (so they fill the cache and are not waster)
                    size_t newStart = m_searchStart;

                    // searching from the last endpoint
                    // does not make any difference in general case
                    // is faster when few deallocations are performed
                    size_t i = m_searchStart;
                    for (; i < numCells && m_availableCellsCache.size() < numCellsToCache; ++i)
                    {
                        if (m_isUsed[i] == false) m_availableCellsCache.emplace_back(i);
                    }
                    newStart = i;

                    // if cache is not filled then look at indices before the initial start
                    if (m_availableCellsCache.size() < numCellsToCache)
                    {
                        size_t j = 0;
                        for (; j < m_searchStart && m_availableCellsCache.size() < numCellsToCache; ++j)
                        {
                            if (m_isUsed[j] == false) m_availableCellsCache.emplace_back(i);
                        }
                        newStart = j;
                    }

                    // allocate new pool if the cache is not yet filled
                    if (m_availableCellsCache.size() < numCellsToCache)
                    {
                        allocateNewPool();

                        for (; i < m_isUsed.size() && m_availableCellsCache.size() < numCellsToCache; ++i)
                        {
                            if (m_isUsed[i] == false) m_availableCellsCache.emplace_back(i);
                        }
                        newStart = i;
                    }

                    m_searchStart = newStart;
                }

                const size_t freeCell = m_availableCellsCache.back();
                m_availableCellsCache.pop_back();
                return freeCell;
            }

            // returns first global index that points to the pool i
            size_t poolInitialIndex(const size_t i) const
            {
                if (i == 0) return 0;

                return pow2(i + initialPoolSizeLog2 - 1);
            }

            // returns pool id of the pool that owns ptr
            // requires that ptr was allocated by this object
            size_t findPool(const void* const ptr) const
            {
                size_t poolId = m_pools.size() - 1;
                for (;;)
                {
                    if (m_pools[poolId].contains(ptr)) break;
                    --poolId;
                }

                return poolId;
            }

            // allocates new Pool, updates cache size and bitfield
            void allocateNewPool()
            {
                const size_t newPoolSize = m_isUsed.size();
                m_pools.emplace_back(newPoolSize);

                m_isUsed.insert(m_isUsed.end(), newPoolSize, false);

                updateCacheSize();
            }

            // returns pool id that owns cell with index i
            size_t indexToPool(const size_t i)
            {
                const size_t shiftedIndex = i >> initialPoolSizeLog2;
                return msbi(shiftedIndex) + 1;
            }

            // updates bifield and cache
            void updateOnErased(const size_t i)
            {
                m_isUsed[i] = false;

                if (m_availableCellsCache.size() < m_currentMaxCachedCells)
                {
                    m_availableCellsCache.emplace_back(i);
                }
            }

            // updates bitfield
            void updateOnInserted(const size_t i)
            {
                m_isUsed[i] = true;
            }

            void updateCacheSize()
            {
                m_currentMaxCachedCells = std::max(minCachedFreeCells, static_cast<size_t>(m_isUsed.size()*cachedFreeCellsRatio));
                m_availableCellsCache.reserve(m_currentMaxCachedCells);
            }
        };

        // overrides new and delete operator to use
        // statically allocated (instanced for each type)
        // HomogeneousPoolAllocator over standard new/delete behaviour
        // should be applied by publicly inheriting by CRTP
        template <class T>
        class StaticEmbeddedHomogeneousMemoryPool
        {
        public:
            static void* operator new(size_t sz)
            {
                return pool().requestMemory();
            }
            static void operator delete(void* ptr)
            {
                pool().abandonMemory(ptr);
            }

            static HomogeneousMemoryPool<T>& pool()
            {
                static thread_local HomogeneousMemoryPool<T> singleton;
                return singleton;
            }
        };
    }
}