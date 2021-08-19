#include "Timestep.h"

namespace Ethereal
{
	Timestep::Timestep(float time)
		: time(time)
	{
	}
	float Timestep::GetSeconds() const
	{
		return time;
	}
	float Timestep::GetMilliseconds() const
	{
		return time * 1000.0f;
	}
}

