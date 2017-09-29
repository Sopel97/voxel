#pragma once

class Map;
class MapChunk;
class MapChunkBlockData;

class MapGenerator
{
public:
    MapGenerator(Map& map);

    void generateChunk(MapChunkBlockData& chunk) const;

private:
    Map* m_map;
};
