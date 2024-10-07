//=============================================================================
// 
//  HPゲージの先端ヘッダー [hp_gauge_tip_billboard.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _HPGAUGE_TIP_BILLBOARD_H_
#define _HPGAUGE_TIP_BILLBOARD_H_	// 二重インクルード防止

#include "hp_gauge_tip.h"
#include "objectBillboard.h"

//==========================================================================
// クラス定義
//==========================================================================
// HPゲージの先端クラス
class CHPGaugeTip_Billboard : public CHPGaugeTip
{
public:

	CHPGaugeTip_Billboard(int nPriority = 10);
	~CHPGaugeTip_Billboard();


	// オーバーライドされた関数
	//HRESULT Init(const MyLib::Vector3& leftpos, const MyLib::Vector3& rightpos) override;
	void Uninit() override;

	void Kill();

	void SetLeftPosition(const MyLib::Vector3& pos) override;
	void SetRightPosition(const MyLib::Vector3& pos) override;

private:

	//=============================
	// 列挙型定義
	//=============================
	enum VTXTYPE
	{
		VTXTYPE_LEFT = 0,	// 左
		VTXTYPE_RIGHT,		// 右
		VTXTYPE_MAX
	};


	void CreateTip(int nTexIdx) override;


	//=============================
	// メンバ変数
	//=============================
	CObjectBillboard* m_pObjBillboard[VTXTYPE_MAX];	// オブジェクト2Dのポインタ
};


#endif