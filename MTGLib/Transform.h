#pragma once

#include "ComponentBase.h"
#include "MyMath.h"

// class GameObject;

class Transform :
	public ComponentBase
{
private:
	Transform( const Transform& ) = delete; // コピーコンストラクタ
	Transform& operator = ( const Transform& ) = delete; // コピー代入演算子
	Transform( Transform&& ) = delete; // ムーブコンストラクタ
	Transform& operator = ( const Transform&& ) = delete; // ムーブ代入演算子
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
	MyMath::Vector3 GetRotationEuler() const;					// 要検証
	void GetRotationEuler(float* x, float* y, float* z) const;	// 要検証

	// 軸回りの回転
	// SetRotationAxis(float degree　角度, Vector3 vec　軸 );
	// オイラー角での回転 各軸回りの回転。引数の度数に設定
	// SetRotationEuler(float x, float y, float z);
	// 向きたい方向ベクトルに向く　たぶんカメラに実装したいやつに近い
	void SetRotationLookAt( const MyMath::Vector3& target, const MyMath::Vector3& upVector = MyMath::Vector3::Up());
	void SetRotationQ(MyMath::Vector4& vec) { m_LocalQuaternion = vec; }
	void SetLocalPosition( MyMath::Vector3 pos );
	void SetLocalPosition(float x, float y, float z);
	void SetLocalRotation( MyMath::Vector3 rot );
	void SetLocalScale( MyMath::Vector3 scale );

	void AddLocalPosition(MyMath::Vector3 v);
};

