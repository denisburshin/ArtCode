#pragma once

#include <string>

namespace Ethereal
{
	class Time
	{
	public:
		Time() = default;
		~Time() = default;

		static std::string GetTime();
	};
};


