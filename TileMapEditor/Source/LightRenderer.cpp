#include "LightRenderer.h"
#include "Lighting/LightQueue.h"
#include "Constants.h"
#define NR_OF_RENDER_TARGETS 3

LightRenderer::LightRenderer()
{
    this->fullscreenboi = sf::RectangleShape(sf::Vector2f(WIN_WIDTH, WIN_HEIGHT));
    this->fullscreenboi.setPosition(0, 0);

    for (int i = 0; i < NR_OF_RENDER_TARGETS; i++)
    {
        this->renderTargets[i].create(WIN_WIDTH, WIN_HEIGHT);
    }
}

void LightRenderer::renderLights(sf::RenderTarget& target, sf::RenderStates states)
{
    this->fullscreenboi.setPosition(target.getView().getCenter() - (target.getView().getSize() / 2.f));
    sf::Vector2f offset = target.getView().getCenter() - (target.getView().getSize() / 2.f);
    
    this->renderTargets[0].clear(sf::Color::Black);
    this->renderTargets[0].setView(target.getView());
    for (size_t i = 0; i < LightQueue::get().getQueue().size(); i++)
    {
        Light* light = LightQueue::get().getQueue()[i];
        ShaderHandler::getShader(SHADER::lightingNoShadow).setUniform("pos", light->pos - offset);
        ShaderHandler::getShader(SHADER::lightingNoShadow).setUniform("radius", light->radius);
        ShaderHandler::getShader(SHADER::lightingNoShadow).setUniform("color", light->color);

        sf::RenderStates state;
        state.shader = &ShaderHandler::getShader(SHADER::lightingNoShadow);
        state.blendMode = sf::BlendAdd;

        sf::RectangleShape sprite(sf::Vector2f(light->radius * 2, light->radius * 2));
        sprite.setPosition(light->pos - (sf::Vector2f(sprite.getSize() / 2.f)));
        renderTargets[0].draw(sprite, state);
    }

    this->renderTargets[0].display();

    this->fullscreenboi.setTexture(&this->renderTargets[0].getTexture());



    for (int i = 0; i < 2; i++)
    {
        this->renderTargets[i + 1].setView(target.getView());
        this->renderTargets[i + 1].clear(sf::Color::Transparent);
        this->renderTargets[i + 1].draw(this->fullscreenboi, &shaders[shaders.BLUR_PASS[i]]);
        this->renderTargets[i + 1].display();

        this->fullscreenboi.setTexture(&this->renderTargets[i + 1].getTexture());
    }

    target.draw(this->fullscreenboi, states);
    LightQueue::get().clear();
}
