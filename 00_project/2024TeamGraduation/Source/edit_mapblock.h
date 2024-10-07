//=============================================================================
// 
//  マップブロックでの配置物エディタ―ヘッダー [edit_mapblock.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _EDIT_MAPBLOCK_H_
#define _EDIT_MAPBLOCK_H_	// 二重インクルード防止

#include "listmanager.h"
#include "objectX.h"
#include "edithandle.h"
#include "edit.h"
#include "collisionLine_Box.h"

class CEdit_MapBlock_Super;
class CMap_Obstacle;

//==========================================================================
// クラス定義
//==========================================================================
// 障害物エディタ―クラス
class CEdit_MapBlock : public CEdit
{
public:

	CEdit_MapBlock();
	~CEdit_MapBlock();

	// オーバーライド関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

private:

	//=============================
	// メンバ関数
	//=============================

	//=============================
	// メンバ変数
	//=============================
	CEdit_MapBlock_Super* m_pEditControl;	// 種類操作
};

//------------------------------------
// 障害物エディタ―のスーパークラス
//------------------------------------
class CEdit_MapBlock_Super
{
public:

	struct SMapModelInfo
	{
		int nIdx;
		std::string fileName;
	};

	CEdit_MapBlock_Super() {}
	~CEdit_MapBlock_Super() {}

	virtual void Init() = 0;
	virtual void Update() = 0;

	// 終了処理
	virtual void Uninit()
	{
		// 当たり判定ボックス削除
		DeleteBoxLine();
		delete this;
	}
	virtual void CreateBoxLine() = 0;	// 当たり判定ボックス生成

	// 当たり判定ボックス削除
	virtual void DeleteBoxLine()
	{
		for (const auto& box : m_pCollisionLineBox)
		{
			box->Kill();
		}
		m_pCollisionLineBox.clear();
	}

protected:
	std::vector<CCollisionLine_Box*> m_pCollisionLineBox;	// 当たり判定ボックス
	SMapModelInfo m_MapInfo;	// 障害物情報
};

//------------------------------------
// 障害物エディタ―の配置
//------------------------------------
class CEdit_MapBlock_Arrangment : public CEdit_MapBlock_Super
{
public:

	CEdit_MapBlock_Arrangment() : CEdit_MapBlock_Super() { m_FileName.clear(); }
	~CEdit_MapBlock_Arrangment() {}

	void Init() override;	// 初期化
	void Update() override; // 更新
	void CreateBoxLine() override;	// 当たり判定ボックス生成

private:

	void ObjectSelect();	// オブジェクト選択
	void ChangeHandle();
	void Transform();

	bool m_bButtonDrag = false;		// 掴み判定
	bool m_bReGrab = false;			// 再掴み判定
	bool m_bGrabHandle = false;			// 掴み判定
	bool m_bHoverWindow = false;	// ウィンドウのホバー判定
	CObjectX* m_pDragObj = nullptr;	// ドラッグ中のオブジェクト
	CObjectX* m_pGrabObj = nullptr;	// 掴みオブジェクト

	CHandle* m_pHandle = nullptr;					// 移動ハンドル
	CHandle::HandleType m_HandleType = CHandle::HandleType::TYPE_MOVE;	// ハンドルの種類
	CHandle::HandleAngle m_moveAngle = CHandle::HandleAngle::ANGLE_X;	// 移動の向き
	std::vector<std::string> m_FileName;

};

#endif
