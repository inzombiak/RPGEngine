#include "Debug.h"

Debug* Debug::m_debugger = NULL;
bool Debug::m_clockRunning;
std::clock_t Debug::m_clock = 0;

Debug::Debug()
{
	m_clockRunning = false;
}

void Debug::StartTimer(std::string displayText)
{
	if (!m_clockRunning)
	{
		std::cout << displayText << std::endl;
		m_clock = std::clock();
		m_clockRunning = true;

		return;
	}
	
	std::cout << "CLOCK ALREADY RUNNING" << std::endl;
}

void Debug::EndTimer(std::string displayText)
{
	if (m_clockRunning)
	{
		double duration = (std::clock() - m_clock) / (double) CLOCKS_PER_SEC;
		std::cout << displayText << duration << std::endl;
		m_clockRunning = false;

		return;
	}
	
	std::cout << "CLOCK NOT RUNNING" << std::endl;
}