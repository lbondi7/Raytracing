#include "ThreadManager.h"
// ThreadManager.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "DebugPrinter.h"

#include <iostream>

std::mutex ThreadManager::threadsMutex;
std::mutex ThreadManager::threadsMutex2;

const int maxThreadCount = std::thread::hardware_concurrency() * 2;

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
					//std::cout << "thread: " << i << " was destroyed!" << std::endl;
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
	while (true)
	{
		for (auto& t : threads)
		{
			if (t)
			{
				if (t->IsAlive() && t->IsIdle())
				{
					t->SetIdle(false);
					t->SetPermanent(_perminate);
					return t->func;
				}
			}
		}
	}
}

std::function<void()>& ThreadManager::SetTask(bool _perminate, int& threadID)
{
	while (true)
	{
		for (auto& t : threads)
		{
			if (t)
			{
				if (t->IsAlive() && t->IsIdle())
				{
					t->SetIdle(false);
					t->SetPermanent(_perminate);
					threadID = t->id;
					return t->func;
				}
			}
		}
	}
}

bool ThreadManager::IsExecutingTask()
{
	for (auto& t : threads)
	{
		if (t)
		{
			if (t->IsAlive() && !t->IsIdle())
			{
				return true;
			}
		}
	}
	return false;
}

void ThreadManager::WaitForAllThreads()
{
	std::lock_guard<std::mutex> lock(threadsMutex);
	bool wait = true;
	while (wait)
	{
		wait = false;
		for (auto& t : threads)
		{
			if (t && t->IsAlive() &&
				!t->IsPermanent() && !t->IsIdle())
			{
				wait = true;
			}
		}
	}
}

void ThreadManager::WaitForThreads(std::vector<int>& ths)
{
	std::lock_guard<std::mutex> lock(threadsMutex2);
	bool wait = true;
	while (wait)
	{
		wait = false;
		for (auto t : ths)
		{
			if (threads[t] && threads[t]->IsAlive() &&
				!threads[t]->IsPermanent() && !threads[t]->IsIdle())
			{
				wait = true;
			}
		}
	}
}
