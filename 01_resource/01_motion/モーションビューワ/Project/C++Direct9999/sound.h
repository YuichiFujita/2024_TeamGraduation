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
	typedef enum
	{
		LABEL_BGM_TITLE = 0,	// タイトル
		LABEL_SE_SHOT,		// ショット
		LABEL_SE_EXPLOSION,	// 爆発
		LABEL_SE_KYUPIIN,	// キュピーン
		LABEL_MAX,
	} LABEL;

	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDINFO;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(LABEL label);
	void StopSound(void);
	void StopSound(LABEL label);
	void VolumeChange(float fVolume);
	int GetVolume(void);

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[LABEL_MAX];						// オーディオデータ
	DWORD m_aSizeAudio[LABEL_MAX];						// オーディオデータサイズ
	DSBUFFERDESC SetVolume;								// 音量調整
	float m_fVolume;									// 音量
	static SOUNDINFO m_aSoundInfo[LABEL_MAX];			// サウンドの情報
};


#endif
