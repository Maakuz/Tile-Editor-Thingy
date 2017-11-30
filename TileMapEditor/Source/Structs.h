#pragma once
#include <vector>

struct Tile
{
    int x;
    int y;
    //What texture.
    int textureID;
    //Where in the texture.
    int tileID;
};

//Coordianates are in relation to each other in tile-space...?
struct ActiveTile
{
    int id;
    int x;
    int y;
};