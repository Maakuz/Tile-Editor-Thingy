#include "TileMaps.h"

 sf::Texture & TileMaps::getTexture(int id)
 {
     assert(id < textures.size()); // todo: NOT THIS
     return textures[id];
 }

 int TileMaps::getTextureIndex(std::string name)
 {
     for (int i = 0; i < textureNames.size(); i++)
     {
         if (textureNames[i] == name)
             return i;
     }

     return -1;
 }

 sf::IntRect & TileMaps::getTileRect(int textureID, int tileID)
 {
     return textureRects[textureID][tileID]; // TODO: CATCH THIS SHIT
 }

 sf::Vector2i TileMaps::getSheetSize(int textureID)
 {
     return sheetSize[textureID];
 }

 void TileMaps::addTexture(std::string newName)
 {
     //sf::Texture tex;
     //tex.create(DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE);
     //printf("%d, %d\n", tex.getSize().x, tex.getSize().y);
     //

     //sf::Uint8* pixels = new sf::Uint8[DEFAULT_TILE_SIZE * DEFAULT_TILE_SIZE * 4];
     //for (int i = 0; i < DEFAULT_TILE_SIZE * DEFAULT_TILE_SIZE; i += 4)
     //{
     //    //TODO: Change to the palette mapping...
     //    pixels[i] = 0;
     //    pixels[i + 1] = 0;
     //    pixels[i + 2] = 0;
     //    pixels[i + 3] = 255;
     //}

     //tex.update(pixels);
     //delete[] pixels;

     for (std::string & name : textureNames)
     {
         if (newName == name)
             throw "Texture aleady exists";
     }

     sf::Texture tex;
     tex.loadFromFile(TEXTURE_PATH(+newName)); // TODO: CATCH THIS SHIT & check for dupes

     int x = tex.getSize().x / DEFAULT_TILE_SIZE;
     int y = tex.getSize().y / DEFAULT_TILE_SIZE;

     textures.push_back(tex);
     textureNames.push_back(newName);
     sheetSize.push_back(sf::Vector2i(x, y));

     //initialize with the size it needs
     textureRects.push_back(std::vector<sf::IntRect>(x * y));

     for (int i = 0; i < y; i++)
     {
         for (int j = 0; j < x; j++)
         {
             textureRects[textureRects.size() - 1][j + (i * x)] = sf::IntRect(
                 j * DEFAULT_TILE_SIZE,
                 i * DEFAULT_TILE_SIZE,
                 DEFAULT_TILE_SIZE,
                 DEFAULT_TILE_SIZE);
         }
     }
 }

 std::ostream & operator<<(std::ostream & out, const TileMaps & tileMaps)
 {
     



     return out;
 }

 std::istream & operator>>(std::istream & in, TileMaps & tileMaps)
 {
     sf::Texture failed;
     failed.create(DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE);

     sf::Uint8* pixels = new sf::Uint8[DEFAULT_TILE_SIZE * DEFAULT_TILE_SIZE * 4];
     for (int i = 0; i < DEFAULT_TILE_SIZE * DEFAULT_TILE_SIZE; i += 4)
     {
         //TODO: Change to the palette mapping...
         pixels[i] = 0;
         pixels[i + 1] = 0;
         pixels[i + 2] = 0;
         pixels[i + 3] = 255;
     }

     failed.update(pixels);
     delete[] pixels;

     return in;
 }
