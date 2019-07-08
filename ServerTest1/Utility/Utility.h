#pragma once

#include <math.h>
#include "FLOAT2.h"
#include "MyMath.h"


#ifndef UTILITY
#define UTILITY


namespace Utility
{
#define PI 3.14159265359


	struct Vector2 :public FLOAT2
	{
		Vector2() :FLOAT2(0.0f, 0.0f) {}
		explicit Vector2(float x) :FLOAT2(x, x) {}
		Vector2(float x, float y) { x = x; y = y; }

		Vector2& operator= (const Vector2& v) { x = v.x; y = v.y; return *this; }
		Vector2& operator+= (const Vector2& v) { x += v.x; y += v.y; return *this; }
		Vector2& operator-= (const Vector2& v) { x -= v.x; y -= v.y; return *this; }
		Vector2& operator*= (const float& s) { x *= s; y *= s; return *this; }
		Vector2& operator/= (const float& s) { if (s != 0) { x /= s; y /= s;  return *this; }return Vector2(0, 0); }

		Vector2& operator+ () { return *this; }
		Vector2& operator- () { return Vector2(-x, -y); }


		//float Length() { float d = (float)sqrt((x*x) + (y*y)); return d; }
		float Length()
		{
			__m128 v1 = MyMath::LoadFloat2(this);
			__m128 X = MyMath::Vector2Length(v1);
			return MyMath::VectorGetX(X);
		}
	};

	inline Vector2 operator+ (const Vector2& v1, const Vector2& v2)
	{
		__m128 vec1 = MyMath::LoadFloat2(&v1);
		__m128 vec2 = MyMath::LoadFloat2(&v2);
		__m128 X = MyMath::VectorAdd(vec1, vec2);
		Vector2 ret;
		MyMath::StoreFloat2(&ret, X);
		return ret;
	}
	inline Vector2 operator- (const Vector2& v1, const Vector2& v2)
	{
		__m128 vec1 = MyMath::LoadFloat2(&v1);
		__m128 vec2 = MyMath::LoadFloat2(&v2);
		__m128 X = MyMath::VectorSubtract(vec1, vec2);
		Vector2 ret;
		MyMath::StoreFloat2(&ret, X);
		return ret;
	}
	inline Vector2 operator* (const Vector2& v, float s)
	{
		__m128 v1 = MyMath::LoadFloat2(&v);
		__m128 X = MyMath::VectorScale(v1, s);
		Vector2 ret;
		MyMath::StoreFloat2(&ret, X);
		return ret;
	}

	struct Vector4 :public FLOAT4
	{
		Vector4() :FLOAT4(0.0f, 0.0f, 0.0f, 0.0f) {}
		explicit Vector4(float x) :FLOAT4(x, x, x, x) {}
		Vector4(float x, float y, float z, float w) { x = x; y = y; z = z; w = w; }

