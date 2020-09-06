#pragma once

#include "Timer.h"

class DeltaTimerUpdater
{
private:
	friend class GameBasicSystemObject<DeltaTimerUpdater>;
	DeltaTimerUpdater() {}

public:
	void UpdateTimer()
	{
		GameBasicSystemObject<DeltaTimer>::Instance()->Update();
	}
};
