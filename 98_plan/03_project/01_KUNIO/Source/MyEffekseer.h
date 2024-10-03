//=============================================================================
// 
//  エフェクシアヘッダー [MyEffekseer.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MYEFFEKSEER_H_
#define _MYEFFEKSEER_H_	// 二重インクルード防止

#include <EffekseerRendererDX9.h>
#include <XAudio2.h>
#include <d3d9.h>
#include <functional>
#include <wrl/client.h>
#include "listmanager.h"

//==========================================================================
// クラス定義
//==========================================================================
// エフェクシアクラス定義
class CMyEffekseer
{
public:
	
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

	//=============================
	// 列挙型定義
	//=============================
	enum EFKLABEL
	{
		EFKLABEL_SAMPLE_LASER = 0,	// サンプルのレーザー
		EFKLABEL_BRESS,				// 息
		EFKLABEL_RIVER_SAMPLE,		// 川サンプル
		EFKLABEL_IMPACT,			// 衝撃
		EFKLABEL_SPRAYWATER,		// 水しぶき
		EFKLABEL_SPRAYWATER_MINI,	// 水しぶき
		EFKLABEL_WATERJUMP,			// 魚入水
		EFKLABEL_PLAYERMOVE_LINE,	// プレイヤーの移動線
		EFKLABEL_PHOTON,			// 死亡
		EFKLABEL_BAGGAGEGET,		// 荷物獲得
		EFKLABEL_AIR,				// 空気
		EFKLABEL_CROWD,				// 群衆
		EFKLABEL_TP,				// テレポート
		EFKLABEL_HIT,				// ヒット
		EFKLABEL_WING,				// 羽ばたき
		EFKLABEL_SMASH,				// 潰れる
		EFKLABEL_AURA,				// オーラ
		EFKLABEL_MAX
	};

	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	CMyEffekseer();
	~CMyEffekseer();

	void Uninit();
	void Update();
	void StopAll();	// 全て停止

	Effekseer::EffectRef LoadEffect(std::string efkpath);	// エフェクト読み込み
	Effekseer::EffectRef LoadEffect(EFKLABEL label);		// エフェクト読み込み
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
	void UpdateAll();

	void SetupEffekseerModules(::Effekseer::ManagerRef efkManager);	// モジュールのセットアップ
	Effekseer::EffectRef LoadProcess(const std::u16string& efkpath);	// 読み込み処理

	//=============================
	// メンバ変数
	//=============================
	int32_t time = 0;
	Effekseer::Handle efkHandle = 0;
	EffekseerRendererDX9::RendererRef efkRenderer;
	Effekseer::ManagerRef m_efkManager;
	Effekseer::Vector3D viewerPosition;
	Effekseer::Matrix44 projectionMatrix;
	Effekseer::Matrix44 cameraMatrix;
	std::function<void()> onLostDevice;
	std::function<void()> onResetDevice;

	// 自作変数
	static std::string m_EffectName[EFKLABEL_MAX];	// エフェクトのファイル名
	static CMyEffekseer* m_pMyEffekseer;			// 自身のポインタ
};



#endif