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
		/// 円と線分のあたり判定
		/// </summary>
		/// <param name="lineStart">線分の始点</param>
		/// <param name="lineEnd">線分の終点</param>
		/// <param name="circlePos">円の中心</param>
		/// <param name="radius">半径</param>
		/// <returns></returns>
		static bool HitLineAndCircle(Vector2 lineStart, Vector2 lineEnd, Vector2 circlePos, float radius)
		{

			Vector2 lineSegment = GetDifference(lineStart, lineEnd);
			Vector2 startToCircle = GetDifference(lineStart, circlePos);
			Vector2 endToCircle = GetDifference(lineEnd, circlePos);
			float startPointAngleR = ConvertDegree2Radian(lineSegment, startToCircle);
			float verticalLine = (float)(startToCircle.Length() * sin(startPointAngleR));

			// 線分と円の中心との垂直線が半径より小さい場合
			if (verticalLine <= radius)
			{
				// 例外 ->　円の中心が線分の両端より外にある場合
				// 線分とベクトルAの内角 * 線分とベクトルBの内角 がプラスの時当たってないかもしれない。
				if (CalculateInnerProduct(startToCircle, lineSegment, ConvertDegree2Radian(lineSegment, startToCircle))
					*CalculateInnerProduct(endToCircle, lineSegment, ConvertDegree2Radian(lineSegment, endToCircle)) > 0)
				{
					// 例外中に当たっている条件
					// ベクトルA や ベクトルB が円の半径より小さい場合は当たっている
					if (startToCircle.Length() < radius || endToCircle.Length() < radius) { return true; }
					return false;
				}
				return true;
			}
			return false;
		}

		/// <summary>
		/// 円と線分のあたり判定
		/// </summary>
		/// <param name="sX">線分の始点X</param>
		/// <param name="sY">線分の始点Y</param>
		/// <param name="eX">線分の終点X</param>
		/// <param name="eY">線分の終点Y</param>
		/// <param name="circleX">円の中心X</param>
		/// <param name="circleY">円の中心Y</param>
		/// <param name="radius">半径</param>
		/// <returns></returns>
		static bool HitLineAndCircle(float sX, float sY, float eX, float eY, float circleX, float circleY, float radius)
		{
			Vector2 lineSegment = GetDifference(sX, sY, eX, eY);
			Vector2 startToCircle = GetDifference(sX, sY, circleX, circleY);
			Vector2 endToCircle = GetDifference(eX, eY, circleX, circleY);
			float startPointAngleR = ConvertDegree2Radian(lineSegment, startToCircle);
			float verticalLine = (float)(startToCircle.Length() * sin(startPointAngleR));

			// 線分と円の中心との垂直線が半径より小さい場合
			if (verticalLine <= radius)
			{
				// 例外 ->　円の中心が線分の両端より外にある場合
				// 線分とベクトルAの内角 * 線分とベクトルBの内角 がプラスの時当たってないかもしれない。
				if (CalculateInnerProduct(startToCircle, lineSegment, ConvertDegree2Radian(lineSegment, startToCircle))
					*CalculateInnerProduct(endToCircle, lineSegment, ConvertDegree2Radian(lineSegment, endToCircle)) > 0)
				{
					// 例外中に当たっている条件
					// ベクトルA や ベクトルB が円の半径より小さい場合は当たっている
					if (startToCircle.Length() < radius || endToCircle.Length() < radius) { return true; }
					return false;
				}
				return true;
			}
			return false;

		}
		// 
		/// <summary>
		/// 円と円のあたり判定
		/// </summary>
		/// <param name="circle1Pos">円1の中心</param>
		/// <param name="radius1">円1の半径</param>
		/// <param name="circle2Pos">円2の中心</param>
		/// <param name="radius2">円2の半径</param>
		/// <returns></returns>
		static bool HitCircleAndCircle(Vector2 circle1Pos, float radius1, Vector2 circle2Pos, float radius2)
		{
			// 二点間の距離の二乗を算出
			float x = (circle1Pos.x - circle2Pos.x)*(circle1Pos.x - circle2Pos.x);
			float y = (circle1Pos.y - circle2Pos.y)*(circle1Pos.y - circle2Pos.y);

			// 物の半径の合計値の二乗を算出
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
		/// <param name="x1">円1の中心X</param>
		/// <param name="y1">円1の中心Y</param>
		/// <param name="radius1">円1の半径</param>
		/// <param name="x2">円2の中心X</param>
		/// <param name="y2">円2の中心Y</param>
		/// <param name="radius2">円2の半径</param>
		/// <returns></returns>
		static bool HitCircleAndCircle(float x1, float y1, float radius1, float x2, float y2, float radius2)
		{

			// 二点間の距離の二乗を算出
			float x = (x1 - x2)*(x1 - x2);
			float y = (y1 - y2)*(y1 - y2);

			// 物の半径の合計値の二乗を算出
			float r = (radius2 + radius1)*(radius2 + radius1);

			if (x + y <= r)
			{
				return true;
			}
			return false;
		}

		/// <summary>
		/// 円と点のあたり判定
		/// </summary>
		/// <param name="point">点</param>
		/// <param name="circlePos">円の中心</param>
		/// <param name="radius">円の半径</param>
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
		/// 円と点のあたり判定
		/// </summary>
		/// <param name="pointX">点のX</param>
		/// <param name="pointY">点のY</param>
		/// <param name="circleX">円の中心X</param>
		/// <param name="circleY">円の中心Y</param>
		/// <param name="radius">円の半径</param>
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
		/// 三角形と点のあたり判定
		/// </summary>
		/// <param name="point">点</param>
		/// <param name="triPosA">三角形の頂点A</param>
		/// <param name="triPosB">三角形の頂点B</param>
		/// <param name="triPosC">三角形の頂点C</param>
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


		//// 円と線分　START
		//Vector2 mlineSegment;		// 線分ベクトル

		//Vector2 mstartToCircle;	// 線分の始点から円の中心までのベクトル
		//Vector2 mendToCircle;		// 線分の終点から円の中心までのベクトル

		//float mverticalLine;	// 線分に対する円の中心までの垂直線

		//float mstartPointAngleD;	// 始点を基準とする線分と円の中心までの角度Θ(デグリー角)
		//float mendPointAngleD;		// 終点を基準とする線分と円の中心までの角度Θ(デグリー角)

		//float mstartPointAngleR;	// 始点を基準とする線分と円の中心までの角度Θ(ラジアン角)
		//float mendPointAngleR;		// 終点を基準とする線分と円の中心までの角度Θ(ラジアン角)

		//							// 円と線分　END

		//							// 円と円　START
		//							// 円と円　END
	private:
		// 2点を結ぶベクトル
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

		// ベクトル同士のなす角(デグリー角出力)
		static float ConvertRadian2Degree(Vector2 A, Vector2 B)
		{
			double cos;

			cos = ((A.x*B.x) + (A.y*B.y)) / (A.Length()*B.Length());
			cos = ((A.x*B.x) + (A.y*B.y)) / (A.Length()*B.Length());
			float radian = (float)acos(cos);
			float degree = (float)(radian*(180 / PI));
			return degree;
		}
		// ベクトル同士のなす角(ラジアン角出力)
		static float ConvertDegree2Radian(Vector2 A, Vector2 B)
		{
			double cos;
			cos = ((A.x*B.x) + (A.y*B.y)) / (A.Length()*B.Length());
			float radian = (float)acos(cos);
			return radian;
		}

		// 二つのベクトルの内積
		static float CalculateInnerProduct(Vector2 A, Vector2 B, float DegreeAngle)
		{
			return (float)(A.Length() * B.Length() * cos(DegreeAngle));
		}
		// 二つのベクトルの内積
		static float CalculateInnerProduct(Vector2 A, Vector2 B)
		{
			return (A.x * B.x) + (A.y * B.y);
		}

		// ベクトルの引き算
		static Vector2 Subtract(Vector2 A, Vector2 B)
		{
			return A - B;
		}
	
	};

}
#endif // UTILITY