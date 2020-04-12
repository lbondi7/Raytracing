#include "ThreadManager.h"
// ThreadManager.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>

const int maxThreadCount = 7;

ThreadManager::ThreadManager(int numOfThreads)
{
	Init(numOfThreads);
}

ThreadManager::~ThreadManager()
{
	while (!threads.empty())
	{
		int nullCount = 0;
		for (int i = 0; i < threads.size(); ++i)
		{
			if (threads[i])
			{
				if (threads[i]->IsAlive())
					threads[i]->SetAlive(false);

				if (threads[i]->IsDestroyed())
				{
					threads[i].release();
					std::cout << "thread: " << i << " was destroyed!" << std::endl;
				}
			}
			else
				nullCount++;
		}
		if (nullCount == threads.size())
			threads.clear();
	}
}

void ThreadManager::Init(int numOfThreads)
{
	if (numOfThreads > maxThreadCount)
		numOfThreads = maxThreadCount;

	for (size_t i = 0; i < numOfThreads; i++)
	{
		threads.emplace_back(std::move(std::make_unique<PoolableThread>(i)));
	}
}

std::function<void()>& ThreadManager::SetTask(bool _perminate)
{
	for (size_t i = 0; i < threads.size(); i++)
	{
		if (threads[i])
		{
			if (threads[i]->IsAlive() && threads[i]->IsIdle())
			{
				threads[i]->SetIdle(false);
				return threads[i]->func;
			}
		}
	}
}

bool ThreadManager::IsExecutingTask()
{
	for (size_t i = 0; i < threads.size(); i++)
	{
		if (threads[i])
		{
			if (threads[i]->IsAlive() && !threads[i]->IsIdle())
			{
				return true;
			}
		}
	}
	return false;
}
