#include "DebugPrinter.h"

#include <iostream>

std::mutex DebugPrinter::printQueueMutex;

void DebugPrinter::Print(const std::string& strToPrint)
{
	std::lock_guard<std::mutex> lock(printQueueMutex);
	std::cout << strToPrint << std::endl;
}