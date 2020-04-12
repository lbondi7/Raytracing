#pragma once

#include <string>
#include <mutex>

struct DebugPrinter
{
public:
	static void Print(const std::string& strToPrint);

private:
	static std::mutex printQueueMutex;
};