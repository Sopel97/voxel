#pragma once

class Map;
class MapChunk;

class MapGenerator
{
public:
    MapGenerator(Map& map);

    void generateChunk(MapChunk& chunk) const;

private:
    Map* m_map;
};
