//=============================================================================
// 
// スプラインヘッダー [spline.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _SPLINE_H_
#define _SPLINE_H_		// 二重インクルード防止

#include "calculation.h"

/**
@brief	スプライン
*/
namespace MySpline
{

	/**
	@brief	セグメント内の長さを細かく分割してアーク長テーブルを作成
	@param	p0		[in]	セグメントの開始点
	@param	p1		[in]	セグメントの終了点
	@param	numSteps [in]	分割数
	@return	セグメント内のアーク長テーブル
	*/
	inline std::vector<float> BuildSegmentArcLengths(MyLib::Vector3 p0, MyLib::Vector3 p1, MyLib::Vector3 p2, MyLib::Vector3 p3, int numSteps = 100)
	{
		std::vector<float> segmentLengths(numSteps + 1);
		segmentLengths[0] = 0.0f;
		float length = 0.0f;

		MyLib::Vector3 prevPoint = p1;
		for (int i = 1; i <= numSteps; ++i)
		{
			float t = static_cast<float>(i) / numSteps;
			MyLib::Vector3 point = UtilFunc::Correction::CatmullRomSplineInterp(p0, p1, p2, p3, t);
			length += (point - prevPoint).Length();
			segmentLengths[i] = length;
			prevPoint = point;
		}

		return segmentLengths;
	}

	/**
	@brief	スプライン上の距離を一定速度で取得するためのアーク長テーブルを作成
	@param	posvec		[in]	全ての地点(vector)
	@param	arcLengths	[out]	各セグメントの累積アーク長
	@return	スプライン全体の長さ
	*/
	inline float BuildArcLengthTable(const std::vector<MyLib::Vector3>& posvec, std::vector<float>& arcLengths)
	{
		float totalLength = 0.0f;
		arcLengths.clear();
		arcLengths.push_back(0.0f);

		for (size_t i = 1; i < posvec.size(); ++i)
		{
			float segmentLength = (posvec[i] - posvec[i - 1]).Length();
			totalLength += segmentLength;
			arcLengths.push_back(totalLength);
		}

		return totalLength;
	}

	/**
	@brief	アーク長から正規化された時間を求める
	@param	segmentLengths	[in]	セグメント内のアーク長テーブル
	@param	targetArcLength	[in]	ターゲットのアーク長
	@return	セグメント内の正規化された時間(0.0f ～ 1.0f)
	*/
	inline float FindSegmentTime(const std::vector<float>& segmentLengths, float targetArcLength)
	{
		float totalLength = segmentLengths.back();
		targetArcLength = fmod(targetArcLength, totalLength);  // 長さをループさせる

		// 二分探索でアーク長に対応する時間を探す
		int low = 0;
		int high = static_cast<int>(segmentLengths.size()) - 1;

		while (low < high)
		{
			int mid = (low + high) / 2;
			if (segmentLengths[mid] < targetArcLength)
			{
				low = mid + 1;
			}
			else
			{
				high = mid;
			}
		}

		// 線形補間でより正確な時間を計算
		if (low > 0 && low < segmentLengths.size())
		{
			float l0 = segmentLengths[low - 1];
			float l1 = segmentLengths[low];
			return (low - 1 + (targetArcLength - l0) / (l1 - l0)) / (segmentLengths.size() - 1);
		}

		return low / static_cast<float>(segmentLengths.size() - 1);
	}


	/**
	@brief	セグメントの長さ計算（ループ）
	@param	posvec		[in]	全ての地点(vector)
	@param	pVecLength	[inout]	各セグメントの長さ
	@return	セグメントの長さ
	*/
	inline float CalSegmentLength(const std::vector<MyLib::Vector3>& posvec, std::vector<float> *pVecLength)
	{
		int posvecSize = posvec.size();

		// セグメントの長さを計算
		float totalLength = 0.0f;
		for (int i = 0; i < posvecSize; ++i)
		{
			// 次回のインデックス(ループ)
			int next = (i + 1) % posvecSize;

			// 点同士の距離
			(*pVecLength)[i] = posvec[i].Distance(posvec[next]);

			// 全ての長さ計算
			totalLength += (*pVecLength)[i];
		}

		return totalLength;
	}

	/**
	@brief	セグメントの長さ計算（非ループ）
	@param	posvec		[in]	全ての地点(vector)
	@param	pVecLength	[inout]	各セグメントの長さ
	@param	endDistance	[in]	終端の長さ
	@return	セグメントの長さ
	*/
	inline float CalSegmentLength_NonLoop(const std::vector<MyLib::Vector3>& posvec, std::vector<float>* pVecLength, float endDistance)
	{
		int posvecSize = posvec.size();

		// セグメントの長さを計算
		float totalLength = 0.0f;
		for (int i = 0; i < posvecSize; ++i)
		{
			// 次回のインデックス(ループ)
			int next = (i + 1) % posvecSize;

			// 点同士の距離
			if (next == 0)
			{
				(*pVecLength)[i] = endDistance;
			}
			else
			{
				(*pVecLength)[i] = posvec[i].Distance(posvec[next]);
			}

			// 全ての長さ計算
			totalLength += (*pVecLength)[i];
		}

		return totalLength;
	}

