#pragma once
namespace Ethereal
{
	class Timestep
	{
	public:
		Timestep(float time);

		float GetSeconds() const;
		float GetMilliseconds() const;
	private:
		float time;
	};
}


