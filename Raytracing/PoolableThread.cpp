#include "PoolableThread.h"

#include <iostream>
#include <chrono>
#include "DebugPrinter.h"

using namespace std::chrono_literals;

PoolableThread::PoolableThread()
{
	alive = true;
	thread = std::thread(&PoolableThread::Run, this);
}

PoolableThread::PoolableThread(int _id)
{
	alive = true;
	id = _id;
	thread = std::thread(&PoolableThread::Run, this);
}

PoolableThread::~PoolableThread()
{

}

void PoolableThread::Run()
{
	while (alive)
	{
		if (!idle && func)
		{
			//std::cout << "thread: (" << std::this_thread::get_id() << ") is executing!" << std::endl;
			//DebugPrinter::Print("thread: (" + std::to_string(id) + ") is executing!");
			func();
			func = nullptr;
			idle = true;
			//DebugPrinter::Print("thread: (" + std::to_string(id) + ") is finished!");
		}
		else
			std::this_thread::sleep_for(0.5ms);
	}
	destroyed = true;
}
