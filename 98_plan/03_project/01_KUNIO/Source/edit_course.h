//=============================================================================
// 
//  コースエディタ―ヘッダー [edit_course.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _EDIT_COURSE_H_
#define _EDIT_COURSE_H_	// 二重インクルード防止

#include "listmanager.h"
#include "objectX.h"
#include "edithandle.h"
#include "edit.h"

//==========================================================================
// クラス定義
//==========================================================================
// コースエディタ―クラス
class CEdit_Course : public CEdit
{
public:

	CEdit_Course();
	~CEdit_Course();
	
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

	//=============================
	// メンバ関数
	//=============================
	void FileControl();			// ファイル操作
	void ChangeLevel();			// レベル変更
	void TransCheckPoint();		// チェックポイント編集
	void TransJudge();			// ジャッジ編集
	void TransObstacle();		// 障害物編集
	void ChangeEditCourse();	// 編集するコース変更
	void SelectLine();		// ライン選択
	void DragLine();		// ライン掴み
	void Transform();		// トランスフォーム
	void ChangeLineNum();	// 辺の数変更
	void ChangeMode();		// モード変更
	void TransformBeginEnd();	// 最初と最後変形
	void TransformBeginEnd(std::vector<MyLib::Vector3>* pVecpos);	// 最初と最後変形
	void AddPoint();		// 地点追加

	void ObstacleEdit();	// 障害物エディット
	void ResetObstacle();	// 障害物リセット
	void SaveObstacle();	// 障害物セーブ

	void MapEdit();		// マップエディット
	void ResetMap();	// マップリセット
	void SaveMap();		// マップセーブ

	void WaterStoneEdit();	// 水中岩エディット
	void ResetWaterStone();	// 水中岩リセット
	void SaveWaterStone();	// 水中岩セーブ


	//=============================
	// メンバ変数
	//=============================
	LineData m_lineData;	// 辺情報
	int m_nCourseEditIdx;	// 操作するインデックス番号
	int m_nCheckPointEditIdx;	// 操作するインデックス番号
	int m_nJudgeEditIdx;	// 操作するインデックス番号
	int m_nJudgeObjEditIdx;	// 操作するインデックス番号
	int m_nObstacleEditIdx;	// 操作するインデックス番号
	int m_nVtxEditIdx;		// 操作するインデックス番号
	bool m_bEdit;			// 操作中判定
	bool m_bDrag;			// 掴み判定
	bool m_bHoverWindow;	// マウスのウィンドウホバー判定
	bool m_bSetMode;		// 設定モード判定
	bool m_bAutoCreateMode;	// 自動生成判定
	CEdit* m_pEditObstacle;	// 障害物エディター
	CEdit* m_pEditMap;		// マップエディター
	CEdit* m_pEditWaterStone;	// 水中岩エディター
};


#endif