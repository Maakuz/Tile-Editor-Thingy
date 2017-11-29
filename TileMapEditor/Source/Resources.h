#pragma once
#include <vector>
#include "SFML\Graphics\Texture.hpp"
#include <assert.h>
#include "Constants.h"

class Resources
{
public:
    static Resources & get()
    {
        static Resources resources;
        return resources;
    }
    virtual ~Resources() {};

    //Saving a pointer to this could be dangerous!!
    sf::Texture & getTexture(int id)
    {
        assert(id < textures.size()); // todo: NOT THIS
        return textures[id];
    }

    sf::IntRect & getTileRect(int textureID, int tileID)
    {
        return textureRects[textureID][tileID]; // TODO: CATCH THIS SHIT
    }

    void addTexture(std::string name)
    {
        sf::Texture tex;
        tex.loadFromFile(TEXTURE_PATH(+ name)); // TODO: CATCH THIS SHIT

        textures.push_back(tex);
        textureNames.push_back(name);

        int x = tex.getSize().x / DEFAULT_TILE_SIZE;
        int y = tex.getSize().y / DEFAULT_TILE_SIZE;

        //initialize with the size it needs
        textureRects.push_back(std::vector<sf::IntRect>(x * y));

        for (int i = 0; i < y; i++)
        {
            for (int j = 0; j < x; j++)
            {
                textureRects[0][j + (i * x)] = sf::IntRect(
                    j * DEFAULT_TILE_SIZE,
                    i * DEFAULT_TILE_SIZE,
                    DEFAULT_TILE_SIZE,
                    DEFAULT_TILE_SIZE);
            }
        }
    }

private:
    std::vector<sf::Texture> textures;
    std::vector<std::string> textureNames;
    std::vector<std::vector<sf::IntRect>> textureRects;
};