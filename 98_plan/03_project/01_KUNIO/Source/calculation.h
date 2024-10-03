//=============================================================================
// 
// 便利関数 [calculation.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _CALCULATION_H_
#define _CALCULATION_H_		// 二重インクルード防止のマクロを定義する

#include "constans.h"
#include "3D_effect.h"

#include <DirectXMath.h>

using namespace DirectX;

/**
@brief	簡単な説明
@details 詳細な説明
@param	hoge	[in][out][inout]	引数の説明
@return	piyo	戻り値の説明
*/

//==========================================================================
// マクロ定義
//==========================================================================
#define MAX_COMMENT	(512)	// コメントの最大数

/**
@brief	便利関数
*/
namespace UtilFunc
{
	namespace Transformation
	{
		template<class T> void ValueNormalize(T& Value, T MaxValue, T MinValue);	// 値の正規化処理
		template<class T> const T Clamp(T Value, T MinValue, T MaxValue);			
			MyLib::Vector3 RotationChangeToForwardVector(float rot);
		MyLib::Vector3 WorldMtxChangeToPosition(D3DXMATRIX worldmtx);	// ワールドマトリックスをposに変換
		MyLib::Vector3 MtxChangeToMatrix(const D3DXMATRIX& matrix);
	}

	namespace Collision
	{
		bool CollisionTriangle(const MyLib::Vector3& posCenter, const MyLib::Vector3& posLeft, const MyLib::Vector3& posRight, const MyLib::Vector3& MainPos, const MyLib::Vector3& MainPosOld);	// 三角形の内側にいるか判定

	}
}

//==========================================================================
// ネームスペース定義
//==========================================================================
namespace UtilFunc	// 便利関数
{

	/**
	@brief	補正関数
	*/
	namespace Correction
	{
		/**
		@brief	慣性補正
		@details 補正係数を1.0fにすると1Fで補正が完了します
		@param	fMain	[inout]	補正する値
		@param	fTarget	[in]	目標の値
		@param	fFactor	[in]	補正係数(0.0f ～ 1.0f)
		@return	void
		*/
		inline void InertiaCorrection(float& fMain, float fTarget, float fFactor)
		{
			// 範囲外補正
			UtilFunc::Transformation::ValueNormalize(fFactor, 1.0f, 0.0f);

			fMain += (fTarget - fMain) * fFactor;
		}

		/**
		@brief	慣性補正
		@details 補正係数を1.0fにすると1Fで補正が完了します
		@param	main	[inout]	補正する値
		@param	target	[in]	目標の値
		@param	fFactor	[in]	補正係数(0.0f ～ 1.0f)
		@return	void
		*/
		inline void InertiaCorrection(MyLib::Vector3& main, const MyLib::Vector3& target, float fFactor)
		{
			// 範囲外補正
			UtilFunc::Transformation::ValueNormalize(fFactor, 1.0f, 0.0f);

			main.x += (target.x - main.x) * fFactor;
			main.y += (target.y - main.y) * fFactor;
			main.z += (target.z - main.z) * fFactor;
		}

		/**
		@brief	線形補正(等速, リニア)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/linear.mp4
		@param	start	[in]	初期値
		@param	end		[in]	目標値
		@param	time	[in]	補正割合(0.0f ～ 1.0f)
		@return	補正されたfloat値
		*/
		inline float EasingLinear(float start, float end, float time)
		{
			// 範囲外補正
			UtilFunc::Transformation::ValueNormalize(time, 1.0f, 0.0f);
			return start + (end - start) * time;
		}

		/**
		@brief	線形補正(等速, リニア)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/linear.mp4
		@param	start	[in]	初期値
		@param	end		[in]	目標値
		@param	time	[in]	補正割合(0.0f ～ 1.0f)
		@return	補正されたVector3値
		*/
		inline MyLib::Vector3 EasingLinear(const MyLib::Vector3& start, const MyLib::Vector3& end, float time)
		{
			// 範囲外補正
			UtilFunc::Transformation::ValueNormalize(time, 1.0f, 0.0f);
			return start + (end - start) * time;
		}

		/**
		@brief	線形補正(等速, リニア)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/linear.mp4
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@return	補正されたfloat値
		*/
		inline float EasingLinear(float start, float end, float startTime, float endTime, float currentTime)
		{
			// 割合
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return EasingLinear(start, end, ratio);
		}

		/**
		@brief	線形補正(等速, リニア)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/linear.mp4
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@return	補正されたVector3値
		*/
		inline MyLib::Vector3 EasingLinear(const MyLib::Vector3& start, const MyLib::Vector3& end, float startTime, float endTime, float currentTime)
		{
			// 割合
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return EasingLinear(start, end, ratio);
		}

		/**
		@brief	線形補正(徐々に加速, イーズイン)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easein_1.mp4
		@param	start	[in]	初期値
		@param	end		[in]	目標値
		@param	time	[in]	補正割合(0.0f ～ 1.0f)
		@return	補正されたfloat値
		*/
		inline float EasingEaseIn(float start, float end, float time)
		{
			// 範囲外補正
			UtilFunc::Transformation::ValueNormalize(time, 1.0f, 0.0f);
			return start + (end - start) * time * time;
		}

		/**
		@brief	線形補正(徐々に加速, イーズイン)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easein_1.mp4
		@param	start	[in]	初期値
		@param	end		[in]	目標値
		@param	time	[in]	補正割合(0.0f ～ 1.0f)
		@return	補正されたVector3値
		*/
		inline MyLib::Vector3 EasingEaseIn(const MyLib::Vector3& start, const MyLib::Vector3& end, float time)
		{
			// 範囲外補正
			UtilFunc::Transformation::ValueNormalize(time, 1.0f, 0.0f);
			return start + (end - start) * time * time;
		}

		/**
		@brief	線形補正(徐々に加速, イーズイン)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easein_1.mp4
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@return	補正されたfloat値
		*/
		inline float EasingEaseIn(float start, float end, float startTime, float endTime, float currentTime)
		{
			// 割合
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return EasingEaseIn(start, end, ratio);
		}

		/**
		@brief	線形補正(徐々に加速, イーズイン)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easein_1.mp4
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@return	補正されたVector3値
		*/
		inline MyLib::Vector3 EasingEaseIn(const MyLib::Vector3& start, const MyLib::Vector3& end, float startTime, float endTime, float currentTime)
		{
			// 割合
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return EasingEaseIn(start, end, ratio);
		}

		/**
		@brief	線形補正(徐々に減速, イーズアウト)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeout_1.mp4
		@param	start	[in]	初期値
		@param	end		[in]	目標値
		@param	time	[in]	補正割合(0.0f ～ 1.0f)
		@return	補正されたfloat値
		*/
		inline float EasingEaseOut(float start, float end, float time)
		{
			// 範囲外補正
			UtilFunc::Transformation::ValueNormalize(time, 1.0f, 0.0f);
			float fSquaredTime = time * time;

			return start + (end - start) * (1.0f - (1.0f - fSquaredTime) * (1.0f - fSquaredTime));
		}

		/**
		@brief	線形補正(徐々に減速, イーズアウト)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeout_1.mp4
		@param	start	[in]	初期値
		@param	end		[in]	目標値
		@param	time	[in]	補正割合(0.0f ～ 1.0f)
		@return	補正されたVector3値
		*/
		inline MyLib::Vector3 EasingEaseOut(const MyLib::Vector3& start, const MyLib::Vector3& end, float time)
		{
			// 範囲外補正
			UtilFunc::Transformation::ValueNormalize(time, 1.0f, 0.0f);
			float fSquaredTime = time * time;

			return start + (end - start) * (1.0f - (1.0f - fSquaredTime) * (1.0f - fSquaredTime));
		}

		/**
		@brief	線形補正(徐々に減速, イーズアウト)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeout_1.mp4
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@return	補正されたfloat値
		*/
		inline float EasingEaseOut(float start, float end, float startTime, float endTime, float currentTime)
		{
			// 割合
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return EasingEaseOut(start, end, ratio);
		}

		/**
		@brief	線形補正(徐々に減速, イーズアウト)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeout_1.mp4
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@return	補正されたVector3値
		*/
		inline MyLib::Vector3 EasingEaseOut(const MyLib::Vector3& start, const MyLib::Vector3& end, float startTime, float endTime, float currentTime)
		{
			// 割合
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return EasingEaseOut(start, end, ratio);
		}

