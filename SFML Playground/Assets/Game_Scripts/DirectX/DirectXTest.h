#pragma once
#include "Runtime_Script.h"
#include "iostream"

using namespace std;

class DirectX: public RuntimeScript
{
public:
	void Start()
	{
		cout << "Hello World!" << endl;
	}

	void Update()
	{

	}
};