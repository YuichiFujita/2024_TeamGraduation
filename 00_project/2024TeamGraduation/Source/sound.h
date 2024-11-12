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
	enum TYPE
	{
		TYPE_BGM,
		TYPE_SE,
		TYPE_MAX
	};

	// サウンドラベル
	enum LABEL
	{
		LABEL_BGM_TITLE = 0,	// タイトル
		LABEL_BGM_GAME,			// ゲーム
		LABEL_BGM_RESULT,		// リザルト
		LABEL_BGM_TUTORIAL,		// チュートリアル
		LABEL_BGM_RANKING,		// ランキング
		LABEL_SE_COIN,			// コイン取得
		LABEL_MAX
	};

	//=============================
	// 構造体定義
	//=============================
	struct SOUNDINFO
	{
		CSound::TYPE type;	// 音種類
		std::string file;	// ファイル名
		int nCntLoop;		// ループカウント
	};

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit();
	HRESULT PlaySound(LABEL label, bool stop = false);		// サウンド再生
	void StopSound();										// サウンド停止
	void StopSound(LABEL label);							// サウンド停止(ラベル指定)
	void VolumeChange(LABEL label, float volume);			// 音量設定
	void VolumeChange(float fVolume);						// マスターボリューム設定
	void VolumeChange(TYPE type, float fVolume);			// 種類別ボリューム設定
	float GetVolume() { return m_fMasterVolume; }			// 音量取得（マスターボリューム）
	float GetVolume(TYPE type) { return m_aVolume[type]; }	// 音量取得（種類別）
	void SetFrequency(LABEL label, float fValue);			// 周波数設定

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
	IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[LABEL_MAX];						// オーディオデータ
	DWORD m_aSizeAudio[LABEL_MAX];						// オーディオデータサイズ
	DSBUFFERDESC SetVolume;								// 音量調整
	float m_fMasterVolume;								// 音量（マスターボイス）
	float m_aVolume[TYPE_MAX];							// 音量（種類別）
	static SOUNDINFO m_aSoundInfo[LABEL_MAX];			// サウンドの情報
	WAVEFORMATEXTENSIBLE m_wfx[LABEL_MAX];
	static CSound* m_pThisPtr;							// 自身のポインタ
};


#endif
