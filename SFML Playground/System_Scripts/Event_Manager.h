#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

static class EventManager
{
public:
	void Refresh_Events(sf::Window& window);

	sf::Event& HasEvent(sf::Event::EventType eventType);

private:
	std::vector<sf::Event> events;
};