		/**
		@brief	線形補正(加速してから減速, イーズインアウト)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeinout_1.mp4
		@param	start	[in]	初期値
		@param	end		[in]	目標値
		@param	time	[in]	補正割合(0.0f ～ 1.0f)
		@return	補正されたfloat値
		*/
		inline float EasingEaseInOut(float start, float end, float time)
		{
			// 範囲外補正
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
		@brief	線形補正(加速してから減速, イーズインアウト)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeinout_1.mp4
		@param	start	[in]	初期値
		@param	end		[in]	目標値
		@param	time	[in]	補正割合(0.0f ～ 1.0f)
		@return	補正されたfloat値
		*/
		inline MyLib::Vector3 EasingEaseInOut(const MyLib::Vector3& start, const MyLib::Vector3& end, float time)
		{
			// 範囲外補正
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
		@brief	線形補正(加速してから減速, イーズインアウト)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeinout_1.mp4
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@return	補正されたfloat値
		*/
		inline float EasingEaseInOut(float start, float end, float startTime, float endTime, float currentTime)
		{
			// 割合
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return EasingEaseInOut(start, end, ratio);
		}

		/**
		@brief	線形補正(加速してから減速, イーズインアウト)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeinout_1.mp4
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@return	補正されたfloat値
		*/
		inline MyLib::Vector3 EasingEaseInOut(const MyLib::Vector3& start, const MyLib::Vector3& end, float startTime, float endTime, float currentTime)
		{
			// 割合
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return EasingEaseInOut(start, end, ratio);
		}

		/**
		@brief	線形補正(サインカーブベースの補間、減速→加速→減速, イーズインアウト)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeinout_1.mp4
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@return	補正されたfloat値
		*/
		inline float EasingEaseInOutSine(float start, float end, float startTime, float endTime, float currentTime)
		{
			// 割合
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return 0.5f * (1.0f - cosf(D3DX_PI * ratio));
		}

		/**
		@brief	線形補正(サインカーブベースの補間、減速→加速→減速, イーズインアウト)
		@details https://mo-no.design/blog/wp-content/uploads/2023/02/easeinout_1.mp4
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@return	補正されたfloat値
		*/
		inline MyLib::Vector3 EasingEaseInOutSine(const MyLib::Vector3& start, const MyLib::Vector3& end, float startTime, float endTime, float currentTime)
		{
			// 割合
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			return 0.5f * (1.0f - cosf(D3DX_PI * ratio));
		}




		/**
		@brief	easeInBack関数
		@details https://easings.net/ja#easeInBack
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@param	c			[in]	バックの強さ
		@return	補正されたfloat値
		*/
		inline float EaseInBack(float start, float end, float startTime, float endTime, float currentTime, float c = 1.70158f)
		{
			// 割合
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			float cal = ratio * ratio * ((c + 1) * ratio - c);
			return start + (end - start) * cal;
		}

		/**
		@brief	easeOutBack関数
		@details https://easings.net/ja#easeInBack
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@param	c			[in]	バックの強さ
		@return	補正されたfloat値
		*/
		inline float EaseOutBack(float start, float end, float startTime, float endTime, float currentTime, float c = 1.70158f)
		{
			// 割合
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			ratio -= 1; // t を (t - 1) に変換

			float cal = ratio * ratio * ((c + 1) * ratio + c) + 1;
			return start + (end - start) * cal;
		}

		/**
		@brief	easeOutBack関数
		@details https://easings.net/ja#easeInBack
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@param	c			[in]	バックの強さ
		@return	補正されたfloat値
		*/
		inline MyLib::Vector3 EaseOutBack(const MyLib::Vector3& start, const MyLib::Vector3& end, float startTime, float endTime, float currentTime, float c = 1.70158f)
		{
			// 割合
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			ratio -= 1; // t を (t - 1) に変換

			float cal = ratio * ratio * ((c + 1) * ratio + c) + 1;
			return start + (end - start) * cal;
		}

		/**
		@brief	easeInOutBack関数
		@details https://easings.net/ja#easeInBack
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@param	c			[in]	バックの強さ
		@return	補正されたfloat値
		*/
		inline float EaseInOutBack(float start, float end, float startTime, float endTime, float currentTime, float c = 1.70158f)
		{
			c *= 1.525f; // より強調された効果

			// 割合
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
		@brief	easeInExpo関数
		@details https://easings.net/ja#easeInExpo
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@return	補正されたfloat値
		*/
		inline float EaseInExpo(float start, float end, float startTime, float endTime, float currentTime)
		{
			// 割合
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			// イージング計算
			float eased = (ratio == 0) ? 0 : pow(2, 10 * (ratio - 1));

			// 線形補間（Lerp）を使用して結果をstartからendの範囲に変換
			return start + (end - start) * eased;
		}

		/**
		@brief	easeInExpo関数
		@details https://easings.net/ja#easeInExpo
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@return	補正されたfloat値
		*/
		inline MyLib::Vector3 EaseInExpo(const MyLib::Vector3& start, const MyLib::Vector3& end, float startTime, float endTime, float currentTime)
		{
			// 割合
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			// イージング計算
			float eased = (ratio == 0) ? 0 : pow(2, 10 * (ratio - 1));

			// 線形補間（Lerp）を使用して結果をstartからendの範囲に変換
			return start + (end - start) * eased;
		}

		/**
		@brief	easeOutExpo関数
		@details https://easings.net/ja#easeOutExpo
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@return	補正されたfloat値
		*/
		inline float EaseOutExpo(float start, float end, float startTime, float endTime, float currentTime)
		{
			// 割合
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			// イージング計算
			float eased = (ratio == 1) ? 1 : (1 - pow(2, -10 * ratio));

			// 線形補間（Lerp）を使用して結果をstartからendの範囲に変換
			return start + (end - start) * eased;
		}

		/**
		@brief	easeOutExpo関数
		@details https://easings.net/ja#easeOutExpo
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@return	補正されたfloat値
		*/
		inline MyLib::Vector3 EaseOutExpo(const MyLib::Vector3& start, const MyLib::Vector3& end, float startTime, float endTime, float currentTime)
		{
			// 割合
			float ratio = (currentTime - startTime) / (endTime - startTime);
			UtilFunc::Transformation::ValueNormalize(ratio, 1.0f, 0.0f);

			// イージング計算
			float eased = (ratio == 1) ? 1 : (1 - pow(2, -10 * ratio));

			// 線形補間（Lerp）を使用して結果をstartからendの範囲に変換
			return start + (end - start) * eased;
		}

		/**
		@brief	easeInOutExpo関数
		@details https://easings.net/ja#easeInOutExpo
		@param	start		[in]	初期値
		@param	end			[in]	目標値
		@param	startTime	[in]	開始時間
		@param	endTime		[in]	終了時間
		@param	currentTime	[in]	現在の時間
		@return	補正されたfloat値
		*/
		inline float EaseInOutExpo(float start, float end, float startTime, float endTime, float currentTime)
		{
			// 割合
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

			// 線形補間（Lerp）を使用して結果をstartからendの範囲に変換
			return start + (end - start) * eased;
		}








		/**
		@brief	Catmull-Rom Spline補間(全ての点を通る線形補間)
		@details https://storage.googleapis.com/zenn-user-upload/cu4a6qn5njn0pskn6pfx6lypic0h
		@param	PointMili1	[in]	基準の前の制御点(f-1)
		@param	Point0		[in]	基準の最初の制御点(f0)
		@param	Point1		[in]	基準の2番目の制御点(f1)
		@param	Point2		[in]	基準の次の制御点 (f2)
		@param	time		[in]	補正割合(0.0f ～ 1.0f)
		@return	補間された値
		*/
		inline MyLib::Vector3 CatmullRomSplineInterp(MyLib::Vector3 PointMili1, MyLib::Vector3 Point0, MyLib::Vector3 Point1, MyLib::Vector3 Point2, float time)
		{
			// 4点で補正
			MyLib::Vector3 a4 = Point0;
			MyLib::Vector3 a3 = (Point1 - PointMili1) / 2.0f;
			MyLib::Vector3 a1 = (Point2 - Point0) / 2.0f - 2.0f * Point1 + a3 + 2.0f * a4;
			MyLib::Vector3 a2 = 3.0f * Point1 - (Point2 - Point0) / 2.0f - 2.0f * a3 - 3.0f * a4;

			return (a1 * time * time * time + a2 * time * time + a3 * time + a4);
		}

		/**
		@brief	二次曲線補正
		@param	Value		[inout]	補完される値
		@param	nRange		[in]	補完の範囲
		@param	RangeOrigin	[in]	補完範囲の起点となる値
		@param	Max			[in]	補完の最大値
		@param	nAlphaCnt	[in]	二次曲線補正用のカウンター変数への参照
		@return	void
		*/
		template<class T> inline void CuadricCurveComp(T& Value, const int nRange, T RangeOrigin, T Max, int& nAlphaCnt)
		{
			// 補完の値割り出し
			T Comp = (static_cast<T>(Max) / static_cast<T>((nRange * nRange)));

			if (nAlphaCnt > nRange)
			{// 範囲超えたら戻す
				nAlphaCnt = -nRange;
			}
			else if (nAlphaCnt < -nRange)
			{// 範囲超えたら戻す
				nAlphaCnt = nRange;
			}

			// 不透明のカウント加算
			nAlphaCnt++;

			// 曲線補正
			Value = static_cast<T>(((Comp * (nAlphaCnt * nAlphaCnt)) - (2 * (Comp * nAlphaCnt * RangeOrigin)) + (Comp * (RangeOrigin * RangeOrigin))));
			Value += RangeOrigin;
		}

	}

	/**
	@brief	計算関数
	*/
	namespace Calculation
	{
		/**
		@brief	桁数取得
		@param	nNum	[in]	桁数を求めたい値
		@return	桁数
		*/
		inline int GetDigit(int nNum)
		{
			if (nNum <= 0)
			{
				nNum = 0;
			}

			// 整数を文字列に変換
			std::string number;
			sprintf((char*)number.c_str(), "%d", nNum);

			// 文字列の長さ（桁数）を取得
			int nDigit = strlen(number.c_str());
			if (nDigit <= 0)
			{
				nDigit = 1;
			}

			return nDigit;
		}

		/**
		@brief	指定された許容誤差範囲内にあるか判定
		@param	value	[in]	判定したい値
		@param	target	[in]	基準にする値
		@param	range	[in]	許容範囲
		@return	判定結果
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
		@brief	二点間の長さ取得(2D)
		@param	pos1	[in]	始点
		@param	pos2	[in]	終点
		@return	長さ
		*/
		inline float GetPosLength2D(MyLib::Vector3 pos1, MyLib::Vector3 pos2)
		{
			return sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
				+ (pos1.y - pos2.y) * (pos1.y - pos2.y));
		}

		/**
		@brief	二点間の長さの絶対値取得(2D)
		@param	pos1	[in]	始点
		@param	pos2	[in]	終点
		@return	長さの絶対値
		*/
		inline float GetFabsPosLength2D(MyLib::Vector3 pos1, MyLib::Vector3 pos2)
		{
			return fabsf(GetPosLength2D(pos1, pos2));
		}

		/**
		@brief	二点間の長さ取得(3D)
		@param	pos1	[in]	始点
		@param	pos2	[in]	終点
		@return	長さ
		*/
		inline float GetPosLength3D(MyLib::Vector3 pos1, MyLib::Vector3 pos2)
		{
			return sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
				+ (pos1.z - pos2.z) * (pos1.z - pos2.z));
		}

		/**
		@brief	二点間の長さの絶対値取得(3D)
		@param	pos1	[in]	始点
		@param	pos2	[in]	終点
		@return	長さの絶対値
		*/
		inline float GetFabsPosLength3D(MyLib::Vector3 pos1, MyLib::Vector3 pos2)
		{
			return fabsf(GetPosLength3D(pos1, pos2));
		}

		/**
		@brief	二点の中心点取得(3D)
		@param	pos1	[in]	始点
		@param	pos2	[in]	終点
		@return	中心点
		*/
		inline MyLib::Vector3 GetCenterPosition3D(MyLib::Vector3 pos1, MyLib::Vector3 pos2)
		{
			return MyLib::Vector3(
				(pos1.x + pos2.x),
				(pos1.y + pos2.y),
				(pos1.z + pos2.z)) * 0.5f;
		}

		/**
		@brief	辺上の位置取得(2D)
		@param	point1	[in]	始点の頂点座標
		@param	point2	[in]	終点の頂点座標
		@param	fRate	[in]	割合
		@return	位置
		*/
		inline MyLib::Vector3 GetPointOnEdge2D(MyLib::Vector3 point1, MyLib::Vector3 point2, float fRate)
		{
			return MyLib::Vector3(
				point1.x + fRate * (point2.x - point1.x),
				point1.y + fRate * (point2.y - point1.y),
				0.0f);
		}

		/**
		@brief	放物線の位置取得
		@param	start		[in]	始点
		@param	end			[in]	終点
		@param	fMaxHeight	[in]	高さの最大値
		@param	time		[in]	補正割合(0.0f ～ 1.0f)
		@return	放物線上の位置取得
		*/
		inline MyLib::Vector3 GetParabola3D(MyLib::Vector3 start, MyLib::Vector3 end, float fMaxHeight, float time)
		{
			// 位置
			MyLib::Vector3 pos = mylib_const::DEFAULT_VECTOR3;

			// XZ移動
			pos.x = start.x + (end.x - start.x) * time;
			pos.z = start.z + (end.z - start.z) * time;

			// 高さ
			pos.y = start.y + (end.y - start.y) * time + sinf(D3DX_PI * time) * fMaxHeight;

			return pos;
		}

		/**
		@brief	比率を計算
		@param	size		[in]	二次元サイズ
		@return	比率
		*/
		inline float AspectRatio(const D3DXVECTOR2& size) 
		{
			return size.x / size.y;
		}

		/**
		@brief	モデルの頂点情報計算
		@param	RotY		[in]	モデルのY軸向き
		@param	pVtxMax		[inout]	頂点座標の最大値
		@param	pVtxMin		[inout]	頂点座標の最小値
		@param	pMesh		[in]	モデルのメッシュオブジェクト
		@param	pVtxBuff	[in]	頂点バッファ
		@return	void
		*/
		inline void CalModelVtx(float RotY, MyLib::Vector3* pVtxMax, MyLib::Vector3* pVtxMin, LPD3DXMESH pMesh, BYTE* pVtxBuff)
		{
			// リセット
			*pVtxMin = MyLib::Vector3(10000.0f, 10000.0f, 10000.0f);
			*pVtxMax = MyLib::Vector3(-10000.0f, -10000.0f, -10000.0f);

			// モデルの頂点数を取得
			int nNumVtx = pMesh->GetNumVertices();

			// 頂点フォーマットのサイズを取得
			DWORD dwSizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

			// 頂点バッファをロック
			pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			// 全ての頂点をチェックする
			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{
				// 今回のデータ
				MyLib::Vector3 vtx = *(MyLib::Vector3*)pVtxBuff;

				// X
				if (pVtxMin->x > vtx.x)
				{// 今の最小値よりも今回の値がちいさかったら

					pVtxMin->x = vtx.x;
				}
				if (pVtxMax->x < vtx.x)
				{// 今の最大値よりも今回の値が大きかったら

					pVtxMax->x = vtx.x;
				}

				// Y
				if (pVtxMin->y > vtx.y)
				{// 今の最小値よりも今回の値がちいさかったら

					pVtxMin->y = vtx.y;
				}
				if (pVtxMax->y < vtx.y)
				{// 今の最大値よりも今回の値が大きかったら

					pVtxMax->y = vtx.y;
				}

				// Z
				if (pVtxMin->z > vtx.z)
				{// 今の最小値よりも今回の値がちいさかったら

					pVtxMin->z = vtx.z;
				}
				if (pVtxMax->z < vtx.z)
				{// 今の最大値よりも今回の値が大きかったら

					pVtxMax->z = vtx.z;
				}

				pVtxBuff += dwSizeFVF;	// サイズ分ポインタ移動
			}

			// 頂点バッファをアンロック
			pMesh->UnlockVertexBuffer();

			MyLib::Vector3 VtxMax = *pVtxMax, VtxMin = *pVtxMin;

			if (RotY <= D3DX_PI * 0.5f + 0.05f && RotY >= D3DX_PI * 0.5f - 0.05f)
			{// 1.57fのとき

				pVtxMax->x = VtxMax.z;
				pVtxMin->x = VtxMin.z;

				pVtxMax->z = -VtxMin.x;
				pVtxMin->z = -VtxMax.x;
			}
			else if (RotY <= -D3DX_PI * 0.5f + 0.05f && RotY >= -D3DX_PI * 0.5f - 0.05f)
			{// -1.57fのとき

				pVtxMax->x = -VtxMin.z;
				pVtxMin->x = -VtxMax.z;

				pVtxMax->z = VtxMax.x;
				pVtxMin->z = VtxMin.x;
			}
			else if (RotY <= D3DX_PI + 0.05f && RotY >= D3DX_PI - 0.05f)
			{// D3DX_PIのとき

				pVtxMax->x = -VtxMin.x;
				pVtxMin->x = -VtxMax.x;

				pVtxMax->z = -VtxMin.z;
				pVtxMin->z = -VtxMax.z;
			}
			else if (RotY <= -D3DX_PI + 0.05f && RotY >= -D3DX_PI - 0.05f)
			{// -D3DX_PIのとき

				pVtxMax->x = -VtxMin.x;
				pVtxMin->x = -VtxMax.x;

				pVtxMax->z = -VtxMin.z;
				pVtxMin->z = -VtxMax.z;
			}
		}

		/**
		@brief	3頂点からなる高さ取得
		@param	pos			[in]	判定するオブジェクトの位置
		@param	posCenter	[in]	判定する三角形の手前の頂点座標
		@param	posLeft		[in]	判定する三角形の左の頂点座標
		@param	posRight	[in]	判定する三角形の右の頂点座標
		@param	posRight	[out]	着地判定
		@return	3頂点からなる高さ
		*/
		inline float GetVtxHeight(const MyLib::Vector3& pos, const MyLib::Vector3& posCenter, const MyLib::Vector3& posRight, const MyLib::Vector3& posLeft, bool* pLand)
		{
			// 高さ
			float fHeight = 0.0f;

			// 直角部分と位置のベクトル
			MyLib::Vector3 calvec = pos - posCenter;

			// ベクトルと法線
			MyLib::Vector3
				vec1 = posRight - posCenter,
				vec2 = posLeft - posRight,
				nor = MyLib::Vector3(0.0f, 0.0f, 0.0f);

			if (UtilFunc::Collision::CollisionTriangle(posCenter, posRight, posLeft, pos, pos))
			{// 三角に入っていたら

				// 外積を求める
				D3DXVec3Cross(&nor, &vec1, &vec2);

				// 外積の正規化をして法線にする
				D3DXVec3Normalize(&nor, &nor);

				if (nor.y != 0.0f)
				{// 法線が0.0fじゃなかったら

					// 高さを求める
					fHeight = -((calvec.x * nor.x) + (-posCenter.y * nor.y) + (calvec.z * nor.z)) / nor.y;
					*pLand = true;
				}
				else
				{
					// 高さを求める
					fHeight = -((calvec.x * nor.x) + (-posCenter.y * nor.y) + (calvec.z * nor.z)) / nor.y;
					*pLand = true;
				}
			}

			return fHeight;
		}

		/**
		@brief	入れ替え処理
		@param	pValue		[out]	ソートする値
		@param	nStart		[in]	ソートする値の初期値
		@param	nMax		[in]	ソートの最大値
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
		@brief	スプライント曲線補間
		@param	PointMili1		[in]	地点 -1
		@param	Point0			[in]	地点 0
		@param	Point1			[in]	地点 1
		@param	Point2			[in]	地点 2
		@param	ratio			[in]	割合
		@return	void
		*/
		inline MyLib::Vector3 CatmullRomSplineInterp(
			const MyLib::Vector3& PointMili1,
			const MyLib::Vector3& Point0,
			const MyLib::Vector3& Point1,
			const MyLib::Vector3& Point2,
			float ratio)
		{
			// 4点で補正
			MyLib::Vector3 a4 = Point0;
			MyLib::Vector3 a3 = (Point1 - PointMili1) / 2.0f;
			MyLib::Vector3 a1 = (Point2 - Point0) / 2.0f - 2.0f * Point1 + a3 + 2.0f * a4;
			MyLib::Vector3 a2 = 3.0f * Point1 - (Point2 - Point0) / 2.0f - 2.0f * a3 - 3.0f * a4;

			return (a1 * ratio * ratio * ratio + a2 * ratio * ratio + a3 * ratio + a4);
		}
	}

