#include "TileMaps.h"
#include "Misc\MissingTexture.h"
#include <filesystem>
#include "GUI.h"

namespace fs = std::filesystem;

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

 void TileMaps::addTexture(std::string newName, int width, int height)
 {
     for (std::string & name : textureNames)
     {
         if (newName == name)
             throw "Texture aleady exists";
     }
     
     sf::Texture tex;
     int x = width;
     int y = height;

     //load a temp texture if texture is missing
     if (!fs::exists(TILE_MAP_PATH(+newName)))
     {
         tex.create(DEFAULT_TILE_SIZE * x, DEFAULT_TILE_SIZE * y);

         for (int i = 0; i < y; i++)
         {
             for (int j = 0; j < x; j++)
             {
                 tex.update(missingColor, DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE, j * DEFAULT_TILE_SIZE, i * DEFAULT_TILE_SIZE);
             }
         }
     }

     else
     {
         tex.loadFromFile(TILE_MAP_PATH(+newName));

         x = tex.getSize().x / DEFAULT_TILE_SIZE;
         y = tex.getSize().y / DEFAULT_TILE_SIZE;
     }

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
     out << std::to_string(tileMaps.textureNames.size()) << " ";

     for (size_t i = 0; i < tileMaps.textures.size(); i++)
     {
         out << tileMaps.textureNames[i] << " ";
         out << tileMaps.sheetSize[i].x << " " << tileMaps.sheetSize[i].y << " ";
     }

     return out;
 }

 std::istream & operator>>(std::istream & in, TileMaps & tileMaps)
 {
     int textureCount = 0;
     in >> textureCount;

     tileMaps.textures.clear();
     tileMaps.textureNames.clear();
     tileMaps.sheetSize.clear();
     tileMaps.textureRects.clear();

     auto textureInfo = Global::gui->get<tgui::Panel>(Global::Elements::infoBox::panel)->get<tgui::ComboBox>(Global::Elements::infoBox::textureBox);
     textureInfo->removeAllItems();

     for (int i = 0; i < textureCount; i++)
     {
         std::string name;
         int x = 0;
         int y = 0;

         in >> name;
         in >> x >> y;

         tileMaps.addTexture(name, x, y);

         fs::path currentDir = fs::current_path();
         currentDir /= TILE_MAP_PATH(+name);

         textureInfo->addItem(name, currentDir.string());
     }

     return in;
 }
