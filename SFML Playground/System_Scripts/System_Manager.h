#pragma once

#include <iostream>
using namespace std;

#include "Event.h"

class SystemManager
{
public:
	//To prevent cloning:
	SystemManager(SystemManager& sys) = delete;
	SystemManager operator=(SystemManager& sys) = delete;

	static SystemManager* GetInstance()
	{
		if (SystemManager::_instance == nullptr)
		{
			SystemManager* systemManager = new SystemManager();
			_instance = systemManager;
			return _instance;
		}
		else
		{
			return _instance;
		}
	}

	static Event systemScripts;
	static Event gameScripts;

	void UpdateSystemScripts()
	{
		this->systemScripts.Trigger();
	}

	void  UpdateGameScripts()
	{
		this->gameScripts.Trigger();
	}

private:
	SystemManager() {};

	static SystemManager* _instance;
};