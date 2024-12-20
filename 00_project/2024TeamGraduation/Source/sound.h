//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//==========================================================================
// クラス定義
//==========================================================================
// サウンドクラス
class CSound
{
public:

	//=============================
	// 列挙型定義
	//=============================
	// サウンドタイプ
	enum EType
	{
		TYPE_BGM,
		TYPE_SE,
		TYPE_MAX
	};

	// サウンドラベル
	enum ELabel
	{
		LABEL_NONE = -1,		// 指定なし
		LABEL_BGM_TITLE,		// タイトル
		LABEL_BGM_TITLE_SUSURU,	// タイトル(SUSURU)
		LABEL_BGM_ENTRY,		// エントリー
		LABEL_BGM_GAME,			// ゲーム
		LABEL_BGM_SPAWN,		// 登場時
		LABEL_BGM_RESULT,		// リザルト
		LABEL_BGM_TUTORIAL,		// チュートリアル
		LABEL_BGM_RANKING,		// ランキング
		LABEL_SE_SPAWNEND,		// 登場時演出終了時
		LABEL_SE_WALK,			// 歩き
		LABEL_SE_RUN01,			// 走り
		LABEL_SE_RUN02,			// 走り
		LABEL_SE_GRIP01,		// グリップ
		LABEL_SE_THROW_NORMAL,	// 通常投げ
		LABEL_SE_THROW_JUMP,	// ジャンプ投げ
		LABEL_SE_CATCH,			// キャッチ
		LABEL_SE_JUSTCATCH,		// ジャストキャッチ
		LABEL_SE_PUSH,			// 押し戻し
		LABEL_SE_BOUND_HIGH,	// バウンド(高め)
		LABEL_SE_BOUND_MEDIUM,	// バウンド(普通)
		LABEL_SE_BOUND_LOW,		// バウンド(低め)
		LABEL_SE_BOUND_REPEAT,	// バウンド(繰り返し)
		LABEL_SE_HIT_NORMAL,	// ヒット(通常)
		LABEL_SE_HIT_JUMP,		// ヒット(ジャンプ)
		LABEL_SE_WHISTLE,		// ホイッスル
		LABEL_SE_AI,			// AI読みあげ
		LABEL_MAX
	};

	//=============================
	// 構造体定義
	//=============================
	struct SOUNDINFO
	{
		CSound::EType type;	// 音種類
		std::string file;	// ファイル名
		int nCntLoop;		// ループカウント
	};

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit();
	HRESULT PlaySound(ELabel label, bool stop = false);		// サウンド再生
	void StopSound();										// サウンド停止
	void StopSound(ELabel label);							// サウンド停止(ラベル指定)
	void VolumeChange(ELabel label, float volume);			// 音量設定
	void VolumeChange(float fVolume);						// マスターボリューム設定
	void VolumeChange(EType type, float fVolume);			// 種類別ボリューム設定
	float GetVolume() { return m_fMasterVolume; }			// 音量取得（マスターボリューム）
	float GetVolume(EType type) { return m_aVolume[type]; }	// 音量取得（種類別）
	void SetFrequency(ELabel label, float fValue);			// 周波数設定

	static CSound* Create(HWND hWnd);						// 生成処理
	static CSound* GetInstance() { return m_pThisPtr; }		// インスタンス取得

private:

	//=============================
	// メンバ関数
	//=============================
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);	// チャンクのチェック
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);	// チャンクデータの読み込み

	//=============================
	// メンバ変数
	//=============================
	IXAudio2 *m_pXAudio2;										// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;					// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[ELabel::LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[ELabel::LABEL_MAX];						// オーディオデータ
	DWORD m_aSizeAudio[ELabel::LABEL_MAX];						// オーディオデータサイズ
	DSBUFFERDESC SetVolume;										// 音量調整
	float m_fMasterVolume;										// 音量（マスターボイス）
	float m_aVolume[ELabel::LABEL_MAX];							// 音量（種類別）
	static SOUNDINFO m_aSoundInfo[ELabel::LABEL_MAX];			// サウンドの情報
	WAVEFORMATEXTENSIBLE m_wfx[ELabel::LABEL_MAX];
	static CSound* m_pThisPtr;									// 自身のポインタ
};


#endif
