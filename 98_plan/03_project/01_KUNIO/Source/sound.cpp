//==========================================================================
//
// �T�E���h���� [sound.cpp]
// Author : ���n�Ή�
//
//==========================================================================
#include "sound.h"
#include "calculation.h"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CSound::SOUNDINFO CSound::m_aSoundInfo[LABEL_MAX] = 
{
	{ TYPE_BGM,"data/BGM/title.wav", -1 },			// �^�C�g��
	{ TYPE_BGM,"data/BGM/BGM_game_000.wav", -1 },	// �Q�[��
	{ TYPE_BGM,"data/BGM/BGM_water_flow.wav", -1 },	// ����
	{ TYPE_BGM,"data/BGM/result.wav", -1 },			// ���U���g
	{ TYPE_BGM,"data/BGM/tutorial.wav", -1 },		// �`���[�g���A��
	{ TYPE_BGM,"data/BGM/ranking.wav", -1 },		// �����L���O
	{ TYPE_SE,"data/SE/walk01.wav", 0 },			// ���s1
	{ TYPE_SE,"data/SE/cursor_move.wav", 0 },		// �J�[�\���ړ�
	{ TYPE_SE,"data/SE/cursor_end.wav", 0 },		// �J�[�\����
	{ TYPE_SE,"data/SE/cancel01.wav", 0 },			// �L�����Z��1
	{ TYPE_SE,"data/SE/cancel02.wav", 0 },			// �L�����Z��2
	{ TYPE_SE,"data/SE/SE_drown.wav", -1 },			// �M��
	{ TYPE_SE,"data/SE/SE_collide.wav", 0 },			// �q�b�g
	{ TYPE_SE,"data/SE/SE_cracks_grass.wav", 0 },		// ��ʊ���
	{ TYPE_SE,"data/SE/SE_breath_stream.wav", -1 },		// �����[�v
	{ TYPE_SE,"data/SE/oksd_000.wav", 0 },
	{ TYPE_SE,"data/SE/oksd_001.wav", 0 },
	{ TYPE_SE,"data/SE/oksd_002.wav", 0 },
	{ TYPE_SE,"data/SE/sega_000.wav", 0 },
	{ TYPE_SE,"data/SE/sega_001.wav", 0 },
	{ TYPE_SE,"data/SE/sega_002.wav", 0 },
	{ TYPE_SE,"data/SE/ore_000.wav", 0 },
	{ TYPE_SE,"data/SE/ore_001.wav", 0 },
	{ TYPE_SE,"data/SE/ore_002.wav", 0 },
	{ TYPE_SE,"data/SE/karakurisa-kasu.wav", 0 },
	{ TYPE_SE,"data/SE/nageru.wav", 0 },
	{ TYPE_SE,"data/SE/receive.wav", 0 },
	{ TYPE_SE,"data/SE/oboreru.wav", 0 },
	{ TYPE_SE,"data/SE/habataku.wav", 0 },
	{ TYPE_SE,"data/SE/gaya.wav", -1 },
	{ TYPE_SE,"data/SE/countdown00.wav", 0 },
	{ TYPE_SE,"data/SE/countdown01.wav", 0 },
	{ TYPE_SE,"data/SE/start.wav", 0 },
	{ TYPE_SE,"data/SE/mizusibuki.wav", 0 },
	{ TYPE_SE,"data/SE/raou.wav", 0 },
	{ TYPE_SE,"data/SE/shock_wood.wav", 0 },
	{ TYPE_SE,"data/SE/shock_creature.wav", 0 },
	{ TYPE_SE,"data/SE/inthewater1.wav", 0 },
	{ TYPE_SE,"data/SE/inthewater2.wav", 0 },
	{ TYPE_SE,"data/SE/inthewater3.wav", 0 },
	{ TYPE_SE,"data/SE/writing.wav", 0 },
	{ TYPE_SE,"data/SE/writeFinish.wav", 0 },
	{ TYPE_SE,"data/SE/cluckbubbles.wav", 0 },
	{ TYPE_SE,"data/SE/open.wav", 0 },
	{ TYPE_SE,"data/SE/dive.wav", 0 },
	{ TYPE_SE,"data/SE/good.wav", 0 },
	{ TYPE_SE,"data/SE/worst.wav", 0 },
	{ TYPE_SE,"data/SE/bad.wav", 0 },
	{ TYPE_SE,"data/SE/smashattack.wav", 0 },
	{ TYPE_SE,"data/SE/pressure.wav", 0 },
	{ TYPE_SE,"data/SE/teleportation.wav", 0 },
	{ TYPE_SE,"data/SE/awesome.wav", 0 },
	{ TYPE_SE,"data/BGM/goal.wav", -1 },
	{ TYPE_SE,"data/SE/select.wav", 0 },
	{ TYPE_SE,"data/SE/diction.wav", 0 },
	{ TYPE_SE,"data/SE/playermove_01.wav", 0 },	// �v���C���[�ړ�01
	{ TYPE_SE,"data/SE/playermove_02.wav", 0 },	// �v���C���[�ړ�02
	{ TYPE_SE,"data/SE/playermove_03.wav", 0 },	// �v���C���[�ړ�03
	{ TYPE_SE,"data/SE/playermove_04.wav", 0 },	// �v���C���[�ړ�04
	{ TYPE_SE,"data/SE/playermove_05.wav", 0 },	// �v���C���[�ړ�05
	{ TYPE_SE,"data/SE/playermove_06.wav", 0 },	// �v���C���[�ړ�06
	{ TYPE_SE,"data/SE/playermove_07.wav", 0 },	// �v���C���[�ړ�07
	{ TYPE_SE,"data/SE/playermove_08.wav", 0 },	// �v���C���[�ړ�08
	{ TYPE_SE,"data/SE/toridasu.wav",0},
	{ TYPE_SE,"data/SE/coin.wav",0},			// �R�C���擾

};	// �T�E���h�̏��
CSound* CSound::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �}�N����`
//==========================================================================
CSound::CSound()
{
	
}

//==========================================================================
// �}�N����`
//==========================================================================
CSound::~CSound()
{

}

