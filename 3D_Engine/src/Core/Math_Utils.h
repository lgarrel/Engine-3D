#pragma once
#include "pch.h"

namespace _Math
{
	static const float Infinity		= FLT_MAX;
	static const float PI			= 3.1415926535f;

	enum vect_value
	{
		_X,
		_Y,
		_Z,
		_W
	};

	static XMFLOAT4X4 Identity4x4()
	{
		static XMFLOAT4X4 id4X4
		{
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		};

		return id4X4; 
	}

	//////////////////////////////////

	static XMFLOAT3& MultiplyXMFLT3_Float(XMFLOAT3& _flt3, float _multi)
	{
		//XMFLOAT3& operator*=(const XMFLOAT3&);
		float _x = _flt3.x * _multi;
		float _y = _flt3.y * _multi;
		float _z = _flt3.z * _multi;

		XMFLOAT3 _rflt3{ _x, _y, _z };
		_flt3 = _rflt3;

		return _flt3;
	}

	static XMFLOAT3& MultiplyXMFLT3_XMFLT3(XMFLOAT3& _flt3, XMFLOAT3 _multi)
	{
		//XMFLOAT3& operator*=(const XMFLOAT3&);
		float _x = _flt3.x * _multi.x;
		float _y = _flt3.y * _multi.y;
		float _z = _flt3.z * _multi.z;

		XMFLOAT3 _rflt3{ _x, _y, _z };
		_flt3 = _rflt3;

		return _flt3;
	}

	//////////////////////////////////

	//	ScaleFLT4
	///////////////
	static XMFLOAT4 ScaleXMFLOAT4(XMFLOAT4 _xmflt, float _scale = 1.f)
	{
		_xmflt.x *= _scale;
		_xmflt.y *= _scale;
		_xmflt.z *= _scale;
		_xmflt.w *= _scale;

		return _xmflt;
	}

	static XMFLOAT4 ScaleXMFLOAT4(
		float _1, float _2, float _3, float _4,
		float _scale = 1.f)
	{
		_1 *= _scale;
		_2 *= _scale;
		_3 *= _scale;
		_4 *= _scale;

		XMFLOAT4 xmflt{ _1, _2, _3, _4 };

		return xmflt;
	}

	static XMFLOAT4 ScaleXMFLOAT4(float _flt4[4], float _scale = 1.f)
	{
		float _1 = _flt4[0] * _scale;
		float _2 = _flt4[1] * _scale;
		float _3 = _flt4[2] * _scale;
		float _4 = _flt4[3] * _scale;

		XMFLOAT4 xmflt{ _1, _2, _3, _4 };

		return xmflt;
	}

	//////////////////////////////////

	/*static XMFLOAT3 PartialXMFLT3Add(XMFLOAT3& _xmfloat, std::vector<vect_value> _values, std::vector<float> _addition)//WHY ARE YOU NOT WORKING?!
	{
		for (int i = 0; i < _values.size(); i++)
		{
			switch (_values[i])
			{
			case _Math::_X:
				_xmfloat.x += _addition[i];
				break;
			case _Math::_Y:
				_xmfloat.y += _addition[i];
				break;
			case _Math::_Z:
				_xmfloat.z += _addition[i];
				break;
			default:
				break;
			}
		}

		return _xmfloat;
	}*/

	static XMVECTOR ComputeNormal(FXMVECTOR p0, FXMVECTOR p1, FXMVECTOR p2)
	{
		XMVECTOR u = p1 - p0;
		XMVECTOR v = p2 - p0;

		return XMVector3Normalize(XMVector3Cross(u, v));
	}

	static XMMATRIX InverseTranspose(CXMMATRIX M)
	{
		XMMATRIX A = M;
		A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMVECTOR det = XMMatrixDeterminant(A);

		return XMMatrixTranspose(XMMatrixInverse(&det, A));
	}
};

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

namespace _Utils
{
	static XMFLOAT4X4 XMFLT4ToXMFLT4X4(XMFLOAT4 _1, XMFLOAT4 _2, XMFLOAT4 _3, XMFLOAT4 _4)
	{
		XMFLOAT4X4 mat;
		mat._11 = _1.x;	mat._12 = _1.y;	mat._13 = _1.z;	mat._14 = _1.w;
		mat._21 = _2.x;	mat._22 = _2.y;	mat._23 = _2.z;	mat._24 = _2.w;
		mat._31 = _3.x; mat._32 = _3.y; mat._33 = _3.z; mat._34 = _3.w;
		mat._41 = _4.x;	mat._42 = _4.y;	mat._43 = _4.z;	mat._44 = _4.w;

		XMFLOAT4X4 _mat = mat;

		return _mat;
	}


	////////////////////////////
	
	//jsp si ces 3 truc sont vraiment utiles....

	// matrix
	///////////
	static void AxisRotation4X4(XMFLOAT4X4& _mat, XMFLOAT3& _right, XMFLOAT3& _up, XMFLOAT3& _forward)
	{
		_mat._11 = _right.x;
		_mat._12 = _right.y;
		_mat._13 = _right.z;
		_mat._14 = 0.f;

		_mat._21 = _up.x;
		_mat._22 = _up.y;
		_mat._23 = _up.z;
		_mat._24 = 0.f;

		_mat._31 = _forward.x;
		_mat._32 = _forward.y;
		_mat._33 = _forward.z;
		_mat._34 = 0.f;

		_mat._41 = 0.f;
		_mat._42 = 0.f;
		_mat._43 = 0.f;
		_mat._44 = 1.f;
	}

	static void MatrixRotation4X4(XMFLOAT4X4& _mat, XMFLOAT3& _right, XMFLOAT3& _up, XMFLOAT3& _forward)
	{
		_right.x = _mat._11;
		_right.y = _mat._12;
		_right.z = _mat._13;

		_up.x = _mat._21;
		_up.y = _mat._22;
		_up.z = _mat._23;

		_forward.x = _mat._31;
		_forward.y = _mat._32;
		_forward.z = _mat._33;
	}

	static void QuaternionRotation4X4(XMFLOAT4X4& _mat, XMFLOAT3& _right, XMFLOAT3& _up, XMFLOAT3& _forward)
	{
		_right.x = _mat._11;
		_right.y = _mat._12;
		_right.z = _mat._13;

		_up.x = _mat._21;
		_up.y = _mat._22;
		_up.z = _mat._23;

		_forward.x = _mat._31;
		_forward.y = _mat._32;
		_forward.z = _mat._33;
	}

};