		Vector4& operator= (const Vector4& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
		Vector4& operator-= (const Vector4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
		Vector4& operator+= (const Vector4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
		Vector4& operator*= (const float& s) { x *= s; y *= s;  z *= s; w *= s; return *this; }
		Vector4& operator/= (const float& s) { if (s != 0) { x /= s; y /= s;  z /= s; w /= s; return *this; }return Vector4(0, 0, 0, 0); }

		Vector4& operator+ () { return *this; }
		Vector4& operator- () { return Vector4(-x, -y, -z, -w); }

	};


	class Collistions
	{
	public:

		/// <summary>
		/// �~�Ɛ����̂����蔻��
		/// </summary>
		/// <param name="lineStart">�����̎n�_</param>
		/// <param name="lineEnd">�����̏I�_</param>
		/// <param name="circlePos">�~�̒��S</param>
		/// <param name="radius">���a</param>
		/// <returns></returns>
		static bool HitLineAndCircle(Vector2 lineStart, Vector2 lineEnd, Vector2 circlePos, float radius)
		{

			Vector2 lineSegment = GetDifference(lineStart, lineEnd);
			Vector2 startToCircle = GetDifference(lineStart, circlePos);
			Vector2 endToCircle = GetDifference(lineEnd, circlePos);
			float startPointAngleR = ConvertDegree2Radian(lineSegment, startToCircle);
			float verticalLine = (float)(startToCircle.Length() * sin(startPointAngleR));

			// �����Ɖ~�̒��S�Ƃ̐����������a��菬�����ꍇ
			if (verticalLine <= radius)
			{
				// ��O ->�@�~�̒��S�������̗��[���O�ɂ���ꍇ
				// �����ƃx�N�g��A�̓��p * �����ƃx�N�g��B�̓��p ���v���X�̎��������ĂȂ���������Ȃ��B
				if (CalculateInnerProduct(startToCircle, lineSegment, ConvertDegree2Radian(lineSegment, startToCircle))
					*CalculateInnerProduct(endToCircle, lineSegment, ConvertDegree2Radian(lineSegment, endToCircle)) > 0)
				{
					// ��O���ɓ������Ă������
					// �x�N�g��A �� �x�N�g��B ���~�̔��a��菬�����ꍇ�͓������Ă���
					if (startToCircle.Length() < radius || endToCircle.Length() < radius) { return true; }
					return false;
				}
				return true;
			}
			return false;
		}

		/// <summary>
		/// �~�Ɛ����̂����蔻��
		/// </summary>
		/// <param name="sX">�����̎n�_X</param>
		/// <param name="sY">�����̎n�_Y</param>
		/// <param name="eX">�����̏I�_X</param>
		/// <param name="eY">�����̏I�_Y</param>
		/// <param name="circleX">�~�̒��SX</param>
		/// <param name="circleY">�~�̒��SY</param>
		/// <param name="radius">���a</param>
		/// <returns></returns>
		static bool HitLineAndCircle(float sX, float sY, float eX, float eY, float circleX, float circleY, float radius)
		{
			Vector2 lineSegment = GetDifference(sX, sY, eX, eY);
			Vector2 startToCircle = GetDifference(sX, sY, circleX, circleY);
			Vector2 endToCircle = GetDifference(eX, eY, circleX, circleY);
			float startPointAngleR = ConvertDegree2Radian(lineSegment, startToCircle);
			float verticalLine = (float)(startToCircle.Length() * sin(startPointAngleR));

			// �����Ɖ~�̒��S�Ƃ̐����������a��菬�����ꍇ
			if (verticalLine <= radius)
			{
				// ��O ->�@�~�̒��S�������̗��[���O�ɂ���ꍇ
				// �����ƃx�N�g��A�̓��p * �����ƃx�N�g��B�̓��p ���v���X�̎��������ĂȂ���������Ȃ��B
				if (CalculateInnerProduct(startToCircle, lineSegment, ConvertDegree2Radian(lineSegment, startToCircle))
					*CalculateInnerProduct(endToCircle, lineSegment, ConvertDegree2Radian(lineSegment, endToCircle)) > 0)
				{
					// ��O���ɓ������Ă������
					// �x�N�g��A �� �x�N�g��B ���~�̔��a��菬�����ꍇ�͓������Ă���
					if (startToCircle.Length() < radius || endToCircle.Length() < radius) { return true; }
					return false;
				}
				return true;
			}
			return false;

		}
		// 
		/// <summary>
		/// �~�Ɖ~�̂����蔻��
		/// </summary>
		/// <param name="circle1Pos">�~1�̒��S</param>
		/// <param name="radius1">�~1�̔��a</param>
		/// <param name="circle2Pos">�~2�̒��S</param>
		/// <param name="radius2">�~2�̔��a</param>
		/// <returns></returns>
		static bool HitCircleAndCircle(Vector2 circle1Pos, float radius1, Vector2 circle2Pos, float radius2)
		{
			// ��_�Ԃ̋����̓����Z�o
			float x = (circle1Pos.x - circle2Pos.x)*(circle1Pos.x - circle2Pos.x);
			float y = (circle1Pos.y - circle2Pos.y)*(circle1Pos.y - circle2Pos.y);

			// ���̔��a�̍��v�l�̓����Z�o
			float r = (radius2 + radius1)*(radius2 + radius1);

			if (x + y <= r)
			{
				return true;
			}
			return false;
		}
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="x1">�~1�̒��SX</param>
		/// <param name="y1">�~1�̒��SY</param>
		/// <param name="radius1">�~1�̔��a</param>
		/// <param name="x2">�~2�̒��SX</param>
		/// <param name="y2">�~2�̒��SY</param>
		/// <param name="radius2">�~2�̔��a</param>
		/// <returns></returns>
		static bool HitCircleAndCircle(float x1, float y1, float radius1, float x2, float y2, float radius2)
		{

			// ��_�Ԃ̋����̓����Z�o
			float x = (x1 - x2)*(x1 - x2);
			float y = (y1 - y2)*(y1 - y2);

			// ���̔��a�̍��v�l�̓����Z�o
			float r = (radius2 + radius1)*(radius2 + radius1);

			if (x + y <= r)
			{
				return true;
			}
			return false;
		}

		/// <summary>
		/// �~�Ɠ_�̂����蔻��
		/// </summary>
		/// <param name="point">�_</param>
		/// <param name="circlePos">�~�̒��S</param>
		/// <param name="radius">�~�̔��a</param>
		/// <returns></returns>
		static bool HitCircleAndPoint(Vector2 point, Vector2 circlePos, float radius)
		{
			float x = (point.x - circlePos.x)*(point.x - circlePos.x);
			float y = (point.y - circlePos.y)*(point.y - circlePos.y);

			if (x + y <= radius*radius)
			{
				return true;
			}
			return false;
		}
		/// <summary>
		/// �~�Ɠ_�̂����蔻��
		/// </summary>
		/// <param name="pointX">�_��X</param>
		/// <param name="pointY">�_��Y</param>
		/// <param name="circleX">�~�̒��SX</param>
		/// <param name="circleY">�~�̒��SY</param>
		/// <param name="radius">�~�̔��a</param>
		/// <returns></returns>
		static bool HitCircleAndPoint(float pointX, float pointY, float circleX, float circleY, float radius)
		{
			float x = (pointX - circleX)*(pointX - circleX);
			float y = (pointY - circleY)*(pointY - circleY);

			if (x + y <= radius*radius)
			{
				return true;
			}
			return false;
		}


		/// <summary>
		/// �O�p�`�Ɠ_�̂����蔻��
		/// </summary>
		/// <param name="point">�_</param>
		/// <param name="triPosA">�O�p�`�̒��_A</param>
		/// <param name="triPosB">�O�p�`�̒��_B</param>
		/// <param name="triPosC">�O�p�`�̒��_C</param>
		/// <returns></returns>
		static bool HitTriangleAndPoint(Vector2 point, Vector2 triPosA, Vector2 triPosB, Vector2 triPosC)
		{
			Vector2 difAB = triPosB - triPosA;
			Vector2 difBP = point - triPosB;

			Vector2 difBC = triPosC - triPosB;
			Vector2 difCP = point - triPosC;

			Vector2 difCA = triPosA - triPosC;
			Vector2 difAP = point - triPosA;

			double c1 = (difAB.x * difBP.y) - (difAB.y * difBP.x);
			double c2 = (difBC.x * difCP.y) - (difBC.y * difCP.x);
			double c3 = (difCA.x * difAP.y) - (difCA.y * difAP.x);

			if ((c1 > 0 && c2 > 0 && c3 > 0) || (c1 < 0 && c2 < 0 && c3 < 0)) { return true; }
			else { return false; }

		}
	

	private:


		//// �~�Ɛ����@START
		//Vector2 mlineSegment;		// �����x�N�g��

		//Vector2 mstartToCircle;	// �����̎n�_����~�̒��S�܂ł̃x�N�g��
		//Vector2 mendToCircle;		// �����̏I�_����~�̒��S�܂ł̃x�N�g��

		//float mverticalLine;	// �����ɑ΂���~�̒��S�܂ł̐�����

		//float mstartPointAngleD;	// �n�_����Ƃ�������Ɖ~�̒��S�܂ł̊p�x��(�f�O���[�p)
		//float mendPointAngleD;		// �I�_����Ƃ�������Ɖ~�̒��S�܂ł̊p�x��(�f�O���[�p)

		//float mstartPointAngleR;	// �n�_����Ƃ�������Ɖ~�̒��S�܂ł̊p�x��(���W�A���p)
		//float mendPointAngleR;		// �I�_����Ƃ�������Ɖ~�̒��S�܂ł̊p�x��(���W�A���p)

		//							// �~�Ɛ����@END

		//							// �~�Ɖ~�@START
		//							// �~�Ɖ~�@END
	private:
		// 2�_�����ԃx�N�g��
		static Vector2 GetDifference(Vector2 start, Vector2 end)
		{
			Vector2 vec = end - start;
			return vec;
		}
		static Vector2 GetDifference(float sX, float sY, float eX, float eY)
		{
			Vector2 vec = Vector2(sX - eX, sY - eY);
			return vec;
		}

		// �x�N�g�����m�̂Ȃ��p(�f�O���[�p�o��)
		static float ConvertRadian2Degree(Vector2 A, Vector2 B)
		{
			double cos;

			cos = ((A.x*B.x) + (A.y*B.y)) / (A.Length()*B.Length());
			cos = ((A.x*B.x) + (A.y*B.y)) / (A.Length()*B.Length());
			float radian = (float)acos(cos);
			float degree = (float)(radian*(180 / PI));
			return degree;
		}
		// �x�N�g�����m�̂Ȃ��p(���W�A���p�o��)
		static float ConvertDegree2Radian(Vector2 A, Vector2 B)
		{
			double cos;
			cos = ((A.x*B.x) + (A.y*B.y)) / (A.Length()*B.Length());
			float radian = (float)acos(cos);
			return radian;
		}

		// ��̃x�N�g���̓���
		static float CalculateInnerProduct(Vector2 A, Vector2 B, float DegreeAngle)
		{
			return (float)(A.Length() * B.Length() * cos(DegreeAngle));
		}
		// ��̃x�N�g���̓���
		static float CalculateInnerProduct(Vector2 A, Vector2 B)
		{
			return (A.x * B.x) + (A.y * B.y);
		}

		// �x�N�g���̈����Z
		static Vector2 Subtract(Vector2 A, Vector2 B)
		{
			return A - B;
		}
	
	};

}
#endif // UTILITY