	/**
	@brief	当たり判定関数
	*/
	namespace Collision
	{
		/**
		@brief	1と2の当たり判定(円)(2D)
		@param	pos1		[in]	対象1の位置
		@param	pos2		[in]	対象2の位置
		@param	fRadius1	[in]	対象1の半径
		@param	fRadius2	[in]	対象2の半径
		@return	当たったかのbool値
		*/
		inline bool CircleRange2D(const MyLib::Vector3& pos1, const MyLib::Vector3& pos2, float fRadius1, float fRadius2)
		{
			float fLength =
				(pos1.x - pos2.x) * (pos1.x - pos2.x) +
				(pos1.y - pos2.y) * (pos1.y - pos2.y);

			if (fLength <= (fRadius1 + fRadius2) * (fRadius1 + fRadius2))
			{// 円の中に入ったら
				return true;
			}

			return false;
		}

		/**
		@brief	1と2の当たり判定(円)(3D)
		@param	pos1		[in]	対象1の位置
		@param	pos2		[in]	対象2の位置
		@param	fRadius1	[in]	対象1の半径
		@param	fRadius2	[in]	対象2の半径
		@return	当たったかのbool値
		*/
		inline bool CircleRange3D(const MyLib::Vector3& pos1, const MyLib::Vector3& pos2, float fRadius1, float fRadius2)
		{
			float fLength =
				(pos1.x - pos2.x) * (pos1.x - pos2.x) +
				(pos1.z - pos2.z) * (pos1.z - pos2.z);

			if (fLength <= (fRadius1 + fRadius2) * (fRadius1 + fRadius2))
			{// 円の中に入ったら

				MyLib::Vector3 collisionPoint = pos1 + (pos2 - pos1).Normal() * fRadius1;

				return true;
			}

			return false;
		}

		/**
		@brief	1と2の当たり判定(球)
		@param	pos1		[in]	対象1の位置
		@param	pos2		[in]	対象2の位置
		@param	fRadius1	[in]	対象1の半径
		@param	fRadius2	[in]	対象2の半径
		@return	当たったかのbool値
		*/
		inline MyLib::HitResult SphereRange(const MyLib::Vector3& pos1, const MyLib::Vector3& pos2, float fRadius1, float fRadius2)
		{
			float fLength =
				(pos1.x - pos2.x) * (pos1.x - pos2.x) +
				(pos1.y - pos2.y) * (pos1.y - pos2.y) +
				(pos1.z - pos2.z) * (pos1.z - pos2.z);

			if (fLength <= (fRadius1 + fRadius2) * (fRadius1 + fRadius2))
			{// 球の中に入ったら

				MyLib::Vector3 collisionVector = (pos2 - pos1).Normal() * fRadius1;
				MyLib::Vector3 collisionPoint = pos1 + collisionVector;
				return MyLib::HitResult(true, collisionPoint);
			}

			return MyLib::HitResult(false, MyLib::Vector3());
		}

		/**
		@brief	1と2の当たり判定(筒)
		@param	pos1		[in]	対象1の位置
		@param	pos2		[in]	対象2の位置
		@param	fRadius1	[in]	対象1の半径
		@param	fRadius2	[in]	対象2の半径
		@param	fHeight1	[in]	対象1の高さ
		@param	fHeight2	[in]	対象2の高さ
		@return	当たったかのbool値
		*/
		inline bool CylinderRange(MyLib::Vector3 pos1, MyLib::Vector3 pos2, float fRadius1, float fRadius2, float fHeight1, float fHeight2)
		{
			if (CircleRange3D(pos1, pos2, fHeight1, fHeight2) &&
				pos1.y + fHeight1 >= pos2.y &&
				pos1.y <= pos2.y + fHeight2)
			{// 円の中に入る && 高さもぶつかる
				return true;
			}

			return false;
		}

		/**
		@brief	キャラクター同士の当たり判定 & 補正
		@param	MainPos			[inout]	自分の位置
		@param	MainPosOld		[inout]	自分の前回の位置
		@param	MainVtxMax		[in]	自分の頂点最大値
		@param	MainVtxMin		[in]	自分の頂点最小値
		@param	TargetPos		[inout]	対象の位置
		@param	TargetVtxMax	[in]	対象の頂点最大値
		@param	TargetVtxMin	[in]	対象の頂点最小値
		@return	void
		*/
		inline void CollisionCharacter(MyLib::Vector3& MainPos, MyLib::Vector3& MainPosOld, MyLib::Vector3 MainVtxMax, MyLib::Vector3 MainVtxMin, MyLib::Vector3& TargetPos, MyLib::Vector3 TargetVtxMax, MyLib::Vector3 TargetVtxMin)
		{
			// 横の判定
			if (MainPos.z + MainVtxMax.z > TargetPos.z + TargetVtxMin.z &&	// 敵の最小値に入ってる
				MainPos.z + MainVtxMin.z < TargetPos.z + TargetVtxMax.z)	// 敵の最大値に入ってる
			{// 敵のZ幅の中にいるとき

				if (MainPos.x + MainVtxMin.x <= TargetPos.x + TargetVtxMax.x &&		// 現在の最小値が敵の最大値よりも小さい
					MainPosOld.x + MainVtxMin.x >= TargetPos.x + TargetVtxMax.x)	// 前回の最小値が敵の最大値よりも大きい
				{// 右から

					MainPos.x = (TargetPos.x + TargetVtxMax.x) - MainVtxMin.x + 0.1f;
				}
				if (MainPos.x + MainVtxMax.x >= TargetPos.x + TargetVtxMin.x &&		// 現在の最大値が敵の最小値よりも大きい
					MainPosOld.x + MainVtxMax.x <= TargetPos.x + TargetVtxMin.x)	// 前回の最大値が敵の最小値よりも小さい
				{// 左から

					MainPos.x = (TargetPos.x + TargetVtxMin.x) - MainVtxMax.x - 0.1f;
				}
			}

			// 縦の判定
			if (MainPos.x + MainVtxMax.x > TargetPos.x + TargetVtxMin.x &&	// 敵の最小値に入ってる
				MainPos.x + MainVtxMin.x < TargetPos.x + TargetVtxMax.x)	// 敵の最大値に入ってる
			{// 敵のX幅の中にいるとき

				if (MainPos.z + MainVtxMin.z <= TargetPos.z + TargetVtxMax.z &&		// 現在の最小値が敵の最大値よりも小さい
					MainPosOld.z + MainVtxMin.z >= TargetPos.z + TargetVtxMax.z)	// 前回の最小値が敵の最大値よりも大きい
				{// 奥から

					MainPos.z = (TargetPos.z + TargetVtxMax.z) - MainVtxMin.z + 0.1f;
				}
				if (MainPos.z + MainVtxMax.z >= TargetPos.z + TargetVtxMin.z &&		// 現在の最大値が敵の最小値よりも大きい
					MainPosOld.z + MainVtxMax.z <= TargetPos.z + TargetVtxMin.z)	// 前回の最大値が敵の最小値よりも小さい
				{// 手前から

					MainPos.z = (TargetPos.z + TargetVtxMin.z) - MainVtxMax.z - 0.1f;
				}
			}
		}

