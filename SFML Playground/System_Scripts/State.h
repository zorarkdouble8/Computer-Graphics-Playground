#pragma once
#include "../Libraries/JSON/nlohmann/json.hpp"
#include <queue>

using namespace nlohmann;

class SystemState
{
public:
	static SystemState* getInstance()
	{
		if (_instance == nullptr)
		{
			//create new instance and refresh the current state
			_instance = new SystemState();
		}
		else
		{
			return _instance;
		}
	}

	//this merges the input state with the nextState
	void commitState(json newState)
	{
		string test3 = newState.dump();
		string test2 = nextState.dump();
		nextState.merge_patch(newState);
		string test = nextState.dump();
	}

	//this moves the current state into the next state
	void pushToNextState()
	{
		currentState.merge_patch(nextState);
		string test = currentState.dump();
		nextState.clear();
	}

	json& getCurrentState()
	{
		return currentState;
	}

private:
	json nextState;
	json currentState;

	nlohmann::json RetrieveSavedData(bool& isSuccessful)
	{
		//PC: "C:\\Users\\User101\\Desktop\\SFML and OpenGL Playground\\SFML Playground\\System_Scripts\\Data.json"
		ifstream json("C:\\Users\\User101\\Desktop\\SFML and OpenGL Playground\\SFML Playground\\System_Scripts\\Data.json");
		nlohmann::json data;
		try
		{
			data = nlohmann::json::parse(json);
			isSuccessful = true;
		}
		catch (const nlohmann::json::exception e)
		{
			isSuccessful = false;
		}
	
		return data;
	}
	
	SystemState() 
	{
		bool isSuccessful = false;
		this->currentState = RetrieveSavedData(isSuccessful);
		if (!isSuccessful)
		{
			//CAUSE ERROR SOMEHOW
		}
	}

	static SystemState* _instance;
};

SystemState* SystemState::_instance = nullptr;