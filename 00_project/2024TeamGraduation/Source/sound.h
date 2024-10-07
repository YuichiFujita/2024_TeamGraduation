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

	// 列挙型定義
	enum TYPE
	{
		TYPE_BGM,
		TYPE_SE,
		TYPE_MAX
	};
	enum LABEL
	{
		LABEL_BGM_TITLE = 0,			// タイトル
		LABEL_BGM_GAME,					// ゲーム
		LABEL_BGM_WATER_FLOW,			// 水流
		LABEL_BGM_RESULT,				// リザルト
		LABEL_BGM_TUTORIAL,				// 観客
		LABEL_BGM_RANKING,				// 観客
		LABEL_SE_WALK1,					// 歩行1
		LABEL_SE_CURSOR_MOVE,			// カーソル移動
		LABEL_SE_CURSOR_END,			// カーソル閉じ
		LABEL_SE_CANCEL1,				// キャンセル1
		LABEL_SE_CANCEL2,				// キャンセル2
		LABEL_SE_DROWN,				// 溺れ
		LABEL_SE_HIT,				// ヒット
		LABEL_SE_CRACK_GRASS,				// 画面割れ
		LABEL_SE_BRESS_STREAM,				// 息ループ
		LABEL_SE_OKSD_000,
		LABEL_SE_OKSD_001,
		LABEL_SE_OKSD_002,
		LABEL_SE_SEGA_000,
		LABEL_SE_SEGA_001,
		LABEL_SE_SEGA_002,
		LABEL_SE_HNSW_000,
		LABEL_SE_HNSW_001,
		LABEL_SE_HNSW_002,
		LABEL_SE_KARAKURI,		// からくりサーカス
		LABEL_SE_NAGERU,
		LABEL_SE_RECIEVE,
		LABEL_SE_OBORERU,
		LABEL_SE_HABATAKI,
		LABEL_SE_GAYA,
		LABEL_SE_COUNTDOWN1,
		LABEL_SE_COUNTDOWN2,
		LABEL_SE_START,
		LABEL_SE_MIZUSIBUKI,
		LABEL_SE_RAOU,
		LABEL_SE_SHOCK_WOOD,
		LABEL_SE_SHOCK_CREATURE,
		LABEL_SE_INTHEWATER1,
		LABEL_SE_INTHEWATER2,
		LABEL_SE_INTHEWATER3,
		LABEL_SE_WRITING,
		LABEL_SE_WRITING_FINISH,
		LABEL_SE_CLUCKBUBBLES,
		LABEL_SE_OPEN,
		LABEL_SE_DIVE,
		LABEL_SE_GOOD,
		LABEL_SE_WORST,
		LABEL_SE_BAD,
		LABEL_SE_SMASHATTACK,
		LABEL_SE_PRESSURE,
		LABEL_SE_TELEPORTATION,
		LABEL_SE_AWESOME,
		LABEL_BGM_GOAL,
		LABEL_SE_SELECT,
		LABEL_SE_DICTION,
		LABEL_SE_PLAYERMOVE01,	// プレイヤー移動01
		LABEL_SE_PLAYERMOVE02,	// プレイヤー移動02
		LABEL_SE_PLAYERMOVE03,	// プレイヤー移動03
		LABEL_SE_PLAYERMOVE04,	// プレイヤー移動04
		LABEL_SE_PLAYERMOVE05,	// プレイヤー移動05
		LABEL_SE_PLAYERMOVE06,	// プレイヤー移動06
		LABEL_SE_PLAYERMOVE07,	// プレイヤー移動07
		LABEL_SE_PLAYERMOVE08,	// プレイヤー移動08
		LABEL_SE_TORIDASU,
		LABEL_SE_COIN,			// コイン取得
		LABEL_MAX
	};

	typedef struct
	{
		CSound::TYPE type;	// 音種類
		std::string file;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDINFO;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit();
	HRESULT PlaySound(LABEL label, bool stop = false);
	void StopSound();
	void StopSound(LABEL label);
	void VolumeChange(LABEL label, float volume);	// 音量設定
	void VolumeChange(float fVolume);				// マスターボリューム設定
	void VolumeChange(TYPE type, float fVolume);	// 種類別ボリューム設定
	int GetVolumeNum();								// 音量取得（マスターボリューム）
	int GetVolumeNum(TYPE type);					// 音量取得（種類別）
	float GetVolume() { return m_fMasterVolume; }			// 音量取得（マスターボリューム）
	float GetVolume(TYPE type) { return m_aVolume[type]; }	// 音量取得（種類別）
	void SetFrequency(LABEL label, float fValue);	// 周波数設定

	static CSound* Create(HWND hWnd);	// 生成処理
	static CSound* GetInstance() { return m_pThisPtr; }

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

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
	static CSound* m_pThisPtr;	// 自身のポインタ
};


#endif
