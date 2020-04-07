#include <SFML/Graphics.hpp>
#include <windows.h>
#include <iostream>
#include <stdio.h>


const int WINDOW_WEIGHT = 800;
const int WINDOW_HEIGHT = 600;
const int SIZE_ = 5;
const int PUT_SPEED = 1000;//мсек
const int GET_SPEED = 1500;//мсек

HANDLE hSemEmpty;
HANDLE hSemFull;
HANDLE hSemMutex;

CRITICAL_SECTION cs;

int full_count;
int Buffer[SIZE_];
int num = 1;
HANDLE hThr;
HANDLE hThr2;
unsigned long uThrID;
bool Quit = false;

void put_item()
{

	Buffer[full_count] = num;
	full_count++;
	printf("Buffer: %d %d %d %d %d ", Buffer[0], Buffer[1], Buffer[2], Buffer[3], Buffer[4]);
	printf("Put Item %d\n", num);
	num++;
}


void get_item() {

	int tmp = Buffer[full_count - 1];
	Buffer[full_count - 1] = 0;
	full_count--;
	printf("Buffer: %d %d %d %d %d ", Buffer[0], Buffer[1], Buffer[2], Buffer[3], Buffer[4]);
	printf("Get Item %d\n", tmp);
}


void Thread(void* pParams)//Produser
{
	while (!Quit)
	{
		Sleep(PUT_SPEED);
		WaitForSingleObject(hSemEmpty, INFINITE);
		WaitForSingleObject(hSemMutex, INFINITE);
		put_item();
		ReleaseSemaphore(hSemMutex, 1, NULL);
		ReleaseSemaphore(hSemFull, 1, NULL);
	}
}

void Thread2(void* pParams)//Consumer
{
	while (!Quit)
	{
		Sleep(GET_SPEED);
		WaitForSingleObject(hSemFull, INFINITE);
		WaitForSingleObject(hSemMutex, INFINITE);
		get_item();
		ReleaseSemaphore(hSemMutex, 1, NULL);
		ReleaseSemaphore(hSemEmpty, 1, NULL);

	}
}

int main()
{
	sf::Font m_font;
	sf::RenderWindow m_window(sf::VideoMode(WINDOW_WEIGHT, WINDOW_HEIGHT), "OS6_G");
	m_window.setFramerateLimit(10);
	if (!m_font.loadFromFile("font.ttf"))
	{
		std::cout << "Error cant load font" << std::endl;
		exit(-1);
	}

	hSemMutex = CreateSemaphore(NULL, 1, 1, "Mutex");
	hSemFull = CreateSemaphore(NULL, 0, SIZE_, "full");
	hSemEmpty = CreateSemaphore(NULL, SIZE_, SIZE_, "Empty");
	InitializeCriticalSection(&cs);
	


	hThr = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread, NULL, 0, &uThrID);
	hThr2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread2, NULL, 0, &uThrID);

	while (m_window.isOpen() && !Quit)
	{
		sf::Vector2f v(20, 20);
	
		sf::Text button1("Producer", m_font, 20);
		button1.setPosition(button1.getLocalBounds().width + 30, 350);
		button1.setFillColor(sf::Color::White);
		sf::Vector2f tmp(button1.getLocalBounds().width+20,button1.getGlobalBounds().height+20);
		sf::RectangleShape button1_fill(tmp);
		button1_fill.setPosition(button1.getGlobalBounds().left-10 , button1.getGlobalBounds().top-10);
		button1_fill.setFillColor(sf::Color::Green);

		
	
		sf::RectangleShape* buffer[5];
		for (size_t i = 0; i < 5; i++)
		{
			
			buffer[i] = new sf::RectangleShape(v);
			buffer[i]->setPosition(300 + i * (buffer[i]->getLocalBounds().width + 10), 370);
			buffer[i]->setFillColor(sf::Color::Red);
		}
		sf::Text button2("Consumer", m_font, 20);
		button2.setPosition(button2.getLocalBounds().width + 400, 350);
		button2.setFillColor(sf::Color::White);
		sf::Vector2f tmp2(button2.getLocalBounds().width + 20, button2.getGlobalBounds().height + 20);
		sf::RectangleShape button2_fill(tmp2);
		button2_fill.setPosition(button2.getGlobalBounds().left - 10, button2.getGlobalBounds().top - 10);
		button2_fill.setFillColor(sf::Color::Green);

		sf::Event event;
		bool quit = false;
		while (!Quit) {
			while (m_window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)Quit = true;;
				if (event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::Escape) Quit = true;
				}



				WaitForSingleObject(hSemMutex, INFINITE);
				int b[SIZE_];
				for (size_t i = 0; i < 5; i++)
				{
					b[i] = Buffer[i];
				}
				ReleaseSemaphore(hSemMutex, 1, NULL);







				m_window.clear();
				
				m_window.draw(button1_fill);
				m_window.draw(button1);
				for (size_t i = 0; i < 5; i++)
				{
					if(b[i]!=0) m_window.draw(*buffer[i]);
				}
				m_window.draw(button2_fill);
				m_window.draw(button2);
				
				m_window.display();
			}
		}
	}
	m_window.close();














	return 0;
}