#include <Utility/Time.h>
#include <chrono>
#include <iostream>

namespace Ethereal
{
	std::string Ethereal::Time::GetTime()
	{
		std::chrono::system_clock clock;

		auto now = std::chrono::system_clock::to_time_t(clock.now());
		
		char buffer[26];
		ctime_s(buffer, 26, &now);
		
		return std::string(buffer, buffer + 24);
	}
};
