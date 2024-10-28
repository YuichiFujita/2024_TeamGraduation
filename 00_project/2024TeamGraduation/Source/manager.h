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

// 前方宣言
class CRenderer;
class CLightManager;
class CCamera;
class CInput;
class CSound;
class CTexture;
class CDebugProc;
class CPlayer;
class CMap;
class CXLoad;
class CFont;
class CCharacterAnim;
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
	CLightManager *GetLight();
	CCamera *GetCamera();
	CEdit *GetEdit();
	CFont *GetFont();	// フォント取得
	CCharacterAnim* GetCharacterAnim();	// キャラクターアニメーション取得
	CFade *GetFade();
	CInstantFade *GetInstantFade();	// 遷移なしフェード取得
	CPause *GetPause();
	CResultManager *GetResultManager();		// リザルトマネージャ取得
	CRankingManager *GetRankingManager();	// ランキングマネージャ取得
	CScene *GetScene();	// シーン取得

	int GetNumPlayer();				// プレイヤーの数取得
	void SetNumPlayer(int nNum);	// プレイヤーの数設定

	void UpdateDeltaTime();								// 経過時間更新
	float GetDeltaTime() const { return m_fDeltaTime; }	// 経過時間取得
	void UpdateDeltaRate();								// 経過時間の割合更新
	float GetDeltaRate() const { return m_fDeltaRate; }	// 経過時間の割合取得
	void SetSlowRate(const float fRate) { m_fSlowRate = fRate; }	// 速度低下の割合更新
	float GetSlowRate() const { return m_fSlowRate; }				// 速度低下の割合取得

	void SetMode(CScene::MODE mode);				// 次のモード設定
	CScene::MODE GetMode();							// 現在のモード取得
	void ChangePauseMode(CScene::MODE mode);		// ポーズの種類切り替え
	bool IsWireframe() { return m_bWireframe; }		// ワイヤーフレーム
	CScene::MODE GetOldMode() { return m_OldMode; }	// 前回のモード取得
	bool IsLoadComplete() { return m_bLoadComplete; }
	bool Is2DDisp() { return m_bDisp_2D; }			// 2D表示の判定
	void SerEnableDisp_UI(bool bDisp) { m_bDisp_UI = bDisp; }			// UI表示の判定
	bool IsDisp_UI() { return m_bDisp_UI; }			// UI表示の判定

	bool IsWindowed() { return m_bWindowed; }	// ウィンドウモードか
	void ChangeDisplayMode(bool bWindow);		// ディスプレイモード変更

	static bool IsDisp_ImGui()			{ return m_bDisp_ImGui; }			// ImGuiの描画判定
	static bool IsDisp_BoxColliders()	{ return m_bDisp_BoxColliders; }	// 当たり判定ボックスの描画判定
	static bool IsDisp_CheckPoint()		{ return m_bDisp_CheckPoint; }		// チェックポイントの描画判定

private:

	void Reset(CScene::MODE mode);
	void NoLoadSetMode(CScene::MODE mode);		// 次のモード設定

	CRenderer *m_pRenderer;				// レンダラー
	CInput*m_pInput;					// 入力機器
	CSound* m_pSound;					// サウンド
	CDebugProc *m_pDebugProc;			// デバッグ表示
	CLightManager *m_pLight;			// ライト
	CCamera *m_pCamera;					// カメラ
	CTexture *m_pTexture;				// テクスチャ
	CXLoad* m_pXLoad;					// Xファイル
	CFont* m_pFont;						// フォント
	CCharacterAnim* m_pCharacterAnim;	// キャラクターアニメーション
	CEdit *m_pEdit;						// エディット
	CScene *m_pScene;					// シーン
	CFade *m_pFade;						// フェード
	CInstantFade *m_pInstantFade;		// 遷移なしフェード
	CPause *m_pPause;					// ポーズ
	CResultManager *m_pResultManager;	// リザルトマネージャ
	CRankingManager *m_pRankingManager;	// ランキングマネージャ
	CMyEffekseer* m_pMyEffekseer;		// エフェクシア

	bool m_bWireframe;		// ワイヤーフレーム
	CScene::MODE m_OldMode;	// 前回のモード
	DWORD m_dwOldTime;		// 前回の処理開始時刻
	DWORD m_dwCurTime;		// 今回の処理開始時刻
	float m_fDeltaTime;		// 経過時間
	float m_fDeltaRate;		// 経過時間の割合
	float m_fSlowRate;		// 速度低下の割合
	int m_nNumPlayer;		// プレイヤーの数
	float m_fLoadTimer;		// ロードのタイマー
	bool m_bLoadComplete;	// ロード完了のフラグ
	bool m_bLoadFadeSet;	// ロードのフェード設定フラグ
	bool m_bNowLoading;		// ロード完了のフラグ
	bool m_bFirstLoad;		// 初回ロード
	bool m_bDisp_2D;		// 2Dの表示
	bool m_bDisp_UI;		// UIの表示
	bool m_bWindowed;		// ウィンドウモードか

	static CManager *m_pManager;	// マネージャ

	static bool m_bDisp_ImGui;			// ImGuiの描画判定
	static bool m_bDisp_BoxColliders;	// 当たり判定ボックスの描画判定
	static bool m_bDisp_CheckPoint;		// チェックポイントの描画判定
};


#endif