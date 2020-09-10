#pragma once

#include "ComponentBase.h"
#include "MyMath.h"

// class GameObject;

class Transform :
	public ComponentBase
{
private:
	Transform( const Transform& ) = delete; // �R�s�[�R���X�g���N�^
	Transform& operator = ( const Transform& ) = delete; // �R�s�[������Z�q
	Transform( Transform&& ) = delete; // ���[�u�R���X�g���N�^
	Transform& operator = ( const Transform&& ) = delete; // ���[�u������Z�q
public:
	Transform();
	~Transform() {}
private:
	MyMath::Vector3	m_LocalPosition;
	MyMath::Vector3	m_LocalRotation;
	MyMath::Vector3	m_LocalScale;
	MyMath::Vector4	m_LocalQuaternion;
private:
	MyMath::Vector4 GetRotation(MyMath::Float4X4& m);
public:
	const MyMath::Vector3* GetLocalPosition( void ) const	{ return &m_LocalPosition; }
	const MyMath::Vector3* GetLocalRotation( void ) const	{ return &m_LocalRotation; }
	const MyMath::Vector3* GetLocalScale( void )	const	{ return &m_LocalScale; }

	const MyMath::Vector4* GetRotationQ() const { return &m_LocalQuaternion; }
	// MyMath::Float4X4 GetRotationMatrix() const { return }
	DirectX::XMMATRIX GetRotationMatrix() const { return DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(m_LocalQuaternion.x, m_LocalQuaternion.y, m_LocalQuaternion.z, m_LocalQuaternion.w )); }
	MyMath::Vector3 GetRotationEuler() const;					// �v����
	void GetRotationEuler(float* x, float* y, float* z) const;	// �v����

	// �����̉�]
	// SetRotationAxis(float degree�@�p�x, Vector3 vec�@�� );
	// �I�C���[�p�ł̉�] �e�����̉�]�B�����̓x���ɐݒ�
	// SetRotationEuler(float x, float y, float z);
	// �������������x�N�g���Ɍ����@���Ԃ�J�����Ɏ�����������ɋ߂�
	void SetRotationLookAt( const MyMath::Vector3& target, const MyMath::Vector3& upVector = MyMath::Vector3::Up());
	void SetRotationQ(MyMath::Vector4& vec) { m_LocalQuaternion = vec; }
	void SetLocalPosition( MyMath::Vector3 pos );
	void SetLocalPosition(float x, float y, float z);
	void SetLocalRotation( MyMath::Vector3 rot );
	void SetLocalScale( MyMath::Vector3 scale );

	void AddLocalPosition(MyMath::Vector3 v);
};

