//=============================================================================
// 
//  水中岩エディタ―ヘッダー [edit_waterstone.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _EDIT_WATERSTONE_H_
#define _EDIT_WATERSTONE_H_	// 二重インクルード防止

#include "listmanager.h"
#include "objectX.h"
#include "edithandle.h"
#include "edit.h"

//==========================================================================
// クラス定義
//==========================================================================
// 水中岩クラス
class CEdit_WaterStone : public CEdit
{
public:

	CEdit_WaterStone();
	~CEdit_WaterStone();
	
	// オーバーライド関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

private:

	//=============================
	// 構造体定義
	//=============================
	struct LineData
	{
		MyLib::Vector3 pos;	// 位置
		MyLib::Vector3 rot;	// 向き
		float width;		// 幅
	};

	// メンバ関数
	void FileControl();		// ファイル操作
	void SelectLine();		// ライン選択
	void DragLine();		// ライン掴み
	void Transform();		// トランスフォーム
	void ChangeMode();		// モード変更
	void AddPoint();		// 地点追加

	LineData m_lineData;	// 辺情報
	int m_nEditIdx;			// 操作するインデックス番号
	bool m_bEdit;			// 操作中判定
	bool m_bDrag;			// 掴み判定
	bool m_bHoverWindow;	// マウスのウィンドウホバー判定
	bool m_bSetMode;		// 設定モード判定
};


#endif