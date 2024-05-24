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
	void Trigger(types...);
	void AddObserver(Observer<types...>* observer);
	void RemoveObserver(Observer<types...>* observer);
private:
	std::vector<Observer<types...>*> observers;
};