//=============================================================================
// 
// �֗��֐� [calculation.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _CALCULATION_H_
#define _CALCULATION_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "constans.h"
#include "3D_effect.h"

#include <DirectXMath.h>

using namespace DirectX;

/**
@brief	�ȒP�Ȑ���
@details �ڍׂȐ���
@param	hoge	[in][out][inout]	�����̐���
@return	piyo	�߂�l�̐���
*/

//==========================================================================
// �}�N����`
//==========================================================================
#define MAX_COMMENT	(512)	// �R�����g�̍ő吔

/**
@brief	�֗��֐�
*/
namespace UtilFunc
{
	namespace Transformation
	{
		template<class T> void ValueNormalize(T& Value, T MaxValue, T MinValue);	// �l�̐��K������
		template<class T> const T Clamp(T Value, T MinValue, T MaxValue);			
			MyLib::Vector3 RotationChangeToForwardVector(float rot);
		MyLib::Vector3 WorldMtxChangeToPosition(D3DXMATRIX worldmtx);	// ���[���h�}�g���b�N�X��pos�ɕϊ�
		MyLib::Vector3 MtxChangeToMatrix(const D3DXMATRIX& matrix);
	}

	namespace Collision
	{
		bool CollisionTriangle(const MyLib::Vector3& posCenter, const MyLib::Vector3& posLeft, const MyLib::Vector3& posRight, const MyLib::Vector3& MainPos, const MyLib::Vector3& MainPosOld);	// �O�p�`�̓����ɂ��邩����

	}
}

//==========================================================================
// �l�[���X�y�[�X��`
//==========================================================================
namespace UtilFunc	// �֗��֐�
{

	/**
	@brief	�␳�֐�
	*/
	namespace Correction
	{
		/**
		@brief	�����␳
		@details �␳�W����1.0f�ɂ����1F�ŕ␳���������܂�
		@param	fMain	[inout]	�␳����l
		@param	fTarget	[in]	�ڕW�̒l
		@param	fFactor	[in]	�␳�W��(0.0f �` 1.0f)
		@return	void
		*/
		inline void InertiaCorrection(float& fMain, float fTarget, float fFactor)
		{
			// �͈͊O�␳
			UtilFunc::Transformation::ValueNormalize(fFactor, 1.0f, 0.0f);

			fMain += (fTarget - fMain) * fFactor;
		}

		/**
		@brief	�����␳
		@details �␳�W����1.0f�ɂ����1F�ŕ␳���������܂�
		@param	main	[inout]	�␳����l
		@param	target	[in]	�ڕW�̒l
		@param	fFactor	[in]	�␳�W��(0.0f �` 1.0f)
		@return	void
		*/
		inline void InertiaCorrection(MyLib::Vector3& main, const MyLib::Vector3& target, float fFactor)
		{
			// �͈͊O�␳
			UtilFunc::Transformation::ValueNormalize(fFactor, 1.0f, 0.0f);

			main.x += (target.x - main.x) * fFactor;
			main.y += (target.y - main.y) * fFactor;
			main.z += (target.z - main.z) * fFactor;
		}

		/**
		@brief	���`�␳(����, ���j�A)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/linear.mp4
		@param	start	[in]	�����l
		@param	end		[in]	�ڕW�l
		@param	time	[in]	�␳����(0.0f �` 1.0f)
		@return	�␳���ꂽfloat�l
		*/
		inline float EasingLinear(float start, float end, float time)
		{
			// �͈͊O�␳
			UtilFunc::Transformation::ValueNormalize(time, 1.0f, 0.0f);
			return start + (end - start) * time;
		}

		/**
		@brief	���`�␳(����, ���j�A)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/linear.mp4
		@param	start	[in]	�����l
		@param	end		[in]	�ڕW�l
		@param	time	[in]	�␳����(0.0f �` 1.0f)
		@return	�␳���ꂽVector3�l
		*/
		inline MyLib::Vector3 EasingLinear(const MyLib::Vector3& start, const MyLib::Vector3& end, float time)
		{
			// �͈͊O�␳
			UtilFunc::Transformation::ValueNormalize(time, 1.0f, 0.0f);
			return start + (end - start) * time;
		}

		/**
		@brief	���`�␳(����, ���j�A)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/linear.mp4
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@return	�␳���ꂽfloat�l
		*/
		inline float EasingLinear(float start, float end, float startTime, float endTime, float currentTime)
		{
			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return EasingLinear(start, end, ratio);
		}

		/**
		@brief	���`�␳(����, ���j�A)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/linear.mp4
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@return	�␳���ꂽVector3�l
		*/
		inline MyLib::Vector3 EasingLinear(const MyLib::Vector3& start, const MyLib::Vector3& end, float startTime, float endTime, float currentTime)
		{
			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return EasingLinear(start, end, ratio);
		}

		/**
		@brief	���`�␳(���X�ɉ���, �C�[�Y�C��)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easein_1.mp4
		@param	start	[in]	�����l
		@param	end		[in]	�ڕW�l
		@param	time	[in]	�␳����(0.0f �` 1.0f)
		@return	�␳���ꂽfloat�l
		*/
		inline float EasingEaseIn(float start, float end, float time)
		{
			// �͈͊O�␳
			UtilFunc::Transformation::ValueNormalize(time, 1.0f, 0.0f);
			return start + (end - start) * time * time;
		}

		/**
		@brief	���`�␳(���X�ɉ���, �C�[�Y�C��)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easein_1.mp4
		@param	start	[in]	�����l
		@param	end		[in]	�ڕW�l
		@param	time	[in]	�␳����(0.0f �` 1.0f)
		@return	�␳���ꂽVector3�l
		*/
		inline MyLib::Vector3 EasingEaseIn(const MyLib::Vector3& start, const MyLib::Vector3& end, float time)
		{
			// �͈͊O�␳
			UtilFunc::Transformation::ValueNormalize(time, 1.0f, 0.0f);
			return start + (end - start) * time * time;
		}

		/**
		@brief	���`�␳(���X�ɉ���, �C�[�Y�C��)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easein_1.mp4
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@return	�␳���ꂽfloat�l
		*/
		inline float EasingEaseIn(float start, float end, float startTime, float endTime, float currentTime)
		{
			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return EasingEaseIn(start, end, ratio);
		}

		/**
		@brief	���`�␳(���X�ɉ���, �C�[�Y�C��)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easein_1.mp4
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@return	�␳���ꂽVector3�l
		*/
		inline MyLib::Vector3 EasingEaseIn(const MyLib::Vector3& start, const MyLib::Vector3& end, float startTime, float endTime, float currentTime)
		{
			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return EasingEaseIn(start, end, ratio);
		}

		/**
		@brief	���`�␳(���X�Ɍ���, �C�[�Y�A�E�g)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeout_1.mp4
		@param	start	[in]	�����l
		@param	end		[in]	�ڕW�l
		@param	time	[in]	�␳����(0.0f �` 1.0f)
		@return	�␳���ꂽfloat�l
		*/
		inline float EasingEaseOut(float start, float end, float time)
		{
			// �͈͊O�␳
			UtilFunc::Transformation::ValueNormalize(time, 1.0f, 0.0f);
			float fSquaredTime = time * time;

			return start + (end - start) * (1.0f - (1.0f - fSquaredTime) * (1.0f - fSquaredTime));
		}

		/**
		@brief	���`�␳(���X�Ɍ���, �C�[�Y�A�E�g)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeout_1.mp4
		@param	start	[in]	�����l
		@param	end		[in]	�ڕW�l
		@param	time	[in]	�␳����(0.0f �` 1.0f)
		@return	�␳���ꂽVector3�l
		*/
		inline MyLib::Vector3 EasingEaseOut(const MyLib::Vector3& start, const MyLib::Vector3& end, float time)
		{
			// �͈͊O�␳
			UtilFunc::Transformation::ValueNormalize(time, 1.0f, 0.0f);
			float fSquaredTime = time * time;

			return start + (end - start) * (1.0f - (1.0f - fSquaredTime) * (1.0f - fSquaredTime));
		}

		/**
		@brief	���`�␳(���X�Ɍ���, �C�[�Y�A�E�g)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeout_1.mp4
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@return	�␳���ꂽfloat�l
		*/
		inline float EasingEaseOut(float start, float end, float startTime, float endTime, float currentTime)
		{
			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return EasingEaseOut(start, end, ratio);
		}

		/**
		@brief	���`�␳(���X�Ɍ���, �C�[�Y�A�E�g)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeout_1.mp4
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@return	�␳���ꂽVector3�l
		*/
		inline MyLib::Vector3 EasingEaseOut(const MyLib::Vector3& start, const MyLib::Vector3& end, float startTime, float endTime, float currentTime)
		{
			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return EasingEaseOut(start, end, ratio);
		}

		/**
		@brief	���`�␳(�������Ă��猸��, �C�[�Y�C���A�E�g)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeinout_1.mp4
		@param	start	[in]	�����l
		@param	end		[in]	�ڕW�l
		@param	time	[in]	�␳����(0.0f �` 1.0f)
		@return	�␳���ꂽfloat�l
		*/
		inline float EasingEaseInOut(float start, float end, float time)
		{
			// �͈͊O�␳
			UtilFunc::Transformation::ValueNormalize(time, 1.0f, 0.0f);

			float fValue = 0.0f;
			if (time < 0.5f)
			{
				time = 2.0f * time;
				fValue = start + (end - start) * 0.5f * time * time;
			}
			else 
			{
				time = 2.0f * time - 1.0f;
				float fSquaredTime = time * time;
				fValue = start + (end - start) * (0.5f * (1.0f - (1.0f - fSquaredTime) * (1.0f - fSquaredTime)) + 0.5f);
			}
			return fValue;
		}

		/**
		@brief	���`�␳(�������Ă��猸��, �C�[�Y�C���A�E�g)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeinout_1.mp4
		@param	start	[in]	�����l
		@param	end		[in]	�ڕW�l
		@param	time	[in]	�␳����(0.0f �` 1.0f)
		@return	�␳���ꂽfloat�l
		*/
		inline MyLib::Vector3 EasingEaseInOut(const MyLib::Vector3& start, const MyLib::Vector3& end, float time)
		{
			// �͈͊O�␳
			UtilFunc::Transformation::ValueNormalize(time, 1.0f, 0.0f);

			MyLib::Vector3 fValue;
			if (time < 0.5f)
			{
				time = 2.0f * time;
				fValue = start + (end - start) * 0.5f * time * time;
			}
			else
			{
				time = 2.0f * time - 1.0f;
				float fSquaredTime = time * time;
				fValue = start + (end - start) * (0.5f * (1.0f - (1.0f - fSquaredTime) * (1.0f - fSquaredTime)) + 0.5f);
			}
			return fValue;
		}

		/**
		@brief	���`�␳(�������Ă��猸��, �C�[�Y�C���A�E�g)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeinout_1.mp4
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@return	�␳���ꂽfloat�l
		*/
		inline float EasingEaseInOut(float start, float end, float startTime, float endTime, float currentTime)
		{
			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return EasingEaseInOut(start, end, ratio);
		}

		/**
		@brief	���`�␳(�������Ă��猸��, �C�[�Y�C���A�E�g)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeinout_1.mp4
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@return	�␳���ꂽfloat�l
		*/
		inline MyLib::Vector3 EasingEaseInOut(const MyLib::Vector3& start, const MyLib::Vector3& end, float startTime, float endTime, float currentTime)
		{
			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return EasingEaseInOut(start, end, ratio);
		}

		/**
		@brief	���`�␳(�T�C���J�[�u�x�[�X�̕�ԁA����������������, �C�[�Y�C���A�E�g)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeinout_1.mp4
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@return	�␳���ꂽfloat�l
		*/
		inline float EasingEaseInOutSine(float start, float end, float startTime, float endTime, float currentTime)
		{
			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return 0.5f * (1.0f - cosf(D3DX_PI * ratio));
		}

		/**
		@brief	���`�␳(�T�C���J�[�u�x�[�X�̕�ԁA����������������, �C�[�Y�C���A�E�g)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeinout_1.mp4
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@return	�␳���ꂽfloat�l
		*/
		inline MyLib::Vector3 EasingEaseInOutSine(const MyLib::Vector3& start, const MyLib::Vector3& end, float startTime, float endTime, float currentTime)
		{
			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return 0.5f * (1.0f - cosf(D3DX_PI * ratio));
		}




		/**
		@brief	easeInBack�֐�
		@details https://easings.net/ja#easeInBack
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@param	c			[in]	�o�b�N�̋���
		@return	�␳���ꂽfloat�l
		*/
		inline float EaseInBack(float start, float end, float startTime, float endTime, float currentTime, float c = 1.70158f)
		{
			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			float cal = ratio * ratio * ((c + 1) * ratio - c);
			return start + (end - start) * cal;
		}

		/**
		@brief	easeOutBack�֐�
		@details https://easings.net/ja#easeInBack
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@param	c			[in]	�o�b�N�̋���
		@return	�␳���ꂽfloat�l
		*/
		inline float EaseOutBack(float start, float end, float startTime, float endTime, float currentTime, float c = 1.70158f)
		{
			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			ratio -= 1; // t �� (t - 1) �ɕϊ�

			float cal = ratio * ratio * ((c + 1) * ratio + c) + 1;
			return start + (end - start) * cal;
		}

		/**
		@brief	easeOutBack�֐�
		@details https://easings.net/ja#easeInBack
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@param	c			[in]	�o�b�N�̋���
		@return	�␳���ꂽfloat�l
		*/
		inline MyLib::Vector3 EaseOutBack(const MyLib::Vector3& start, const MyLib::Vector3& end, float startTime, float endTime, float currentTime, float c = 1.70158f)
		{
			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			ratio -= 1; // t �� (t - 1) �ɕϊ�

			float cal = ratio * ratio * ((c + 1) * ratio + c) + 1;
			return start + (end - start) * cal;
		}

		/**
		@brief	easeInOutBack�֐�
		@details https://easings.net/ja#easeInBack
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@param	c			[in]	�o�b�N�̋���
		@return	�␳���ꂽfloat�l
		*/
		inline float EaseInOutBack(float start, float end, float startTime, float endTime, float currentTime, float c = 1.70158f)
		{
			c *= 1.525f; // ��苭�����ꂽ����

			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			float cal = 0.0f;
			if (ratio < 0.5f)
			{
				cal = 0.5f * (ratio * 2) * (ratio * 2) * ((c + 1) * (ratio * 2) - c);
			}
			else 
			{
				ratio = ratio * 2 - 2;
				cal = 0.5f * (ratio * ratio * ((c + 1) * ratio + c) + 2);
			}
			return start + (end - start) * cal;
		}


		/**
		@brief	easeInExpo�֐�
		@details https://easings.net/ja#easeInExpo
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@return	�␳���ꂽfloat�l
		*/
		inline float EaseInExpo(float start, float end, float startTime, float endTime, float currentTime)
		{
			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			// �C�[�W���O�v�Z
			float eased = (ratio == 0) ? 0 : pow(2, 10 * (ratio - 1));

			// ���`��ԁiLerp�j���g�p���Č��ʂ�start����end�͈̔͂ɕϊ�
			return start + (end - start) * eased;
		}

