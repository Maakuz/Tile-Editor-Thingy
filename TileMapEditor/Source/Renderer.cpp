#include "Renderer.h"
#include "Queues\OverlayQueue.h"
#include "Queues\BackgroundQueue.h"
#include "Queues\TileQueue.h"


Renderer::Renderer()
{
}

void Renderer::update()
{
    tileManager.prepareTiles();
}

void Renderer::clearAllQueues()
{
    TileQueue::get().clear();
    BackgroundQueue::get().clear();
    OverlayQueue::get().clear();
}

void Renderer::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    for (sf::RectangleShape rect : BackgroundQueue::get().getQueue())
    {
        target.draw(rect, states);
    }

    target.draw(tileManager, states);

    for (sf::RectangleShape rect : OverlayQueue::get().getQueue())
    {
        target.draw(rect, states);
    }
}
