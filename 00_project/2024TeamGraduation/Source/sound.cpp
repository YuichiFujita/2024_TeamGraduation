//==========================================================================
//
// サウンド処理 [sound.cpp]
// Author : 相馬靜雅
//
//==========================================================================
#include "sound.h"
#include "calculation.h"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CSound::SOUNDINFO CSound::m_aSoundInfo[ELabel::LABEL_MAX] = 
{
	// モード別BGM
	{ TYPE_BGM,"data/BGM/title.wav", -1 },			// タイトル
	{ TYPE_BGM,"data/BGM/SUSURU.wav", -1 },			// タイトル(SUSURU)
	{ TYPE_BGM,"data/BGM/entry.wav", -1 },			// エントリー
	{ TYPE_BGM,"data/BGM/game01.wav", -1 },			// ゲーム
	{ TYPE_BGM,"data/BGM/spawn.wav", -1 },			// 登場時
	{ TYPE_BGM,"data/BGM/result.wav", -1 },			// リザルト
	{ TYPE_BGM,"data/BGM/result_AF.wav", -1 },		// リザルト後
	{ TYPE_BGM,"data/BGM/tutorial.wav", -1 },		// チュートリアル
	{ TYPE_BGM,"data/BGM/ranking.wav", -1 },		// ランキング

	// BGM関連
	{ TYPE_SE,"data/SE/spawn_end.wav",0},			// 登場時終了

	// 歩き
	{ TYPE_SE,"data/SE/walk.wav",0},				// 歩き
	{ TYPE_SE,"data/SE/run00.wav",0},				// 走り1
	{ TYPE_SE,"data/SE/run01.wav",0},				// 走り2
	{ TYPE_SE,"data/SE/grip01.wav",0},				// グリップ
	{ TYPE_SE,"data/SE/grip02.wav",0},				// グリップ
	{ TYPE_SE,"data/SE/grip03.wav",0},				// グリップ

	// ジャンプ
	{ TYPE_SE,"data/SE/jump.wav",0},				// ジャンプ

	// 回避
	{ TYPE_SE,"data/SE/dodge.wav",0},				// 回避

	// 投げ
	{ TYPE_SE,"data/SE/throw00.wav",0},				// 通常投げ
	{ TYPE_SE,"data/SE/throw01.wav",0},				// ジャンプ投げ

	// キャッチ
	{ TYPE_SE,"data/SE/catch01.wav",0},				// キャッチ
	{ TYPE_SE,"data/SE/justcatch.wav",0},			// ジャストキャッチ
	{ TYPE_SE,"data/SE/push_normal.wav",0},			// 押し戻し

	// バウンド
	{ TYPE_SE,"data/SE/Bound00.wav",0},				// バウンド(高め)
	{ TYPE_SE,"data/SE/Bound01.wav",0},				// バウンド(普通)
	{ TYPE_SE,"data/SE/Bound02.wav",0},				// バウンド(低め)
	{ TYPE_SE,"data/SE/Bound03.wav",0},				// バウンド(繰り返し)

	// ヒット
	{ TYPE_SE,"data/SE/hit00.wav",0},				// 通常投げ
	{ TYPE_SE,"data/SE/hit01.wav",0},				// ジャンプ投げ

	// 登場演出
	{ TYPE_SE,"data/SE/door_open.wav",0},			// ドア開け
	{ TYPE_SE,"data/SE/walk_spawn.wav",0},			// 歩き
	{ TYPE_SE,"data/SE/walk_loop.wav",0},			// 歩きループ
	{ TYPE_SE,"data/SE/walk_loop01.wav",0},			// 歩きループ
	{ TYPE_SE,"data/SE/dash_loop.wav",0},			// 走りループ

	// スペシャル
	{ TYPE_SE,"data/SE/SP.wav",0},					// スペシャル
	{ TYPE_SE,"data/SE/SP_jump.wav",0},				// スペシャルジャンプ
	{ TYPE_SE,"data/SE/SP_land.wav",0},				// スペシャル着地
	{ TYPE_SE,"data/SE/SP_audience01.wav",0},		// スペシャル観客01
	{ TYPE_SE,"data/SE/SP_audience02.wav",0},		// スペシャル観客02
	{ TYPE_SE,"data/SE/SP_catch.wav",-1},			// スペシャルキャッチ
	{ TYPE_SE,"data/SE/SP_catchMomment.wav", 0},	// スペシャルキャッチ時
	{ TYPE_SE,"data/SE/SP_slide.wav", -1},			// スペシャルずざざ
	{ TYPE_SE,"data/SE/SP_brake.wav", -1},			// スペシャル耐え
	{ TYPE_SE,"data/SE/SP_catchSucc.wav",0},		// スペシャルキャッチ成功

	// かめはめ波
	{ TYPE_SE,"data/SE/KMHM_junbi.wav",0},			// かめはめ準備
	{ TYPE_SE,"data/SE/KMHM_start.wav",0},			// かめはめ開始
	{ TYPE_SE,"data/SE/KMHM_charge.wav",0},			// かめはめチャージ
	{ TYPE_SE,"data/SE/KMHM_loop.wav",-1},			// かめはめチャージ
	{ TYPE_SE,"data/SE/KMHM_brust.wav",0},			// かめはめ発射

	// その他
	{ TYPE_SE,"data/SE/whistle00.wav",0},			// ホイッスル
	{ TYPE_SE,"data/SE/AI_yomiage.wav",0},			// AI

	// アウトゲーム
	{ TYPE_SE,"data/SE/outgame/cursor.wav",0},			// カーソル
	{ TYPE_SE,"data/SE/outgame/cancel.wav",0},			// キャンセル
	{ TYPE_SE,"data/SE/outgame/write01.wav",0},			// 書き
	{ TYPE_SE,"data/SE/outgame/write02.wav",0},			// 書き
	{ TYPE_SE,"data/SE/outgame/arrow.wav",0},			// 矢印
	{ TYPE_SE,"data/SE/outgame/arrow_up.wav",0},		// 矢印(上げ)
	{ TYPE_SE,"data/SE/outgame/arrow_down.wav",0},		// 矢印(下げ)
	{ TYPE_SE,"data/SE/outgame/optionGo.wav",0},		// オプション行く
	{ TYPE_SE,"data/SE/outgame/optionBack.wav",0},		// オプション戻る
	{ TYPE_SE,"data/SE/outgame/join.wav",0},			// 参加
	{ TYPE_SE,"data/SE/outgame/controllerMove.wav",0},	// コントローラー移動
	{ TYPE_SE,"data/SE/outgame/select000.wav",0},		// 選択音
	{ TYPE_SE,"data/SE/outgame/select001.wav",0},		// 選択音
	{ TYPE_SE,"data/SE/outgame/select002.wav",0},		// 選択音
	{ TYPE_SE,"data/SE/outgame/select003.wav",0},		// 選択音
	{ TYPE_SE,"data/SE/outgame/decision000.wav",0},		// 決定音
	{ TYPE_SE,"data/SE/outgame/decision001.wav",0},		// 決定音
	{ TYPE_SE,"data/SE/outgame/decision002.wav",0},		// 決定音
	{ TYPE_SE,"data/SE/outgame/decision003.wav",0},		// 決定音
	{ TYPE_SE,"data/SE/outgame/decision004.wav",0},		// 決定音
	{ TYPE_SE,"data/SE/outgame/decision005.wav",0},		// 決定音
	{ TYPE_SE,"data/SE/outgame/decision006.wav",0},		// 決定音
	{ TYPE_SE,"data/SE/outgame/decision007.wav",0},		// 決定音
	{ TYPE_SE,"data/SE/outgame/next.wav",0},			// チュートリアル次遷移
	{ TYPE_SE,"data/SE/outgame/back.wav",0},			// チュートリアル前遷移

	// リザルト
	{ TYPE_SE,"data/SE/outgame/drumroll.wav",-1},		// ドラムロール
	{ TYPE_SE,"data/SE/outgame/drumroll_end.wav",0},	// ドラムロール終了
	{ TYPE_SE,"data/SE/outgame/draw.wav",0 },			// 引き分け

	// 観客
	{ TYPE_SE,"data/SE/kansei_loop01.wav", -1},			// 観客01
	{ TYPE_SE,"data/SE/kansei_loop02.wav", -1},			// 観客02


};	// サウンドの情報
CSound* CSound::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CSound::CSound()
{
}

