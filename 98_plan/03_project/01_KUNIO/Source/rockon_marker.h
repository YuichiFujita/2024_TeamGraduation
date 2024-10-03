//=============================================================================
// 
//  ロックオンマーカーヘッダー [rockon_marker.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _ROCKON_MARKER_H_
#define _ROCKON_MARKER_H_	// 二重インクルード防止

#include "objectBillboard.h"

//==========================================================================
// クラス定義
//==========================================================================
// ロックオンマーカークラス
class CRockOnMarker : public CObjectBillboard
{
public:

	CRockOnMarker(int nPriority = 7);
	~CRockOnMarker();


	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();

	static CRockOnMarker* Create(const MyLib::Vector3& pos);

private:

	//=============================
	// 列挙型定義
	//=============================
	enum VTXTYPE
	{
		VTXTYPE_RIGHTUP = 0,	// 右上
		VTXTYPE_RIGHTDW,		// 右下
		VTXTYPE_LEFTDW,		// 左下
		VTXTYPE_LEFTUP,		// 左上
		VTXTYPE_MAX
	};

	// 構造体定義
	struct SRockOnMarker
	{
		CObjectBillboard *pBillboard;	// ビルボードのオブジェクト
		float distance;		// 間隔
		int Angle;			// 向き
		int HeightAngle;	// 高さの向き
		float distancetimer;	// 距離時間

		SRockOnMarker() : pBillboard(nullptr), distance(0.0f), Angle(0), HeightAngle(0), distancetimer(0.0f) {}
	};

	//=============================
	// メンバ関数
	//=============================

	//=============================
	// メンバ変数
	//=============================
	SRockOnMarker m_RockOnInfo[VTXTYPE_MAX];	// マーカー情報
};


#endif