		/**
		@brief	easeInExpo�֐�
		@details https://easings.net/ja#easeInExpo
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@return	�␳���ꂽfloat�l
		*/
		inline MyLib::Vector3 EaseInExpo(const MyLib::Vector3& start, const MyLib::Vector3& end, float startTime, float endTime, float currentTime)
		{
			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			// �C�[�W���O�v�Z
			float eased = (ratio == 0) ? 0 : pow(2, 10 * (ratio - 1));

			// ���`��ԁiLerp�j���g�p���Č��ʂ�start����end�͈̔͂ɕϊ�
			return start + (end - start) * eased;
		}

		/**
		@brief	easeOutExpo�֐�
		@details https://easings.net/ja#easeOutExpo
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@return	�␳���ꂽfloat�l
		*/
		inline float EaseOutExpo(float start, float end, float startTime, float endTime, float currentTime)
		{
			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			// �C�[�W���O�v�Z
			float eased = (ratio == 1) ? 1 : (1 - pow(2, -10 * ratio));

			// ���`��ԁiLerp�j���g�p���Č��ʂ�start����end�͈̔͂ɕϊ�
			return start + (end - start) * eased;
		}

		/**
		@brief	easeOutExpo�֐�
		@details https://easings.net/ja#easeOutExpo
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@return	�␳���ꂽfloat�l
		*/
		inline MyLib::Vector3 EaseOutExpo(const MyLib::Vector3& start, const MyLib::Vector3& end, float startTime, float endTime, float currentTime)
		{
			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			// �C�[�W���O�v�Z
			float eased = (ratio == 1) ? 1 : (1 - pow(2, -10 * ratio));

			// ���`��ԁiLerp�j���g�p���Č��ʂ�start����end�͈̔͂ɕϊ�
			return start + (end - start) * eased;
		}

		/**
		@brief	easeInOutExpo�֐�
		@details https://easings.net/ja#easeInOutExpo
		@param	start		[in]	�����l
		@param	end			[in]	�ڕW�l
		@param	startTime	[in]	�J�n����
		@param	endTime		[in]	�I������
		@param	currentTime	[in]	���݂̎���
		@return	�␳���ꂽfloat�l
		*/
		inline float EaseInOutExpo(float start, float end, float startTime, float endTime, float currentTime)
		{
			// ����
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			float eased;
			if (ratio == 0) {
				eased = 0;
			}
			else if (ratio == 1) {
				eased = 1;
			}
			else if (ratio < 0.5f) {
				eased = pow(2, 20 * ratio - 10) / 2;
			}
			else {
				eased = (2 - pow(2, -20 * ratio + 10)) / 2;
			}

			// ���`��ԁiLerp�j���g�p���Č��ʂ�start����end�͈̔͂ɕϊ�
			return start + (end - start) * eased;
		}








		/**
		@brief	Catmull-Rom Spline���(�S�Ă̓_��ʂ���`���)
		@details https://storage.googleapis.com/zenn-user-upload/cu4a6qn5njn0pskn6pfx6lypic0h
		@param	PointMili1	[in]	��̑O�̐���_(f-1)
		@param	Point0		[in]	��̍ŏ��̐���_(f0)
		@param	Point1		[in]	���2�Ԗڂ̐���_(f1)
		@param	Point2		[in]	��̎��̐���_ (f2)
		@param	time		[in]	�␳����(0.0f �` 1.0f)
		@return	��Ԃ��ꂽ�l
		*/
		inline MyLib::Vector3 CatmullRomSplineInterp(MyLib::Vector3 PointMili1, MyLib::Vector3 Point0, MyLib::Vector3 Point1, MyLib::Vector3 Point2, float time)
		{
			// 4�_�ŕ␳
			MyLib::Vector3 a4 = Point0;
			MyLib::Vector3 a3 = (Point1 - PointMili1) / 2.0f;
			MyLib::Vector3 a1 = (Point2 - Point0) / 2.0f - 2.0f * Point1 + a3 + 2.0f * a4;
			MyLib::Vector3 a2 = 3.0f * Point1 - (Point2 - Point0) / 2.0f - 2.0f * a3 - 3.0f * a4;

			return (a1 * time * time * time + a2 * time * time + a3 * time + a4);
		}

		/**
		@brief	�񎟋Ȑ��␳
		@param	Value		[inout]	�⊮�����l
		@param	nRange		[in]	�⊮�͈̔�
		@param	RangeOrigin	[in]	�⊮�͈͂̋N�_�ƂȂ�l
		@param	Max			[in]	�⊮�̍ő�l
		@param	nAlphaCnt	[in]	�񎟋Ȑ��␳�p�̃J�E���^�[�ϐ��ւ̎Q��
		@return	void
		*/
		template<class T> inline void CuadricCurveComp(T& Value, const int nRange, T RangeOrigin, T Max, int& nAlphaCnt)
		{
			// �⊮�̒l����o��
			T Comp = (static_cast<T>(Max) / static_cast<T>((nRange * nRange)));

			if (nAlphaCnt > nRange)
			{// �͈͒�������߂�
				nAlphaCnt = -nRange;
			}
			else if (nAlphaCnt < -nRange)
			{// �͈͒�������߂�
				nAlphaCnt = nRange;
			}

			// �s�����̃J�E���g���Z
			nAlphaCnt++;

			// �Ȑ��␳
			Value = static_cast<T>(((Comp * (nAlphaCnt * nAlphaCnt)) - (2 * (Comp * nAlphaCnt * RangeOrigin)) + (Comp * (RangeOrigin * RangeOrigin))));
			Value += RangeOrigin;
		}

	}

	/**
	@brief	�v�Z�֐�
	*/
	namespace Calculation
	{
		/**
		@brief	�����擾
		@param	nNum	[in]	���������߂����l
		@return	����
		*/
		inline int GetDigit(int nNum)
		{
			if (nNum <= 0)
			{
				nNum = 0;
			}

			// �����𕶎���ɕϊ�
			std::string number;
			sprintf((char*)number.c_str(), "%d", nNum);

			// ������̒����i�����j���擾
			int nDigit = strlen(number.c_str());
			if (nDigit <= 0)
			{
				nDigit = 1;
			}

			return nDigit;
		}

		/**
		@brief	�w�肳�ꂽ���e�덷�͈͓��ɂ��邩����
		@param	value	[in]	���肵�����l
		@param	target	[in]	��ɂ���l
		@param	range	[in]	���e�͈�
		@return	���茋��
		*/
		inline bool IsNearlyTarget(float value, float target, float range)
		{
			if (abs(value) <= range + target)
			{
				return true;
			}
			return false;
		}

		/**
		@brief	��_�Ԃ̒����擾(2D)
		@param	pos1	[in]	�n�_
		@param	pos2	[in]	�I�_
		@return	����
		*/
		inline float GetPosLength2D(MyLib::Vector3 pos1, MyLib::Vector3 pos2)
		{
			return sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
				+ (pos1.y - pos2.y) * (pos1.y - pos2.y));
		}

		/**
		@brief	��_�Ԃ̒����̐�Βl�擾(2D)
		@param	pos1	[in]	�n�_
		@param	pos2	[in]	�I�_
		@return	�����̐�Βl
		*/
		inline float GetFabsPosLength2D(MyLib::Vector3 pos1, MyLib::Vector3 pos2)
		{
			return fabsf(GetPosLength2D(pos1, pos2));
		}

