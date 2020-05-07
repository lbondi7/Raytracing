#pragma once
#include "PoolableThread.h"

#include <vector>
#include <functional>
#include <optional>
#include <mutex>

class ThreadManager
{
public:

	ThreadManager() = default;
	ThreadManager(int numOfThreads);
	~ThreadManager();

	void Init(int numOfThreads);

	std::function<void()>& SetTask(bool _perminate = false);

	std::function<void()>& SetTask(bool _perminate, int& threadId);

	const int ThreadCount() { return static_cast<int>(threads.size()); }

	bool IsExecutingTask();

	void WaitForAllThreads();

	void WaitForThreads(std::vector<int>& ths);


private:

	std::vector<std::unique_ptr<PoolableThread>> threads;

	static std::mutex threadsMutex;
	static std::mutex threadsMutex2;
};