//==========================================================================
// デストラクタ
//==========================================================================
CSound::~CSound()
{
}

//==========================================================================
// 生成処理
//==========================================================================
CSound* CSound::Create(HWND hWnd)
{
	if (m_pThisPtr != nullptr)
	{
		return m_pThisPtr;
	}

	// メモリ確保
	m_pThisPtr = DEBUG_NEW CSound;
	if (m_pThisPtr != nullptr)
	{
		m_pThisPtr->Init(hWnd);
	}

	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CSound::Init(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}
	
	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if(m_pXAudio2 != nullptr)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
			m_pXAudio2 = nullptr;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for(int nCntSound = 0; nCntSound < ELabel::LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(m_aSoundInfo[nCntSound].file.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if(SetFilePointer(hFile, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// ソースボイスの生成
		hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = m_aSoundInfo[nCntSound].nCntLoop;
		SetVolume.dwFlags = DSBCAPS_CTRLVOLUME;		//音量調整のフラグ

		m_fMasterVolume = 1.0f;
		m_aVolume[CSound::EType::TYPE_BGM] = 1.0f;
		m_aVolume[CSound::EType::TYPE_SE] = 1.0f;

		//音量をセットする
		m_pMasteringVoice->SetVolume(m_fMasterVolume);

		// オーディオバッファの登録
		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		m_wfx[nCntSound] = wfx;

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CSound::Uninit()
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < ELabel::LABEL_MAX; nCntSound++)
	{
		if(m_apSourceVoice[nCntSound] != nullptr)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);
	
			// ソースボイスの破棄
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = nullptr;
	
			// オーディオデータの開放
			free(m_apDataAudio[nCntSound]);
			m_apDataAudio[nCntSound] = nullptr;
		}
	}
	
	// マスターボイスの破棄
	if (m_pMasteringVoice != nullptr)
	{
		m_pMasteringVoice->DestroyVoice();
		m_pMasteringVoice = nullptr;
	}

	if(m_pXAudio2 != nullptr)
	{
		// XAudio2オブジェクトの開放
		m_pXAudio2->Release();
		m_pXAudio2 = nullptr;
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}

//==========================================================================
// セグメント再生(再生中なら停止)
//==========================================================================
HRESULT CSound::PlaySound(ELabel label, bool stop)
{

	// オーディオのバッファ
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[label];
	buffer.pAudioData = m_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = m_aSoundInfo[label].nCntLoop;

	if (m_apSourceVoice[label] != nullptr)
	{
		// 状態取得
		XAUDIO2_VOICE_STATE xa2state;
		m_apSourceVoice[label]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0 &&
			(buffer.LoopCount == -1 || stop))
		{// 再生中
			// 一時停止
			m_apSourceVoice[label]->Stop(0);

			// オーディオバッファの削除
			m_apSourceVoice[label]->FlushSourceBuffers();
		}
	}

	// ソースボイスを新しく作成して再生
	HRESULT hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[label], &(m_wfx[label].Format));
	if (FAILED(hr))
	{
		MessageBox(GetWnd(), "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
		return S_FALSE;
	}

	// オーディオバッファの登録
	m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	m_apSourceVoice[label]->Start(0);

	// 周波数リセット
	m_apSourceVoice[label]->SetFrequencyRatio(1.0f);

	// 音量リセット
	m_apSourceVoice[label]->SetVolume(m_aVolume[m_aSoundInfo[label].type]);

	return S_OK;
}

//==========================================================================
// セグメント停止(ラベル指定)
//==========================================================================
void CSound::StopSound(ELabel label)
{
	XAUDIO2_VOICE_STATE xa2state;

	if (m_apSourceVoice[label] == nullptr)
	{
		return;
	}

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);

	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[label]->FlushSourceBuffers();

		// 周波数リセット
		m_apSourceVoice[label]->SetFrequencyRatio(1.0f);
	}
}