		/**
		@brief	��_�Ԃ̒����擾(3D)
		@param	pos1	[in]	�n�_
		@param	pos2	[in]	�I�_
		@return	����
		*/
		inline float GetPosLength3D(MyLib::Vector3 pos1, MyLib::Vector3 pos2)
		{
			return sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
				+ (pos1.z - pos2.z) * (pos1.z - pos2.z));
		}

		/**
		@brief	��_�Ԃ̒����̐�Βl�擾(3D)
		@param	pos1	[in]	�n�_
		@param	pos2	[in]	�I�_
		@return	�����̐�Βl
		*/
		inline float GetFabsPosLength3D(MyLib::Vector3 pos1, MyLib::Vector3 pos2)
		{
			return fabsf(GetPosLength3D(pos1, pos2));
		}

		/**
		@brief	��_�̒��S�_�擾(3D)
		@param	pos1	[in]	�n�_
		@param	pos2	[in]	�I�_
		@return	���S�_
		*/
		inline MyLib::Vector3 GetCenterPosition3D(MyLib::Vector3 pos1, MyLib::Vector3 pos2)
		{
			return MyLib::Vector3(
				(pos1.x + pos2.x),
				(pos1.y + pos2.y),
				(pos1.z + pos2.z)) * 0.5f;
		}

		/**
		@brief	�ӏ�̈ʒu�擾(2D)
		@param	point1	[in]	�n�_�̒��_���W
		@param	point2	[in]	�I�_�̒��_���W
		@param	fRate	[in]	����
		@return	�ʒu
		*/
		inline MyLib::Vector3 GetPointOnEdge2D(MyLib::Vector3 point1, MyLib::Vector3 point2, float fRate)
		{
			return MyLib::Vector3(
				point1.x + fRate * (point2.x - point1.x),
				point1.y + fRate * (point2.y - point1.y),
				0.0f);
		}

		/**
		@brief	�������̈ʒu�擾
		@param	start		[in]	�n�_
		@param	end			[in]	�I�_
		@param	fMaxHeight	[in]	�����̍ő�l
		@param	time		[in]	�␳����(0.0f �` 1.0f)
		@return	��������̈ʒu�擾
		*/
		inline MyLib::Vector3 GetParabola3D(MyLib::Vector3 start, MyLib::Vector3 end, float fMaxHeight, float time)
		{
			// �ʒu
			MyLib::Vector3 pos = mylib_const::DEFAULT_VECTOR3;

			// XZ�ړ�
			pos.x = start.x + (end.x - start.x) * time;
			pos.z = start.z + (end.z - start.z) * time;

			// ����
			pos.y = start.y + (end.y - start.y) * time + sinf(D3DX_PI * time) * fMaxHeight;

			return pos;
		}

		/**
		@brief	�䗦���v�Z
		@param	size		[in]	�񎟌��T�C�Y
		@return	�䗦
		*/
		inline float AspectRatio(const D3DXVECTOR2& size) 
		{
			return size.x / size.y;
		}

		/**
		@brief	���f���̒��_���v�Z
		@param	RotY		[in]	���f����Y������
		@param	pVtxMax		[inout]	���_���W�̍ő�l
		@param	pVtxMin		[inout]	���_���W�̍ŏ��l
		@param	pMesh		[in]	���f���̃��b�V���I�u�W�F�N�g
		@param	pVtxBuff	[in]	���_�o�b�t�@
		@return	void
		*/
		inline void CalModelVtx(float RotY, MyLib::Vector3* pVtxMax, MyLib::Vector3* pVtxMin, LPD3DXMESH pMesh, BYTE* pVtxBuff)
		{
			// ���Z�b�g
			*pVtxMin = MyLib::Vector3(10000.0f, 10000.0f, 10000.0f);
			*pVtxMax = MyLib::Vector3(-10000.0f, -10000.0f, -10000.0f);

			// ���f���̒��_�����擾
			int nNumVtx = pMesh->GetNumVertices();

			// ���_�t�H�[�}�b�g�̃T�C�Y���擾
			DWORD dwSizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

			// ���_�o�b�t�@�����b�N
			pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			// �S�Ă̒��_���`�F�b�N����
			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				// ����̃f�[�^
				MyLib::Vector3 vtx = *(MyLib::Vector3*)pVtxBuff;

				// X
				if (pVtxMin->x > vtx.x)
				{// ���̍ŏ��l��������̒l����������������

					pVtxMin->x = vtx.x;
				}
				if (pVtxMax->x < vtx.x)
				{// ���̍ő�l��������̒l���傫��������

					pVtxMax->x = vtx.x;
				}

				// Y
				if (pVtxMin->y > vtx.y)
				{// ���̍ŏ��l��������̒l����������������

					pVtxMin->y = vtx.y;
				}
				if (pVtxMax->y < vtx.y)
				{// ���̍ő�l��������̒l���傫��������

					pVtxMax->y = vtx.y;
				}

				// Z
				if (pVtxMin->z > vtx.z)
				{// ���̍ŏ��l��������̒l����������������

					pVtxMin->z = vtx.z;
				}
				if (pVtxMax->z < vtx.z)
				{// ���̍ő�l��������̒l���傫��������

					pVtxMax->z = vtx.z;
				}

				pVtxBuff += dwSizeFVF;	// �T�C�Y���|�C���^�ړ�
			}

			// ���_�o�b�t�@���A�����b�N
			pMesh->UnlockVertexBuffer();

			MyLib::Vector3 VtxMax = *pVtxMax, VtxMin = *pVtxMin;

			if (RotY <= D3DX_PI * 0.5f + 0.05f && RotY >= D3DX_PI * 0.5f - 0.05f)
			{// 1.57f�̂Ƃ�

				pVtxMax->x = VtxMax.z;
				pVtxMin->x = VtxMin.z;

				pVtxMax->z = -VtxMin.x;
				pVtxMin->z = -VtxMax.x;
			}
			else if (RotY <= -D3DX_PI * 0.5f + 0.05f && RotY >= -D3DX_PI * 0.5f - 0.05f)
			{// -1.57f�̂Ƃ�

				pVtxMax->x = -VtxMin.z;
				pVtxMin->x = -VtxMax.z;

				pVtxMax->z = VtxMax.x;
				pVtxMin->z = VtxMin.x;
			}
			else if (RotY <= D3DX_PI + 0.05f && RotY >= D3DX_PI - 0.05f)
			{// D3DX_PI�̂Ƃ�

				pVtxMax->x = -VtxMin.x;
				pVtxMin->x = -VtxMax.x;

				pVtxMax->z = -VtxMin.z;
				pVtxMin->z = -VtxMax.z;
			}
			else if (RotY <= -D3DX_PI + 0.05f && RotY >= -D3DX_PI - 0.05f)
			{// -D3DX_PI�̂Ƃ�

				pVtxMax->x = -VtxMin.x;
				pVtxMin->x = -VtxMax.x;

				pVtxMax->z = -VtxMin.z;
				pVtxMin->z = -VtxMax.z;
			}
		}

		/**
		@brief	3���_����Ȃ鍂���擾
		@param	pos			[in]	���肷��I�u�W�F�N�g�̈ʒu
		@param	posCenter	[in]	���肷��O�p�`�̎�O�̒��_���W
		@param	posLeft		[in]	���肷��O�p�`�̍��̒��_���W
		@param	posRight	[in]	���肷��O�p�`�̉E�̒��_���W
		@param	posRight	[out]	���n����
		@return	3���_����Ȃ鍂��
		*/
		inline float GetVtxHeight(const MyLib::Vector3& pos, const MyLib::Vector3& posCenter, const MyLib::Vector3& posRight, const MyLib::Vector3& posLeft, bool* pLand)
		{
			// ����
			float fHeight = 0.0f;

			// ���p�����ƈʒu�̃x�N�g��
			MyLib::Vector3 calvec = pos - posCenter;

			// �x�N�g���Ɩ@��
			MyLib::Vector3
				vec1 = posRight - posCenter,
				vec2 = posLeft - posRight,
				nor = MyLib::Vector3(0.0f, 0.0f, 0.0f);

			if (UtilFunc::Collision::CollisionTriangle(posCenter, posRight, posLeft, pos, pos))
			{// �O�p�ɓ����Ă�����

				// �O�ς����߂�
				D3DXVec3Cross(&nor, &vec1, &vec2);

				// �O�ς̐��K�������Ė@���ɂ���
				D3DXVec3Normalize(&nor, &nor);

				if (nor.y != 0.0f)
				{// �@����0.0f����Ȃ�������

					// ���������߂�
					fHeight = -((calvec.x * nor.x) + (-posCenter.y * nor.y) + (calvec.z * nor.z)) / nor.y;
					*pLand = true;
				}
				else
				{
					// ���������߂�
					fHeight = -((calvec.x * nor.x) + (-posCenter.y * nor.y) + (calvec.z * nor.z)) / nor.y;
					*pLand = true;
				}
			}

			return fHeight;
		}

		/**
		@brief	����ւ�����
		@param	pValue		[out]	�\�[�g����l
		@param	nStart		[in]	�\�[�g����l�̏����l
		@param	nMax		[in]	�\�[�g�̍ő�l
		@return	void
		*/
		template<class T> inline void CalSort(T* pValue, int nStart, int nMax)
		{
			for (int nCntData = nStart; nCntData < nMax - 1; nCntData++)
			{
				for (int nCntSort = nCntData + 1; nCntSort < nMax; nCntSort++)
				{
					T nTemp = pValue[nCntData];
					pValue[nCntData] = pValue[nCntSort];
					pValue[nCntSort] = nTemp;
				}
			}
		}


		/**
		@brief	�X�v���C���g�Ȑ����
		@param	PointMili1		[in]	�n�_ -1
		@param	Point0			[in]	�n�_ 0
		@param	Point1			[in]	�n�_ 1
		@param	Point2			[in]	�n�_ 2
		@param	ratio			[in]	����
		@return	void
		*/
		inline MyLib::Vector3 CatmullRomSplineInterp(
			const MyLib::Vector3& PointMili1,
			const MyLib::Vector3& Point0,
			const MyLib::Vector3& Point1,
			const MyLib::Vector3& Point2,
			float ratio)
		{
			// 4�_�ŕ␳
			MyLib::Vector3 a4 = Point0;
			MyLib::Vector3 a3 = (Point1 - PointMili1) / 2.0f;
			MyLib::Vector3 a1 = (Point2 - Point0) / 2.0f - 2.0f * Point1 + a3 + 2.0f * a4;
			MyLib::Vector3 a2 = 3.0f * Point1 - (Point2 - Point0) / 2.0f - 2.0f * a3 - 3.0f * a4;

			return (a1 * ratio * ratio * ratio + a2 * ratio * ratio + a3 * ratio + a4);
		}
	}

	/**
	@brief	�����蔻��֐�
	*/
	namespace Collision
	{
		/**
		@brief	1��2�̓����蔻��(�~)(2D)
		@param	pos1		[in]	�Ώ�1�̈ʒu
		@param	pos2		[in]	�Ώ�2�̈ʒu
		@param	fRadius1	[in]	�Ώ�1�̔��a
		@param	fRadius2	[in]	�Ώ�2�̔��a
		@return	������������bool�l
		*/
		inline bool CircleRange2D(const MyLib::Vector3& pos1, const MyLib::Vector3& pos2, float fRadius1, float fRadius2)
		{
			float fLength =
				(pos1.x - pos2.x) * (pos1.x - pos2.x) +
				(pos1.y - pos2.y) * (pos1.y - pos2.y);

			if (fLength <= (fRadius1 + fRadius2) * (fRadius1 + fRadius2))
			{// �~�̒��ɓ�������
				return true;
			}

			return false;
		}

		/**
		@brief	1��2�̓����蔻��(�~)(3D)
		@param	pos1		[in]	�Ώ�1�̈ʒu
		@param	pos2		[in]	�Ώ�2�̈ʒu
		@param	fRadius1	[in]	�Ώ�1�̔��a
		@param	fRadius2	[in]	�Ώ�2�̔��a
		@return	������������bool�l
		*/
		inline bool CircleRange3D(const MyLib::Vector3& pos1, const MyLib::Vector3& pos2, float fRadius1, float fRadius2)
		{
			float fLength =
				(pos1.x - pos2.x) * (pos1.x - pos2.x) +
				(pos1.z - pos2.z) * (pos1.z - pos2.z);

			if (fLength <= (fRadius1 + fRadius2) * (fRadius1 + fRadius2))
			{// �~�̒��ɓ�������

				MyLib::Vector3 collisionPoint = pos1 + (pos2 - pos1).Normal() * fRadius1;

				return true;
			}

			return false;
		}

		/**
		@brief	1��2�̓����蔻��(��)
		@param	pos1		[in]	�Ώ�1�̈ʒu
		@param	pos2		[in]	�Ώ�2�̈ʒu
		@param	fRadius1	[in]	�Ώ�1�̔��a
		@param	fRadius2	[in]	�Ώ�2�̔��a
		@return	������������bool�l
		*/
		inline MyLib::HitResult SphereRange(const MyLib::Vector3& pos1, const MyLib::Vector3& pos2, float fRadius1, float fRadius2)
		{
			float fLength =
				(pos1.x - pos2.x) * (pos1.x - pos2.x) +
				(pos1.y - pos2.y) * (pos1.y - pos2.y) +
				(pos1.z - pos2.z) * (pos1.z - pos2.z);

			if (fLength <= (fRadius1 + fRadius2) * (fRadius1 + fRadius2))
			{// ���̒��ɓ�������

				MyLib::Vector3 collisionVector = (pos2 - pos1).Normal() * fRadius1;
				MyLib::Vector3 collisionPoint = pos1 + collisionVector;
				return MyLib::HitResult(true, collisionPoint);
			}

			return MyLib::HitResult(false, MyLib::Vector3());
		}

		/**
		@brief	1��2�̓����蔻��(��)
		@param	pos1		[in]	�Ώ�1�̈ʒu
		@param	pos2		[in]	�Ώ�2�̈ʒu
		@param	fRadius1	[in]	�Ώ�1�̔��a
		@param	fRadius2	[in]	�Ώ�2�̔��a
		@param	fHeight1	[in]	�Ώ�1�̍���
		@param	fHeight2	[in]	�Ώ�2�̍���
		@return	������������bool�l
		*/
		inline bool CylinderRange(MyLib::Vector3 pos1, MyLib::Vector3 pos2, float fRadius1, float fRadius2, float fHeight1, float fHeight2)
		{
			if (CircleRange3D(pos1, pos2, fHeight1, fHeight2) &&
				pos1.y + fHeight1 >= pos2.y &&
				pos1.y <= pos2.y + fHeight2)
			{// �~�̒��ɓ��� && �������Ԃ���
				return true;
			}

			return false;
		}

		/**
		@brief	�L�����N�^�[���m�̓����蔻�� & �␳
		@param	MainPos			[inout]	�����̈ʒu
		@param	MainPosOld		[inout]	�����̑O��̈ʒu
		@param	MainVtxMax		[in]	�����̒��_�ő�l
		@param	MainVtxMin		[in]	�����̒��_�ŏ��l
		@param	TargetPos		[inout]	�Ώۂ̈ʒu
		@param	TargetVtxMax	[in]	�Ώۂ̒��_�ő�l
		@param	TargetVtxMin	[in]	�Ώۂ̒��_�ŏ��l
		@return	void
		*/
		inline void CollisionCharacter(MyLib::Vector3& MainPos, MyLib::Vector3& MainPosOld, MyLib::Vector3 MainVtxMax, MyLib::Vector3 MainVtxMin, MyLib::Vector3& TargetPos, MyLib::Vector3 TargetVtxMax, MyLib::Vector3 TargetVtxMin)
		{
			// ���̔���
			if (MainPos.z + MainVtxMax.z > TargetPos.z + TargetVtxMin.z &&	// �G�̍ŏ��l�ɓ����Ă�
				MainPos.z + MainVtxMin.z < TargetPos.z + TargetVtxMax.z)	// �G�̍ő�l�ɓ����Ă�
			{// �G��Z���̒��ɂ���Ƃ�

				if (MainPos.x + MainVtxMin.x <= TargetPos.x + TargetVtxMax.x &&		// ���݂̍ŏ��l���G�̍ő�l����������
					MainPosOld.x + MainVtxMin.x >= TargetPos.x + TargetVtxMax.x)	// �O��̍ŏ��l���G�̍ő�l�����傫��
				{// �E����

					MainPos.x = (TargetPos.x + TargetVtxMax.x) - MainVtxMin.x + 0.1f;
				}
				if (MainPos.x + MainVtxMax.x >= TargetPos.x + TargetVtxMin.x &&		// ���݂̍ő�l���G�̍ŏ��l�����傫��
					MainPosOld.x + MainVtxMax.x <= TargetPos.x + TargetVtxMin.x)	// �O��̍ő�l���G�̍ŏ��l����������
				{// ������

					MainPos.x = (TargetPos.x + TargetVtxMin.x) - MainVtxMax.x - 0.1f;
				}
			}

			// �c�̔���
			if (MainPos.x + MainVtxMax.x > TargetPos.x + TargetVtxMin.x &&	// �G�̍ŏ��l�ɓ����Ă�
				MainPos.x + MainVtxMin.x < TargetPos.x + TargetVtxMax.x)	// �G�̍ő�l�ɓ����Ă�
			{// �G��X���̒��ɂ���Ƃ�

				if (MainPos.z + MainVtxMin.z <= TargetPos.z + TargetVtxMax.z &&		// ���݂̍ŏ��l���G�̍ő�l����������
					MainPosOld.z + MainVtxMin.z >= TargetPos.z + TargetVtxMax.z)	// �O��̍ŏ��l���G�̍ő�l�����傫��
				{// ������

					MainPos.z = (TargetPos.z + TargetVtxMax.z) - MainVtxMin.z + 0.1f;
				}
				if (MainPos.z + MainVtxMax.z >= TargetPos.z + TargetVtxMin.z &&		// ���݂̍ő�l���G�̍ŏ��l�����傫��
					MainPosOld.z + MainVtxMax.z <= TargetPos.z + TargetVtxMin.z)	// �O��̍ő�l���G�̍ŏ��l����������
				{// ��O����

					MainPos.z = (TargetPos.z + TargetVtxMin.z) - MainVtxMax.z - 0.1f;
				}
			}
		}

		/**
		@brief	�L�����N�^�[���m�̓����蔻��
		@param	MainPos			[in]	�����̈ʒu
		@param	MainPosOld		[in]	�����̑O��̈ʒu
		@param	MainVtxMax		[in]	�����̒��_�ő�l
		@param	MainVtxMin		[in]	�����̒��_�ŏ��l
		@param	TargetPos		[in]	�Ώۂ̈ʒu
		@param	TargetVtxMax	[in]	�Ώۂ̒��_�ő�l
		@param	TargetVtxMin	[in]	�Ώۂ̒��_�ŏ��l
		@return	������������bool�l
		*/
		inline bool bHitCharacter(MyLib::Vector3 MainPos, MyLib::Vector3 MainPosOld, MyLib::Vector3 MainVtxMax, MyLib::Vector3 MainVtxMin, MyLib::Vector3 TargetPos, MyLib::Vector3 TargetVtxMax, MyLib::Vector3 TargetVtxMin)
		{
			// ���̔���
			if (MainPos.z + MainVtxMax.z > TargetPos.z + TargetVtxMin.z &&	// �G�̍ŏ��l�ɓ����Ă�
				MainPos.z + MainVtxMin.z < TargetPos.z + TargetVtxMax.z)	// �G�̍ő�l�ɓ����Ă�
			{// �G��Z���̒��ɂ���Ƃ�

				if (MainPos.x + MainVtxMin.x <= TargetPos.x + TargetVtxMax.x &&	// ���݂̍ŏ��l���G�̍ő�l����������
					MainPosOld.x + MainVtxMin.x >= TargetPos.x + TargetVtxMax.x)// �O��̍ŏ��l���G�̍ő�l�����傫��
				{// �E����
					return true;
				}
				if (MainPos.x + MainVtxMax.x >= TargetPos.x + TargetVtxMin.x &&	// ���݂̍ő�l���G�̍ŏ��l�����傫��
					MainPosOld.x + MainVtxMax.x <= TargetPos.x + TargetVtxMin.x)// �O��̍ő�l���G�̍ŏ��l����������
				{// ������
					return true;
				}
			}

			// �c�̔���
			if (MainPos.x + MainVtxMax.x > TargetPos.x + TargetVtxMin.x &&	// �G�̍ŏ��l�ɓ����Ă�
				MainPos.x + MainVtxMin.x < TargetPos.x + TargetVtxMax.x)	// �G�̍ő�l�ɓ����Ă�
			{// �G��X���̒��ɂ���Ƃ�

				if (MainPos.z + MainVtxMin.z <= TargetPos.z + TargetVtxMax.z &&	// ���݂̍ŏ��l���G�̍ő�l����������
					MainPosOld.z + MainVtxMin.z >= TargetPos.z + TargetVtxMax.z)// �O��̍ŏ��l���G�̍ő�l�����傫��
				{// ������
					return true;
				}
				if (MainPos.z + MainVtxMax.z >= TargetPos.z + TargetVtxMin.z &&	// ���݂̍ő�l���G�̍ŏ��l�����傫��
					MainPosOld.z + MainVtxMax.z <= TargetPos.z + TargetVtxMin.z)// �O��̍ő�l���G�̍ŏ��l����������
				{// ��O����
					return true;
				}
			}

			return false;
		}

		/**
		@brief	�����̓����蔻��
		@param	pos0		[in]	�������[�̈ʒu
		@param	pos1		[in]	�����E�[�̈ʒu
		@param	pPos		[inout]	�����̈ʒu
		@param	posOld		[in]	�����̑O��ʒu
		@return	void
		*/
		inline void CollisionLimitLine(const MyLib::Vector3& pos0, const MyLib::Vector3& pos1, MyLib::Vector3& pPos, MyLib::Vector3 posOld)
		{
			// ���E���̃x�N�g��
			MyLib::Vector3 vecLine;
			vecLine.x = pos1.x - pos0.x;
			vecLine.z = pos1.z - pos0.z;

			// �v���C���[�̋��E���̃x�N�g��
			MyLib::Vector3 vecLinePlayer;
			vecLinePlayer.x = pPos.x - posOld.x;
			vecLinePlayer.z = pPos.z - posOld.z;

			// �v���C���[�ƕǂ̃x�N�g��
			MyLib::Vector3 vecToPosPlayer;
			vecToPosPlayer.x = pos1.x - posOld.x;
			vecToPosPlayer.z = pos1.z - posOld.z;

			// �ʐς̍ő�l
			float fMaxAreaPlayer = (vecLinePlayer.z * vecLine.x) - (vecLinePlayer.x * vecLine.z);

			// ����̖ʐ�
			float fNowAreaPlayer = (vecToPosPlayer.z * vecLine.x) - (vecToPosPlayer.x * vecLine.z);

			// ����
			float fRatePlayer = fNowAreaPlayer / fMaxAreaPlayer;

			if (fRatePlayer >= 0.0f && fRatePlayer <= 1.0f)
			{// �ʐς͈͓̔��ɂ����画��

				if ((vecLinePlayer.z * vecToPosPlayer.x) - (vecLinePlayer.x * vecToPosPlayer.z) > 0)
				{// �ǂɓ���������

					// �@���x�N�g��(���E���x�N�g����X��Z���])
					MyLib::Vector3 vecNor = MyLib::Vector3(vecLine.z, 0.0f, -vecLine.x);

					// �x�N�g���̐��K��
					D3DXVec3Normalize(&vecNor, &vecNor);

					// �v���C���[�̋t�ړ���
					MyLib::Vector3 PlayerInverceMove;
					PlayerInverceMove.x = vecLinePlayer.x * (fRatePlayer - 1.0f);
					PlayerInverceMove.z = vecLinePlayer.z * (fRatePlayer - 1.0f);

					// ����(�ǂ̖@���ƃv���C���[�̋t�ړ���)
					float fDot = (PlayerInverceMove.x * vecNor.x) + (PlayerInverceMove.z * vecNor.z);

					// �ǂ���ړ���
					MyLib::Vector3 MoveWall = vecNor * fDot;

					// �Ԃ������_�ɕ␳
					pPos += MoveWall + (vecNor * 0.1f);

				}
			}
		}

		/**
		@brief	�����̉E�ɂ��邩�ǂ���(2D)
		@details �����ɑ΂��ĉE�ɂ��邩�̔���Ȃ̂ŁA���ɋ��ė~�����ꍇ��pos0��pos1�����ւ��Ďg�p
		@param	pos0		[in]	�������[�̈ʒu
		@param	pos1		[in]	�����E�[�̈ʒu
		@param	MainPos		[in]	���肷��Ώۂ̈ʒu
		@param	MainPos		[in]	���肷��Ώۂ̑O��̈ʒu
		@return	�����̉E���ɂ��邩��bool�l
		*/
		inline bool CollisionLine2D(const MyLib::Vector3& pos0, const MyLib::Vector3& pos1, const MyLib::Vector3& MainPos, const MyLib::Vector3& MainPosOld)
		{
			// ���E���̃x�N�g��
			MyLib::Vector3 vecLine;
			vecLine.x = pos1.x - pos0.x;
			vecLine.y = pos0.y - pos1.y;

			// �e�ƕǂ̃x�N�g��
			MyLib::Vector3 vecToPos;
			vecToPos.x = MainPos.x - pos0.x;
			vecToPos.y = pos0.y - MainPos.y;

			if ((vecLine.y * vecToPos.x) - (vecLine.x * vecToPos.y) <= 0)
			{// ���𒴂���
				return true;
			}

			return false;
		}

		/**
		@brief	�����̉E�ɂ��邩�ǂ���(3D)
		@details �����ɑ΂��ĉE�ɂ��邩�̔���Ȃ̂ŁA���ɋ��ė~�����ꍇ��pos0��pos1�����ւ��Ďg�p
		@param	pos0		[in]	�������[�̈ʒu
		@param	pos1		[in]	�����E�[�̈ʒu
		@param	MainPos		[in]	���肷��Ώۂ̈ʒu
		@param	MainPos		[in]	���肷��Ώۂ̑O��̈ʒu
		@return	�����̉E���ɂ��邩��bool�l
		*/
		inline bool CollisionLine3D(const MyLib::Vector3& pos0, const MyLib::Vector3& pos1, const MyLib::Vector3& MainPos, const MyLib::Vector3& MainPosOld)
		{
			// ���E���̃x�N�g��
			MyLib::Vector3 vecLine;
			vecLine.x = pos1.x - pos0.x;
			vecLine.z = pos1.z - pos0.z;

			// �e�ƕǂ̃x�N�g��
			MyLib::Vector3 vecToPos;
			vecToPos.x = MainPos.x - pos0.x;
			vecToPos.z = MainPos.z - pos0.z;

			if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) <= 0)
			{// ���𒴂���
				return true;
			}

			return false;
		}

		/**
		@brief	�O�p�̓����ɂ��邩����
		@param	posCenter	[in]	���肷��O�p�`�̎�O�̒��_���W
		@param	posLeft		[in]	���肷��O�p�`�̍��̒��_���W
		@param	posRight	[in]	���肷��O�p�`�̉E�̒��_���W
		@param	MainPos		[in]	�����̈ʒu
		@param	MainPosOld	[in]	�����̑O��ʒu
		@return	���ɂ��邩��bool�l
		*/
		inline bool CollisionTriangle(const MyLib::Vector3& posCenter, const MyLib::Vector3& posLeft, const MyLib::Vector3& posRight, const MyLib::Vector3& MainPos, const MyLib::Vector3& MainPosOld)
		{
			// �����������̔���
			bool bHit = false;
			bool bLine1 = false, bLine2 = false, bLine3 = false;

			// �^�񒆂ƍ��̐�
			bLine1 = UtilFunc::Collision::CollisionLine3D(posCenter, posLeft, MainPos, MainPosOld);

			// ���ƉE�̐�
			bLine2 = UtilFunc::Collision::CollisionLine3D(posLeft, posRight, MainPos, MainPosOld);

			// �E�Ɛ^�񒆂̐�
			bLine3 = UtilFunc::Collision::CollisionLine3D(posRight, posCenter, MainPos, MainPosOld);

			// �S�Ă͈̔͂ɓ����Ă�����
			if (bLine1 && bLine2 && bLine3)
			{
				bHit = true;
			}

			return bHit;
		}

		/**
		@brief	�l�p�̓����ɂ��邩����
		@details 4�̒��_���S�Č��܂��Ă���ꍇ�Ɏg��
		@param	posLeftUP	[in]	���肷��l�p�`����̒��_���W
		@param	posRightUP	[in]	���肷��l�p�`�E��̒��_���W
		@param	posLeftDW	[in]	���肷��l�p�`�����̒��_���W
		@param	posRightDW	[in]	���肷��l�p�`�E���̒��_���W
		@param	MainPos		[in]	�����̈ʒu
		@return	���ɂ��邩��bool�l
		*/
		inline bool CollisionSquare(const MyLib::Vector3& posLeftUP, const MyLib::Vector3& posRightUP, const MyLib::Vector3& posLeftDW, const MyLib::Vector3& posRightDW, const MyLib::Vector3& MainPos)
		{
			// �����������̔���
			bool bHit = false;
			bool bLine1 = false, bLine2 = false, bLine3 = false, bLine4 = false;

			// ����ƉE��
			bLine1 = UtilFunc::Collision::CollisionLine3D(posRightUP, posLeftUP, MainPos, MainPos);

			// �E��ƉE��
			bLine2 = UtilFunc::Collision::CollisionLine3D(posRightDW, posRightUP, MainPos, MainPos);

			// �E���ƍ���
			bLine3 = UtilFunc::Collision::CollisionLine3D(posLeftDW, posRightDW, MainPos, MainPos);

			// �����ƍ���
			bLine4 = UtilFunc::Collision::CollisionLine3D(posLeftUP, posLeftDW, MainPos, MainPos);

			// �S�Ă͈̔͂ɓ����Ă�����
			if (bLine1 && bLine2 && bLine3 && bLine4)
			{
				// ��������
				bHit = true;
			}

			return bHit;
		}

		/**
		@brief	�l�p�̓����ɂ��邩����
		@details 4�̒��_���܂����߂��Ă��Ȃ��ꍇ�Ɏg��
		@param	posSquare	[in]	���肷��l�p�`�̒��S���W
		@param	SquareSize	[in]	���肷��l�p�`�̃T�C�Y
		@param	fSquareRot	[in]	���肷��l�p�`�̃T�C�Y
		@param	MainPos		[in]	�����̈ʒu
		@return	���ɂ��邩��bool�l
		*/
		inline bool CollisionSquare(MyLib::Vector3 posSquare, D3DXVECTOR2 SquareSize, float fSquareRot, MyLib::Vector3 MainPos)
		{
			// �����������̔���
			bool bHit = false;
			bool bLine1 = false, bLine2 = false, bLine3 = false, bLine4 = false;

			float fLength = sqrtf(SquareSize.x * SquareSize.x + SquareSize.y * SquareSize.y);	// �Ίp���̒���
			float fAngle = atan2f(SquareSize.x, SquareSize.y);									// �Ίp���̌���

			// ���肷��l�p��4���_
			MyLib::Vector3 LeftUp = MyLib::Vector3(posSquare.x + sinf(fSquareRot - fAngle) * fLength, posSquare.y, posSquare.z + cosf(fSquareRot - fAngle) * fLength);
			MyLib::Vector3 RightUp = MyLib::Vector3(posSquare.x + sinf(fSquareRot + fAngle) * fLength, posSquare.y, posSquare.z + cosf(fSquareRot + fAngle) * fLength);
			MyLib::Vector3 LeftDown = MyLib::Vector3(posSquare.x + sinf(fSquareRot - D3DX_PI + fAngle) * fLength, posSquare.y, posSquare.z + cosf(fSquareRot - D3DX_PI + fAngle) * fLength);
			MyLib::Vector3 RightDown = MyLib::Vector3(posSquare.x + sinf(fSquareRot + D3DX_PI - fAngle) * fLength, posSquare.y, posSquare.z + cosf(fSquareRot + D3DX_PI - fAngle) * fLength);

			// ����ƉE��
			bLine1 = UtilFunc::Collision::CollisionLine3D(RightUp, LeftUp, MainPos, MainPos);

			// �E��ƉE��
			bLine2 = UtilFunc::Collision::CollisionLine3D(RightDown, RightUp, MainPos, MainPos);

			// �E���ƍ���
			bLine3 = UtilFunc::Collision::CollisionLine3D(LeftDown, RightDown, MainPos, MainPos);

			// �����ƍ���
			bLine4 = UtilFunc::Collision::CollisionLine3D(LeftUp, LeftDown, MainPos, MainPos);

			// �S�Ă͈̔͂ɓ����Ă�����
			if (bLine1 && bLine2 && bLine3 && bLine4)
			{
				// ��������
				bHit = true;
			}

			return bHit;
		}

		/**
		@brief	�l�p�̓����ɂ��邩����(2D)
		@details 4�̒��_���S�Č��܂��Ă���ꍇ�Ɏg��
		@param	posLeftUP	[in]	���肷��l�p�`����̒��_���W
		@param	posRightUP	[in]	���肷��l�p�`�E��̒��_���W
		@param	posLeftDW	[in]	���肷��l�p�`�����̒��_���W
		@param	posRightDW	[in]	���肷��l�p�`�E���̒��_���W
		@param	MainPos		[in]	�����̈ʒu
		@return	���ɂ��邩��bool�l
		*/
		inline bool CollisionSquare2D(MyLib::Vector3 posLeftUP, MyLib::Vector3 posRightUP, MyLib::Vector3 posLeftDW, MyLib::Vector3 posRightDW, MyLib::Vector3 MainPos)
		{
			// �����������̔���
			bool bHit = false;
			bool bLine1 = false, bLine2 = false, bLine3 = false, bLine4 = false;

			// ����ƉE��
			bLine1 = UtilFunc::Collision::CollisionLine2D(posRightUP, posLeftUP, MainPos, MainPos);

			// �E��ƉE��
			bLine2 = UtilFunc::Collision::CollisionLine2D(posRightDW, posRightUP, MainPos, MainPos);

			// �E���ƍ���
			bLine3 = UtilFunc::Collision::CollisionLine2D(posLeftDW, posRightDW, MainPos, MainPos);

			// �����ƍ���
			bLine4 = UtilFunc::Collision::CollisionLine2D(posLeftUP, posLeftDW, MainPos, MainPos);

			// �S�Ă͈̔͂ɓ����Ă�����
			if (bLine1 && bLine2 && bLine3 && bLine4)
			{
				bHit = true;
			}

			return bHit;
		}

		/**
		@brief	�l�p�Ǝl�p�̓����蔻��(2D)
		@details 4�̒��_���܂����߂��Ă��Ȃ��ꍇ�Ɏg��
		@param	MainPos		[inout]	�����̈ʒu
		@param	TargetPos	[inout]	�Ώۂ̈ʒu
		@param	MainSize	[in]	�����̃T�C�Y
		@param	TargetSize	[in]	�Ώۂ̃T�C�Y
		@param	fMainRot	[in]	������Y���̌���
		@param	fTargetRot	[in]	�Ώۂ�Y���̌���
		@return	�Փ˂�������bool�l
		*/
		inline bool CollisionSquareSquare2D(MyLib::Vector3& MainPos, MyLib::Vector3& TargetPos, D3DXVECTOR2 MainSize, D3DXVECTOR2 TargetSize, float fMainRot, float fTargetRot)
		{
			float fTargetLength = sqrtf(
				TargetSize.x * TargetSize.x +
				TargetSize.y * TargetSize.y);							// �Ίp���̒���
			float fTargetAngle = atan2f(TargetSize.x, TargetSize.y);	// �Ίp���̌���

			// ���肷��l�p��4���_
			MyLib::Vector3 LeftUp = MyLib::Vector3(
				TargetPos.x + sinf(fTargetRot - fTargetAngle) * fTargetLength,
				TargetPos.y,
				TargetPos.z + cosf(fTargetRot - fTargetAngle) * fTargetLength);

			MyLib::Vector3 RightUp = MyLib::Vector3(
				TargetPos.x + sinf(fTargetRot + fTargetAngle) * fTargetLength,
				TargetPos.y,
				TargetPos.z + cosf(fTargetRot + fTargetAngle) * fTargetLength);

			MyLib::Vector3 LeftDown = MyLib::Vector3(
				TargetPos.x + sinf(fTargetRot - D3DX_PI + fTargetAngle) * fTargetLength,
				TargetPos.y,
				TargetPos.z + cosf(fTargetRot - D3DX_PI + fTargetAngle) * fTargetLength);

			MyLib::Vector3 RightDown = MyLib::Vector3(
				TargetPos.x + sinf(fTargetRot + D3DX_PI - fTargetAngle) * fTargetLength,
				TargetPos.y,
				TargetPos.z + cosf(fTargetRot + D3DX_PI - fTargetAngle) * fTargetLength);

			//***********************
			// ��`�̔���
			//***********************
			if (UtilFunc::Collision::CollisionSquare(MainPos, MainSize, fMainRot, LeftUp))
			{// ����
				return true;
			}
			if (UtilFunc::Collision::CollisionSquare(MainPos, MainSize, fMainRot, RightUp))
			{// �E��
				return true;
			}
			if (UtilFunc::Collision::CollisionSquare(MainPos, MainSize, fMainRot, LeftDown))
			{// ����
				return true;
			}
			if (UtilFunc::Collision::CollisionSquare(MainPos, MainSize, fMainRot, RightDown))
			{// �E��
				return true;
			}

			return false;
		}

		/**
		@brief	�~�Ƌ�`�̓����蔻��(2D)
		@param	posCircle		[inout]	�~�̒��S���W
		@param	posSquare		[inout]	��`�̒��S���W
		@param	rotSquare		[in]	��`�̌���
		@param	fCircleRadius	[in]	�~�̔��a
		@param	SquareSize		[in]	��`�̃T�C�Y
		@return	�Փ˂�������bool�l
		*/
		inline bool CollisionCircleSquare2D(MyLib::Vector3& posCircle, MyLib::Vector3& posSquare, MyLib::Vector3 rotSquare, float fCircleRadius, D3DXVECTOR2 SquareSize)
		{
			float fSquareSizeX = SquareSize.x;	// ��`�̃T�C�YX
			float fSquareSizeY = SquareSize.y;	// ��`�̃T�C�YY

			float fLength = 0.0f;	// �Ίp���̒���
			float fAngle = 0.0f;	// �Ίp���̌���

			MyLib::Vector3 LeftUp, RightUp, LeftDown, RightDown;

			//****************************************************
			// X�̃T�C�Y�ɉ~�̔��a�𑫂�����`�̔���
			//****************************************************
			fSquareSizeX += fCircleRadius;	// ��`�̃T�C�YX
			fSquareSizeY = SquareSize.y;	// ��`�̃T�C�YY

			fLength = sqrtf(fSquareSizeX * fSquareSizeX + fSquareSizeY * fSquareSizeY);	// �Ίp���̒���
			fAngle = atan2f(fSquareSizeX, fSquareSizeY);								// �Ίp���̌���

			// ���肷��l�p��4���_
			LeftUp.x = posSquare.x + sinf(rotSquare.z - D3DX_PI + fAngle) * fLength;
			LeftUp.y = posSquare.y + cosf(rotSquare.z - D3DX_PI + fAngle) * fLength;
			LeftUp.z = 0.0f;

			RightUp.x = posSquare.x + sinf(rotSquare.z + D3DX_PI - fAngle) * fLength;
			RightUp.y = posSquare.y + cosf(rotSquare.z + D3DX_PI - fAngle) * fLength;
			RightUp.z = 0.0f;

			LeftDown.x = posSquare.x + sinf(rotSquare.z - fAngle) * fLength;
			LeftDown.y = posSquare.y + cosf(rotSquare.z - fAngle) * fLength;
			LeftDown.z = 0.0f;

			RightDown.x = posSquare.x + sinf(rotSquare.z + fAngle) * fLength;
			RightDown.y = posSquare.y + cosf(rotSquare.z + fAngle) * fLength;
			RightDown.z = 0.0f;

			// ��`�̔���
			if (UtilFunc::Collision::CollisionSquare2D(LeftUp, RightUp, LeftDown, RightDown, posCircle))
			{// ��`�ɓ��������ꍇ
				return true;
			}

			//****************************************************
			// Y�̃T�C�Y�ɉ~�̔��a�𑫂�����`�̔���
			//****************************************************
			fSquareSizeX = SquareSize.x;	// ��`�̃T�C�YX
			fSquareSizeY += fCircleRadius;	// ��`�̃T�C�YY

			fLength = sqrtf(fSquareSizeX * fSquareSizeX + fSquareSizeY * fSquareSizeY);	// �Ίp���̒���
			fAngle = atan2f(fSquareSizeX, fSquareSizeY);								// �Ίp���̌���

			// ���肷��l�p��4���_
			LeftUp.x = posSquare.x + sinf(rotSquare.z - D3DX_PI + fAngle) * fLength;
			LeftUp.y = posSquare.y + cosf(rotSquare.z - D3DX_PI + fAngle) * fLength;
			LeftUp.z = 0.0f;

			RightUp.x = posSquare.x + sinf(rotSquare.z + D3DX_PI - fAngle) * fLength;
			RightUp.y = posSquare.y + cosf(rotSquare.z + D3DX_PI - fAngle) * fLength;
			RightUp.z = 0.0f;

			LeftDown.x = posSquare.x + sinf(rotSquare.z - fAngle) * fLength;
			LeftDown.y = posSquare.y + cosf(rotSquare.z - fAngle) * fLength;
			LeftDown.z = 0.0f;

			RightDown.x = posSquare.x + sinf(rotSquare.z + fAngle) * fLength;
			RightDown.y = posSquare.y + cosf(rotSquare.z + fAngle) * fLength;
			RightDown.z = 0.0f;

			// ��`�̔���
			if (UtilFunc::Collision::CollisionSquare2D(LeftUp, RightUp, LeftDown, RightDown, posCircle))
			{// ��`�ɓ��������ꍇ
				return true;
			}

			//***********************
			// �e���_���̉~�̔���
			//***********************
			//****************************************************
			// ���̃T�C�Y
			//****************************************************
			fSquareSizeX = SquareSize.x;	// ��`�̃T�C�YX
			fSquareSizeY = SquareSize.y;	// ��`�̃T�C�YY

			fLength = sqrtf(fSquareSizeX * fSquareSizeX + fSquareSizeY * fSquareSizeY);	// �Ίp���̒���
			fAngle = atan2f(fSquareSizeX, fSquareSizeY);								// �Ίp���̌���

			// ���肷��l�p��4���_
			LeftUp.x = posSquare.x + sinf(rotSquare.z - D3DX_PI + fAngle) * fLength;
			LeftUp.y = posSquare.y + cosf(rotSquare.z - D3DX_PI + fAngle) * fLength;
			LeftUp.z = 0.0f;

			RightUp.x = posSquare.x + sinf(rotSquare.z + D3DX_PI - fAngle) * fLength;
			RightUp.y = posSquare.y + cosf(rotSquare.z + D3DX_PI - fAngle) * fLength;
			RightUp.z = 0.0f;

			LeftDown.x = posSquare.x + sinf(rotSquare.z - fAngle) * fLength;
			LeftDown.y = posSquare.y + cosf(rotSquare.z - fAngle) * fLength;
			LeftDown.z = 0.0f;

			RightDown.x = posSquare.x + sinf(rotSquare.z + fAngle) * fLength;
			RightDown.y = posSquare.y + cosf(rotSquare.z + fAngle) * fLength;
			RightDown.z = 0.0f;

			if (UtilFunc::Collision::CircleRange2D(posCircle, LeftUp, fCircleRadius, 0.0f))
			{// ����
				return true;
			}
			if (UtilFunc::Collision::CircleRange2D(posCircle, RightUp, fCircleRadius, 0.0f))
			{// �E��
				return true;
			}
			if (UtilFunc::Collision::CircleRange2D(posCircle, LeftDown, fCircleRadius, 0.0f))
			{// ����
				return true;
			}
			if (UtilFunc::Collision::CircleRange2D(posCircle, RightDown, fCircleRadius, 0.0f))
			{// �E��
				return true;
			}

			// �������ĂȂ������Ԃ�
			return false;
		}

		/**
		@brief	�~�Ƌ�`�̓����蔻��(3D)
		@param	posCircle		[inout]	�~�̒��S���W
		@param	posSquare		[inout]	��`�̒��S���W
		@param	rotSquare		[in]	��`�̌���
		@param	fCircleRadius	[in]	�~�̔��a
		@param	SquareSize		[in]	��`�̃T�C�Y
		@return	�Փ˂�������bool�l
		*/
		inline bool CollisionCircleSquare3D(MyLib::Vector3& posCircle, MyLib::Vector3& posSquare, MyLib::Vector3 rotSquare, float fCircleRadius, D3DXVECTOR2 SquareSize)
		{
			float fSquareSizeX = SquareSize.x;	// ��`�̃T�C�YX
			float fSquareSizeY = SquareSize.y;	// ��`�̃T�C�YY

			float fLength = 0.0f;	// �Ίp���̒���
			float fAngle = 0.0f;	// �Ίp���̌���

			MyLib::Vector3 LeftUp, RightUp, LeftDown, RightDown;

			//****************************************************
			// X�̃T�C�Y�ɉ~�̔��a�𑫂�����`�̔���
			//****************************************************
			fSquareSizeX += fCircleRadius;	// ��`�̃T�C�YX
			fSquareSizeY = SquareSize.y;	// ��`�̃T�C�YY

			fLength = sqrtf(fSquareSizeX * fSquareSizeX + fSquareSizeY * fSquareSizeY);	// �Ίp���̒���
			fAngle = atan2f(fSquareSizeX, fSquareSizeY);								// �Ίp���̌���

			// ���肷��l�p��4���_
			LeftUp = MyLib::Vector3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y - fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y - fAngle) * fLength);
			RightUp = MyLib::Vector3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y + fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y + fAngle) * fLength);
			LeftDown = MyLib::Vector3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y - D3DX_PI + fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y - D3DX_PI + fAngle) * fLength);
			RightDown = MyLib::Vector3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y + D3DX_PI - fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y + D3DX_PI - fAngle) * fLength);

			// ��`�̔���
			if (UtilFunc::Collision::CollisionSquare(LeftUp, RightUp, LeftDown, RightDown, posCircle))
			{// ��`�ɓ��������ꍇ
				return true;
			}

			//****************************************************
			// Y�̃T�C�Y�ɉ~�̔��a�𑫂�����`�̔���
			//****************************************************
			fSquareSizeX = SquareSize.x;	// ��`�̃T�C�YX
			fSquareSizeY += fCircleRadius;	// ��`�̃T�C�YY

			fLength = sqrtf(fSquareSizeX * fSquareSizeX + fSquareSizeY * fSquareSizeY);	// �Ίp���̒���
			fAngle = atan2f(fSquareSizeX, fSquareSizeY);								// �Ίp���̌���

			// ���肷��l�p��4���_
			LeftUp = MyLib::Vector3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y - fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y - fAngle) * fLength);
			RightUp = MyLib::Vector3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y + fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y + fAngle) * fLength);
			LeftDown = MyLib::Vector3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y - D3DX_PI + fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y - D3DX_PI + fAngle) * fLength);
			RightDown = MyLib::Vector3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y + D3DX_PI - fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y + D3DX_PI - fAngle) * fLength);

			// ��`�̔���
			if (UtilFunc::Collision::CollisionSquare(LeftUp, RightUp, LeftDown, RightDown, posCircle))
			{// ��`�ɓ��������ꍇ
				return true;
			}

			//***********************
			// �e���_���̉~�̔���
			//***********************
			if (UtilFunc::Collision::CircleRange3D(posCircle, LeftUp, fCircleRadius, fCircleRadius))
			{// ����
				return true;
			}
			if (UtilFunc::Collision::CircleRange3D(posCircle, RightUp, fCircleRadius, fCircleRadius))
			{// �E��
				return true;
			}
			if (UtilFunc::Collision::CircleRange3D(posCircle, LeftDown, fCircleRadius, fCircleRadius))
			{// ����
				return true;
			}
			if (UtilFunc::Collision::CircleRange3D(posCircle, RightDown, fCircleRadius, fCircleRadius))
			{// �E��
				return true;
			}

			// �������ĂȂ������Ԃ�
			return false;
		}

		/**
		@brief	��̓����蔻��(3D)
		@param	posCenter		[in]	��̎n�_���_���W
		@param	posLeft			[in]	��̍������_���W
		@param	posRight		[in]	��̉E�����_���W
		@param	TargetPos		[in]	���肷��Ώۂ̈ʒu
		@param	fTargetRadius	[in]	���肷��Ώۂ̔��a
		@return	�Փ˂�������bool�l
		*/
		inline bool CollisionFan3D(MyLib::Vector3 posCenter, MyLib::Vector3 posLeft, MyLib::Vector3 posRight, MyLib::Vector3 TargetPos, float fTargetRadius)
		{
			// �����������̔���
			bool bHit = false;
			bool bLine1 = false, bLine2 = false, bCircle = false;

			// ���̓_�Ƃ̐���
			bLine1 = UtilFunc::Collision::CollisionLine3D(posLeft, posCenter, TargetPos, TargetPos);

			// �E�̓_�Ƃ̐���
			bLine2 = UtilFunc::Collision::CollisionLine3D(posCenter, posRight, TargetPos, TargetPos);

			// ��̔��a
			float fLen =
				sqrtf((posLeft.x - posCenter.x) * (posLeft.x - posCenter.x)
					+ (posLeft.z - posCenter.z) * (posLeft.z - posCenter.z));

			// �~�̓����蔻��
			if (UtilFunc::Collision::CircleRange3D(posCenter, TargetPos, fLen, fTargetRadius))
			{// �������Ă�����
				bCircle = true;
			}

			// �S�Ă͈̔͂ɓ����Ă�����
			if (bLine1 && bLine2 && bCircle)
			{
				bHit = true;
			}

			return bHit;
		}

		/**
		@brief	�����͈͓̔�����
		@param	angle		[in]	���肷�����
		@param	maxAngle	[in]	������̍ő����
		@param	minAngle	[in]	������̍ŏ�����
		@return	�Փ˂�������bool�l
		*/
		inline bool CollisionRangeAngle(float angle, float maxAngle, float minAngle)
		{
			return (angle <= maxAngle && angle >= minAngle);
		}

		/**
		@brief	�_���O�p�`�̓����ɂ��邩�𔻒肷��w���p�[�֐�
		@param	p	[in]	���肷��_
		@param	a	[in]	�O�p�`�̒��_1
		@param	b	[in]	�O�p�`�̒��_2
		@param	c	[in]	�O�p�`�̒��_3
		@return	true�Ȃ�����Afalse�Ȃ�O��
		*/
		inline bool PointInTriangle(const MyLib::Vector3& p, const MyLib::Vector3& a, const MyLib::Vector3& b, const MyLib::Vector3& c) 
		{
			MyLib::Vector3 v0 = c - a;
			MyLib::Vector3 v1 = b - a;
			MyLib::Vector3 v2 = p - a;

			// �o���Z���g���b�N���W���g�����O�p�`������
			float dot00 = D3DXVec3Dot(&v0, &v0);
			float dot01 = D3DXVec3Dot(&v0, &v1);
			float dot02 = D3DXVec3Dot(&v0, &v2);
			float dot11 = D3DXVec3Dot(&v1, &v1);
			float dot12 = D3DXVec3Dot(&v1, &v2);

			float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
			float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
			float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

			// u >= 0, v >= 0, u + v <= 1 �Ȃ�O�p�`�̓���
			return (u >= 0) && (v >= 0) && (u + v <= 1);
		}

		/**
		@brief	���E���̔���(3D)
		@param	posMain			[in]	���S�ƂȂ�l�̈ʒu
		@param	posTarget		[in]	�Ώۂ̈ʒu
		@param	mainRotY		[in]	����
		@param	fieldofview		[in]	����p
		@return	�Փ˂�������bool�l
		*/
		inline bool CollisionViewRange3D(const MyLib::Vector3& posMain, MyLib::Vector3& posTarget, float mainRotY, float fieldofview)
		{

			// 2�L�����̃x�N�g��
			D3DXVECTOR3 toPlayer = posTarget - posMain;

			// �O���x�N�g��
			D3DXVECTOR3 forward = UtilFunc::Transformation::RotationChangeToForwardVector(mainRotY);

			// �x�N�g���ƑO���x�N�g���̊p�x�v�Z
			float angle = D3DXVec3Dot(&toPlayer, &forward) / (D3DXVec3Length(&toPlayer) * D3DXVec3Length(&forward));
			angle = acos(angle);

			// ���E�̒��ɂ��邩����
			fieldofview = D3DXToRadian(fieldofview);
			if (angle <= fieldofview / 2.0f)
			{
				return true;
			}

			return false;
		}

		/**
		@brief	���C�Ƌ��̏Փ˔���(3D)
		@param	rayPos				[in]	���C�̎n�_
		@param	rayVec				[in]	���C�̕����x�N�g��
		@param	spherePos			[in]	���̒��S�_
		@param	sphereRadius		[in]	���̔��a
		@return	�Փ˔���
		*/
		inline bool CollisionRaySphere(const MyLib::Vector3& rayPos, const MyLib::Vector3& rayVec,
			const MyLib::Vector3& spherePos, float sphereRadius)
		{
			// �ϊ���̋����W
			MyLib::Vector3 sphereCoord;
			sphereCoord.x = rayPos.x - spherePos.x;
			sphereCoord.y = rayPos.y - spherePos.y;
			sphereCoord.z = rayPos.z - spherePos.z;

			float A = rayVec.Dot(rayVec);
			float B = sphereCoord.Dot(rayVec) * 2.0f;
			float C = sphereCoord.Dot(rayVec) - (sphereRadius * sphereRadius);

			if (A == 0.0f)
				return false; // ���C�̒�����0

			float s = B * B - 4 * A * C;
			if (s < 0.0f)
				return false; // �Փ˂��Ă��Ȃ�

			s = sqrtf(s);
			float a1 = (B - s) / A;
			float a2 = (B + s) / A;

			if (a1 < 0.0f || a2 < 0.0f)
				return false; // ���C�̔��΂ŏՓ�

			return true;
		}

		/**
		@brief	���C�Ƌ��̏Փ˔���(3D)
		@param	rayPos				[in]	���C�̎n�_
		@param	rayVec				[in]	���C�̕����x�N�g��
		@param	spherePos			[in]	���̒��S�_
		@param	sphereRadius		[in]	���̔��a
		@param	collisionStartPoint	[in]	�ՓˊJ�n�_
		@param	collisionEndPoint	[in]	�ՓˏI���_
		@return	�Փ˔���
		*/
		inline bool CollisionRaySphere(const MyLib::Vector3& rayPos, const MyLib::Vector3& rayVec, const MyLib::Vector3& spherePos,
			float sphereRadius, MyLib::Vector3& collisionStartPoint, MyLib::Vector3& collisionEndPoint) 
		{
			// �ϊ���̋����W
			MyLib::Vector3 sphereCoord;
			sphereCoord.x = spherePos.x - rayPos.x;
			sphereCoord.y = spherePos.y - rayPos.y;
			sphereCoord.z = spherePos.z - rayPos.z;

			float A = rayVec.x * rayVec.x + rayVec.y * rayVec.y + rayVec.z * rayVec.z;
			float B = rayVec.x * sphereCoord.x + rayVec.y * sphereCoord.y + rayVec.z * sphereCoord.z;
			float C = sphereCoord.x * sphereCoord.x + sphereCoord.y * sphereCoord.y + sphereCoord.z * sphereCoord.z - sphereRadius * sphereRadius;

			if (A == 0.0f)
				return false; // ���C�̒�����0

			float s = B * B - A * C;
			if (s < 0.0f)
				return false; // �Փ˂��Ă��Ȃ�

			s = sqrtf(s);
			float a1 = (B - s) / A;
			float a2 = (B + s) / A;

			if (a1 < 0.0f || a2 < 0.0f)
				return false; // ���C�̔��΂ŏՓ�

			// �ՓˊJ�n�E�I���n�_����o��
			collisionStartPoint.x = rayPos.x + a1 * rayVec.x;
			collisionStartPoint.y = rayPos.y + a1 * rayVec.y;
			collisionStartPoint.z = rayPos.z + a1 * rayVec.z;
			collisionEndPoint.x = rayPos.x + a2 * rayVec.x;
			collisionEndPoint.y = rayPos.y + a2 * rayVec.y;
			collisionEndPoint.z = rayPos.z + a2 * rayVec.z;

			return true;
		}

		/**
		@brief	���C�ƒ����̂̏Փ˔���(3D)
		@param	pos		[in]	���C�̎n�_
		@param	dir_w	[in]	���C�̕����x�N�g��
		@param	aabb	[in]	���E�{�b�N�X�i���[�J���j
		@param	mat		[in]	���E�{�b�N�X�̃��[���h�ϊ��s��
		@param	t		[in]	�ՓˊԊu�i�o�́j
		@return		�Փ˔���
		*/
		inline bool CollisionRayAABB(MyLib::Vector3* pos, MyLib::Vector3* dir_w, MyLib::AABB* aabb, MyLib::Matrix* mtx, float& t, MyLib::Vector3* colPos = 0)
		{

			// ���C�����E�{�b�N�X�̈ʒu�ֈړ�
			D3DXMATRIX invMat, trans = mtx->ConvertD3DXMATRIX();
			D3DXMatrixInverse(&invMat, 0, &trans);

			MyLib::Vector3 p_l, dir_l;
			D3DXVec3TransformCoord(&p_l, pos, &invMat);
			invMat._41 = 0.0f;
			invMat._42 = 0.0f;
			invMat._43 = 0.0f;
			D3DXVec3TransformCoord(&dir_l, dir_w, &invMat);

			// ��������
			float p[3], d[3], min[3], max[3];
			memcpy(p, &p_l, sizeof(MyLib::Vector3));
			memcpy(d, &dir_l, sizeof(MyLib::Vector3));
			memcpy(min, &aabb->vtxMin, sizeof(MyLib::Vector3));
			memcpy(max, &aabb->vtxMax, sizeof(MyLib::Vector3));

			t = -FLT_MAX;
			float t_max = FLT_MAX;

			for (int i = 0; i < 3; ++i) 
			{
				if (abs(d[i]) < FLT_EPSILON) 
				{
					if (p[i] < min[i] || p[i] > max[i])
						return false;	// �������Ă��Ȃ�
				}
				else 
				{
					// �X���u�Ƃ̋������Z�o
					// t1���߃X���u�At2�����X���u�Ƃ̋���
					float odd = 1.0f / d[i];
					float t1 = (min[i] - p[i]) * odd;
					float t2 = (max[i] - p[i]) * odd;

					if (t1 > t2) {
						float tmp = t1; t1 = t2; t2 = tmp;
					}

					if (t1 > t) t = t1;
					if (t2 < t_max) t_max = t2;

					// �X���u�����`�F�b�N
					if (t >= t_max)
						return false;
				}
			}

			// �������Ă���
			if (colPos) {
				*colPos = *pos + t * (*dir_w);
			}

			return true;
		}


		/**
		@brief	���C��4�_�������镽�ʂ̏Փ˔���(3D)
		@param	pos		[in]	���C�̎n�_
		@param	dir_w	[in]	���C�̕����x�N�g��
		@param	v0		[in]	�l�p�`��1�_�ځi�����v���Œ�`�j
		@param	v1		[in]	�l�p�`��2�_��
		@param	v2		[in]	�l�p�`��3�_��
		@param	v3		[in]	�l�p�`��4�_��
		@param	t		[out]	�Փ˂܂ł̋����i���C�̒����ɔ��j
		@param	colPos	[out]	�Փ˓_���W
		@param	colPos	[out]	�Փ˓_���W
		@return	�Փ˔���
		*/
		inline bool CollisionRayQuad(MyLib::Vector3* pos, MyLib::Vector3* dir_w, MyLib::Vector3* v0, MyLib::Vector3* v1, MyLib::Vector3* v2, MyLib::Vector3* v3, float& t, MyLib::Vector3* colPos = 0) 
		{
			// �l�p�`�̖ʂ��\������3�̒��_����@�������߂�i�����v��菇�j
			MyLib::Vector3 edge1 = *v1 - *v0;
			MyLib::Vector3 edge2 = *v2 - *v0;
			MyLib::Vector3 normal;
			D3DXVec3Cross(&normal, &edge1, &edge2); // �O�ςŖ@�������߂�
			D3DXVec3Normalize(&normal, &normal);    // ���K��

			// ���ʕ������̌W�������߂�
			float d = -D3DXVec3Dot(&normal, v0);

			// ���C�̕����x�N�g���Ɩ@���̓��ς����߂�
			float dotDirNormal = D3DXVec3Dot(dir_w, &normal);
			if (abs(dotDirNormal) < FLT_EPSILON) 
			{
				// ���C�����ʂƕ��s�ȏꍇ�A�Փ˂��Ȃ�
				return false;
			}

			// ���C�ƕ��ʂ̌�_�����߂�
			float dotPosNormal = D3DXVec3Dot(pos, &normal);
			t = -(dotPosNormal + d) / dotDirNormal;

			if (t < 0.0f) 
			{
				// ���C�����ʂ̌��ɂ���ꍇ�A�Փ˂��Ȃ�
				return false;
			}

			// ��_���v�Z
			MyLib::Vector3 intersection = *pos + t * (*dir_w);

			// ��_���l�p�`�̓����ɂ��邩���`�F�b�N
			// �l�p�`��2�̎O�p�`�ɕ������A���ꂼ��̎O�p�`�̓����ɂ��邩�𔻒�

			bool bLand1 = false, bLand2 = false;

			float fHeight1 = UtilFunc::Calculation::GetVtxHeight(intersection, *v0, *v1, *v2, &bLand1);
			float fHeight2 = UtilFunc::Calculation::GetVtxHeight(intersection, *v2, *v3, *v0, &bLand2);

			if (bLand1 || bLand2)
			{
				*colPos = intersection;

				if (bLand1)
				{
					colPos->y = fHeight1;
				}

				if (bLand2)
				{
					colPos->y = fHeight2;
				}

				return true;
			}

			return false;
		}

		/**
		@brief	�~��AABB�̓����蔻��
		@param	circleCenter		[in]	�~�̒��S�ʒu
		@param	radius				[in]	���a
		@param	aabb				[in]	���E�{�b�N�X�i���[�J���j
		@param	crossPosition		[inout]	��_
		@return	void
		*/
		inline bool CollisionCircleToAABB(const MyLib::Vector3& circleCenter, const float radius, MyLib::AABB aabb, MyLib::Vector3& crossPosition)
		{
			// AABB�̒��S
			MyLib::Vector3 aabbCenter = {
				(aabb.vtxMin.x + aabb.vtxMax.x) * 0.5f,
				(aabb.vtxMin.y + aabb.vtxMax.y) * 0.5f,
				(aabb.vtxMin.z + aabb.vtxMax.z) * 0.5f
			};

			// AABB�̔���
			MyLib::Vector3 aabbHalfExtents = {
				(aabb.vtxMax.x - aabb.vtxMin.x) * 0.5f,
				(aabb.vtxMax.y - aabb.vtxMin.y) * 0.5f,
				(aabb.vtxMax.z - aabb.vtxMin.z) * 0.5f
			};

			// AABB�Ɖ~�̒��S�_�̋����̐�Βl
			MyLib::Vector3 difference = {
				std::abs(circleCenter.x - aabbCenter.x),
				std::abs(circleCenter.y - aabbCenter.y),
				std::abs(circleCenter.z - aabbCenter.z)
			};

			UtilFunc::Transformation::ValueNormalize(difference.x, aabbHalfExtents.x, -aabbHalfExtents.x);
			UtilFunc::Transformation::ValueNormalize(difference.y, aabbHalfExtents.y, -aabbHalfExtents.y);
			UtilFunc::Transformation::ValueNormalize(difference.z, aabbHalfExtents.z, -aabbHalfExtents.z);

			MyLib::Vector3 clamped = difference;
				
			MyLib::Vector3 closestPoint = {
				aabbCenter.x + clamped.x,
				aabbCenter.y + clamped.y,
				aabbCenter.z + clamped.z
			};
			MyLib::Vector3 direction = {
				circleCenter.x - closestPoint.x,
				circleCenter.y - closestPoint.y,
				circleCenter.z - closestPoint.z
			};
			float distance = direction.Length();

			// �~��AABB�̒��ɂ���ꍇ
			if (distance < radius)
			{
				crossPosition = {
					direction.x * (radius - distance),
					direction.y * (radius - distance),
					direction.z * (radius - distance)
				};
				return true;
			}

			return false;
		}

		/**
		@brief	�~��AABB�̓����蔻��
		@param	circleCenter	[in]	�~�̒��S�ʒu
		@param	radius			[in]	���a
		@param	aabb			[in]	���E�{�b�N�X�i���[�J���j
		@param	aabbMatrix		[in]	AABB�̃��[���h�}�g���b�N�X
		@return	void
		*/
		inline bool CollisionCircleToAABB(MyLib::Vector3& circleCenter, const float radius, MyLib::AABB aabb, MyLib::Matrix aabbMatrix)
		{
			// AABB�̒��S
			MyLib::Vector3 aabbCenter = {
				(aabb.vtxMin.x + aabb.vtxMax.x) * 0.5f,
				(aabb.vtxMin.y + aabb.vtxMax.y) * 0.5f,
				(aabb.vtxMin.z + aabb.vtxMax.z) * 0.5f
			};
			aabbCenter += aabbMatrix.GetWorldPosition();

			// AABB�̃X�P�[�����O���l�����Ē��S���v�Z
			aabbCenter.x = aabbMatrix.GetWorldPosition().x + aabbCenter.x * aabbMatrix.GetWorldPosition().x;
			aabbCenter.y = aabbMatrix.GetWorldPosition().y + aabbCenter.y * aabbMatrix.GetWorldPosition().y;
			aabbCenter.z = aabbMatrix.GetWorldPosition().z + aabbCenter.z * aabbMatrix.GetWorldPosition().z;

			// AABB�̔���
			MyLib::Vector3 aabbHalfExtents = {
				(aabb.vtxMax.x - aabb.vtxMin.x) * 0.5f,
				(aabb.vtxMax.y - aabb.vtxMin.y) * 0.5f,
				(aabb.vtxMax.z - aabb.vtxMin.z) * 0.5f
			};

			// AABB�̃X�P�[�����O���l�����Ĕ������v�Z
			D3DXVECTOR3 scale = aabbMatrix.GetScale();
			aabbHalfExtents.x *= scale.x;
			aabbHalfExtents.y *= scale.y;
			aabbHalfExtents.z *= scale.z;

			// AABB�̃��[�J����Ԃɉ~�̈ʒu��ϊ�
			MyLib::Vector3 localCenter, diffCenter = { circleCenter.x - aabbCenter.x, circleCenter.y - aabbCenter.y, circleCenter.z - aabbCenter.z };
			localCenter = aabbMatrix.Coord(diffCenter);

			// AABB�̃��[�J����ԓ��ł̍ł��߂��_�̍��W���v�Z
			float closestPointX = UtilFunc::Transformation::Clamp(localCenter.x, -aabbHalfExtents.x, aabbHalfExtents.x);
			float closestPointY = UtilFunc::Transformation::Clamp(localCenter.y, -aabbHalfExtents.y, aabbHalfExtents.y);
			float closestPointZ = UtilFunc::Transformation::Clamp(localCenter.z, -aabbHalfExtents.z, aabbHalfExtents.z);

			// �ł��߂��_�Ɖ~�̒��S�_�̋�����2��
			float distanceSquared = 
				powf(closestPointX - localCenter.x, 2) +
				powf(closestPointY - localCenter.y, 2) +
				powf(closestPointZ - localCenter.z, 2);

			// ���ς��v�Z
			float dotProduct = 
				(closestPointX - localCenter.x) * (aabb.vtxMax.x - aabb.vtxMin.x) +
				(closestPointY - localCenter.y) * (aabb.vtxMax.y - aabb.vtxMin.y) +
				(closestPointZ - localCenter.z) * (aabb.vtxMax.z - aabb.vtxMin.z);


			// �~��AABB�̏Փ˔��肪���������ꍇ�̏���
			if (distanceSquared <= radius * radius)
			{
				// �ł��߂��_�������߂��x�N�g���Ƃ��Ďg�p����
				MyLib::Vector3 pushBackVector = { 0.0f, 0.0f, 0.0f };

				// �ł��߂��_���~�̊O���ɂ���ꍇ�A�����߂��x�N�g���͉~�̒��S����ł��߂��_�֌������x�N�g���ɂȂ�
				if (distanceSquared > 0.0f)
				{
					// �ł��߂��_�������߂��x�N�g���Ƃ��Ďg�p����
					MyLib::Vector3 pushBackVector = { 0.0f, 0.0f, 0.0f };

					// �ł��߂��_��AABB�̕ӏ�ɂ���ꍇ�͉����߂����s��Ȃ�
					if (closestPointX != localCenter.x)
					{
						float distance = sqrt(distanceSquared);
						pushBackVector.x = (localCenter.x - closestPointX) * (radius / distance);
					}
					if (closestPointY != localCenter.y)
					{
						float distance = sqrt(distanceSquared);
						pushBackVector.y = (localCenter.y - closestPointY) * (radius / distance);
					}
					if (closestPointZ != localCenter.z)
					{
						float distance = sqrt(distanceSquared);
						pushBackVector.z = (localCenter.z - closestPointZ) * (radius / distance);
					}

					// �����߂��x�N�g����AABB�̃��[�J����Ԃ��猳�̍��W�n�ɕϊ�
					D3DXMATRIX inverseAABBMatrix, invAABBMTX = aabbMatrix.ConvertD3DXMATRIX();
					D3DXMatrixInverse(&inverseAABBMatrix, nullptr, &invAABBMTX);
					D3DXVec3TransformCoord(&pushBackVector, &pushBackVector, &inverseAABBMatrix);

					// �~�̒��S�������߂�
					circleCenter.x += pushBackVector.x;
					circleCenter.y += pushBackVector.y;
					circleCenter.z += pushBackVector.z;
				}

				return true;
			}

			return false;
		}


		// ��]����AABB�ƃ{�b�N�X�̓����蔻����s���֐�
		inline bool checkCollision(MyLib::AABB aabb, const MyLib::Vector3& boxCenter, const MyLib::Vector3& boxSize, const D3DXMATRIX& rotationMatrix)
		{
			// AABB�̒��S
			MyLib::Vector3 aabbCenter = {
				(aabb.vtxMin.x + aabb.vtxMax.x) * 0.5f,
				(aabb.vtxMin.y + aabb.vtxMax.y) * 0.5f,
				(aabb.vtxMin.z + aabb.vtxMax.z) * 0.5f
			};

			// AABB�̔���
			MyLib::Vector3 aabbSize = {
				(aabb.vtxMax.x - aabb.vtxMin.x) * 0.5f,
				(aabb.vtxMax.y - aabb.vtxMin.y) * 0.5f,
				(aabb.vtxMax.z - aabb.vtxMin.z) * 0.5f
			};

			// ��]�O��AABB�̍ŏ�����эő���W���v�Z
			MyLib::Vector3 aabbMin = aabbCenter - aabbSize;
			MyLib::Vector3 aabbMax = aabbCenter + aabbSize;

			// �{�b�N�X�̍ŏ�����эő���W���v�Z
			MyLib::Vector3 boxMin = boxCenter - boxSize;
			MyLib::Vector3 boxMax = boxCenter + boxSize;

			// ��]����AABB�̍ŏ�����эő���W���v�Z
			MyLib::Vector3 rotatedAABBMin, rotatedAABBMax;
			D3DXVec3TransformCoord(&rotatedAABBMin, &aabbMin, &rotationMatrix);
			D3DXVec3TransformCoord(&rotatedAABBMax, &aabbMax, &rotationMatrix);

			CEffect3D::Create(
				rotatedAABBMin,
				MyLib::Vector3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
				40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
			CEffect3D::Create(
				rotatedAABBMax,
				MyLib::Vector3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
				40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

			// AABB�̋��E�{�b�N�X�ƃ{�b�N�X�̋��E�{�b�N�X���ꕔ�ł��d�Ȃ��Ă��邩�ǂ������m�F
			bool collisionX = rotatedAABBMax.x >= boxMin.x && rotatedAABBMin.x <= boxMax.x;
			bool collisionY = rotatedAABBMax.y >= boxMin.y && rotatedAABBMin.y <= boxMax.y;
			bool collisionZ = rotatedAABBMax.z >= boxMin.z && rotatedAABBMin.z <= boxMax.z;

			return collisionX && collisionY && collisionZ;
		}


		/**
		@brief	���ɑ΂���ˉe�擾
		@param	vertices		[in]	���_�z��ւ̃|�C���^
		@param	numVertices		[in]	���_��
		@param	axis			[in]	��
		@return	D3DXVECTOR2 �ˉe
		*/
		inline D3DXVECTOR2 ProjectOntoAxis(const MyLib::Vector3* vertices, int numVertices, const MyLib::Vector3& axis)
		{
			float minProjection = D3DXVec3Dot(&vertices[0], &axis);
			float maxProjection = minProjection;

			for (int i = 1; i < numVertices; ++i) 
			{
				float projection = D3DXVec3Dot(&vertices[i], &axis);
				minProjection = min(minProjection, projection);
				maxProjection = max(maxProjection, projection);
			}

			return D3DXVECTOR2(minProjection, maxProjection);
		}

		/**
		@brief	�ˉe�̌����`�F�b�N
		@param	projection1		[in]	�ˉe1
		@param	projection2		[in]	�ˉe2
		@return	bool ��������
		*/
		inline bool Intersect(const D3DXVECTOR2& projection1, const D3DXVECTOR2& projection2) 
		{
			return !(projection1.y < projection2.x || projection1.x > projection2.y);
		}

		/**
		@brief	AABB�ƃ{�b�N�X�̓����蔻��
		@param	aabb		[in]	���C����AABB���
		@param	aabbMtx		[in]	���C���̃}�g���b�N�X
		@param	boxAABB		[in]	�{�b�N�X��AABB���
		@param	boxMtx		[in]	�{�b�N�X�̃}�g���b�N�X
		@return	bool �Փˌ���
		*/
		inline bool IsAABBCollidingWithBox(const MyLib::AABB& aabb, MyLib::Matrix aabbMtx, const MyLib::AABB& boxAABB, MyLib::Matrix boxMtx)
		{
			//=============================
			// AABB���
			//=============================
			// AABB�̒��S
			MyLib::Vector3 aabbCenter = {
				(aabb.vtxMin.x + aabb.vtxMax.x) * 0.5f,
				(aabb.vtxMin.y + aabb.vtxMax.y) * 0.5f,
				(aabb.vtxMin.z + aabb.vtxMax.z) * 0.5f
			};

			// AABB�̃T�C�Y
			MyLib::Vector3 aabbExtents = {
				(aabb.vtxMax.x - aabb.vtxMin.x) * 0.5f,
				(aabb.vtxMax.y - aabb.vtxMin.y) * 0.5f,
				(aabb.vtxMax.z - aabb.vtxMin.z) * 0.5f
			};

			//=============================
			// �{�b�N�X���
			//=============================
			// �{�b�N�X�̒��S
			MyLib::Vector3 boxCenter = {
				(boxAABB.vtxMin.x + boxAABB.vtxMax.x) * 0.5f,
				(boxAABB.vtxMin.y + boxAABB.vtxMax.y) * 0.5f,
				(boxAABB.vtxMin.z + boxAABB.vtxMax.z) * 0.5f
			};

			// �{�b�N�X�̃T�C�Y
			MyLib::Vector3 boxExtents = {
				(boxAABB.vtxMax.x - boxAABB.vtxMin.x) * 0.5f,
				(boxAABB.vtxMax.y - boxAABB.vtxMin.y) * 0.5f,
				(boxAABB.vtxMax.z - boxAABB.vtxMin.z) * 0.5f
			};


			//=============================
			// ���_�ϊ�
			//=============================
			// AABB��8���_�����[���h���W�ɂ���
			MyLib::Vector3 aabbVertices[8];
			for (int i = 0; i < 8; ++i) 
			{
				// �����̓}�C�i�X�A��̓v���X����
				MyLib::Vector3 corner(
					(i & 1) ? aabbExtents.x : -aabbExtents.x,
					(i & 2) ? aabbExtents.y : -aabbExtents.y,
					(i & 4) ? aabbExtents.z : -aabbExtents.z);

				// �}�g���b�N�X�����ɒ��_�ϊ�
				aabbVertices[i] = aabbMtx.Coord(corner);
			}

			// �{�b�N�X��8���_�����[���h���W�ɂ���
			MyLib::Vector3 boxVertices[8];
			for (int i = 0; i < 8; ++i) 
			{
				// �����̓}�C�i�X�A��̓v���X����
				MyLib::Vector3 corner(
					(i & 1) ? boxExtents.x : -boxExtents.x,
					(i & 2) ? boxExtents.y : -boxExtents.y,
					(i & 4) ? boxExtents.z : -boxExtents.z);

				// �}�g���b�N�X�����ɒ��_�ϊ�
				boxVertices[i] = boxMtx.Coord(corner);
			}

			//=============================
			// �������藝
			//=============================
			// AABB���̔���
			for (int i = 0; i < 3; ++i)
			{
				MyLib::Vector3 axis(aabbMtx.m[i][0], aabbMtx.m[i][1], aabbMtx.m[i][2]);

				// AABB�̎ˉe
				D3DXVECTOR2 aabbProjection = ProjectOntoAxis(aabbVertices, 8, axis);

				// �{�b�N�X�̎ˉe
				D3DXVECTOR2 boxProjection = ProjectOntoAxis(boxVertices, 8, axis);

				// AABB�ƃ{�b�N�X�̌�������
				if (!Intersect(aabbProjection, boxProjection)) 
				{
					return false;
				}
			}

			// �{�b�N�X���̔���
			for (int i = 0; i < 3; ++i)
			{
				MyLib::Vector3 axis(boxMtx.m[i][0], boxMtx.m[i][1], boxMtx.m[i][2]);

				// AABB�̎ˉe
				D3DXVECTOR2 aabbProjection = ProjectOntoAxis(aabbVertices, 8, axis);

				// �{�b�N�X�̎ˉe
				D3DXVECTOR2 boxProjection = ProjectOntoAxis(boxVertices, 8, axis);

				// AABB�ƃ{�b�N�X�̌�������
				if (!Intersect(aabbProjection, boxProjection))
				{
					return false;
				}
			}

			return true;
		}
	}

	/**
	@brief	�ϊ��֐�
	*/
	namespace Transformation
	{
		/**
		@brief	�p�x�̐��K��(�P��)
		@details -�� or �΂𒴂����p�x��␳����B�͈͓��Ɏ��܂�܂Ŗ������[�v
		@param	fRot	[inout]	�␳����p�x
		@return	void
		*/
		inline void RotNormalize(float& fRot)
		{
			while (1)
			{
				// 1�����z������␳����
				if (fRot < -D3DX_PI)
				{
					fRot += D3DX_PI * 2.0f;
				}
				else if (fRot > D3DX_PI)
				{
					fRot -= D3DX_PI * 2.0f;
				}

				if (fRot >= -D3DX_PI && fRot <= D3DX_PI)
				{// �S�Đ��K���ς݂ŏI��
					break;
				}
			}
		}

		/**
		@brief	�p�x�̐��K��(VECTOR3)
		@details -�� or �΂𒴂����p�x��␳����B�͈͓��Ɏ��܂�܂Ŗ������[�v
		@param	fRot	[inout]	�␳����p�x
		@return	void
		*/
		inline void RotNormalize(MyLib::Vector3& rot)
		{
			// ���ꂼ��̐��K��
			UtilFunc::Transformation::RotNormalize(rot.x);
			UtilFunc::Transformation::RotNormalize(rot.y);
			UtilFunc::Transformation::RotNormalize(rot.z);
		}

		/**
		@brief	�x���@�����W�A���ɕϊ�
		@param	degree	[in]	�x���@�̊p�x
		@return	���W�A���p�x
		*/
		inline float DegreeChangeToRadian(float degree)
		{
			return degree * (D3DX_PI / 180.0f);
		}

		/**
		@brief	���W�A���p�x��x���@�ɕϊ�
		@param	radian	[in]	���W�A���p�x
		@return	�x���@
		*/
		inline float RadianChangeToDegree(float radian)
		{
			return radian * (180.0f / D3DX_PI);
		}

		/**
		@brief	�l�̐��K������
		@param	Value		[inout]	�␳�������l
		@param	MaxValue	[in]	�ő�l
		@param	MinValue	[in]	�ŏ��l
		@return	void
		*/
		template<class T>inline void ValueNormalize(T& Value, T MaxValue, T MinValue)
		{
			if (Value >= MaxValue)
			{
				// �ő�l�ɕ␳
				Value = MaxValue;
			}
			else if (Value <= MinValue)
			{
				// �ŏ��l�ɕ␳
				Value = MinValue;
			}
		}

		/**
		@brief	�l�̐��K������
		@param	Value		[inout]	�␳�������l
		@param	MinValue	[in]	�ŏ��l
		@param	MaxValue	[in]	�ő�l
		@return	void
		*/
		template<class T>inline const T Clamp(T Value, T MinValue, T MaxValue)
		{
			if (Value >= MaxValue)
			{
				// �ő�l�ɕ␳
				return MaxValue;
			}
			else if (Value <= MinValue)
			{
				// �ŏ��l�ɕ␳
				return MinValue;
			}
			return Value;
		}

		/**
		@brief	���[���h�}�g���b�N�X��pos�ɕϊ�
		@param	worldmtx	[in]	���[���h�}�g���b�N�X
		@return	�ʒu
		*/
		inline MyLib::Vector3 WorldMtxChangeToPosition(D3DXMATRIX worldmtx)
		{
			return MyLib::Vector3(worldmtx._41, worldmtx._42, worldmtx._43);
		}

		inline MyLib::Vector3 MtxChangeToMatrix(const D3DXMATRIX& matrix)
		{
			MyLib::Vector3 scale;
			scale.x = sqrt(matrix._11 * matrix._11 + matrix._12 * matrix._12 + matrix._13 * matrix._13);
			scale.y = sqrt(matrix._21 * matrix._21 + matrix._22 * matrix._22 + matrix._23 * matrix._23);
			scale.z = sqrt(matrix._31 * matrix._31 + matrix._32 * matrix._32 + matrix._33 * matrix._33);
			return scale;
		}

		/**
		@brief	���[���h�}�g���b�N�X��rot�ɕϊ�
		@param	worldmtx	[in]	���[���h�}�g���b�N�X
		@return	����(��Βl)
		*/
		inline MyLib::Vector3 WorldMtxChangeToRotation(D3DXMATRIX worldmtx)
		{
			MyLib::Vector3 rot = mylib_const::DEFAULT_VECTOR3;

			rot.x = atan2f(worldmtx._32, worldmtx._33);
			rot.y = atan2f(-worldmtx._31, sqrtf(worldmtx._32 * worldmtx._32 + worldmtx._33 * worldmtx._33));
			rot.z = atan2f(worldmtx._21, worldmtx._11);

			// XYZ ���� (���[�� �� �s�b�` �� ���[)
			/*rot.x = atan2f(-worldmtx._23, worldmtx._22);
			rot.y = asinf(worldmtx._21);
			rot.z = atan2f(-worldmtx._12, worldmtx._11);*/

			// ZYX ���� (���[ �� �s�b�` �� ���[��)
			// rot.z = atan2f(worldmtx._21, worldmtx._11);
			// rot.y = asinf(-worldmtx._31);
			// rot.x = atan2f(worldmtx._32, worldmtx._33);

			// ZYZ ���� (���[ �� �s�b�` �� ���[)
			// rot.z = atan2f(worldmtx._32, worldmtx._31);
			// rot.y = acosf(worldmtx._33);
			// rot.z = atan2f(worldmtx._23, -worldmtx._13);
			return rot;
		}

		/**
		@brief	������O���x�N�g���ɕϊ�
		@param	rot	[in]	����
		@return	�O���x�N�g��
		*/
		inline MyLib::Vector3 RotationChangeToForwardVector(float rot)
		{
			return MyLib::Vector3(sinf(D3DX_PI + rot), 0.0f, cosf(D3DX_PI + rot));
		}

		/**
		@brief	�����_���l�擾
		@param	nMinNum	[in]	�͈͂̍ŏ��l
		@param	nMaxNum	[in]	�͈͂̍ő�l
		@return	���I���ꂽ�����_���l
		*/
		inline int Random(int nMinNum, int nMaxNum)
		{
			// �͈�
			int nRange = nMaxNum - nMinNum;

			if (nMinNum == 0)
			{
				// �͈͂�1���Z
				nRange += 1;
			}

			if (nMinNum + 1 == nMaxNum || nMaxNum - 1 == nMinNum)
			{
				nRange += 1;
			}

			// �����_������
			return (rand() % nRange + nMinNum);
		}

		/**
		@brief	�����_���ȉ~�����擾
		@return	���I���ꂽ�����_���l
		*/
		inline float GetRandomPi()
		{
			return static_cast<float>(UtilFunc::Transformation::Random(-314, 314) * 0.01f);
		}

		/**
		@brief	���͈͂̃����_���x�N�g���擾
		@return	���̃����_���ȃx�N�g���l
		*/
		inline MyLib::Vector3 GetRandomVecSphere()
		{
			// ���x�N�g��
			MyLib::Vector3 vecSphere = mylib_const::DEFAULT_VECTOR3;

			// ���͈͓̔��Ń����_������
			float fRot = UtilFunc::Transformation::GetRandomPi(), fRotPhi = UtilFunc::Transformation::GetRandomPi();

			vecSphere.x = cosf(fRot) * sinf(fRotPhi);
			vecSphere.y = sinf(fRot);
			vecSphere.z = cosf(fRot) * cosf(fRotPhi);

			return vecSphere;
		}

		/**
		@brief	���͈͂̃����_���Ȉʒu�擾
		@param	pos		[in]	���̒��S���W
		@param	fRadius	[in]	�擾���鋅�̔��a
		@return	���I���ꂽ�����_���Ȉʒu
		*/
		inline MyLib::Vector3 GetRandomPositionSphere(MyLib::Vector3 pos, float fRadius)
		{
			MyLib::Vector3 spherepos = mylib_const::DEFAULT_VECTOR3;
			MyLib::Vector3 vec = GetRandomVecSphere();

			// �������v�Z
			spherepos = vec * fRadius;
			spherepos += pos;

			return spherepos;
		}

		/**
		@brief	��������ɕ��������ϊ�
		@param	size		[in]	���̃T�C�Y
		@param	height		[in]	�ύX��̍���
		@return	�񎟌��T�C�Y
		*/
		inline D3DXVECTOR2 AdjustSizeByHeight(D3DXVECTOR2 size, float height)
		{
			D3DXVECTOR2 result = size;
			result.y = height;

			// �����Ɣ䗦���畝���v�Z
			result.x = height * UtilFunc::Calculation::AspectRatio(size);
			return result;
		}

		/**
		@brief	������ɍ����������v�Z
		@param	size		[in]	���̃T�C�Y
		@param	width		[in]	�ύX��̕�
		@return	�񎟌��T�C�Y
		*/
		inline D3DXVECTOR2 AdjustSizeByWidth(D3DXVECTOR2 size, float width)
		{
			D3DXVECTOR2 result = size;
			result.x = width;

			// ���Ɣ䗦���獂�����v�Z
			result.y = width / UtilFunc::Calculation::AspectRatio(size);
			return result;
		}

		/**
		@brief	�X�N���[�����W�����[���h���W�ɕϊ�
		@param	Sx			[in]	�X�N���[��X���W
		@param	Sy			[in]	�X�N���[��Y���W
		@param	fZ			[in]	�ˉe��Ԃł�Z�l�i0�`1�j
		@param	ScreenSize	[in]	�X�N���[���T�C�Y
		@param	View		[in]	�r���[�}�g���b�N�X
		@param	Prj			[in]	�v���W�F�N�V�����}�g���b�N�X
		@return	�񎟌��T�C�Y
		*/
		inline MyLib::Vector3 CalcScreenToWorld(
			int Sx, int Sy, float fZ,
			D3DXVECTOR2 ScreenSize,
			D3DXMATRIX View, D3DXMATRIX Prj) 
		{
			MyLib::Vector3 pos;

			// �e�s��̋t�s����Z�o
			D3DXMATRIX InvView, InvPrj, VP, InvViewport;
			D3DXMatrixInverse(&InvView, NULL, &View);
			D3DXMatrixInverse(&InvPrj, NULL, &Prj);
			D3DXMatrixIdentity(&VP);
			VP._11 = ScreenSize.x / 2.0f;
			VP._22 = -ScreenSize.y / 2.0f;
			VP._41 = ScreenSize.x / 2.0f;
			VP._42 = ScreenSize.y / 2.0f;
			D3DXMatrixInverse(&InvViewport, NULL, &VP);

			// �t�ϊ�
			D3DXMATRIX tmp = InvViewport * InvPrj * InvView;

			// �ϊ���̍��W�ɕϊ�
			MyLib::Vector3 screenPos(static_cast<float>(Sx), static_cast<float>(Sy), fZ);
			D3DXVec3TransformCoord(&pos, &screenPos, &tmp);

			return pos;
		}

		// XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�
		inline MyLib::Vector3  CalcScreenToXZ(
			D3DXVECTOR2 mousepos,
			D3DXVECTOR2 ScreenSize,
			D3DXMATRIX View, D3DXMATRIX Prj)
		{
			MyLib::Vector3 pos;

			MyLib::Vector3 nearpos;
			MyLib::Vector3 farpos;
			MyLib::Vector3 ray;

			int Sx = static_cast<int>(mousepos.x), Sy = static_cast<int>(mousepos.y);
			nearpos = CalcScreenToWorld(Sx, Sy, 0.0f, ScreenSize, View, Prj);
			farpos = CalcScreenToWorld(Sx, Sy, 1.0f, ScreenSize, View, Prj);
			ray = farpos - nearpos;
			D3DXVec3Normalize(&ray, &ray);

			// ���Ƃ̌������N���Ă���ꍇ�͌�_��
			// �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
			if (ray.y <= 0) {

				MyLib::Vector3 planevec(0.0f, 1.0f, 0.0f);

				// ����_
				float Lray = ray.Dot(planevec);

				MyLib::Vector3 transNearPos = -nearpos;
				float LP0 = transNearPos.Dot(planevec);
				pos = nearpos + (LP0 / Lray) * ray;
			}
			else {
				pos = farpos;
			}

			return pos;
		}

		// XZ���ʂƃX�N���[�����W�̌�_�Z�o�֐�
		inline MyLib::Vector3  CalcScreenToXZ(
			D3DXVECTOR2 mousepos,
			D3DXVECTOR2 ScreenSize,
			D3DXMATRIX View, D3DXMATRIX Prj,
			float farDistance)
		{
			MyLib::Vector3 pos;

			MyLib::Vector3 nearpos;
			MyLib::Vector3 farpos;
			MyLib::Vector3 ray;

			int Sx = static_cast<int>(mousepos.x), Sy = static_cast<int>(mousepos.y);
			nearpos = CalcScreenToWorld(Sx, Sy, 0.0f, ScreenSize, View, Prj);
			farpos = CalcScreenToWorld(Sx, Sy, 1.0f, ScreenSize, View, Prj);
			ray = farpos - nearpos;
			D3DXVec3Normalize(&ray, &ray);

			// farpos���Čv�Z����
			farpos = nearpos + ray * farDistance;

			// ���Ƃ̌������N���Ă���ꍇ�͌�_��
			// �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
			if (ray.y <= 0) {

				MyLib::Vector3 planevec(0.0f, 1.0f, 0.0f);

				// ����_
				float Lray = ray.Dot(planevec);

				MyLib::Vector3 transNearPos = -nearpos;
				float LP0 = transNearPos.Dot(planevec);
				pos = nearpos + (LP0 / Lray) * ray;
			}
			else {
				pos = farpos;
			}

			return pos;
		}

		// Y���ʂƃX�N���[�����W�̌�_�Z�o�֐�
		inline MyLib::Vector3  CalcScreenToY(
			D3DXVECTOR2 mousepos,
			D3DXVECTOR2 ScreenSize,
			D3DXMATRIX View, D3DXMATRIX Prj)
		{
			// �X�N���[�����W�𐳋K���f�o�C�X���W�n�iNDC�j�ɕϊ�
			float ndcX = (2.0f * mousepos.x / ScreenSize.x) - 1.0f;
			float ndcY = 1.0f - (2.0f * mousepos.y / ScreenSize.y);
			float ndcZ = 2.0f * 0.0f - 1.0f;

			// �X�N���[�����W��NDC���W�ɕϊ�
			D3DXVECTOR4 ndcPos(ndcX, ndcY, ndcZ, 1.0f);

			// �t�r���[�ϊ��s��Ƌt�ˉe�ϊ��s���g�ݍ��킹�ă��[���h���W�ɕϊ�
			D3DXMATRIX invViewProjection;
			D3DXMATRIX mtx = (View * Prj);

			D3DXMatrixInverse(&invViewProjection, nullptr, &mtx);

			D3DXVECTOR4 worldPos;
			D3DXVec4Transform(&worldPos, &ndcPos, &invViewProjection);

			// ���[���h���W��Ԃ�
			return MyLib::Vector3(worldPos.x / worldPos.w, worldPos.y / worldPos.w, worldPos.z / worldPos.w);
		}

		// Y���ʂƃX�N���[�����W�̌�_�Z�o�֐�
		inline MyLib::Vector3  CalcScreenToY(
			D3DXVECTOR2 mousepos,
			D3DXVECTOR2 ScreenSize,
			D3DXMATRIX View, D3DXMATRIX Prj,
			float farDistance)
		{
			MyLib::Vector3 pos;

			MyLib::Vector3 nearpos;
			MyLib::Vector3 farpos;
			MyLib::Vector3 ray;

			int Sx = static_cast<int>(mousepos.x), Sy = static_cast<int>(mousepos.y);
			nearpos = CalcScreenToWorld(Sx, Sy, 0.0f, ScreenSize, View, Prj);
			farpos = CalcScreenToWorld(Sx, Sy, 1.0f, ScreenSize, View, Prj);
			ray = farpos - nearpos;
			D3DXVec3Normalize(&ray, &ray);

			// farpos���Čv�Z����
			farpos = nearpos + ray * farDistance;

			pos = farpos;

			return pos;
		}

		/**
		@brief	HSV�l��RGB�l�ɕϊ�
		@details https://ja.wikipedia.org/wiki/HSV%E8%89%B2%E7%A9%BA%E9%96%93
		@param	H	[in]	�F��
		@param	S	[in]	�ʓx
		@param	V	[in]	���x
		@return	RGB�l
		*/
		inline D3DXCOLOR HSVtoRGB(float H, float S, float V)
		{
			if (H > 360.0f)
			{
				H -= 360.0f;
			}
			else if (H < 0.0f)
			{
				H += 360.0f;
			}
			float C = V * S;
			float X = C * (1 - (float)abs(fmod(H / 60.0f, 2) - 1));
			float m = V - C;

			// RGB�ʂ̐��l
			float R = 0.0f, G = 0.0f, B = 0.0f;
			if (H >= 0.0f && H < 60.0f)
			{
				R = C;
				G = X;
				B = 0.0f;
			}
			else if (H >= 60.0f && H < 120.0f)
			{
				R = X;
				G = C;
				B = 0.0f;
			}
			else if (H >= 120.0f && H < 180.0f)
			{
				R = 0.0f;
				G = C;
				B = X;
			}
			else if (H >= 180.0f && H < 240.0f)
			{
				R = 0.0f;
				G = X;
				B = C;
			}
			else if (H >= 240.0f && H < 300.0f)
			{
				R = X;
				G = 0.0f;
				B = C;
			}
			else 
			{
				R = C;
				G = 0.0f;
				B = X;
			}

			return D3DXCOLOR(R + m, G + m, B + m, 1.0f);
		}

		/**
		@brief	���C�h�����񂩂�}���`�o�C�g������ɕϊ�����֐�
		@param	wideStr		[in]	���C�h������
		@return	�}���`�o�C�g������
		*/
		inline std::string WideToMultiByte(const wchar_t* wideStr)
		{
			if (wideStr == nullptr)
				return std::string();

			// ���C�h������̒������擾
			int wideLength = wcslen(wideStr);

			// �}���`�o�C�g������̒������v�Z
			int multiLength = WideCharToMultiByte(CP_UTF8, 0, wideStr, wideLength, nullptr, 0, nullptr, nullptr);

			if (multiLength == 0)
				return std::string();

			// �}���`�o�C�g��������i�[���邽�߂̃o�b�t�@���m��
			std::string multiString(multiLength, 0);

			// ���C�h��������}���`�o�C�g������ɕϊ�
			WideCharToMultiByte(CP_UTF8, 0, wideStr, wideLength, &multiString[0], multiLength, nullptr, nullptr);

			return multiString;
		}


		/**
		@brief	\\\\��\\�ɒu������
		@param	str		[in]	������
		@return	�ϊ����ꂽstring����
		*/
		inline std::string ReplaceBackslash(std::string str)
		{
			size_t pos = 0;

			while ((pos = str.find("\\\\", pos)) != std::string::npos)
			{
				// \\\\��\\�ɒu��
				str.replace(pos, 2, "\\");

				// \\�̎����猟�����ĊJ
				pos += 1;
			}
			return str;
		}

		/**
		@brief	/��\\�ɒu������
		@param	str		[in]	������
		@return	�ϊ����ꂽstring����
		*/
		inline std::string ReplaceForwardSlashes(std::string str)
		{
			for (size_t i = 0; i < str.size(); i++) 
			{
				if (str[i] == '/') 
				{
					str.replace(i, 1, "\\");
				}
			}
			return str;
		}

		/**
		@brief	�t�@�C���p�X������
		@param	filePath	[in]	������
		@return	�ϊ����ꂽstring����
		*/
		inline std::string RemoveFilePath(const std::string& filePath)
		{
			// �Ō�̃X���b�V���܂��̓o�b�N�X���b�V����������
			size_t pos = filePath.find_last_of("/\\");

			if (pos != std::string::npos) 
			{
				// ���������ʒu�̎��̕�������Ō�܂ł��擾
				return filePath.substr(pos + 1);
			}

			return filePath;
		}
	}

}



#endif