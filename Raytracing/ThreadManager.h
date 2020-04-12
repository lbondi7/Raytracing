#pragma once
#include "PoolableThread.h"

#include <vector>
#include <functional>
#include <optional>

class ThreadManager
{
public:

	ThreadManager() = default;
	ThreadManager(int numOfThreads);
	~ThreadManager();

	void Init(int numOfThreads);

	std::function<void()>& SetTask(bool _perminate = false);

	int ThreadCount() { static_cast<int>(threads.size()); }

	bool IsExecutingTask();


private:

	std::vector<std::unique_ptr<PoolableThread>> threads;

};