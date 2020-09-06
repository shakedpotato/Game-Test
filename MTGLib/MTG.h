#pragma once

#include "Timer.h"

namespace MTG
{
	struct Timer
	{
		static float deltaTime()
		{
			return GameBasicSystemObject<DeltaTimer>::Instance()->deltaTime();
		}

		static double deltaTimed()
		{
			return GameBasicSystemObject<DeltaTimer>::Instance()->deltaTimed();
		}
	};
}