//==========================================================================
// ��������
//==========================================================================
CSound* CSound::Create(HWND hWnd)
{
	if (m_pThisPtr != nullptr)
	{
		return m_pThisPtr;
	}

	m_pThisPtr = DEBUG_NEW CSound;
	if (m_pThisPtr != nullptr)
	{
		m_pThisPtr->Init(hWnd);
	}

	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CSound::Init(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&m_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}
	
	// �}�X�^�[�{�C�X�̐���
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if(m_pXAudio2 != nullptr)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			m_pXAudio2->Release();
			m_pXAudio2 = nullptr;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for(int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(m_aSoundInfo[nCntSound].file.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if(SetFilePointer(hFile, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// �\�[�X�{�C�X�̐���
		hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
			return S_FALSE;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = m_aSoundInfo[nCntSound].nCntLoop;
		SetVolume.dwFlags = DSBCAPS_CTRLVOLUME;		//���ʒ����̃t���O

		m_fMasterVolume = 1.0f;
		m_aVolume[CSound::TYPE::TYPE_BGM] = 1.0f;
		m_aVolume[CSound::TYPE::TYPE_SE] = 1.0f;

		//���ʂ��Z�b�g����
		m_pMasteringVoice->SetVolume(m_fMasterVolume);

		// �I�[�f�B�I�o�b�t�@�̓o�^
		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		m_wfx[nCntSound] = wfx;

		// �t�@�C�����N���[�Y
		CloseHandle(hFile);
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CSound::Uninit()
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		if(m_apSourceVoice[nCntSound] != nullptr)
		{
			// �ꎞ��~
			m_apSourceVoice[nCntSound]->Stop(0);
	
			// �\�[�X�{�C�X�̔j��
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = nullptr;
	
			// �I�[�f�B�I�f�[�^�̊J��
			free(m_apDataAudio[nCntSound]);
			m_apDataAudio[nCntSound] = nullptr;
		}
	}
	
	// �}�X�^�[�{�C�X�̔j��
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = nullptr;
	
	if(m_pXAudio2 != nullptr)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		m_pXAudio2->Release();
		m_pXAudio2 = nullptr;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

//==========================================================================
// �Z�O�����g�Đ�(�Đ����Ȃ��~)
//==========================================================================
HRESULT CSound::PlaySound(LABEL label, bool stop)
{
#if 0
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[label];
	buffer.pAudioData = m_apDataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = m_aSoundInfo[label].nCntLoop;

	// ��Ԏ擾
	m_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		m_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_apSourceVoice[label]->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// �Đ�
	m_apSourceVoice[label]->Start(0);

	// ���g�����Z�b�g
	m_apSourceVoice[label]->SetFrequencyRatio(1.0f);

#else
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[label];
	buffer.pAudioData = m_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = m_aSoundInfo[label].nCntLoop;

	if (m_apSourceVoice[label] != nullptr)
	{
		// ��Ԏ擾
		XAUDIO2_VOICE_STATE xa2state;
		m_apSourceVoice[label]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0 &&
			(buffer.LoopCount == -1 || stop))
		{// �Đ���
			// �ꎞ��~
			m_apSourceVoice[label]->Stop(0);

			// �I�[�f�B�I�o�b�t�@�̍폜
			m_apSourceVoice[label]->FlushSourceBuffers();
		}
	}

	// �\�[�X�{�C�X��V�����쐬���čĐ�
	HRESULT hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[label], &(m_wfx[label].Format));
	if (FAILED(hr))
	{
		MessageBox(GetWnd(), "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return S_FALSE;
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// �Đ�
	m_apSourceVoice[label]->Start(0);

	// ���g�����Z�b�g
	m_apSourceVoice[label]->SetFrequencyRatio(1.0f);

	// ���ʃ��Z�b�g
	m_apSourceVoice[label]->SetVolume(m_aVolume[m_aSoundInfo[label].type]);

#endif
	return S_OK;

}

//==========================================================================
// �Z�O�����g��~(���x���w��)
//==========================================================================
void CSound::StopSound(LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	if (m_apSourceVoice[label] == nullptr)
	{
		return;
	}

	// ��Ԏ擾
	m_apSourceVoice[label]->GetState(&xa2state);

	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		m_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		m_apSourceVoice[label]->FlushSourceBuffers();

		// ���g�����Z�b�g
		m_apSourceVoice[label]->SetFrequencyRatio(1.0f);
	}

	/*if (m_apSourceVoice[label] != nullptr)
	{
		m_apSourceVoice[label]->Stop(0);
		m_apSourceVoice[label]->FlushSourceBuffers();
		m_apSourceVoice[label]->DestroyVoice();
		m_apSourceVoice[label] = nullptr;
	}*/
}

//==========================================================================
// �Z�O�����g��~(�S��)
//==========================================================================
void CSound::StopSound()
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		if(m_apSourceVoice[nCntSound] != nullptr)
		{
			// �ꎞ��~
			m_apSourceVoice[nCntSound]->Stop(0);

			// ���g�����Z�b�g
			m_apSourceVoice[nCntSound]->SetFrequencyRatio(1.0f);
		}

		/*if (m_apSourceVoice[nCntSound] != nullptr)
		{
			m_apSourceVoice[nCntSound]->Stop(0);
			m_apSourceVoice[nCntSound]->FlushSourceBuffers();
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = nullptr;
		}*/
	}
}

//==========================================================================
// �`�����N�̃`�F�b�N
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
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, nullptr) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, nullptr) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
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
// �`�����N�f�[�^�̓ǂݍ���
//==========================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, nullptr) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

