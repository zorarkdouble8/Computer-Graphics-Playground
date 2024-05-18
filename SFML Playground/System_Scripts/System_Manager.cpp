#pragma once

#include "System_Manager.h"


//Defining statics (Prevents duplication)
SystemManager* SystemManager::_instance = nullptr;
Event SystemManager::systemScripts;
Event SystemManager::gameScripts;