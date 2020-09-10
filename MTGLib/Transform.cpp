#include <limits>
#include <cmath>
#include "ComponentBase.h"
#include "Transform.h"

using namespace MyMath;
using namespace std;

Transform::Transform()
	:
	ComponentBase(_T("Transform")),
	m_LocalPosition(0.0f, 0.0f, 0.0f),
	m_LocalRotation(0.0f, 0.0f, 0.0f),
	m_LocalQuaternion(Vector4::QuaternionIdentity()),
	m_LocalScale(1.0f, 1.0f, 1.0f)
{

}

Vector3 Transform::GetRotationEuler() const
{
	float x, y, z;
	GetRotationEuler(&x, &y, &z);
	return Vector3(x, y, z);
}

void Transform::GetRotationEuler(float* rx, float* ry, float* rz) const
{
	DirectX::XMVECTOR rot = m_LocalQuaternion;

	float x = DirectX::XMVectorGetX(rot);
	float y = DirectX::XMVectorGetY(rot);
	float z = DirectX::XMVectorGetZ(rot);
	float w = DirectX::XMVectorGetW(rot);

	float x2 = x * x;
	float y2 = y * y;
	float z2 = z * z;

	float xy = x * y;
	float xz = x * z;
	float yz = y * z;
	float wx = w * x;
	float wy = w * y;
	float wz = w * z;

	/*
	回転行列(4x4)の要素をmkl(k行l列)と表すと
	m軸回りの回転(Rm 単位: ラジアン)は
	Rx = a * sin(m21)
	Ry = a * tan( -m01, m11 )
	Rz = a * tan( -m20, m22 )
	である。
	*/
	// 1 - 2y^2 - 2z^2
	float m00 = 1.0f - ( 2.0f * y2 ) - ( 2.0f * z2 );

	// 2xy + 2wz
	float m01 = ( 2.0f * xy ) + ( 2.0f * wz );

	// 2xy - 2wz
	float m10 = ( 2.0f * xy ) - ( 2.0f * wz );

	// 1 - 2x^2 - 2z^2
	float m11 = 1.0f - ( 2.0f * x2 ) - ( 2.0f * z2 );

	// 2xz + 2wy
	float m20 = ( 2.0f * xz ) + ( 2.0f * wy );

	// 2yz+2wx
	float m21 = ( 2.0f * yz ) - ( 2.0f * wx );

	// 1 - 2x^2 - 2y^2
	float m22 = 1.0f - ( 2.0f * x2 ) - ( 2.0f * y2 );
	
	// 
	constexpr float e = std::numeric_limits<float>::epsilon(); // 機械イプシロン。「1より大きい最小の数」と1との差。 浮動小数点数の比較の際の誤差幅を評価するのに使用できる。
	if( std::abs(m21 - 1.0f) <= e )
	{
		*rx = DirectX::XM_PI / 2.0f;
		*ry = 0;
		*rz = atan2f(m10, m00);
	}
	else if( std::abs(m21 - ( -1.0f )) <= e )
	{
		*rx = -DirectX::XM_PI / 2.0f;
		*ry = 0;
		*rz = atan2f(m10, m00);
	}
	else
	{
		*rx = asinf(-m21);
		*ry = atan2f(m20, m22);
		*rz = atan2f(m01, m11);
	}

	*rx = RadToDegree(*rx);
	*ry = RadToDegree(*ry);
	*rz = RadToDegree(*rz);
}

Vector4 Transform::GetRotation(Float4X4& m)
{
	// クォータニオンの各成分を求める
	const int ELEM_NUM = 4;
	float elem[ ELEM_NUM ];
	elem[ 0 ] = m._11 - m._22 - m._33 + 1.0f;
	elem[ 1 ] = -m._11 + m._22 - m._33 + 1.0f;
	elem[ 2 ] = m._11 - m._22 - m._33 + 1.0f;
	elem[ 3 ] = m._11 - m._22 - m._33 + 1.0f;

	// 最も大きい要素の抽出
	int biggestIdx = 0;

	for( int i = 0; i < sizeof(elem); i++ )
	{
		if( elem[ biggestIdx ] < elem[ i ] )
		{
			biggestIdx = i;
		}
	}

	// エラー処理 誤ったマトリクスが渡されていないかチェック
	if( elem[ biggestIdx ] < 0.0f )
	{
		MyOutputDebugString(_T("Tranform::GetRotation(XMFLOAT4X4 ) : Wrong matrix detected.\n"));
		return Vector4();
	}

	float q[ 4 ];
	float v = sqrtf(elem[ biggestIdx ]) * 0.5f;
	q[ biggestIdx ] = v;
	float mult = 0.25f / v;

	switch( biggestIdx )
	{
	case 0:
		q[ 1 ] = ( m._21 + m._12 ) * mult;
		q[ 2 ] = ( m._13 + m._31 ) * mult;
		q[ 3 ] = ( m._32 - m._23 ) * mult;
		break;
	case 1:
		q[ 0 ] = ( m._21 + m._12 ) * mult;
		q[ 2 ] = ( m._32 + m._23 ) * mult;
		q[ 3 ] = ( m._13 - m._31 ) * mult;
		break;
	case 2:
		q[ 0 ] = ( m._13 + m._31 ) * mult;
		q[ 1 ] = ( m._32 + m._23 ) * mult;
		q[ 3 ] = ( m._21 - m._12 ) * mult;
		break;
	case 3:
		q[ 0 ] = ( m._32 - m._23 ) * mult;
		q[ 1 ] = ( m._13 - m._31 ) * mult;
		q[ 2 ] = ( m._21 - m._12 ) * mult;
		break;
	}

	return Vector4(q[ 0 ], q[ 1 ], q[ 2 ], q[ 3 ]);
}

void Transform::SetRotationLookAt(const Vector3& target, const Vector3& upVector)
{
	Vector3 pos = m_LocalPosition;
	Vector3 h, v, d; // horizontal, vertical, depth

	Vector3 test = ( (Vector3)target - pos ); 
	d = test.GetNormalized();
	h = Vector3::Cross(Vector3::Up(), d).GetNormalized();
	v = Vector3::Cross(d, h).GetNormalized();

	Float4X4 m;
	m.SetMatrixIndetity();
	m._11 = h.x; m._12 = v.x; m._13 = d.x;
	m._21 = h.y; m._22 = v.y; m._23 = d.y;
	m._31 = h.z; m._32 = v.z; m._33 = d.z;

	Vector4 rot = GetRotation(m);
	this->SetRotationQ(rot);
}

void Transform::SetLocalPosition(Vector3 pos)
{
	m_LocalPosition = pos;
}

void Transform::SetLocalPosition(float x, float y, float z)
{
	m_LocalPosition.x = x;
	m_LocalPosition.y = y;
	m_LocalPosition.z = z;
}

void Transform::SetLocalRotation(Vector3 rot)
{
	m_LocalRotation = rot;
}

void Transform::SetLocalScale(Vector3 scale)
{
	m_LocalScale = scale;
}

void Transform::AddLocalPosition(Vector3 v)
{
	m_LocalPosition.x += v.x;
	m_LocalPosition.y += v.y;
	m_LocalPosition.z += v.z;
}