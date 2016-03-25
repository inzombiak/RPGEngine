#include <iostream>
#include <ctime>
#include <string>

class Debug
{
public:
	static void StartTimer(std::string);
	static void EndTimer(std::string);
	static void PrintMessage(std::string);
private:
	Debug();
	static std::clock_t m_clock;
	static bool m_clockRunning;
	static Debug* m_debugger;
};