		/**
		@brief	キャラクター同士の当たり判定
		@param	MainPos			[in]	自分の位置
		@param	MainPosOld		[in]	自分の前回の位置
		@param	MainVtxMax		[in]	自分の頂点最大値
		@param	MainVtxMin		[in]	自分の頂点最小値
		@param	TargetPos		[in]	対象の位置
		@param	TargetVtxMax	[in]	対象の頂点最大値
		@param	TargetVtxMin	[in]	対象の頂点最小値
		@return	当たったかのbool値
		*/
		inline bool bHitCharacter(MyLib::Vector3 MainPos, MyLib::Vector3 MainPosOld, MyLib::Vector3 MainVtxMax, MyLib::Vector3 MainVtxMin, MyLib::Vector3 TargetPos, MyLib::Vector3 TargetVtxMax, MyLib::Vector3 TargetVtxMin)
		{
			// 横の判定
			if (MainPos.z + MainVtxMax.z > TargetPos.z + TargetVtxMin.z &&	// 敵の最小値に入ってる
				MainPos.z + MainVtxMin.z < TargetPos.z + TargetVtxMax.z)	// 敵の最大値に入ってる
			{// 敵のZ幅の中にいるとき

				if (MainPos.x + MainVtxMin.x <= TargetPos.x + TargetVtxMax.x &&	// 現在の最小値が敵の最大値よりも小さい
					MainPosOld.x + MainVtxMin.x >= TargetPos.x + TargetVtxMax.x)// 前回の最小値が敵の最大値よりも大きい
				{// 右から
					return true;
				}
				if (MainPos.x + MainVtxMax.x >= TargetPos.x + TargetVtxMin.x &&	// 現在の最大値が敵の最小値よりも大きい
					MainPosOld.x + MainVtxMax.x <= TargetPos.x + TargetVtxMin.x)// 前回の最大値が敵の最小値よりも小さい
				{// 左から
					return true;
				}
			}

			// 縦の判定
			if (MainPos.x + MainVtxMax.x > TargetPos.x + TargetVtxMin.x &&	// 敵の最小値に入ってる
				MainPos.x + MainVtxMin.x < TargetPos.x + TargetVtxMax.x)	// 敵の最大値に入ってる
			{// 敵のX幅の中にいるとき

				if (MainPos.z + MainVtxMin.z <= TargetPos.z + TargetVtxMax.z &&	// 現在の最小値が敵の最大値よりも小さい
					MainPosOld.z + MainVtxMin.z >= TargetPos.z + TargetVtxMax.z)// 前回の最小値が敵の最大値よりも大きい
				{// 奥から
					return true;
				}
				if (MainPos.z + MainVtxMax.z >= TargetPos.z + TargetVtxMin.z &&	// 現在の最大値が敵の最小値よりも大きい
					MainPosOld.z + MainVtxMax.z <= TargetPos.z + TargetVtxMin.z)// 前回の最大値が敵の最小値よりも小さい
				{// 手前から
					return true;
				}
			}

			return false;
		}

		/**
		@brief	線分の当たり判定
		@param	pos0		[in]	線分左端の位置
		@param	pos1		[in]	線分右端の位置
		@param	pPos		[inout]	自分の位置
		@param	posOld		[in]	自分の前回位置
		@return	void
		*/
		inline void CollisionLimitLine(const MyLib::Vector3& pos0, const MyLib::Vector3& pos1, MyLib::Vector3& pPos, MyLib::Vector3 posOld)
		{
			// 境界線のベクトル
			MyLib::Vector3 vecLine;
			vecLine.x = pos1.x - pos0.x;
			vecLine.z = pos1.z - pos0.z;

			// プレイヤーの境界線のベクトル
			MyLib::Vector3 vecLinePlayer;
			vecLinePlayer.x = pPos.x - posOld.x;
			vecLinePlayer.z = pPos.z - posOld.z;

			// プレイヤーと壁のベクトル
			MyLib::Vector3 vecToPosPlayer;
			vecToPosPlayer.x = pos1.x - posOld.x;
			vecToPosPlayer.z = pos1.z - posOld.z;

			// 面積の最大値
			float fMaxAreaPlayer = (vecLinePlayer.z * vecLine.x) - (vecLinePlayer.x * vecLine.z);

			// 今回の面積
			float fNowAreaPlayer = (vecToPosPlayer.z * vecLine.x) - (vecToPosPlayer.x * vecLine.z);

			// 割合
			float fRatePlayer = fNowAreaPlayer / fMaxAreaPlayer;

			if (fRatePlayer >= 0.0f && fRatePlayer <= 1.0f)
			{// 面積の範囲内にいたら判定

				if ((vecLinePlayer.z * vecToPosPlayer.x) - (vecLinePlayer.x * vecToPosPlayer.z) > 0)
				{// 壁に当たったら

					// 法線ベクトル(境界線ベクトルのXとZ反転)
					MyLib::Vector3 vecNor = MyLib::Vector3(vecLine.z, 0.0f, -vecLine.x);

					// ベクトルの正規化
					D3DXVec3Normalize(&vecNor, &vecNor);

					// プレイヤーの逆移動量
					MyLib::Vector3 PlayerInverceMove;
					PlayerInverceMove.x = vecLinePlayer.x * (fRatePlayer - 1.0f);
					PlayerInverceMove.z = vecLinePlayer.z * (fRatePlayer - 1.0f);

					// 内積(壁の法線とプレイヤーの逆移動量)
					float fDot = (PlayerInverceMove.x * vecNor.x) + (PlayerInverceMove.z * vecNor.z);

					// 壁ずり移動量
					MyLib::Vector3 MoveWall = vecNor * fDot;

					// ぶつかった点に補正
					pPos += MoveWall + (vecNor * 0.1f);

				}
			}
		}

		/**
		@brief	線分の右にいるかどうか(2D)
		@details 線分に対して右にいるかの判定なので、左に居て欲しい場合はpos0とpos1を入れ替えて使用
		@param	pos0		[in]	線分左端の位置
		@param	pos1		[in]	線分右端の位置
		@param	MainPos		[in]	判定する対象の位置
		@param	MainPos		[in]	判定する対象の前回の位置
		@return	線分の右側にいるかのbool値
		*/
		inline bool CollisionLine2D(const MyLib::Vector3& pos0, const MyLib::Vector3& pos1, const MyLib::Vector3& MainPos, const MyLib::Vector3& MainPosOld)
		{
			// 境界線のベクトル
			MyLib::Vector3 vecLine;
			vecLine.x = pos1.x - pos0.x;
			vecLine.y = pos0.y - pos1.y;

			// 弾と壁のベクトル
			MyLib::Vector3 vecToPos;
			vecToPos.x = MainPos.x - pos0.x;
			vecToPos.y = pos0.y - MainPos.y;

			if ((vecLine.y * vecToPos.x) - (vecLine.x * vecToPos.y) <= 0)
			{// 線を超えた
				return true;
			}

			return false;
		}

		/**
		@brief	線分の右にいるかどうか(3D)
		@details 線分に対して右にいるかの判定なので、左に居て欲しい場合はpos0とpos1を入れ替えて使用
		@param	pos0		[in]	線分左端の位置
		@param	pos1		[in]	線分右端の位置
		@param	MainPos		[in]	判定する対象の位置
		@param	MainPos		[in]	判定する対象の前回の位置
		@return	線分の右側にいるかのbool値
		*/
		inline bool CollisionLine3D(const MyLib::Vector3& pos0, const MyLib::Vector3& pos1, const MyLib::Vector3& MainPos, const MyLib::Vector3& MainPosOld)
		{
			// 境界線のベクトル
			MyLib::Vector3 vecLine;
			vecLine.x = pos1.x - pos0.x;
			vecLine.z = pos1.z - pos0.z;

			// 弾と壁のベクトル
			MyLib::Vector3 vecToPos;
			vecToPos.x = MainPos.x - pos0.x;
			vecToPos.z = MainPos.z - pos0.z;

			if ((vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) <= 0)
			{// 線を超えた
				return true;
			}

			return false;
		}

		/**
		@brief	三角の内側にいるか判定
		@param	posCenter	[in]	判定する三角形の手前の頂点座標
		@param	posLeft		[in]	判定する三角形の左の頂点座標
		@param	posRight	[in]	判定する三角形の右の頂点座標
		@param	MainPos		[in]	自分の位置
		@param	MainPosOld	[in]	自分の前回位置
		@return	中にいるかのbool値
		*/
		inline bool CollisionTriangle(const MyLib::Vector3& posCenter, const MyLib::Vector3& posLeft, const MyLib::Vector3& posRight, const MyLib::Vector3& MainPos, const MyLib::Vector3& MainPosOld)
		{
			// 当たったかの判定
			bool bHit = false;
			bool bLine1 = false, bLine2 = false, bLine3 = false;

			// 真ん中と左の線
			bLine1 = UtilFunc::Collision::CollisionLine3D(posCenter, posLeft, MainPos, MainPosOld);

			// 左と右の線
			bLine2 = UtilFunc::Collision::CollisionLine3D(posLeft, posRight, MainPos, MainPosOld);

			// 右と真ん中の線
			bLine3 = UtilFunc::Collision::CollisionLine3D(posRight, posCenter, MainPos, MainPosOld);

			// 全ての範囲に入っていたら
			if (bLine1 && bLine2 && bLine3)
			{
				bHit = true;
			}

			return bHit;
		}

		/**
		@brief	四角の内側にいるか判定
		@details 4つの頂点が全て決まっている場合に使う
		@param	posLeftUP	[in]	判定する四角形左上の頂点座標
		@param	posRightUP	[in]	判定する四角形右上の頂点座標
		@param	posLeftDW	[in]	判定する四角形左下の頂点座標
		@param	posRightDW	[in]	判定する四角形右下の頂点座標
		@param	MainPos		[in]	自分の位置
		@return	中にいるかのbool値
		*/
		inline bool CollisionSquare(const MyLib::Vector3& posLeftUP, const MyLib::Vector3& posRightUP, const MyLib::Vector3& posLeftDW, const MyLib::Vector3& posRightDW, const MyLib::Vector3& MainPos)
		{
			// 当たったかの判定
			bool bHit = false;
			bool bLine1 = false, bLine2 = false, bLine3 = false, bLine4 = false;

			// 左上と右上
			bLine1 = UtilFunc::Collision::CollisionLine3D(posRightUP, posLeftUP, MainPos, MainPos);

			// 右上と右下
			bLine2 = UtilFunc::Collision::CollisionLine3D(posRightDW, posRightUP, MainPos, MainPos);

			// 右下と左下
			bLine3 = UtilFunc::Collision::CollisionLine3D(posLeftDW, posRightDW, MainPos, MainPos);

			// 左下と左上
			bLine4 = UtilFunc::Collision::CollisionLine3D(posLeftUP, posLeftDW, MainPos, MainPos);

			// 全ての範囲に入っていたら
			if (bLine1 && bLine2 && bLine3 && bLine4)
			{
				// 当たった
				bHit = true;
			}

			return bHit;
		}

		/**
		@brief	四角の内側にいるか判定
		@details 4つの頂点がまだ求められていない場合に使う
		@param	posSquare	[in]	判定する四角形の中心座標
		@param	SquareSize	[in]	判定する四角形のサイズ
		@param	fSquareRot	[in]	判定する四角形のサイズ
		@param	MainPos		[in]	自分の位置
		@return	中にいるかのbool値
		*/
		inline bool CollisionSquare(MyLib::Vector3 posSquare, D3DXVECTOR2 SquareSize, float fSquareRot, MyLib::Vector3 MainPos)
		{
			// 当たったかの判定
			bool bHit = false;
			bool bLine1 = false, bLine2 = false, bLine3 = false, bLine4 = false;

			float fLength = sqrtf(SquareSize.x * SquareSize.x + SquareSize.y * SquareSize.y);	// 対角線の長さ
			float fAngle = atan2f(SquareSize.x, SquareSize.y);									// 対角線の向き

			// 判定する四角の4頂点
			MyLib::Vector3 LeftUp = MyLib::Vector3(posSquare.x + sinf(fSquareRot - fAngle) * fLength, posSquare.y, posSquare.z + cosf(fSquareRot - fAngle) * fLength);
			MyLib::Vector3 RightUp = MyLib::Vector3(posSquare.x + sinf(fSquareRot + fAngle) * fLength, posSquare.y, posSquare.z + cosf(fSquareRot + fAngle) * fLength);
			MyLib::Vector3 LeftDown = MyLib::Vector3(posSquare.x + sinf(fSquareRot - D3DX_PI + fAngle) * fLength, posSquare.y, posSquare.z + cosf(fSquareRot - D3DX_PI + fAngle) * fLength);
			MyLib::Vector3 RightDown = MyLib::Vector3(posSquare.x + sinf(fSquareRot + D3DX_PI - fAngle) * fLength, posSquare.y, posSquare.z + cosf(fSquareRot + D3DX_PI - fAngle) * fLength);

			// 左上と右上
			bLine1 = UtilFunc::Collision::CollisionLine3D(RightUp, LeftUp, MainPos, MainPos);

			// 右上と右下
			bLine2 = UtilFunc::Collision::CollisionLine3D(RightDown, RightUp, MainPos, MainPos);

			// 右下と左下
			bLine3 = UtilFunc::Collision::CollisionLine3D(LeftDown, RightDown, MainPos, MainPos);

			// 左下と左上
			bLine4 = UtilFunc::Collision::CollisionLine3D(LeftUp, LeftDown, MainPos, MainPos);

			// 全ての範囲に入っていたら
			if (bLine1 && bLine2 && bLine3 && bLine4)
			{
				// 当たった
				bHit = true;
			}

			return bHit;
		}

