#include "Engine.h"
#include <windows.h>
#include <stdio.h>


void Engine::put_item()
{
	Buffer[full_count] = num;
	//printf("Put Item %d\n", num);
	num++;
	full_count++;

}


void Engine::get_item() {
	full_count--;
	//printf("Get Item %d\n", Buffer[full_count]);
	Buffer[full_count] = 0;
}



void Engine::Run()
{
	
}

Engine::Engine()
	
}

Engine::~Engine()
{


}

void Thread(void* pParams)
{
	while (1)
	{

		WaitForSingleObject(hSemEmpty, INFINITE);
		WaitForSingleObject(hSemMutex, INFINITE);
		put_item();
		ReleaseSemaphore(hSemMutex, 1, NULL);
		ReleaseSemaphore(hSemFull, 1, NULL);
	}
}
