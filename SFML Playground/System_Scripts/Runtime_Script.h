#pragma once
#include "Event.h"

class RuntimeScript: public Observer 
{
public:
	virtual void Start() = 0;
	virtual void Update() = 0;

	//DO NOT OVERLOAD
	void EventTrigger();

	//Constructors
	RuntimeScript();
	

private:
	bool _didInitialize = false;
};