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
CSound::SOUNDINFO CSound::m_aSoundInfo[ELabel::LABEL_MAX] = 
{
	// ���[�h��BGM
	{ TYPE_BGM,"data/BGM/title.wav", -1 },			// �^�C�g��
	{ TYPE_BGM,"data/BGM/game.wav", -1 },			// �Q�[��
	{ TYPE_BGM,"data/BGM/result.wav", -1 },			// ���U���g
	{ TYPE_BGM,"data/BGM/tutorial.wav", -1 },		// �`���[�g���A��
	{ TYPE_BGM,"data/BGM/ranking.wav", -1 },		// �����L���O

	// ����
	{ TYPE_SE,"data/SE/walk.wav",0},				// ����
	{ TYPE_SE,"data/SE/run00.wav",0},				// ����1
	{ TYPE_SE,"data/SE/run01.wav",0},				// ����2
	{ TYPE_SE,"data/SE/grip01.wav",0},				// �O���b�v

	// ����
	{ TYPE_SE,"data/SE/throw00.wav",0},				// �ʏ퓊��
	{ TYPE_SE,"data/SE/throw01.wav",0},				// �W�����v����

	// �L���b�`
	{ TYPE_SE,"data/SE/justcatch.wav",0},			// �W���X�g�L���b�`

	// �o�E���h
	{ TYPE_SE,"data/SE/Bound00.wav",0},				// �o�E���h(����)
	{ TYPE_SE,"data/SE/Bound01.wav",0},				// �o�E���h(����)
	{ TYPE_SE,"data/SE/Bound02.wav",0},				// �o�E���h(���)
	{ TYPE_SE,"data/SE/Bound03.wav",0},				// �o�E���h(�J��Ԃ�)

};	// �T�E���h�̏��
CSound* CSound::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CSound::CSound()
{
}

//==========================================================================
// �f�X�g���N�^
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

	// �������m��
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
	for(int nCntSound = 0; nCntSound < ELabel::LABEL_MAX; nCntSound++)
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
		m_aVolume[CSound::EType::TYPE_BGM] = 1.0f;
		m_aVolume[CSound::EType::TYPE_SE] = 1.0f;

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
	for(int nCntSound = 0; nCntSound < ELabel::LABEL_MAX; nCntSound++)
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
HRESULT CSound::PlaySound(ELabel label, bool stop)
{

	// �I�[�f�B�I�̃o�b�t�@
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

	return S_OK;
}

//==========================================================================
// �Z�O�����g��~(���x���w��)
//==========================================================================
void CSound::StopSound(ELabel label)
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
}

//==========================================================================
// �Z�O�����g��~(�S��)
//==========================================================================
void CSound::StopSound()
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < ELabel::LABEL_MAX; nCntSound++)
	{
		if(m_apSourceVoice[nCntSound] != nullptr)
		{
			// �ꎞ��~
			m_apSourceVoice[nCntSound]->Stop(0);

			// ���g�����Z�b�g
			m_apSourceVoice[nCntSound]->SetFrequencyRatio(1.0f);
		}
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
void CSound::VolumeChange(ELabel label, float volume)
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
void CSound::VolumeChange(EType type, float fVolume)
{
	m_aVolume[type] = fVolume;
	UtilFunc::Transformation::ValueNormalize(m_aVolume[type], 2.0f, 0.0f);

	//���ʂ��Z�b�g����
	for (int cnt = 0; cnt < CSound::ELabel::LABEL_MAX; cnt++)
	{
		if (m_aSoundInfo[cnt].type == type)
		{
			VolumeChange((ELabel)cnt, m_aVolume[type]);
		}
	}
}

//==========================================================================
// ���g���ݒ�
//==========================================================================
void CSound::SetFrequency(ELabel label, float fValue)
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
