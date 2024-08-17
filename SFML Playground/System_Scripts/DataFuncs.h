#pragma once
#include <tuple>
#include "../Libraries/JSON/nlohmann/json.hpp"

using namespace nlohmann;
using namespace std;

struct Generic
{
	//get's a piece of data from any dataPath
	//This is not possible because std::get needs to know at compile time what values to return because it needs to know the return types. 
	//It is possible to get around this via classes but it has some bad consequences. (undefined behavior is one of them)
	template<typename returnType, typename ...dataPath>
	static string GetData(const json& windowData, dataPath... dataPack)
	{
		tuple<dataPath...> dataPaths(dataPack...);
		//int num = 3;
		//auto element = get<num>(dataPaths);

		////auto last = test...[sizeof...(test) - 1];

		//json iterator = windowData;
		//for (int x = 0; x < sizeof...(dataPath); x++)
		//{
		//	//iterator = iterator.at(std::get<x, dataPath...>(dataPaths));
		//}

		//return iterator.dump();
		return "";
	}
};

struct WindowData
{
	//Creates a window handle (starting from a window entity)
	static json CreateWindowHandle(const json& windowData, const string& windowName)
	{
		//Creates a window handle and puts the value in the .json file
		json newData;
		json windArray;
		windArray.emplace("Handle", 34);
		json windArray1;
		windArray1.emplace("State", windArray);
		json windArray2;
		windArray2.emplace(windowName, windArray1);
		newData.emplace("Windows", windArray2);

		string test = newData.dump();
		return newData;
	}
};


struct Window
{
	//Creates a window handle (starting from a window entity)
	static void CreateWindowHandle(const json& window, const string& windowName)
	{
		//creates a window in the .json file
	}
};

