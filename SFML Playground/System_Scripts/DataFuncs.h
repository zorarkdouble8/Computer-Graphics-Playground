#pragma once
#include "../Libraries/JSON/nlohmann/json.hpp"

using namespace nlohmann;
using namespace std;

struct WindowData
{
	static void CreateWindow(const json& windowData, const string& windowName)
	{
		Windows::CreateWindow(windowData.at("Windows"), windowName);
	}
};

struct Windows
{
	//Creates a new window in the data
	static void CreateWindow(const json& windows, const string& windowName)
	{
		//creates a window in the .json file
	}
};