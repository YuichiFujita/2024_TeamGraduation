//=============================================================================
// 
//  マネージャヘッダー [manager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MANAGER_H_
#define _MANAGER_H_	// 二重インクルード防止

#include "main.h"
#include "object.h"
#include "scene.h"
#include "sound.h"
#include "MyEffekseer.h"
#include "EffekseerObj.h"

#include "input_mouse.h"
#include "input_keyboard.h"
#include "input_gamepad.h"
#include "keyconfig.h"

// マクロ定義
#define MOVE_LRUP			(0.75f)	// 斜め上移動
#define MOVE_LR				(0.5f)	// 水平移動
#define MOVE_LRDW			(0.25f)	// 斜め上移動

// 前方宣言
class CRenderer;
class CLight;
class CCamera;
class CInput;
class CSound;
class CTexture;
class CDebugProc;
class CPlayer;
class CMap;
class CXLoad;
class CEdit;
class CElevation;
class CEnemyManager;
class CFade;
class CInstantFade;
class CPause;
class CPowerGauge;
class CResultManager;
class CRankingManager;

//==========================================================================
// クラス定義
//==========================================================================
// マネージャクラス定義
class CManager
{
public:

	enum BuildMode
	{
		MODE_DEBUG = 0,
		MODE_RELEASE,
		MODE_MAX
	};

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();
	void Load();// 読み込み

	static CManager *Create();		// 生成処理
	static CManager *GetInstance();	// インスタンス取得

	CRenderer *GetRenderer();
	CDebugProc *GetDebugProc();
	CLight *GetLight();
	CCamera *GetCamera();
	CEdit *GetEdit();
	CFade *GetFade();
	CInstantFade *GetInstantFade();	// 遷移なしフェード取得
	CPause *GetPause();
	CResultManager *GetResultManager();	// リザルトマネージャ取得
	CRankingManager *GetRankingManager();	// ランキングマネージャ
	CScene *GetScene();		// シーン取得

	int GetNumPlayer();			// プレイヤーの数取得
	void SetNumPlayer(int nNum);	// プレイヤーの数設定
	float GetDeltaTime();								// 経過時間取得
	void SetMode(CScene::MODE mode);					// 次のモード設定
	CScene::MODE GetMode();							// 現在のモード取得
	void SetEnableHitStop(int nCntHitStop);			// ヒットストップの設定
	void ChangePauseMode(CScene::MODE mode);		// ポーズの種類切り替え
	bool IsWireframe() { return m_bWireframe; }		// ワイヤーフレーム
	bool IsHitStop() { return m_bHitStop; }			// ワイヤーフレーム
	CScene::MODE GetOldMode() { return m_OldMode; }	// 前回のモード取得
	bool IsLoadComplete() { return m_bLoadComplete; }
	bool Is2DDisp() { return m_bDisp_2D; }			// 2D表示の判定
	void SerEnableDisp_UI(bool bDisp) { m_bDisp_UI = bDisp; }			// UI表示の判定
	bool IsDisp_UI() { return m_bDisp_UI; }			// UI表示の判定

	bool IsWindowed() { return m_bWindowed; }	// ウィンドウモードか
	void ChangeDisplayMode(bool bWindow);	// ディスプレイモード変更

	static bool IsDisp_ImGui() { return m_bDisp_ImGui; }				// ImGuiの描画判定
	static bool IsDisp_BoxColliders() { return m_bDisp_BoxColliders; }	// 当たり判定ボックスの描画判定
	static bool IsDisp_CheckPoint() { return m_bDisp_CheckPoint; }		// チェックポイントの描画判定

private:

	void Reset(CScene::MODE mode);
	void NoLoadSetMode(CScene::MODE mode);		// 次のモード設定

	CRenderer *m_pRenderer;				// レンダラー
	CInput*m_pInput;					// 入力機器
	CSound* m_pSound;					// サウンド
	CDebugProc *m_pDebugProc;			// デバッグ表示
	CLight *m_pLight;					// ライト
	CCamera *m_pCamera;					// カメラ
	CTexture *m_pTexture;				// テクスチャ
	CXLoad* m_pXLoad;					// Xファイル
	CEdit *m_pEdit;						// エディット
	CScene *m_pScene;					// シーン
	CFade *m_pFade;						// フェード
	CInstantFade *m_pInstantFade;		// 遷移なしフェード
	CPause *m_pPause;					// ポーズ
	CResultManager *m_pResultManager;	// リザルトマネージャ
	CRankingManager *m_pRankingManager;	// ランキングマネージャ
	CMyEffekseer* m_pMyEffekseer;		// エフェクシア

	bool m_bWireframe;					// ワイヤーフレーム
	bool m_bHitStop;					// ヒットストップの判定
	int m_nCntHitStop;					// ヒットストップのカウンター
	CScene::MODE m_OldMode;				// 前回のモード
	DWORD m_CurrentTime;				// 現在時間
	DWORD m_OldTime;					// 過去の時間
	float m_fDeltaTime;					// 経過時間
	int m_nNumPlayer;					// プレイヤーの数
	float m_fLoadTimer;					// ロードのタイマー
	bool m_bLoadComplete;				// ロード完了のフラグ
	bool m_bLoadFadeSet;				// ロードのフェード設定フラグ
	bool m_bNowLoading;				// ロード完了のフラグ
	bool m_bFirstLoad;				// 初回ロード
	bool m_bDisp_2D;					// 2Dの表示
	bool m_bDisp_UI;					// UIの表示
	bool m_bWindowed;					// ウィンドウモードか

	static CManager *m_pManager;	// マネージャ

	static bool m_bDisp_ImGui;			// ImGuiの描画判定
	static bool m_bDisp_BoxColliders;	// 当たり判定ボックスの描画判定
	static bool m_bDisp_CheckPoint;		// チェックポイントの描画判定
};


#endif