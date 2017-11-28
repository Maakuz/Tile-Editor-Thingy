#pragma once

struct Tile
{
    //Coords in tile-space
    int x;
    int y;
    //What texture.
    int textureID;
    //Where in the texture.
    int tileID;
};