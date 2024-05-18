#include "Event.h"

void Event::Trigger()
{
	for (int x = 0; x < this->observers.size(); x++)
	{
		if (this->observers.at(x) == nullptr)
		{
			//TODO, create a print statement if system debug mode is on
			this->observers.erase(this->observers.begin() + x, this->observers.end() + x + 1);
			x--;
		}
		else
		{
			this->observers.at(x)->EventTrigger();
		}
	}
}

void Event::AddObserver(Observer* observer)
{
	this->observers.push_back(observer);
}


void Event::RemoveObserver(Observer* observer)
{
	//TODO: Think about what depedences this has
}