//=============================================================================
// 
//  クォータニオンヘッダー [objectQuaternion.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECTQUATERNION_H_
#define _OBJECTQUATERNION_H_	// 二重インクルード防止

#include "objectX.h"

//==========================================================================
// クラス定義
//==========================================================================
// クォータニオンクラス定義
class CObjectQuaternion : public CObjectX
{
public:

	CObjectQuaternion(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CObjectQuaternion();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual HRESULT Init(const std::string& file);
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	// 情報設定
	void SetValueRot(float rot)				{ m_fValueRot = rot; }		// 回転角設定
	float GetValueRot()						{ return m_fValueRot; }		// 回転角取得
	void SetMoveValueRot(float rot)			{ m_fMoveValueRot = rot; }	// 回転角移動量設定
	float GetMoveValueRot()					{ return m_fMoveValueRot; }	// 回転角移動量取得
	void SetMoveFactor(float factor)		{ m_fMoveFactor = factor; }	// 移動量の補正係数設定
	float GetMoveFactor()					{ return m_fMoveFactor; }	// 移動量の補正係数取得
	void SetAxis(const MyLib::Vector3& vec) { m_vecAxis = vec; }		// 回転軸設定
	MyLib::Vector3 GetAxis()				{ return m_vecAxis; }		// 回転軸取得

	void CalQuaternion();	// クォータニオン計算
	void BindQuaternion(const MyLib::Vector3& vecAxis, float valueRot);	// クォータニオン反映

	static CObjectQuaternion *Create();	// 生成

private:

	//=============================
	// メンバ関数
	//=============================
	void CalWorldMtx();		// ワールドマトリックスの計算処理

	//=============================
	// メンバ変数
	//=============================
	D3DXQUATERNION m_quaternion;	// クォータニオン
	MyLib::Vector3 m_vecAxis;		// 回転軸
	float m_fValueRot;				// 回転角
	float m_fMoveValueRot;			// 回転角の移動量
	float m_fMoveFactor;			// 移動量の補正係数
	MyLib::Matrix m_RotationMtx;	// 計算用マトリックス宣言

};



#endif