		/**
		@brief	四角の内側にいるか判定(2D)
		@details 4つの頂点が全て決まっている場合に使う
		@param	posLeftUP	[in]	判定する四角形左上の頂点座標
		@param	posRightUP	[in]	判定する四角形右上の頂点座標
		@param	posLeftDW	[in]	判定する四角形左下の頂点座標
		@param	posRightDW	[in]	判定する四角形右下の頂点座標
		@param	MainPos		[in]	自分の位置
		@return	中にいるかのbool値
		*/
		inline bool CollisionSquare2D(MyLib::Vector3 posLeftUP, MyLib::Vector3 posRightUP, MyLib::Vector3 posLeftDW, MyLib::Vector3 posRightDW, MyLib::Vector3 MainPos)
		{
			// 当たったかの判定
			bool bHit = false;
			bool bLine1 = false, bLine2 = false, bLine3 = false, bLine4 = false;

			// 左上と右上
			bLine1 = UtilFunc::Collision::CollisionLine2D(posRightUP, posLeftUP, MainPos, MainPos);

			// 右上と右下
			bLine2 = UtilFunc::Collision::CollisionLine2D(posRightDW, posRightUP, MainPos, MainPos);

			// 右下と左下
			bLine3 = UtilFunc::Collision::CollisionLine2D(posLeftDW, posRightDW, MainPos, MainPos);

			// 左下と左上
			bLine4 = UtilFunc::Collision::CollisionLine2D(posLeftUP, posLeftDW, MainPos, MainPos);

			// 全ての範囲に入っていたら
			if (bLine1 && bLine2 && bLine3 && bLine4)
			{
				bHit = true;
			}

			return bHit;
		}

		/**
		@brief	四角と四角の当たり判定(2D)
		@details 4つの頂点がまだ求められていない場合に使う
		@param	MainPos		[inout]	自分の位置
		@param	TargetPos	[inout]	対象の位置
		@param	MainSize	[in]	自分のサイズ
		@param	TargetSize	[in]	対象のサイズ
		@param	fMainRot	[in]	自分のY軸の向き
		@param	fTargetRot	[in]	対象のY軸の向き
		@return	衝突したかのbool値
		*/
		inline bool CollisionSquareSquare2D(MyLib::Vector3& MainPos, MyLib::Vector3& TargetPos, D3DXVECTOR2 MainSize, D3DXVECTOR2 TargetSize, float fMainRot, float fTargetRot)
		{
			float fTargetLength = sqrtf(
				TargetSize.x * TargetSize.x +
				TargetSize.y * TargetSize.y);							// 対角線の長さ
			float fTargetAngle = atan2f(TargetSize.x, TargetSize.y);	// 対角線の向き

			// 判定する四角の4頂点
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
			// 矩形の判定
			//***********************
			if (UtilFunc::Collision::CollisionSquare(MainPos, MainSize, fMainRot, LeftUp))
			{// 左上
				return true;
			}
			if (UtilFunc::Collision::CollisionSquare(MainPos, MainSize, fMainRot, RightUp))
			{// 右上
				return true;
			}
			if (UtilFunc::Collision::CollisionSquare(MainPos, MainSize, fMainRot, LeftDown))
			{// 左下
				return true;
			}
			if (UtilFunc::Collision::CollisionSquare(MainPos, MainSize, fMainRot, RightDown))
			{// 右下
				return true;
			}

			return false;
		}

		/**
		@brief	円と矩形の当たり判定(2D)
		@param	posCircle		[inout]	円の中心座標
		@param	posSquare		[inout]	矩形の中心座標
		@param	rotSquare		[in]	矩形の向き
		@param	fCircleRadius	[in]	円の半径
		@param	SquareSize		[in]	矩形のサイズ
		@return	衝突したかのbool値
		*/
		inline bool CollisionCircleSquare2D(MyLib::Vector3& posCircle, MyLib::Vector3& posSquare, MyLib::Vector3 rotSquare, float fCircleRadius, D3DXVECTOR2 SquareSize)
		{
			float fSquareSizeX = SquareSize.x;	// 矩形のサイズX
			float fSquareSizeY = SquareSize.y;	// 矩形のサイズY

			float fLength = 0.0f;	// 対角線の長さ
			float fAngle = 0.0f;	// 対角線の向き

			MyLib::Vector3 LeftUp, RightUp, LeftDown, RightDown;

			//****************************************************
			// Xのサイズに円の半径を足した矩形の判定
			//****************************************************
			fSquareSizeX += fCircleRadius;	// 矩形のサイズX
			fSquareSizeY = SquareSize.y;	// 矩形のサイズY

			fLength = sqrtf(fSquareSizeX * fSquareSizeX + fSquareSizeY * fSquareSizeY);	// 対角線の長さ
			fAngle = atan2f(fSquareSizeX, fSquareSizeY);								// 対角線の向き

			// 判定する四角の4頂点
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

			// 矩形の判定
			if (UtilFunc::Collision::CollisionSquare2D(LeftUp, RightUp, LeftDown, RightDown, posCircle))
			{// 矩形に当たった場合
				return true;
			}

			//****************************************************
			// Yのサイズに円の半径を足した矩形の判定
			//****************************************************
			fSquareSizeX = SquareSize.x;	// 矩形のサイズX
			fSquareSizeY += fCircleRadius;	// 矩形のサイズY

			fLength = sqrtf(fSquareSizeX * fSquareSizeX + fSquareSizeY * fSquareSizeY);	// 対角線の長さ
			fAngle = atan2f(fSquareSizeX, fSquareSizeY);								// 対角線の向き

			// 判定する四角の4頂点
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

			// 矩形の判定
			if (UtilFunc::Collision::CollisionSquare2D(LeftUp, RightUp, LeftDown, RightDown, posCircle))
			{// 矩形に当たった場合
				return true;
			}

			//***********************
			// 各頂点毎の円の判定
			//***********************
			//****************************************************
			// 元のサイズ
			//****************************************************
			fSquareSizeX = SquareSize.x;	// 矩形のサイズX
			fSquareSizeY = SquareSize.y;	// 矩形のサイズY

			fLength = sqrtf(fSquareSizeX * fSquareSizeX + fSquareSizeY * fSquareSizeY);	// 対角線の長さ
			fAngle = atan2f(fSquareSizeX, fSquareSizeY);								// 対角線の向き

			// 判定する四角の4頂点
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
			{// 左上
				return true;
			}
			if (UtilFunc::Collision::CircleRange2D(posCircle, RightUp, fCircleRadius, 0.0f))
			{// 右上
				return true;
			}
			if (UtilFunc::Collision::CircleRange2D(posCircle, LeftDown, fCircleRadius, 0.0f))
			{// 左下
				return true;
			}
			if (UtilFunc::Collision::CircleRange2D(posCircle, RightDown, fCircleRadius, 0.0f))
			{// 右下
				return true;
			}

			// 当たってない判定を返す
			return false;
		}

