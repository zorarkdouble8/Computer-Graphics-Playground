#include<iostream>

#include "Event.h"

template<class... types> void Event<types...>::Trigger(types...)
{
	for (int x = 0; x < this->observers.size(); x++)
	{
		if (this->observers.at(x) == nullptr)
		{
			//TODO, create a print statement if system debug mode is on
			std::cout << "TESTING; Event.cpp line 12" << std::endl;
			this->observers.erase(this->observers.begin() + x, this->observers.end() + x + 1);
			x--;
		}
		else
		{
			this->observers.at(x)->EventTrigger(types...);
		}
	}
}


template<class... types> void Event<types...>::AddObserver(Observer<types...>* observer)
{
	this->observers.push_back(observer);
}


template<class... types> void Event<types...>::RemoveObserver(Observer<types...>* observer)
{
	//TODO: Think about what depedences this has
}