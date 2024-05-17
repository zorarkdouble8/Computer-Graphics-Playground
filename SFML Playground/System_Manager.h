#pragma once


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

private:
	SystemManager() {};

	static SystemManager* _instance;
};

SystemManager* SystemManager::_instance = nullptr;