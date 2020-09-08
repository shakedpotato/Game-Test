#pragma once
#include "OrderedSingleton.h"
#include "TimePiece.h"

class DeltaTimerUpdater;

class DeltaTimer
{
private:
	friend class OrderedSingleton<DeltaTimer>;
	friend class DeltaTimerUpdater;
	DeltaTimer()
		:
		m_DeltaTime(0.0),
		m_LastElapsedTime(0.0)
	{

	}

	~DeltaTimer() {}
private:
	double m_DeltaTime;
	double m_LastElapsedTime;
public:
	double deltaTimed() { return m_DeltaTime; }
	float deltaTime() { return (float)m_DeltaTime; }

private:
	// フレーム間の経過時間の更新
	void Update() // これをpublicにしたくない
	{
		double curr = OrderedSingleton<TimePiece>::Instance()->GetTime();
		m_DeltaTime = curr - OrderedSingleton<DeltaTimer>::Instance()->m_LastElapsedTime;
		m_LastElapsedTime = curr;
	}
};