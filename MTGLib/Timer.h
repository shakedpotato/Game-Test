#pragma once
#include "GameBasicSystem.h"
#include "TimePiece.h"

class DeltaTimerUpdater;

class DeltaTimer
{
private:
	friend class GameBasicSystemObject<DeltaTimer>;
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
	// �t���[���Ԃ̌o�ߎ��Ԃ̍X�V
	void Update() // �����public�ɂ������Ȃ�
	{
		double curr = GameBasicSystemObject<TimePiece>::Instance()->GetTime();
		m_DeltaTime = curr - GameBasicSystemObject<DeltaTimer>::Instance()->m_LastElapsedTime;
		m_LastElapsedTime = curr;
	}
};