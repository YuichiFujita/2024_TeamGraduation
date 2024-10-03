//=============================================================================
// 
//  マップエディタ―ヘッダー [edit_map.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _EDIT_MAP_H_
#define _EDIT_MAP_H_	// 二重インクルード防止

#include "manager.h"
#include "listmanager.h"
#include "objectX.h"
#include "edithandle.h"
#include "edit.h"

//==========================================================================
// クラス定義
//==========================================================================
// マップエディタ―クラス
class CEdit_Map : public CEdit
{
public:

	//=============================
	// 構造体定義
	//=============================
	struct SDragDropData
	{
		int nType;		// 種類
		CObjectX* objX;	// オブジェクトX

		SDragDropData() : nType(0), objX(nullptr) {}
	};

	CEdit_Map();
	~CEdit_Map();
	
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;

	void Save();	// セーブ
	void Load(const std::string& file);	// ロード
	void Delete(CObjectX* obj);	// 削除
	void Regist(int idx, MyLib::Vector3 pos, MyLib::Vector3 rot, float scale, bool bShadow);	// 割り当て

	static CEdit_Map* Create(const std::string& file, CManager::BuildMode mode);	// 生成
	static CListManager<CObjectX> GetListObj() { return m_List; }		// リスト取得
	static std::vector<std::string> GetModelFileName() { return m_ModelFile; }

protected:
	std::string m_Filename;	// ファイル名

private:

	//=============================
	// 構造体定義
	//=============================
	struct SMapModelInfo
	{
		int nIdx;
		std::string fileName;
	};

	//=============================
	// メンバ関数
	//=============================
	HRESULT ReadTexture(const std::string& file);	// ファイル読み込み
	HRESULT ReadXFile(const std::string& file);	// ファイル読み込み
	void ChangeHandle();	// ハンドル切り替え

	// エディット中関数
	void MenuBar();			// メニューバー処理
	void SetObjectButton();	// オブジェクトボタン設定
	void EditImGui();		// ImGuiの操作
	void Grab();			// 掴み中処理
	void Remove();			// 再移動
	void SelectObject();	// オブジェクト選択中

	//=============================
	// メンバ変数
	//=============================
	std::vector<int> m_nModelIdx;				// モデルインデックス
	std::vector<LPDIRECT3DTEXTURE9> m_pTexture;	// テクスチャのポインタ
	static std::vector<CObjectX*> m_pObjX;			// オブジェクトXのポインタ

	LPDIRECT3DTEXTURE9 m_HandleTex[CHandle::HandleType::TYPE_MAX];	// テクスチャのポインタ


	SDragDropData m_DragData;	// ドラッグ中データ
	bool m_bGrab;			// 掴み判定
	bool m_bReGrab;			// 再掴み判定
	bool m_bHoverWindow;	// ウィンドウのホバー判定

	CObjectX* m_pCopyObj;				// コピーオブジェクト
	CObjectX* m_pGrabObj;				// 掴みオブジェクト
	CHandle* m_pHandle;					// 移動ハンドル
	CHandle::HandleType m_HandleType;	// ハンドルの種類
	CHandle::HandleAngle m_moveAngle;	// 移動の向き

	static std::vector<std::string> m_TextureFile;		// テクスチャファイル
	static std::vector<std::string> m_ModelFile;		// モデルファイル
	static bool m_bLoad;					// 読み込み判定
	static CListManager<CObjectX> m_List;	// リスト

};

// マップエディタ―リリース版
class CEdit_Map_Release : public CEdit_Map
{
public:

	CEdit_Map_Release();
	~CEdit_Map_Release();

	virtual HRESULT Init() override;
};
#endif