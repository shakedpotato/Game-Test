// ===============================================================================================
// �v���W�F�N�g��: DirectX9game01172018 
// �t�@�C����: TikTimer.h
// �T�v: 
// �쐬�� XX/XX/XXXX Author: Hashimoto Yuto AT12E-275-29                                       
// �ǋL:                                                                                        
// MM/DD/YYYY Author: ����                                                                      
// �E�~�~�̕ύX�`�`�`�`                                                                         
// ===============================================================================================

#ifndef _TIME_PIECE_H_
#define _TIME_PIECE_H_
#pragma once

#include "GameBasicSystem.h"

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
// 
// �N���X: TimePiece 
// ���ӓ_: GameSystemObject�e���v���[�g����C���X�^���X���擾���邱�ƁB
// 
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
class TimePiece
{
	/* ---------------------
		�ȉ�,���Ԃ̓N���b�N�̗ݐς�����.
	--------------------- */
private:
	friend class GameBasicSystemObject<TimePiece>;
	TimePiece();
public:
	void Reset( void );                            // �V�X�e���^�C�}�[�̃��Z�b�g
	void Start( void );                            // �V�X�e���^�C�}�[�̃X�^�[�g
	void Update( void );                           // �O�t���[���̎��ԂȂǂ̍X�V
	void Stop( void );                             // �V�X�e���^�C�}�[�̃X�g�b�v
	void Put1msAhead( void );                      // �V�X�e���^�C�}�[��0.1�b�i�߂�
	double GetTime( void );                        // �v�����Ԃ̎擾
	double GetAbsoluteTime( void );                // ���ݎ������擾
	double GetDeltaTime( void );                   // �O�̃t���[������̌o�ߎ��Ԃ̎擾
	bool GetIsTimePieceStoped( void );             // �V�X�e���^�C�}�[���~�܂��Ă��邩�Ԃ�
	// void LimitThreadAffinityToCurrentProc( void ); // ���݂̃X���b�h��1�̃v���Z�b�T�i���݂̃X���b�h�j�ɐ���
private:
	bool isTimerStopped;       // �X�g�b�v�t���O
	LONGLONG InitTickPerSec;   // �P�b�Ԃ̌v������ // �R���X�g���N�^�ň�x��������,���̌�̃t���[���ł͗��p����Ă�̂ŉ�������������.
	LONGLONG StopTime;         // �X�g�b�v�������̃J�E���^
	LONGLONG LastElapsedTime;  // �Ō�ɋL�^�����J�E���^
	LONGLONG BaseTime;         // ���������̃J�E���^
	double DeltaTime;          // �O�t���[������̌o�ߎ��ԁB�P��:�b
private:
	LARGE_INTEGER GetAdjustedCurrentTime( void );
};

#endif // _TIME_PIECE_H_