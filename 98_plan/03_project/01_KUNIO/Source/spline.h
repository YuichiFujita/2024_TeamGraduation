//=============================================================================
// 
// �X�v���C���w�b�_�[ [spline.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SPLINE_H_
#define _SPLINE_H_		// ��d�C���N���[�h�h�~

#include "calculation.h"

/**
@brief	�X�v���C��
*/
namespace MySpline
{

	/**
	@brief	�Z�O�����g���̒������ׂ����������ăA�[�N���e�[�u�����쐬
	@param	p0		[in]	�Z�O�����g�̊J�n�_
	@param	p1		[in]	�Z�O�����g�̏I���_
	@param	numSteps [in]	������
	@return	�Z�O�����g���̃A�[�N���e�[�u��
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
	@brief	�X�v���C����̋�������葬�x�Ŏ擾���邽�߂̃A�[�N���e�[�u�����쐬
	@param	posvec		[in]	�S�Ă̒n�_(vector)
	@param	arcLengths	[out]	�e�Z�O�����g�̗ݐσA�[�N��
	@return	�X�v���C���S�̂̒���
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
	@brief	�A�[�N�����琳�K�����ꂽ���Ԃ����߂�
	@param	segmentLengths	[in]	�Z�O�����g���̃A�[�N���e�[�u��
	@param	targetArcLength	[in]	�^�[�Q�b�g�̃A�[�N��
	@return	�Z�O�����g���̐��K�����ꂽ����(0.0f �` 1.0f)
	*/
	inline float FindSegmentTime(const std::vector<float>& segmentLengths, float targetArcLength)
	{
		float totalLength = segmentLengths.back();
		targetArcLength = fmod(targetArcLength, totalLength);  // ���������[�v������

		// �񕪒T���ŃA�[�N���ɑΉ����鎞�Ԃ�T��
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

		// ���`��Ԃł�萳�m�Ȏ��Ԃ��v�Z
		if (low > 0 && low < segmentLengths.size())
		{
			float l0 = segmentLengths[low - 1];
			float l1 = segmentLengths[low];
			return (low - 1 + (targetArcLength - l0) / (l1 - l0)) / (segmentLengths.size() - 1);
		}

		return low / static_cast<float>(segmentLengths.size() - 1);
	}


	/**
	@brief	�Z�O�����g�̒����v�Z�i���[�v�j
	@param	posvec		[in]	�S�Ă̒n�_(vector)
	@param	pVecLength	[inout]	�e�Z�O�����g�̒���
	@return	�Z�O�����g�̒���
	*/
	inline float CalSegmentLength(const std::vector<MyLib::Vector3>& posvec, std::vector<float> *pVecLength)
	{
		int posvecSize = posvec.size();

		// �Z�O�����g�̒������v�Z
		float totalLength = 0.0f;
		for (int i = 0; i < posvecSize; ++i)
		{
			// ����̃C���f�b�N�X(���[�v)
			int next = (i + 1) % posvecSize;

			// �_���m�̋���
			(*pVecLength)[i] = posvec[i].Distance(posvec[next]);

			// �S�Ă̒����v�Z
			totalLength += (*pVecLength)[i];
		}

		return totalLength;
	}

	/**
	@brief	�Z�O�����g�̒����v�Z�i�񃋁[�v�j
	@param	posvec		[in]	�S�Ă̒n�_(vector)
	@param	pVecLength	[inout]	�e�Z�O�����g�̒���
	@param	endDistance	[in]	�I�[�̒���
	@return	�Z�O�����g�̒���
	*/
	inline float CalSegmentLength_NonLoop(const std::vector<MyLib::Vector3>& posvec, std::vector<float>* pVecLength, float endDistance)
	{
		int posvecSize = posvec.size();

		// �Z�O�����g�̒������v�Z
		float totalLength = 0.0f;
		for (int i = 0; i < posvecSize; ++i)
		{
			// ����̃C���f�b�N�X(���[�v)
			int next = (i + 1) % posvecSize;

			// �_���m�̋���
			if (next == 0)
			{
				(*pVecLength)[i] = endDistance;
			}
			else
			{
				(*pVecLength)[i] = posvec[i].Distance(posvec[next]);
			}

			// �S�Ă̒����v�Z
			totalLength += (*pVecLength)[i];
		}

		return totalLength;
	}

