#include "Event.h"

void Event::Trigger()
{
	this->observers.at(0)->EventTrigger();
}

void Event::AddObserver(Observer* observer)
{
	this->observers.push_back(observer);
}


void Event::RemoveObserver(Observer* observer)
{
	//TODO: Think about what depedences this has
}