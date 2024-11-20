//=============================================================================
// 
//  エフェクシアヘッダー [MyEffekseer.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MYEFFEKSEER_H_
#define _MYEFFEKSEER_H_	// 二重インクルード防止

//==========================================================================
// インクルードファイル
//==========================================================================
#include <EffekseerRendererDX9.h>
#include <XAudio2.h>
#include <d3d9.h>
#include <functional>
#include <wrl/client.h>
#include "listmanager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CEffekseerObj;

//==========================================================================
// クラス定義
//==========================================================================
// エフェクシアクラス定義
class CMyEffekseer
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum EEfkLabel
	{
		EFKLABEL_SAMPLE_LASER = 0,		// サンプルのレーザー
		EFKLABEL_THROWLINE_NORMAL,		// 投げた時の線(通常)
		EFKLABEL_THROWLINE_FAST,		// 投げた時の線(早い)
		EFKLABEL_CAMEHAME_ENERGY,		// かめはめ波エネルギー部分
		EFKLABEL_CAMEHAME_CHARGE,		// かめはめ波(チャージ)
		EFKLABEL_CAMEHAME_CHARGE_WIND,	// かめはめ波(チャージ)(風)
		EFKLABEL_CAMEHAME_CHARGE_THUNDER,	// かめはめ波(チャージ)(雷)
		EFKLABEL_CAMEHAME_ATMOSPHERE,	// かめはめ波(空間オーラ)
		EFKLABEL_CAMEHAME_BRUST,		// かめはめ波(発射)
		EFKLABEL_CAMEHAME_FLASH,		// かめはめ波(フラッシュ)
		EFKLABEL_CAMEHAME_BALLAST,		// かめはめ波(空間がれき)
		EFKLABEL_HIT,					// ヒット
		EFKLABEL_SWEAT,					// 汗
		EFKLABEL_BLINK,					// ブリンク
		EFKLABEL_MAX
	};
	
	//=============================
	// 構造体定義
	//=============================
	struct MyEffekseerInfo
	{
		MyLib::Vector3 pos;
		MyLib::Vector3 rot;
		MyLib::Vector3 move;		// 移動量
		float scale;
		Effekseer::EffectRef efcRef;
		Effekseer::Handle handle;	// エフェクトのオブジェクト
		bool bAutoDeath;			// 自動削除のフラグ

		// デフォルトコンストラクタ
		MyEffekseerInfo() : pos(), rot(), move(), scale(), efcRef(), handle(), bAutoDeath() {}

		// 引数付きコンストラクタ
		MyEffekseerInfo(
			const MyLib::Vector3& _pos, const MyLib::Vector3& _rot, const MyLib::Vector3& _move, float _scale,
			const Effekseer::EffectRef& _efcRef, const Effekseer::Handle _handle, bool _bAutoDeath) : 
			pos(_pos), rot(_rot), move(_move), scale(_scale), efcRef(_efcRef), handle(_handle), bAutoDeath(_bAutoDeath) {}
	};

	struct SDebugInfo	// デバッグ情報
	{
		EEfkLabel label;		// エフェクトラベル
		MyLib::Vector3 pos;		// 位置
		MyLib::Vector3 rot;		// 向き
		float scale;			// スケール
		CEffekseerObj* pEfkObj;	// エフェクシアオブジェクト

		SDebugInfo() : label(EEfkLabel::EFKLABEL_SAMPLE_LASER), scale(10.0f), pEfkObj(nullptr) {}
	};

	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	CMyEffekseer();
	~CMyEffekseer();

	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void StopAll();	// 全て停止

	Effekseer::EffectRef LoadEffect(std::string efkpath);	// エフェクト読み込み
	Effekseer::EffectRef LoadEffect(EEfkLabel label);		// エフェクト読み込み
	Effekseer::ManagerRef GetEfkManager() { return m_efkManager; }	// エフェクトのマネージャー取得
	::EffekseerRendererDX9::RendererRef GetEffekseerRenderer() { return efkRenderer; }	// エフェクシアのレンダラー取得

	// 静的関数
	static CMyEffekseer* GetInstance() { return m_pMyEffekseer; }	// インスタンス取得
	static CMyEffekseer* Create();								// インスタンス生成

private:

	//=============================
	// メンバ関数
	//=============================
	HRESULT Init();
	void Draw();
	void UpdateAll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void SetupEffekseerModules(::Effekseer::ManagerRef efkManager);		// モジュールのセットアップ
	Effekseer::EffectRef LoadProcess(const std::u16string& efkpath);	// 読み込み処理
	void Debug();	// デバッグ

	//=============================
	// メンバ変数
	//=============================
	float fTime = 0.0f;
	Effekseer::Handle efkHandle = 0;
	EffekseerRendererDX9::RendererRef efkRenderer;
	Effekseer::ManagerRef m_efkManager;
	Effekseer::Vector3D viewerPosition;
	Effekseer::Matrix44 projectionMatrix;
	Effekseer::Matrix44 cameraMatrix;
	std::function<void()> onLostDevice;
	std::function<void()> onResetDevice;

	// 自作変数
	SDebugInfo m_debugInfo;							// デバッグ情報
	static std::string m_EffectName[EFKLABEL_MAX];	// エフェクトのファイル名
	static CMyEffekseer* m_pMyEffekseer;			// 自身のポインタ
};



#endif