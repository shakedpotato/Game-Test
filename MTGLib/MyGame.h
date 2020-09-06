#pragma once

#include "Main.h"

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
//
// �N���X: MyGame
// �T�v  : �Q�[���̏������A���C�����[�v�A�I������������B
//
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

class MyGame
{
private:
	MyGame();
public:
	static MyGame* Instance(void);
private:
	static MyGame* m_pInstance;
public:
	int Main();
private:
	void Init();
	void Finalize();
	void Update();
	void Render();
private:
	unsigned int m_TimerId;
	double m_TimeCount = 0.0;

private:
	void SetFpsOnCaption(void);
public:
	MyGame(const MyGame&) = delete;
	MyGame& operator=(const MyGame&) = delete;
	MyGame(MyGame&&) = delete;
	MyGame& operator=(MyGame&&) = delete;

};