//==========================================================================
// ���ʐݒ�
//==========================================================================
void CSound::VolumeChange(LABEL label, float volume)
{
	if (m_apSourceVoice[label] == nullptr)
	{
		return;
	}
	UtilFunc::Transformation::ValueNormalize(volume, 2.0f, 0.0f);

	// ���ʐݒ�
	m_apSourceVoice[label]->SetVolume(volume);
}

//==========================================================================
// ���ʒ���(�}�X�^�[�{�����[���ݒ�)
//==========================================================================
void CSound::VolumeChange(float fVolume)
{
	m_fMasterVolume = fVolume;
	UtilFunc::Transformation::ValueNormalize(m_fMasterVolume, 2.0f, 0.0f);

	//���ʂ��Z�b�g����
	m_pMasteringVoice->SetVolume(m_fMasterVolume);
}

//==========================================================================
// ���ʒ���(��ޕʃ{�����[���ݒ�)
//==========================================================================
void CSound::VolumeChange(TYPE type, float fVolume)
{
	m_aVolume[type] = fVolume;
	UtilFunc::Transformation::ValueNormalize(m_aVolume[type], 2.0f, 0.0f);

	//���ʂ��Z�b�g����
	for (int cnt = 0; cnt < CSound::LABEL::LABEL_MAX; cnt++)
	{
		if (m_aSoundInfo[cnt].type == type)
		{
			VolumeChange((LABEL)cnt, m_aVolume[type]);
		}
	}
}

//==========================================================================
// ���ʎ擾(3���\��, �}�X�^�[�{�����[��)
//==========================================================================
int CSound::GetVolumeNum()
{
	return (int)((m_fMasterVolume + 0.009) * 100);
}

//==========================================================================
// ���ʎ擾(3���\��, ��ޕʃ{�����[��)
//==========================================================================
int CSound::GetVolumeNum(TYPE type)
{
	return (int)((m_aVolume[type] + 0.009) * 100);
}

//==========================================================================
// ���g���ݒ�
//==========================================================================
void CSound::SetFrequency(LABEL label, float fValue)
{
	XAUDIO2_VOICE_STATE state;

	if (m_apSourceVoice[label] == nullptr){
		return;
	}

	// ��Ԏ擾
	m_apSourceVoice[label]->GetState(&state);

	if (state.BuffersQueued != 0)
	{// �Đ���
		m_apSourceVoice[label]->SetFrequencyRatio(fValue);
	}
}
