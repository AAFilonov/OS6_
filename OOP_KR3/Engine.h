#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>

class Engine
{


	

public:
	void put_item();
	void get_item();
	friend void Thread(void* pParams);

	void Run();
	Engine();
	~Engine();




};

