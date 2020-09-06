#pragma once
#include <DirectXMath.h>

namespace MyMath
{
	struct Vector3
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		Vector3() = default;
		Vector3(float _x, float _y, float _z)
		{
			x = _x;
			y = _y;
			z = _z;
		}


		Vector3(const Vector3&) = default;
		Vector3& operator=(const Vector3&) = default;

		Vector3(Vector3&&) = default;
		Vector3& operator=(Vector3&&) = default;

		Vector3 operator+(const Vector3& v)
		{
			Vector3 m;
			m.x = this->x + v.x;
			m.y = this->y + v.y;
			m.z = this->z + v.z;
			return m;
		}
		Vector3 operator-(const Vector3& v)
		{
			Vector3 m;
			m.x = this->x - v.x;
			m.y = this->y - v.y;
			m.z = this->z - v.z;
			return m;
		}
		Vector3 operator*(const Vector3& v)
		{
			Vector3 m;
			m.x = this->x * v.x;
			m.y = this->y * v.y;
			m.z = this->z * v.z;
			return m;
		}
		Vector3 operator/(const Vector3& v)
		{
			Vector3 m;
			m.x = this->x / v.x;
			m.y = this->y / v.y;
			m.z = this->z / v.z;
			return m;
		}
		Vector3 operator+=(const Vector3& v)
		{
			this->x += v.x;
			this->y += v.y;
			this->z += v.z;
			return *this;
		}
		Vector3 operator-=(const Vector3& v)
		{
			this->x -= v.x;
			this->y -= v.y;
			this->z -= v.z;
			return *this;
		}
		/* --- XMFLOAT3 --- */
		// キャスト
		operator DirectX::XMFLOAT3() const
		{
			DirectX::XMFLOAT3 f3;
			f3.x = this->x;
			f3.y = this->y;
			f3.z = this->z;
			return f3;
		}

		/* --- XMVECTOR --- */
		// コンストラクタ
		Vector3(const DirectX::XMVECTOR& vec)
			:
			x(0.0f),
			y(0.0f),
			z(0.0f)
			// :
			// DirectX::XMFLOAT3()
		{
			DirectX::XMVECTOR v = vec;
			DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, vec);
		}
		// 代入
		Vector3& operator=(DirectX::XMVECTOR& vec)
		{
			DirectX::XMStoreFloat3((DirectX::XMFLOAT3*)this, vec);

			return *this;
		}
		// キャスト
		operator DirectX::XMVECTOR() const
		{
			return DirectX::XMLoadFloat3((DirectX::XMFLOAT3*)this);
		}

		/* --- 関数 --- */
		float  GetLength() const
		{
			return static_cast<Vector3>(DirectX::XMVector3Length(DirectX::XMVECTOR(*this))).x; // XMVector3Lenghの戻すXMVECTORのx, y, zはどれもベクトルの長さで等しい。
		}
		Vector3 GetNormalized() const
		{
			Vector3 vec;
			vec = static_cast<Vector3>(DirectX::XMVector3Normalize(DirectX::XMVECTOR(*this)));
			return vec;
		}

		// static func
		static Vector3 Cross(const Vector3& v0, const Vector3& v1)
		{
			return DirectX::XMVector3Cross(DirectX::XMVECTOR(v0), DirectX::XMVECTOR(v1));
		}
		static Vector3 Up() // 真上
		{
			return Vector3(0.0f, 1.0f, 0.0f);
		}

	};

	struct Vector4
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;

		Vector4() = default;
		Vector4(float _x, float _y, float _z, float _w)
		{
			x = _x;
			y = _y;
			z = _z;
			w = _w;
		}


		Vector4(const Vector4&) = default;
		Vector4& operator=(const Vector4&) = default;

		Vector4(Vector4&&) = default;
		Vector4& operator=(Vector4&&) = default;

		Vector4 operator+(const Vector4& v)
		{
			Vector4 m;
			m.x = this->x + v.x;
			m.y = this->y + v.y;
			m.z = this->z + v.z;
			m.w = this->w + v.w;
			return m;
		}
		Vector4 operator-(const Vector4& v)
		{
			Vector4 m;
			m.x = this->x - v.x;
			m.y = this->y - v.y;
			m.z = this->z - v.z;
			m.w = this->w - v.w;
			return m;
		}
		Vector4 operator+=(const Vector4& v)
		{
			return *this + v;
		}
		Vector4 operator-=(const Vector4& v)
		{
			return *this - v;
		}

		// --- DirectX::XMVECTOR ---
		// キャスト
		operator DirectX::XMVECTOR() const
		{
			return DirectX::XMVectorSet(this->x, this->y, this->z, this->w);
		}
		// その他
		static Vector4 QuaternionIdentity()
		{
			return Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		}


	};

	struct Float4X4
	{
		union
		{
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
		};

		Float4X4() = default;

		Float4X4(const Float4X4&) = default;
		Float4X4& operator=(const Float4X4&) = default;

		Float4X4(Float4X4&&) = default;
		Float4X4& operator=(Float4X4&&) = default;

		XM_CONSTEXPR Float4X4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
			: _11(m00), _12(m01), _13(m02), _14(m03),
			_21(m10), _22(m11), _23(m12), _24(m13),
			_31(m20), _32(m21), _33(m22), _34(m23),
			_41(m30), _42(m31), _43(m32), _44(m33) {}
	  // explicit Float4X4(_In_reads_(16) const float *pArray);

		float  operator() (size_t Row, size_t Column) const { return m[Row][Column]; }
		float& operator() (size_t Row, size_t Column) { return m[Row][Column]; }

		void SetMatrixIndetity()
		{
			_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14 = 0.0f;
			_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24 = 0.0f;
			_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _34 = 0.0f;
			_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
		}

		static Float4X4 MatrixIndentity()
		{
			Float4X4 m;
			m.SetMatrixIndetity();
			return m;
		}
	};

	inline
		float RadToDegree(float rad)
	{
		return rad * 180.0f / 3.141592f;
	}
}