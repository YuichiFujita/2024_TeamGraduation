//=============================================================================
// 
//  HPゲージの先端ヘッダー [hp_gauge_tip.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _HPGAUGE_TIP_H_
#define _HPGAUGE_TIP_H_	// 二重インクルード防止

#include "object2d.h"

//==========================================================================
// クラス定義
//==========================================================================
// HPゲージの先端クラス
class CHPGaugeTip : public CObject
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum TYPE
	{
		TYPE_2D = 0,
		TYPE_BILLBOARD,
		TYPE_MAX
	};

	CHPGaugeTip(int nPriority = 6);
	~CHPGaugeTip();


	// オーバーライドされた関数
	virtual HRESULT Init() override { return S_OK; }
	virtual HRESULT Init(const MyLib::Vector3& leftpos, const MyLib::Vector3& rightpos);
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override {}

	virtual void Kill();

	virtual void SetLeftPosition(const MyLib::Vector3& pos);
	virtual void SetRightPosition(const MyLib::Vector3& pos);
	static CHPGaugeTip* Create(const MyLib::Vector3& leftpos, const MyLib::Vector3& rightpos, TYPE type = TYPE::TYPE_2D);

protected:

	virtual void CreateTip(int nTexIdx);

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

	//=============================
	// メンバ変数
	//=============================
	CObject2D* m_pObj2D[VTXTYPE_MAX];	// オブジェクト2Dのポインタ
};


#endif