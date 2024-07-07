#pragma once

#include <iostream>
#include <iomanip>

#include <glad/glad.h>
#include<SFML/Graphics.hpp>

#include "Event.h"

class SystemManager
{
public:
	//Don't use any constructors
	
	//To prevent cloning:
	SystemManager(SystemManager& sys) = delete;
	SystemManager operator=(SystemManager& sys) = delete;

	static SystemManager* GetInstance()
	{
		if (SystemManager::_instance == nullptr)
		{
			_instance = new SystemManager();
			
			return _instance;
		}
		else
		{
			return _instance;
		}
	}
	
	//Functions
	void PrintSystemInfomation();

	void SetMainWindow(sf::Window* mainWindow) { this->mainWindow = mainWindow; }
	sf::Window* GetMainWindow() 
	{ 
		if (this->mainWindow == nullptr)
		{
			std::cout << "MAIN WINDOW IS NULLPTR!; a script is trying to access this!" << std::endl;
			return nullptr;
		}

		return this->mainWindow; 
	}

	//Event Functions
	void AddGameScript(Observer<>* gameRuntimeScript) { this->gameScripts.AddObserver(gameRuntimeScript); }
	void AddSystemScript(Observer<>* sysRuntimeScript) { this->systemScripts.AddObserver(sysRuntimeScript); }

	void UpdateSystemScripts() { this->systemScripts.Trigger(); }
	void UpdateGameScripts() { this->gameScripts.Trigger(); }

	static void DeleteInstance() { delete _instance; }

private:
	sf::Window* mainWindow;

	static SystemManager* _instance;

	//Event Variables
	static Event<> systemScripts;
	static Event<> gameScripts;

	SystemManager() {};

	//causes loop so don't use
	~SystemManager() {};
};