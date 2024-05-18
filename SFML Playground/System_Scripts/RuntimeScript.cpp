#pragma once
#include "Runtime_Script.h"
#include "System_Manager.h"

RuntimeScript::RuntimeScript()
{
	SystemManager* sys = SystemManager::GetInstance();
	sys->gameScripts.AddObserver(this);
}

void RuntimeScript::EventTrigger()
{
	if (!_didInitialize)
	{
		this->Start();

		_didInitialize = true;
	}
	else
	{
		this->Update();
	}

}