		/**
		@brief	円と矩形の当たり判定(3D)
		@param	posCircle		[inout]	円の中心座標
		@param	posSquare		[inout]	矩形の中心座標
		@param	rotSquare		[in]	矩形の向き
		@param	fCircleRadius	[in]	円の半径
		@param	SquareSize		[in]	矩形のサイズ
		@return	衝突したかのbool値
		*/
		inline bool CollisionCircleSquare3D(MyLib::Vector3& posCircle, MyLib::Vector3& posSquare, MyLib::Vector3 rotSquare, float fCircleRadius, D3DXVECTOR2 SquareSize)
		{
			float fSquareSizeX = SquareSize.x;	// 矩形のサイズX
			float fSquareSizeY = SquareSize.y;	// 矩形のサイズY

			float fLength = 0.0f;	// 対角線の長さ
			float fAngle = 0.0f;	// 対角線の向き

			MyLib::Vector3 LeftUp, RightUp, LeftDown, RightDown;

			//****************************************************
			// Xのサイズに円の半径を足した矩形の判定
			//****************************************************
			fSquareSizeX += fCircleRadius;	// 矩形のサイズX
			fSquareSizeY = SquareSize.y;	// 矩形のサイズY

			fLength = sqrtf(fSquareSizeX * fSquareSizeX + fSquareSizeY * fSquareSizeY);	// 対角線の長さ
			fAngle = atan2f(fSquareSizeX, fSquareSizeY);								// 対角線の向き

			// 判定する四角の4頂点
			LeftUp = MyLib::Vector3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y - fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y - fAngle) * fLength);
			RightUp = MyLib::Vector3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y + fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y + fAngle) * fLength);
			LeftDown = MyLib::Vector3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y - D3DX_PI + fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y - D3DX_PI + fAngle) * fLength);
			RightDown = MyLib::Vector3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y + D3DX_PI - fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y + D3DX_PI - fAngle) * fLength);

			// 矩形の判定
			if (UtilFunc::Collision::CollisionSquare(LeftUp, RightUp, LeftDown, RightDown, posCircle))
			{// 矩形に当たった場合
				return true;
			}

			//****************************************************
			// Yのサイズに円の半径を足した矩形の判定
			//****************************************************
			fSquareSizeX = SquareSize.x;	// 矩形のサイズX
			fSquareSizeY += fCircleRadius;	// 矩形のサイズY

			fLength = sqrtf(fSquareSizeX * fSquareSizeX + fSquareSizeY * fSquareSizeY);	// 対角線の長さ
			fAngle = atan2f(fSquareSizeX, fSquareSizeY);								// 対角線の向き

			// 判定する四角の4頂点
			LeftUp = MyLib::Vector3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y - fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y - fAngle) * fLength);
			RightUp = MyLib::Vector3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y + fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y + fAngle) * fLength);
			LeftDown = MyLib::Vector3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y - D3DX_PI + fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y - D3DX_PI + fAngle) * fLength);
			RightDown = MyLib::Vector3(posSquare.x + cosf(rotSquare.x) * sinf(rotSquare.y + D3DX_PI - fAngle) * fLength, posSquare.y, posSquare.z + cosf(rotSquare.x) * cosf(rotSquare.y + D3DX_PI - fAngle) * fLength);

			// 矩形の判定
			if (UtilFunc::Collision::CollisionSquare(LeftUp, RightUp, LeftDown, RightDown, posCircle))
			{// 矩形に当たった場合
				return true;
			}

			//***********************
			// 各頂点毎の円の判定
			//***********************
			if (UtilFunc::Collision::CircleRange3D(posCircle, LeftUp, fCircleRadius, fCircleRadius))
			{// 左上
				return true;
			}
			if (UtilFunc::Collision::CircleRange3D(posCircle, RightUp, fCircleRadius, fCircleRadius))
			{// 右上
				return true;
			}
			if (UtilFunc::Collision::CircleRange3D(posCircle, LeftDown, fCircleRadius, fCircleRadius))
			{// 左下
				return true;
			}
			if (UtilFunc::Collision::CircleRange3D(posCircle, RightDown, fCircleRadius, fCircleRadius))
			{// 右下
				return true;
			}

			// 当たってない判定を返す
			return false;
		}

		/**
		@brief	扇の当たり判定(3D)
		@param	posCenter		[in]	扇の始点頂点座標
		@param	posLeft			[in]	扇の左側頂点座標
		@param	posRight		[in]	扇の右側頂点座標
		@param	TargetPos		[in]	判定する対象の位置
		@param	fTargetRadius	[in]	判定する対象の半径
		@return	衝突したかのbool値
		*/
		inline bool CollisionFan3D(MyLib::Vector3 posCenter, MyLib::Vector3 posLeft, MyLib::Vector3 posRight, MyLib::Vector3 TargetPos, float fTargetRadius)
		{
			// 当たったかの判定
			bool bHit = false;
			bool bLine1 = false, bLine2 = false, bCircle = false;

			// 左の点との線分
			bLine1 = UtilFunc::Collision::CollisionLine3D(posLeft, posCenter, TargetPos, TargetPos);

			// 右の点との線分
			bLine2 = UtilFunc::Collision::CollisionLine3D(posCenter, posRight, TargetPos, TargetPos);

			// 扇の半径
			float fLen =
				sqrtf((posLeft.x - posCenter.x) * (posLeft.x - posCenter.x)
					+ (posLeft.z - posCenter.z) * (posLeft.z - posCenter.z));

			// 円の当たり判定
			if (UtilFunc::Collision::CircleRange3D(posCenter, TargetPos, fLen, fTargetRadius))
			{// 当たっていたら
				bCircle = true;
			}

			// 全ての範囲に入っていたら
			if (bLine1 && bLine2 && bCircle)
			{
				bHit = true;
			}

			return bHit;
		}

		/**
		@brief	向きの範囲内判定
		@param	angle		[in]	判定する向き
		@param	maxAngle	[in]	判定内の最大向き
		@param	minAngle	[in]	判定内の最小向き
		@return	衝突したかのbool値
		*/
		inline bool CollisionRangeAngle(float angle, float maxAngle, float minAngle)
		{
			return (angle <= maxAngle && angle >= minAngle);
		}

		/**
		@brief	点が三角形の内部にあるかを判定するヘルパー関数
		@param	p	[in]	判定する点
		@param	a	[in]	三角形の頂点1
		@param	b	[in]	三角形の頂点2
		@param	c	[in]	三角形の頂点3
		@return	trueなら内部、falseなら外部
		*/
		inline bool PointInTriangle(const MyLib::Vector3& p, const MyLib::Vector3& a, const MyLib::Vector3& b, const MyLib::Vector3& c) 
		{
			MyLib::Vector3 v0 = c - a;
			MyLib::Vector3 v1 = b - a;
			MyLib::Vector3 v2 = p - a;

			// バリセントリック座標を使った三角形内判定
			float dot00 = D3DXVec3Dot(&v0, &v0);
			float dot01 = D3DXVec3Dot(&v0, &v1);
			float dot02 = D3DXVec3Dot(&v0, &v2);
			float dot11 = D3DXVec3Dot(&v1, &v1);
			float dot12 = D3DXVec3Dot(&v1, &v2);

			float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
			float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
			float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

			// u >= 0, v >= 0, u + v <= 1 なら三角形の内部
			return (u >= 0) && (v >= 0) && (u + v <= 1);
		}

		/**
		@brief	視界内の判定(3D)
		@param	posMain			[in]	中心となる人の位置
		@param	posTarget		[in]	対象の位置
		@param	mainRotY		[in]	向き
		@param	fieldofview		[in]	視野角
		@return	衝突したかのbool値
		*/
		inline bool CollisionViewRange3D(const MyLib::Vector3& posMain, MyLib::Vector3& posTarget, float mainRotY, float fieldofview)
		{

			// 2キャラのベクトル
			D3DXVECTOR3 toPlayer = posTarget - posMain;

			// 前方ベクトル
			D3DXVECTOR3 forward = UtilFunc::Transformation::RotationChangeToForwardVector(mainRotY);

			// ベクトルと前方ベクトルの角度計算
			float angle = D3DXVec3Dot(&toPlayer, &forward) / (D3DXVec3Length(&toPlayer) * D3DXVec3Length(&forward));
			angle = acos(angle);

			// 視界の中にいるか判定
			fieldofview = D3DXToRadian(fieldofview);
			if (angle <= fieldofview / 2.0f)
			{
				return true;
			}

			return false;
		}

		/**
		@brief	レイと球の衝突判定(3D)
		@param	rayPos				[in]	レイの始点
		@param	rayVec				[in]	レイの方向ベクトル
		@param	spherePos			[in]	球の中心点
		@param	sphereRadius		[in]	球の半径
		@return	衝突判定
		*/
		inline bool CollisionRaySphere(const MyLib::Vector3& rayPos, const MyLib::Vector3& rayVec,
			const MyLib::Vector3& spherePos, float sphereRadius)
		{
			// 変換後の球座標
			MyLib::Vector3 sphereCoord;
			sphereCoord.x = rayPos.x - spherePos.x;
			sphereCoord.y = rayPos.y - spherePos.y;
			sphereCoord.z = rayPos.z - spherePos.z;

			float A = rayVec.Dot(rayVec);
			float B = sphereCoord.Dot(rayVec) * 2.0f;
			float C = sphereCoord.Dot(rayVec) - (sphereRadius * sphereRadius);

			if (A == 0.0f)
				return false; // レイの長さが0

			float s = B * B - 4 * A * C;
			if (s < 0.0f)
				return false; // 衝突していない

			s = sqrtf(s);
			float a1 = (B - s) / A;
			float a2 = (B + s) / A;

			if (a1 < 0.0f || a2 < 0.0f)
				return false; // レイの反対で衝突

			return true;
		}

		/**
		@brief	レイと球の衝突判定(3D)
		@param	rayPos				[in]	レイの始点
		@param	rayVec				[in]	レイの方向ベクトル
		@param	spherePos			[in]	球の中心点
		@param	sphereRadius		[in]	球の半径
		@param	collisionStartPoint	[in]	衝突開始点
		@param	collisionEndPoint	[in]	衝突終了点
		@return	衝突判定
		*/
		inline bool CollisionRaySphere(const MyLib::Vector3& rayPos, const MyLib::Vector3& rayVec, const MyLib::Vector3& spherePos,
			float sphereRadius, MyLib::Vector3& collisionStartPoint, MyLib::Vector3& collisionEndPoint) 
		{
			// 変換後の球座標
			MyLib::Vector3 sphereCoord;
			sphereCoord.x = spherePos.x - rayPos.x;
			sphereCoord.y = spherePos.y - rayPos.y;
			sphereCoord.z = spherePos.z - rayPos.z;

			float A = rayVec.x * rayVec.x + rayVec.y * rayVec.y + rayVec.z * rayVec.z;
			float B = rayVec.x * sphereCoord.x + rayVec.y * sphereCoord.y + rayVec.z * sphereCoord.z;
			float C = sphereCoord.x * sphereCoord.x + sphereCoord.y * sphereCoord.y + sphereCoord.z * sphereCoord.z - sphereRadius * sphereRadius;

			if (A == 0.0f)
				return false; // レイの長さが0

			float s = B * B - A * C;
			if (s < 0.0f)
				return false; // 衝突していない

			s = sqrtf(s);
			float a1 = (B - s) / A;
			float a2 = (B + s) / A;

			if (a1 < 0.0f || a2 < 0.0f)
				return false; // レイの反対で衝突

			// 衝突開始・終了地点割り出し
			collisionStartPoint.x = rayPos.x + a1 * rayVec.x;
			collisionStartPoint.y = rayPos.y + a1 * rayVec.y;
			collisionStartPoint.z = rayPos.z + a1 * rayVec.z;
			collisionEndPoint.x = rayPos.x + a2 * rayVec.x;
			collisionEndPoint.y = rayPos.y + a2 * rayVec.y;
			collisionEndPoint.z = rayPos.z + a2 * rayVec.z;

			return true;
		}

		/**
		@brief	レイと直方体の衝突判定(3D)
		@param	pos		[in]	レイの始点
		@param	dir_w	[in]	レイの方向ベクトル
		@param	aabb	[in]	境界ボックス（ローカル）
		@param	mat		[in]	境界ボックスのワールド変換行列
		@param	t		[in]	衝突間隔（出力）
		@return		衝突判定
		*/
		inline bool CollisionRayAABB(MyLib::Vector3* pos, MyLib::Vector3* dir_w, MyLib::AABB* aabb, MyLib::Matrix* mtx, float& t, MyLib::Vector3* colPos = 0)
		{

			// レイを境界ボックスの位置へ移動
			D3DXMATRIX invMat, trans = mtx->ConvertD3DXMATRIX();
			D3DXMatrixInverse(&invMat, 0, &trans);

			MyLib::Vector3 p_l, dir_l;
			D3DXVec3TransformCoord(&p_l, pos, &invMat);
			invMat._41 = 0.0f;
			invMat._42 = 0.0f;
			invMat._43 = 0.0f;
			D3DXVec3TransformCoord(&dir_l, dir_w, &invMat);

			// 交差判定
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
						return false;	// 交差していない
				}
				else 
				{
					// スラブとの距離を算出
					// t1が近スラブ、t2が遠スラブとの距離
					float odd = 1.0f / d[i];
					float t1 = (min[i] - p[i]) * odd;
					float t2 = (max[i] - p[i]) * odd;

					if (t1 > t2) {
						float tmp = t1; t1 = t2; t2 = tmp;
					}

					if (t1 > t) t = t1;
					if (t2 < t_max) t_max = t2;

					// スラブ交差チェック
					if (t >= t_max)
						return false;
				}
			}

			// 交差している
			if (colPos) {
				*colPos = *pos + t * (*dir_w);
			}

			return true;
		}


		/**
		@brief	レイと4点から作られる平面の衝突判定(3D)
		@param	pos		[in]	レイの始点
		@param	dir_w	[in]	レイの方向ベクトル
		@param	v0		[in]	四角形の1点目（反時計回りで定義）
		@param	v1		[in]	四角形の2点目
		@param	v2		[in]	四角形の3点目
		@param	v3		[in]	四角形の4点目
		@param	t		[out]	衝突までの距離（レイの長さに比例）
		@param	colPos	[out]	衝突点座標
		@param	colPos	[out]	衝突点座標
		@return	衝突判定
		*/
		inline bool CollisionRayQuad(MyLib::Vector3* pos, MyLib::Vector3* dir_w, MyLib::Vector3* v0, MyLib::Vector3* v1, MyLib::Vector3* v2, MyLib::Vector3* v3, float& t, MyLib::Vector3* colPos = 0) 
		{
			// 四角形の面を構成する3つの頂点から法線を求める（反時計回り順）
			MyLib::Vector3 edge1 = *v1 - *v0;
			MyLib::Vector3 edge2 = *v2 - *v0;
			MyLib::Vector3 normal;
			D3DXVec3Cross(&normal, &edge1, &edge2); // 外積で法線を求める
			D3DXVec3Normalize(&normal, &normal);    // 正規化

			// 平面方程式の係数を求める
			float d = -D3DXVec3Dot(&normal, v0);

			// レイの方向ベクトルと法線の内積を求める
			float dotDirNormal = D3DXVec3Dot(dir_w, &normal);
			if (abs(dotDirNormal) < FLT_EPSILON) 
			{
				// レイが平面と平行な場合、衝突しない
				return false;
			}

			// レイと平面の交点を求める
			float dotPosNormal = D3DXVec3Dot(pos, &normal);
			t = -(dotPosNormal + d) / dotDirNormal;

			if (t < 0.0f) 
			{
				// レイが平面の後ろにある場合、衝突しない
				return false;
			}

			// 交点を計算
			MyLib::Vector3 intersection = *pos + t * (*dir_w);

			// 交点が四角形の内部にあるかをチェック
			// 四角形を2つの三角形に分割し、それぞれの三角形の内部にあるかを判定

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
		@brief	円とAABBの当たり判定
		@param	circleCenter		[in]	円の中心位置
		@param	radius				[in]	半径
		@param	aabb				[in]	境界ボックス（ローカル）
		@param	crossPosition		[inout]	交点
		@return	void
		*/
		inline bool CollisionCircleToAABB(const MyLib::Vector3& circleCenter, const float radius, MyLib::AABB aabb, MyLib::Vector3& crossPosition)
		{
			// AABBの中心
			MyLib::Vector3 aabbCenter = {
				(aabb.vtxMin.x + aabb.vtxMax.x) * 0.5f,
				(aabb.vtxMin.y + aabb.vtxMax.y) * 0.5f,
				(aabb.vtxMin.z + aabb.vtxMax.z) * 0.5f
			};

			// AABBの半幅
			MyLib::Vector3 aabbHalfExtents = {
				(aabb.vtxMax.x - aabb.vtxMin.x) * 0.5f,
				(aabb.vtxMax.y - aabb.vtxMin.y) * 0.5f,
				(aabb.vtxMax.z - aabb.vtxMin.z) * 0.5f
			};

			// AABBと円の中心点の距離の絶対値
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

			// 円がAABBの中にある場合
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
		@brief	円とAABBの当たり判定
		@param	circleCenter	[in]	円の中心位置
		@param	radius			[in]	半径
		@param	aabb			[in]	境界ボックス（ローカル）
		@param	aabbMatrix		[in]	AABBのワールドマトリックス
		@return	void
		*/
		inline bool CollisionCircleToAABB(MyLib::Vector3& circleCenter, const float radius, MyLib::AABB aabb, MyLib::Matrix aabbMatrix)
		{
			// AABBの中心
			MyLib::Vector3 aabbCenter = {
				(aabb.vtxMin.x + aabb.vtxMax.x) * 0.5f,
				(aabb.vtxMin.y + aabb.vtxMax.y) * 0.5f,
				(aabb.vtxMin.z + aabb.vtxMax.z) * 0.5f
			};
			aabbCenter += aabbMatrix.GetWorldPosition();

			// AABBのスケーリングを考慮して中心を計算
			aabbCenter.x = aabbMatrix.GetWorldPosition().x + aabbCenter.x * aabbMatrix.GetWorldPosition().x;
			aabbCenter.y = aabbMatrix.GetWorldPosition().y + aabbCenter.y * aabbMatrix.GetWorldPosition().y;
			aabbCenter.z = aabbMatrix.GetWorldPosition().z + aabbCenter.z * aabbMatrix.GetWorldPosition().z;

			// AABBの半幅
			MyLib::Vector3 aabbHalfExtents = {
				(aabb.vtxMax.x - aabb.vtxMin.x) * 0.5f,
				(aabb.vtxMax.y - aabb.vtxMin.y) * 0.5f,
				(aabb.vtxMax.z - aabb.vtxMin.z) * 0.5f
			};

			// AABBのスケーリングを考慮して半幅を計算
			D3DXVECTOR3 scale = aabbMatrix.GetScale();
			aabbHalfExtents.x *= scale.x;
			aabbHalfExtents.y *= scale.y;
			aabbHalfExtents.z *= scale.z;

			// AABBのローカル空間に円の位置を変換
			MyLib::Vector3 localCenter, diffCenter = { circleCenter.x - aabbCenter.x, circleCenter.y - aabbCenter.y, circleCenter.z - aabbCenter.z };
			localCenter = aabbMatrix.Coord(diffCenter);

			// AABBのローカル空間内での最も近い点の座標を計算
			float closestPointX = UtilFunc::Transformation::Clamp(localCenter.x, -aabbHalfExtents.x, aabbHalfExtents.x);
			float closestPointY = UtilFunc::Transformation::Clamp(localCenter.y, -aabbHalfExtents.y, aabbHalfExtents.y);
			float closestPointZ = UtilFunc::Transformation::Clamp(localCenter.z, -aabbHalfExtents.z, aabbHalfExtents.z);

			// 最も近い点と円の中心点の距離の2乗
			float distanceSquared = 
				powf(closestPointX - localCenter.x, 2) +
				powf(closestPointY - localCenter.y, 2) +
				powf(closestPointZ - localCenter.z, 2);

			// 内積を計算
			float dotProduct = 
				(closestPointX - localCenter.x) * (aabb.vtxMax.x - aabb.vtxMin.x) +
				(closestPointY - localCenter.y) * (aabb.vtxMax.y - aabb.vtxMin.y) +
				(closestPointZ - localCenter.z) * (aabb.vtxMax.z - aabb.vtxMin.z);


			// 円とAABBの衝突判定が発生した場合の処理
			if (distanceSquared <= radius * radius)
			{
				// 最も近い点を押し戻しベクトルとして使用する
				MyLib::Vector3 pushBackVector = { 0.0f, 0.0f, 0.0f };

				// 最も近い点が円の外側にある場合、押し戻しベクトルは円の中心から最も近い点へ向かうベクトルになる
				if (distanceSquared > 0.0f)
				{
					// 最も近い点を押し戻しベクトルとして使用する
					MyLib::Vector3 pushBackVector = { 0.0f, 0.0f, 0.0f };

					// 最も近い点がAABBの辺上にある場合は押し戻しを行わない
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

					// 押し戻しベクトルをAABBのローカル空間から元の座標系に変換
					D3DXMATRIX inverseAABBMatrix, invAABBMTX = aabbMatrix.ConvertD3DXMATRIX();
					D3DXMatrixInverse(&inverseAABBMatrix, nullptr, &invAABBMTX);
					D3DXVec3TransformCoord(&pushBackVector, &pushBackVector, &inverseAABBMatrix);

					// 円の中心を押し戻す
					circleCenter.x += pushBackVector.x;
					circleCenter.y += pushBackVector.y;
					circleCenter.z += pushBackVector.z;
				}

				return true;
			}

			return false;
		}


		// 回転するAABBとボックスの当たり判定を行う関数
		inline bool checkCollision(MyLib::AABB aabb, const MyLib::Vector3& boxCenter, const MyLib::Vector3& boxSize, const D3DXMATRIX& rotationMatrix)
		{
			// AABBの中心
			MyLib::Vector3 aabbCenter = {
				(aabb.vtxMin.x + aabb.vtxMax.x) * 0.5f,
				(aabb.vtxMin.y + aabb.vtxMax.y) * 0.5f,
				(aabb.vtxMin.z + aabb.vtxMax.z) * 0.5f
			};

			// AABBの半幅
			MyLib::Vector3 aabbSize = {
				(aabb.vtxMax.x - aabb.vtxMin.x) * 0.5f,
				(aabb.vtxMax.y - aabb.vtxMin.y) * 0.5f,
				(aabb.vtxMax.z - aabb.vtxMin.z) * 0.5f
			};

			// 回転前のAABBの最小および最大座標を計算
			MyLib::Vector3 aabbMin = aabbCenter - aabbSize;
			MyLib::Vector3 aabbMax = aabbCenter + aabbSize;

			// ボックスの最小および最大座標を計算
			MyLib::Vector3 boxMin = boxCenter - boxSize;
			MyLib::Vector3 boxMax = boxCenter + boxSize;

			// 回転したAABBの最小および最大座標を計算
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

			// AABBの境界ボックスとボックスの境界ボックスが一部でも重なっているかどうかを確認
			bool collisionX = rotatedAABBMax.x >= boxMin.x && rotatedAABBMin.x <= boxMax.x;
			bool collisionY = rotatedAABBMax.y >= boxMin.y && rotatedAABBMin.y <= boxMax.y;
			bool collisionZ = rotatedAABBMax.z >= boxMin.z && rotatedAABBMin.z <= boxMax.z;

			return collisionX && collisionY && collisionZ;
		}


		/**
		@brief	軸に対する射影取得
		@param	vertices		[in]	頂点配列へのポインタ
		@param	numVertices		[in]	頂点数
		@param	axis			[in]	軸
		@return	D3DXVECTOR2 射影
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
		@brief	射影の交差チェック
		@param	projection1		[in]	射影1
		@param	projection2		[in]	射影2
		@return	bool 交差結果
		*/
		inline bool Intersect(const D3DXVECTOR2& projection1, const D3DXVECTOR2& projection2) 
		{
			return !(projection1.y < projection2.x || projection1.x > projection2.y);
		}

		/**
		@brief	AABBとボックスの当たり判定
		@param	aabb		[in]	メインのAABB情報
		@param	aabbMtx		[in]	メインのマトリックス
		@param	boxAABB		[in]	ボックスのAABB情報
		@param	boxMtx		[in]	ボックスのマトリックス
		@return	bool 衝突結果
		*/
		inline bool IsAABBCollidingWithBox(const MyLib::AABB& aabb, MyLib::Matrix aabbMtx, const MyLib::AABB& boxAABB, MyLib::Matrix boxMtx)
		{
			//=============================
			// AABB情報
			//=============================
			// AABBの中心
			MyLib::Vector3 aabbCenter = {
				(aabb.vtxMin.x + aabb.vtxMax.x) * 0.5f,
				(aabb.vtxMin.y + aabb.vtxMax.y) * 0.5f,
				(aabb.vtxMin.z + aabb.vtxMax.z) * 0.5f
			};

			// AABBのサイズ
			MyLib::Vector3 aabbExtents = {
				(aabb.vtxMax.x - aabb.vtxMin.x) * 0.5f,
				(aabb.vtxMax.y - aabb.vtxMin.y) * 0.5f,
				(aabb.vtxMax.z - aabb.vtxMin.z) * 0.5f
			};

			//=============================
			// ボックス情報
			//=============================
			// ボックスの中心
			MyLib::Vector3 boxCenter = {
				(boxAABB.vtxMin.x + boxAABB.vtxMax.x) * 0.5f,
				(boxAABB.vtxMin.y + boxAABB.vtxMax.y) * 0.5f,
				(boxAABB.vtxMin.z + boxAABB.vtxMax.z) * 0.5f
			};

			// ボックスのサイズ
			MyLib::Vector3 boxExtents = {
				(boxAABB.vtxMax.x - boxAABB.vtxMin.x) * 0.5f,
				(boxAABB.vtxMax.y - boxAABB.vtxMin.y) * 0.5f,
				(boxAABB.vtxMax.z - boxAABB.vtxMin.z) * 0.5f
			};


			//=============================
			// 頂点変換
			//=============================
			// AABBの8頂点をワールド座標にする
			MyLib::Vector3 aabbVertices[8];
			for (int i = 0; i < 8; ++i) 
			{
				// 偶数はマイナス、奇数はプラス方向
				MyLib::Vector3 corner(
					(i & 1) ? aabbExtents.x : -aabbExtents.x,
					(i & 2) ? aabbExtents.y : -aabbExtents.y,
					(i & 4) ? aabbExtents.z : -aabbExtents.z);

				// マトリックスを元に頂点変換
				aabbVertices[i] = aabbMtx.Coord(corner);
			}

			// ボックスの8頂点をワールド座標にする
			MyLib::Vector3 boxVertices[8];
			for (int i = 0; i < 8; ++i) 
			{
				// 偶数はマイナス、奇数はプラス方向
				MyLib::Vector3 corner(
					(i & 1) ? boxExtents.x : -boxExtents.x,
					(i & 2) ? boxExtents.y : -boxExtents.y,
					(i & 4) ? boxExtents.z : -boxExtents.z);

				// マトリックスを元に頂点変換
				boxVertices[i] = boxMtx.Coord(corner);
			}

			//=============================
			// 分離軸定理
			//=============================
			// AABB元の判定
			for (int i = 0; i < 3; ++i)
			{
				MyLib::Vector3 axis(aabbMtx.m[i][0], aabbMtx.m[i][1], aabbMtx.m[i][2]);

				// AABBの射影
				D3DXVECTOR2 aabbProjection = ProjectOntoAxis(aabbVertices, 8, axis);

				// ボックスの射影
				D3DXVECTOR2 boxProjection = ProjectOntoAxis(boxVertices, 8, axis);

				// AABBとボックスの交差判定
				if (!Intersect(aabbProjection, boxProjection)) 
				{
					return false;
				}
			}

			// ボックス元の判定
			for (int i = 0; i < 3; ++i)
			{
				MyLib::Vector3 axis(boxMtx.m[i][0], boxMtx.m[i][1], boxMtx.m[i][2]);

				// AABBの射影
				D3DXVECTOR2 aabbProjection = ProjectOntoAxis(aabbVertices, 8, axis);

				// ボックスの射影
				D3DXVECTOR2 boxProjection = ProjectOntoAxis(boxVertices, 8, axis);

				// AABBとボックスの交差判定
				if (!Intersect(aabbProjection, boxProjection))
				{
					return false;
				}
			}

			return true;
		}
	}

	/**
	@brief	変換関数
	*/
	namespace Transformation
	{
		/**
		@brief	角度の正規化(単軸)
		@details -π or πを超えた角度を補正する。範囲内に収まるまで無限ループ
		@param	fRot	[inout]	補正する角度
		@return	void
		*/
		inline void RotNormalize(float& fRot)
		{
			while (1)
			{
				// 1周分越えたら補正する
				if (fRot < -D3DX_PI)
				{
					fRot += D3DX_PI * 2.0f;
				}
				else if (fRot > D3DX_PI)
				{
					fRot -= D3DX_PI * 2.0f;
				}

				if (fRot >= -D3DX_PI && fRot <= D3DX_PI)
				{// 全て正規化済みで終了
					break;
				}
			}
		}

		/**
		@brief	角度の正規化(VECTOR3)
		@details -π or πを超えた角度を補正する。範囲内に収まるまで無限ループ
		@param	fRot	[inout]	補正する角度
		@return	void
		*/
		inline void RotNormalize(MyLib::Vector3& rot)
		{
			// それぞれの正規化
			UtilFunc::Transformation::RotNormalize(rot.x);
			UtilFunc::Transformation::RotNormalize(rot.y);
			UtilFunc::Transformation::RotNormalize(rot.z);
		}

		/**
		@brief	度数法をラジアンに変換
		@param	degree	[in]	度数法の角度
		@return	ラジアン角度
		*/
		inline float DegreeChangeToRadian(float degree)
		{
			return degree * (D3DX_PI / 180.0f);
		}

		/**
		@brief	ラジアン角度を度数法に変換
		@param	radian	[in]	ラジアン角度
		@return	度数法
		*/
		inline float RadianChangeToDegree(float radian)
		{
			return radian * (180.0f / D3DX_PI);
		}

		/**
		@brief	値の正規化処理
		@param	Value		[inout]	補正したい値
		@param	MaxValue	[in]	最大値
		@param	MinValue	[in]	最小値
		@return	void
		*/
		template<class T>inline void ValueNormalize(T& Value, T MaxValue, T MinValue)
		{
			if (Value >= MaxValue)
			{
				// 最大値に補正
				Value = MaxValue;
			}
			else if (Value <= MinValue)
			{
				// 最小値に補正
				Value = MinValue;
			}
		}

		/**
		@brief	値の正規化処理
		@param	Value		[inout]	補正したい値
		@param	MinValue	[in]	最小値
		@param	MaxValue	[in]	最大値
		@return	void
		*/
		template<class T>inline const T Clamp(T Value, T MinValue, T MaxValue)
		{
			if (Value >= MaxValue)
			{
				// 最大値に補正
				return MaxValue;
			}
			else if (Value <= MinValue)
			{
				// 最小値に補正
				return MinValue;
			}
			return Value;
		}

		/**
		@brief	ワールドマトリックスをposに変換
		@param	worldmtx	[in]	ワールドマトリックス
		@return	位置
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
		@brief	ワールドマトリックスをrotに変換
		@param	worldmtx	[in]	ワールドマトリックス
		@return	向き(絶対値)
		*/
		inline MyLib::Vector3 WorldMtxChangeToRotation(D3DXMATRIX worldmtx)
		{
			MyLib::Vector3 rot = mylib_const::DEFAULT_VECTOR3;

			rot.x = atan2f(worldmtx._32, worldmtx._33);
			rot.y = atan2f(-worldmtx._31, sqrtf(worldmtx._32 * worldmtx._32 + worldmtx._33 * worldmtx._33));
			rot.z = atan2f(worldmtx._21, worldmtx._11);

			// XYZ 順序 (ロール → ピッチ → ヨー)
			/*rot.x = atan2f(-worldmtx._23, worldmtx._22);
			rot.y = asinf(worldmtx._21);
			rot.z = atan2f(-worldmtx._12, worldmtx._11);*/

			// ZYX 順序 (ヨー → ピッチ → ロール)
			// rot.z = atan2f(worldmtx._21, worldmtx._11);
			// rot.y = asinf(-worldmtx._31);
			// rot.x = atan2f(worldmtx._32, worldmtx._33);

			// ZYZ 順序 (ヨー → ピッチ → ヨー)
			// rot.z = atan2f(worldmtx._32, worldmtx._31);
			// rot.y = acosf(worldmtx._33);
			// rot.z = atan2f(worldmtx._23, -worldmtx._13);
			return rot;
		}

		/**
		@brief	向きを前方ベクトルに変換
		@param	rot	[in]	向き
		@return	前方ベクトル
		*/
		inline MyLib::Vector3 RotationChangeToForwardVector(float rot)
		{
			return MyLib::Vector3(sinf(D3DX_PI + rot), 0.0f, cosf(D3DX_PI + rot));
		}

		/**
		@brief	ランダム値取得
		@param	nMinNum	[in]	範囲の最小値
		@param	nMaxNum	[in]	範囲の最大値
		@return	抽選されたランダム値
		*/
		inline int Random(int nMinNum, int nMaxNum)
		{
			// 範囲
			int nRange = nMaxNum - nMinNum;

			if (nMinNum == 0)
			{
				// 範囲に1加算
				nRange += 1;
			}

			if (nMinNum + 1 == nMaxNum || nMaxNum - 1 == nMinNum)
			{
				nRange += 1;
			}

			// ランダム生成
			return (rand() % nRange + nMinNum);
		}

		/**
		@brief	ランダムな円周率取得
		@return	抽選されたランダム値
		*/
		inline float GetRandomPi()
		{
			return static_cast<float>(UtilFunc::Transformation::Random(-314, 314) * 0.01f);
		}

		/**
		@brief	球範囲のランダムベクトル取得
		@return	球のランダムなベクトル値
		*/
		inline MyLib::Vector3 GetRandomVecSphere()
		{
			// 球ベクトル
			MyLib::Vector3 vecSphere = mylib_const::DEFAULT_VECTOR3;

			// 球の範囲内でランダム生成
			float fRot = UtilFunc::Transformation::GetRandomPi(), fRotPhi = UtilFunc::Transformation::GetRandomPi();

			vecSphere.x = cosf(fRot) * sinf(fRotPhi);
			vecSphere.y = sinf(fRot);
			vecSphere.z = cosf(fRot) * cosf(fRotPhi);

			return vecSphere;
		}

		/**
		@brief	球範囲のランダムな位置取得
		@param	pos		[in]	球の中心座標
		@param	fRadius	[in]	取得する球の半径
		@return	抽選されたランダムな位置
		*/
		inline MyLib::Vector3 GetRandomPositionSphere(MyLib::Vector3 pos, float fRadius)
		{
			MyLib::Vector3 spherepos = mylib_const::DEFAULT_VECTOR3;
			MyLib::Vector3 vec = GetRandomVecSphere();

			// 引数情報計算
			spherepos = vec * fRadius;
			spherepos += pos;

			return spherepos;
		}

		/**
		@brief	高さを基準に幅を自動変換
		@param	size		[in]	元のサイズ
		@param	height		[in]	変更後の高さ
		@return	二次元サイズ
		*/
		inline D3DXVECTOR2 AdjustSizeByHeight(D3DXVECTOR2 size, float height)
		{
			D3DXVECTOR2 result = size;
			result.y = height;

			// 高さと比率から幅を計算
			result.x = height * UtilFunc::Calculation::AspectRatio(size);
			return result;
		}

		/**
		@brief	幅を基準に高さを自動計算
		@param	size		[in]	元のサイズ
		@param	width		[in]	変更後の幅
		@return	二次元サイズ
		*/
		inline D3DXVECTOR2 AdjustSizeByWidth(D3DXVECTOR2 size, float width)
		{
			D3DXVECTOR2 result = size;
			result.x = width;

			// 幅と比率から高さを計算
			result.y = width / UtilFunc::Calculation::AspectRatio(size);
			return result;
		}

		/**
		@brief	スクリーン座標をワールド座標に変換
		@param	Sx			[in]	スクリーンX座標
		@param	Sy			[in]	スクリーンY座標
		@param	fZ			[in]	射影空間でのZ値（0～1）
		@param	ScreenSize	[in]	スクリーンサイズ
		@param	View		[in]	ビューマトリックス
		@param	Prj			[in]	プロジェクションマトリックス
		@return	二次元サイズ
		*/
		inline MyLib::Vector3 CalcScreenToWorld(
			int Sx, int Sy, float fZ,
			D3DXVECTOR2 ScreenSize,
			D3DXMATRIX View, D3DXMATRIX Prj) 
		{
			MyLib::Vector3 pos;

			// 各行列の逆行列を算出
			D3DXMATRIX InvView, InvPrj, VP, InvViewport;
			D3DXMatrixInverse(&InvView, NULL, &View);
			D3DXMatrixInverse(&InvPrj, NULL, &Prj);
			D3DXMatrixIdentity(&VP);
			VP._11 = ScreenSize.x / 2.0f;
			VP._22 = -ScreenSize.y / 2.0f;
			VP._41 = ScreenSize.x / 2.0f;
			VP._42 = ScreenSize.y / 2.0f;
			D3DXMatrixInverse(&InvViewport, NULL, &VP);

			// 逆変換
			D3DXMATRIX tmp = InvViewport * InvPrj * InvView;

			// 変換後の座標に変換
			MyLib::Vector3 screenPos(static_cast<float>(Sx), static_cast<float>(Sy), fZ);
			D3DXVec3TransformCoord(&pos, &screenPos, &tmp);

			return pos;
		}

		// XZ平面とスクリーン座標の交点算出関数
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

			// 床との交差が起きている場合は交点を
			// 起きていない場合は遠くの壁との交点を出力
			if (ray.y <= 0) {

				MyLib::Vector3 planevec(0.0f, 1.0f, 0.0f);

				// 床交点
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

		// XZ平面とスクリーン座標の交点算出関数
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

			// farposを再計算する
			farpos = nearpos + ray * farDistance;

			// 床との交差が起きている場合は交点を
			// 起きていない場合は遠くの壁との交点を出力
			if (ray.y <= 0) {

				MyLib::Vector3 planevec(0.0f, 1.0f, 0.0f);

				// 床交点
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

		// Y平面とスクリーン座標の交点算出関数
		inline MyLib::Vector3  CalcScreenToY(
			D3DXVECTOR2 mousepos,
			D3DXVECTOR2 ScreenSize,
			D3DXMATRIX View, D3DXMATRIX Prj)
		{
			// スクリーン座標を正規化デバイス座標系（NDC）に変換
			float ndcX = (2.0f * mousepos.x / ScreenSize.x) - 1.0f;
			float ndcY = 1.0f - (2.0f * mousepos.y / ScreenSize.y);
			float ndcZ = 2.0f * 0.0f - 1.0f;

			// スクリーン座標をNDC座標に変換
			D3DXVECTOR4 ndcPos(ndcX, ndcY, ndcZ, 1.0f);

			// 逆ビュー変換行列と逆射影変換行列を組み合わせてワールド座標に変換
			D3DXMATRIX invViewProjection;
			D3DXMATRIX mtx = (View * Prj);

			D3DXMatrixInverse(&invViewProjection, nullptr, &mtx);

			D3DXVECTOR4 worldPos;
			D3DXVec4Transform(&worldPos, &ndcPos, &invViewProjection);

			// ワールド座標を返す
			return MyLib::Vector3(worldPos.x / worldPos.w, worldPos.y / worldPos.w, worldPos.z / worldPos.w);
		}

		// Y平面とスクリーン座標の交点算出関数
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

			// farposを再計算する
			farpos = nearpos + ray * farDistance;

			pos = farpos;

			return pos;
		}

		/**
		@brief	HSV値をRGB値に変換
		@details https://ja.wikipedia.org/wiki/HSV%E8%89%B2%E7%A9%BA%E9%96%93
		@param	H	[in]	色相
		@param	S	[in]	彩度
		@param	V	[in]	明度
		@return	RGB値
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

			// RGB別の数値
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
		@brief	ワイド文字列からマルチバイト文字列に変換する関数
		@param	wideStr		[in]	ワイド文字列
		@return	マルチバイト文字列
		*/
		inline std::string WideToMultiByte(const wchar_t* wideStr)
		{
			if (wideStr == nullptr)
				return std::string();

			// ワイド文字列の長さを取得
			int wideLength = wcslen(wideStr);

			// マルチバイト文字列の長さを計算
			int multiLength = WideCharToMultiByte(CP_UTF8, 0, wideStr, wideLength, nullptr, 0, nullptr, nullptr);

			if (multiLength == 0)
				return std::string();

			// マルチバイト文字列を格納するためのバッファを確保
			std::string multiString(multiLength, 0);

			// ワイド文字列をマルチバイト文字列に変換
			WideCharToMultiByte(CP_UTF8, 0, wideStr, wideLength, &multiString[0], multiLength, nullptr, nullptr);

			return multiString;
		}


		/**
		@brief	\\\\を\\に置換する
		@param	str		[in]	文字列
		@return	変換されたstring文字
		*/
		inline std::string ReplaceBackslash(std::string str)
		{
			size_t pos = 0;

			while ((pos = str.find("\\\\", pos)) != std::string::npos)
			{
				// \\\\を\\に置換
				str.replace(pos, 2, "\\");

				// \\の次から検索を再開
				pos += 1;
			}
			return str;
		}

		/**
		@brief	/を\\に置換する
		@param	str		[in]	文字列
		@return	変換されたstring文字
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
		@brief	ファイルパスを消す
		@param	filePath	[in]	文字列
		@return	変換されたstring文字
		*/
		inline std::string RemoveFilePath(const std::string& filePath)
		{
			// 最後のスラッシュまたはバックスラッシュを見つける
			size_t pos = filePath.find_last_of("/\\");

			if (pos != std::string::npos) 
			{
				// 見つかった位置の次の文字から最後までを取得
				return filePath.substr(pos + 1);
			}

			return filePath;
		}
	}

}



#endif