//==========================================================================
// セグメント停止(全て)
//==========================================================================
void CSound::StopSound()
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < ELabel::LABEL_MAX; nCntSound++)
	{
		if(m_apSourceVoice[nCntSound] != nullptr)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);

			// 周波数リセット
			m_apSourceVoice[nCntSound]->SetFrequencyRatio(1.0f);
		}
	}
}

//==========================================================================
// チャンクのチェック
//==========================================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, nullptr) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, nullptr) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//==========================================================================
// チャンクデータの読み込み
//==========================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, nullptr) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

//==========================================================================
// 音量設定
//==========================================================================
void CSound::VolumeChange(ELabel label, float volume)
{
	if (m_apSourceVoice[label] == nullptr)
	{
		return;
	}
	UtilFunc::Transformation::ValueNormalize(volume, 2.0f, 0.0f);

	// 音量設定
	m_apSourceVoice[label]->SetVolume(volume);
}

//==========================================================================
// 音量調整(マスターボリューム設定)
//==========================================================================
void CSound::VolumeChange(float fVolume)
{
	m_fMasterVolume = fVolume;
	UtilFunc::Transformation::ValueNormalize(m_fMasterVolume, 2.0f, 0.0f);

	//音量をセットする
	m_pMasteringVoice->SetVolume(m_fMasterVolume);
}

//==========================================================================
// 音量調整(種類別ボリューム設定)
//==========================================================================
void CSound::VolumeChange(EType type, float fVolume)
{
	m_aVolume[type] = fVolume;
	UtilFunc::Transformation::ValueNormalize(m_aVolume[type], 2.0f, 0.0f);

	//音量をセットする
	for (int cnt = 0; cnt < CSound::ELabel::LABEL_MAX; cnt++)
	{
		if (m_aSoundInfo[cnt].type == type)
		{
			VolumeChange((ELabel)cnt, m_aVolume[type]);
		}
	}
}

//==========================================================================
// 周波数設定
//==========================================================================
void CSound::SetFrequency(ELabel label, float fValue)
{
	XAUDIO2_VOICE_STATE state;

	if (m_apSourceVoice[label] == nullptr){
		return;
	}

	// 状態取得
	m_apSourceVoice[label]->GetState(&state);

	if (state.BuffersQueued != 0)
	{// 再生中
		m_apSourceVoice[label]->SetFrequencyRatio(fValue);
	}
}
