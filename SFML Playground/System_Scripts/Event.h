#pragma once

#include<vector>

template<class... types>
class Observer
{
public:
	virtual void EventTrigger(types...) = 0;
private:

};

template<class... types>
class Event
{
public:
	void Trigger()
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
				this->observers.at(x)->EventTrigger();
			}
		}
	}

	void AddObserver(Observer<>* observer)
	{
		this->observers.push_back(observer);
	}
	//void RemoveObserver(Observer<>* observer);
private:
	std::vector<Observer<>*> observers;
};