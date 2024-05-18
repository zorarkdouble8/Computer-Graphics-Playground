#pragma once

#include<vector>

class Observer
{
public:
	virtual void EventTrigger() = 0;
private:
	

};

class Event
{
public:
	void Trigger();
	void AddObserver(Observer* observer);
	void RemoveObserver(Observer* observer);
private:
	std::vector<Observer*> observers;
};