	/**
	@brief	スプライン上の位置取得
	@param	posvec		[in]	全ての地点(vector)
	@param	distance	[in]	進んだ距離
	@return	void
	*/
	inline MyLib::Vector3 GetSplinePosition(const std::vector<MyLib::Vector3>& posvec, float _distance)
	{
#if 0
		int posvecSize = posvec.size();

		// セグメントの長さを計算
		std::vector<float> vecLength(posvecSize);
		float totalLength = CalSegmentLength(posvec, &vecLength);

		// 距離をループさせる
		float distance = fmod(_distance, totalLength);

		// どの地点にいるかを計算
		int idx = 0;
		while (1)
		{
			if (distance < vecLength[idx])
			{
				break;
			}
			distance -= vecLength[idx];
			idx = (idx + 1) % posvecSize;
		}

		// 割合を計算
		float ratio = distance / vecLength[idx];

		int n0 = (idx + (posvecSize - 1)) % posvecSize;
		int n1 = (n0 + 1) % posvecSize;
		int n2 = (n1 + 1) % posvecSize;
		int n3 = (n2 + 1) % posvecSize;

		return UtilFunc::Correction::CatmullRomSplineInterp(posvec[n0], posvec[n1], posvec[n2], posvec[n3], ratio);

#else


		if (posvec.size() < 4) return MyLib::Vector3(); // 少なくとも4点が必要

		// セグメントのアーク長を計算
		float totalLength = 0.0f;
		std::vector<std::vector<float>> segmentArcLengths;
		for (size_t i = 0; i < posvec.size() - 3; ++i)
		{
			std::vector<float> segmentLengths = BuildSegmentArcLengths(posvec[i], posvec[i + 1], posvec[i + 2], posvec[i + 3]);
			totalLength += segmentLengths.back();
			segmentArcLengths.push_back(segmentLengths);
		}

		// 距離をループさせる
		float distance = fmod(_distance, totalLength);

		// どのセグメントにいるかを計算
		size_t segmentIndex = 0;
		while (distance > segmentArcLengths[segmentIndex].back())
		{
			distance -= segmentArcLengths[segmentIndex].back();
			++segmentIndex;
		}

		// セグメント内の時間を取得
		float localTime = FindSegmentTime(segmentArcLengths[segmentIndex], distance);

		// Catmull-Rom Spline補間で位置を取得
		size_t idx = segmentIndex;
		return UtilFunc::Correction::CatmullRomSplineInterp(posvec[idx], posvec[idx + 1], posvec[idx + 2], posvec[idx + 3], localTime);

#endif
	}

	/**
	@brief	スプライン上の位置取得（非ループ）
	@param	posvec		[in]	全ての地点(vector)
	@param	distance	[in]	進んだ距離
	@param	endDistance	[in]	終端の長さ
	@return	void
	*/
	inline MyLib::Vector3 GetSplinePosition_NonLoop(const std::vector<MyLib::Vector3>& posvec, float _distance, float endDistance = 10.0f, int* pIdx = nullptr)
	{
		//return GetSplinePosition(posvec, _distance);

		int posvecSize = posvec.size();

		// セグメントの長さを計算
		std::vector<float> vecLength(posvecSize);
		float totalLength = CalSegmentLength_NonLoop(posvec, &vecLength, endDistance);

		// 距離をループさせる
		float distance = fmod(_distance, totalLength);

		// どの地点にいるかを計算
		int idx = 0;
		while (1)
		{
			if (distance < vecLength[idx])
			{
				break;
			}
			distance -= vecLength[idx];
			idx = (idx + 1) % posvecSize;
		}

		if (pIdx != nullptr)
		{
			*pIdx = idx;
		}

		// 割合を計算
		float ratio = distance / vecLength[idx];

		int n0 = (idx + (posvecSize - 1)) % posvecSize;
		int n1 = (n0 + 1) % posvecSize;
		int n2 = (n1 + 1) % posvecSize;
		int n3 = (n2 + 1) % posvecSize;

		MyLib::Vector3 pos2 = posvec[n2], pos3 = posvec[n3];
		if (idx >= posvecSize - 1)
		{
			pos2 = posvec[n1] * 1.1f;
			pos3 = posvec[n1] * 1.2f;
		}

		return UtilFunc::Correction::CatmullRomSplineInterp(posvec[n0], posvec[n1], pos2, pos3, ratio);
	}

}

#endif