//=============================================================================
// 
//  障害物エディタ―ヘッダー [edit_obstacle.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _EDIT_OBSTACLE_H_
#define _EDIT_OBSTACLE_H_	// 二重インクルード防止

#include "listmanager.h"
#include "objectX.h"
#include "edithandle.h"
#include "edit.h"
#include "collisionLine_Box.h"
#include "map_obstacleManager.h"

class CEdit_Obstacle_Super;
class CMap_Obstacle;

//==========================================================================
// クラス定義
//==========================================================================
// 障害物エディタ―クラス
class CEdit_Obstacle : public CEdit
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum EditType
	{
		TYPE_ARRANGMENT = 0,
		TYPE_COLLIDER,
		TYPE_MAX
	};

	CEdit_Obstacle();
	~CEdit_Obstacle();
	
	// オーバーライド関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

private:

	//=============================
	// メンバ関数
	//=============================
	void ChangeMode(EditType type);	// モード変更

	//=============================
	// メンバ変数
	//=============================
	EditType m_editType;				// エディットタイプ
	CEdit_Obstacle_Super* m_pEditControl;	// 種類操作
};

//------------------------------------
// 障害物エディタ―のスーパークラス
//------------------------------------
class CEdit_Obstacle_Super
{
public:

	CEdit_Obstacle_Super() {}
	~CEdit_Obstacle_Super() {}

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
};

//------------------------------------
// 障害物エディタ―の配置
//------------------------------------
class CEdit_Obstacle_Arrangment : public CEdit_Obstacle_Super
{
public:

	CEdit_Obstacle_Arrangment() : CEdit_Obstacle_Super() {}
	~CEdit_Obstacle_Arrangment() {}

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
	CMap_Obstacle* m_pGrabObj = nullptr;	// 掴みオブジェクト

	CHandle* m_pHandle = nullptr;					// 移動ハンドル
	CHandle::HandleType m_HandleType = CHandle::HandleType::TYPE_MOVE;	// ハンドルの種類
	CHandle::HandleAngle m_moveAngle = CHandle::HandleAngle::ANGLE_X;	// 移動の向き

	CMap_ObstacleManager::SObstacleInfo m_ObstacleInfo;	// 障害物情報

};

//------------------------------------
// 障害物エディタ―の判定
//------------------------------------
class CEdit_Obstacle_Collider : public CEdit_Obstacle_Super
{
public:

	CEdit_Obstacle_Collider() : CEdit_Obstacle_Super() {}
	~CEdit_Obstacle_Collider() {}

	void Init() override;	// 初期化
	void Update() override; // 更新

	// 終了処理
	void Uninit() override
	{
		for (const auto& obj : m_pObjX)
		{
			obj->Kill();
		}
		m_pObjX.clear();

		CEdit_Obstacle_Super::Uninit();
	}
	void CreateBoxLine() override;	// 当たり判定ボックス生成

private:

	//=============================
	// メンバ関数
	//=============================
	void Resize();	// リサイズ
	void MenuBar();	// メニューバー

	//=============================
	// メンバ変数
	//=============================
	int m_nEditIdx = 0;			// 調整するインデックス
	int m_nColliderIdx = 0;		// 調整するコライダーのインデックス
	std::vector<CObjectX*> m_pObjX;				// オブジェクトXのポインタ

};


#endif