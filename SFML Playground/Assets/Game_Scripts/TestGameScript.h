#pragma once
#include "Runtime_Script.h"
#include <iostream>
using namespace std;

class TestGameScript: public RuntimeScript
{
	void Start()
	{
		cout << "Start" << endl;
	}

	void Update()
	{
		cout << "Updating" << endl;
	}
};