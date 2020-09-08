#pragma once

#include "Timer.h"

class DeltaTimerUpdater
{
private:
	friend class OrderedSingleton<DeltaTimerUpdater>;
	DeltaTimerUpdater() {}

public:
	void UpdateTimer()
	{
		OrderedSingleton<DeltaTimer>::Instance()->Update();
	}
};
