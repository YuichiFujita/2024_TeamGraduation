//=============================================================================
// 
//  マネージャヘッダー [manager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MANAGER_H_
#define _MANAGER_H_	// 二重インクルード防止

#include "main.h"

// マクロ定義
#define MOVE_LRUP			(0.75f)	// 斜め上移動
#define MOVE_LR				(0.5f)	// 水平移動
#define MOVE_LRDW			(0.25f)	// 斜め上移動

// 前方宣言
class CRenderer;
class CLight;
class CCamera;
class CInputKeyboard;
class CInputGamepad;
class CInputMouse;
class CSound;
class CDebugProc;
class CPlayer;
class CParticle;
class CScore;
class CTimer;
class CBG;
class CMap;
class CTexture;
class CXLoad;
class CEdit;
class CElevation;
class CEnemyManager;

//==========================================================================
// クラス定義
//==========================================================================
// マネージャクラス定義
class CManager
{
public:

	// エディットの列挙型定義
	enum EDITTYPE
	{
		EDITTYPE_EDIT = 0,	// エディット
		EDITTYPE_PLAY,		// 実行
		EDITTYPE_SETUP,		// セットアップ
		EDITTYPE_MAX
	};

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Reset(void);
	static CRenderer *GetRenderer(void);
	static CInputKeyboard *GetInputKeyboard(void);
	static CInputGamepad *GetInputGamepad(void);
	static CInputMouse *GetInputMouse(void);
	static CSound *GetSound(void);
	static CDebugProc *GetDebugProc(void);
	static CPlayer *GetPlayer(void);
	static CPlayer **GetPlayer(int n);
	static CParticle *GetParticle(void);
	static CScore *GetScore(void);
	static CTimer *GetTimer(void);
	static CBG *GetBg(void);
	static CLight *GetLight(void);
	static CCamera *GetCamera(void);
	static CMap *GetMap(void);
	static CTexture *GetTexture(void);
	static CXLoad *GetXLoad(void);
	static CEdit *GetEdit(void);
	static CElevation *GetObject3DMesh(void);
	static CEnemyManager *GetEnemyManager(void);
	static EDITTYPE GetEditType(void) { return m_editType; }
	static void SetEditType(EDITTYPE type) { m_editType = type; }
private:

	static CRenderer *m_pRenderer;				// レンダラーのオブジェクト
	static CInputKeyboard *m_pInputKeyboard;	// キーボードのオブジェクト
	static CInputGamepad *m_pInputGamepad;		// ゲームパッドのオブジェクト
	static CInputMouse *m_pInputMouse;			// マウスのオブジェクト
	static CSound *m_pSound;					// サウンドのオブジェクト
	static CDebugProc *m_pDebugProc;			// デバッグ表示のオブジェクト
	static CPlayer *m_pPlayer;					// プレイヤーのオブジェクト
	static CParticle *m_pParticle;				// パーティクルのオブジェクト
	static CScore *m_pScore;					// スコアのオブジェクト
	static CTimer *m_pTimer;					// タイマーのオブジェクト
	static CBG *m_pBg;							// 背景のオブジェクト
	static CLight *m_pLight;					// ライトのオブジェクト
	static CCamera *m_pCamera;					// カメラのオブジェクト
	static CMap *m_pMap;						// マップのオブジェクト
	static CTexture *m_pTexture;				// テクスチャのオブジェクト
	static CXLoad *m_pXLoad;					// Xファイルのオブジェクト
	static CEdit *m_pEdit;						// エディットのオブジェクト
	static CElevation *m_pObject3DMesh;			// オブジェクト3Dメッシュのオブジェクト
	static CEnemyManager *m_pEnemyManager;		// 敵マネージャのオブジェクト
	static bool m_bWireframe;					// ワイヤーフレーム
	static EDITTYPE m_editType;					// エディットの種類
};



#endif