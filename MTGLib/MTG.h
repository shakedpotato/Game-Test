#pragma once

#include "OrderedSingleton.h"
#include "Timer.h"

namespace MTG
{
	struct Timer
	{
		static float GetDeltaTime()
		{
			return OrderedSingleton<DeltaTimer>::Instance()->deltaTime();
		}

		static double GetDeltaTimed()
		{
			return OrderedSingleton<DeltaTimer>::Instance()->deltaTimed();
		}
	};
}