	/**
	@brief	�X�v���C����̈ʒu�擾
	@param	posvec		[in]	�S�Ă̒n�_(vector)
	@param	distance	[in]	�i�񂾋���
	@return	void
	*/
	inline MyLib::Vector3 GetSplinePosition(const std::vector<MyLib::Vector3>& posvec, float _distance)
	{
#if 0
		int posvecSize = posvec.size();

		// �Z�O�����g�̒������v�Z
		std::vector<float> vecLength(posvecSize);
		float totalLength = CalSegmentLength(posvec, &vecLength);

		// ���������[�v������
		float distance = fmod(_distance, totalLength);

		// �ǂ̒n�_�ɂ��邩���v�Z
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

		// �������v�Z
		float ratio = distance / vecLength[idx];

		int n0 = (idx + (posvecSize - 1)) % posvecSize;
		int n1 = (n0 + 1) % posvecSize;
		int n2 = (n1 + 1) % posvecSize;
		int n3 = (n2 + 1) % posvecSize;

		return UtilFunc::Correction::CatmullRomSplineInterp(posvec[n0], posvec[n1], posvec[n2], posvec[n3], ratio);

#else


		if (posvec.size() < 4) return MyLib::Vector3(); // ���Ȃ��Ƃ�4�_���K�v

		// �Z�O�����g�̃A�[�N�����v�Z
		float totalLength = 0.0f;
		std::vector<std::vector<float>> segmentArcLengths;
		for (size_t i = 0; i < posvec.size() - 3; ++i)
		{
			std::vector<float> segmentLengths = BuildSegmentArcLengths(posvec[i], posvec[i + 1], posvec[i + 2], posvec[i + 3]);
			totalLength += segmentLengths.back();
			segmentArcLengths.push_back(segmentLengths);
		}

		// ���������[�v������
		float distance = fmod(_distance, totalLength);

		// �ǂ̃Z�O�����g�ɂ��邩���v�Z
		size_t segmentIndex = 0;
		while (distance > segmentArcLengths[segmentIndex].back())
		{
			distance -= segmentArcLengths[segmentIndex].back();
			++segmentIndex;
		}

		// �Z�O�����g���̎��Ԃ��擾
		float localTime = FindSegmentTime(segmentArcLengths[segmentIndex], distance);

		// Catmull-Rom Spline��Ԃňʒu���擾
		size_t idx = segmentIndex;
		return UtilFunc::Correction::CatmullRomSplineInterp(posvec[idx], posvec[idx + 1], posvec[idx + 2], posvec[idx + 3], localTime);

#endif
	}

	/**
	@brief	�X�v���C����̈ʒu�擾�i�񃋁[�v�j
	@param	posvec		[in]	�S�Ă̒n�_(vector)
	@param	distance	[in]	�i�񂾋���
	@param	endDistance	[in]	�I�[�̒���
	@return	void
	*/
	inline MyLib::Vector3 GetSplinePosition_NonLoop(const std::vector<MyLib::Vector3>& posvec, float _distance, float endDistance = 10.0f, int* pIdx = nullptr)
	{
		//return GetSplinePosition(posvec, _distance);

		int posvecSize = posvec.size();

		// �Z�O�����g�̒������v�Z
		std::vector<float> vecLength(posvecSize);
		float totalLength = CalSegmentLength_NonLoop(posvec, &vecLength, endDistance);

		// ���������[�v������
		float distance = fmod(_distance, totalLength);

		// �ǂ̒n�_�ɂ��邩���v�Z
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

		// �������v�Z
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