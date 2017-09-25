#pragma once

class MapChunk;

class MapChunkRenderer
{
public:
    void draw(MapChunk& chunk);
    void noLongerRendered(MapChunk& chunk);

private:
};
