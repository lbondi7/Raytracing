#pragma once
#include <thread>
#include <functional>

class PoolableThread
{
public:

	PoolableThread();
	PoolableThread(int _id);
	~PoolableThread();

	void Run();

	void SetAlive(bool _alive) { alive = _alive; }
	void SetIdle(bool _idle) { idle = _idle; }
	void SetDestroyed(bool _destroyed) { destroyed = _destroyed; }

	bool IsAlive() { return alive; }
	bool IsIdle() { return idle; }
	bool IsDestroyed() { return destroyed; }

	std::function<void()> func;

	int id = 0;

private:
	std::thread thread;

	bool alive = false;
	bool idle = true;
	bool destroyed = false;
};
