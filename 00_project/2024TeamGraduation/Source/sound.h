//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �T�E���h�N���X
class CSound
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	// �T�E���h�^�C�v
	enum TYPE
	{
		TYPE_BGM,
		TYPE_SE,
		TYPE_MAX
	};

	// �T�E���h���x��
	enum LABEL
	{
		LABEL_BGM_TITLE = 0,	// �^�C�g��
		LABEL_BGM_GAME,			// �Q�[��
		LABEL_BGM_RESULT,		// ���U���g
		LABEL_BGM_TUTORIAL,		// �`���[�g���A��
		LABEL_BGM_RANKING,		// �����L���O
		LABEL_SE_COIN,			// �R�C���擾
		LABEL_MAX
	};

	//=============================
	// �\���̒�`
	//=============================
	struct SOUNDINFO
	{
		CSound::TYPE type;	// �����
		std::string file;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	};

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit();
	HRESULT PlaySound(LABEL label, bool stop = false);		// �T�E���h�Đ�
	void StopSound();										// �T�E���h��~
	void StopSound(LABEL label);							// �T�E���h��~(���x���w��)
	void VolumeChange(LABEL label, float volume);			// ���ʐݒ�
	void VolumeChange(float fVolume);						// �}�X�^�[�{�����[���ݒ�
	void VolumeChange(TYPE type, float fVolume);			// ��ޕʃ{�����[���ݒ�
	float GetVolume() { return m_fMasterVolume; }			// ���ʎ擾�i�}�X�^�[�{�����[���j
	float GetVolume(TYPE type) { return m_aVolume[type]; }	// ���ʎ擾�i��ޕʁj
	void SetFrequency(LABEL label, float fValue);			// ���g���ݒ�

	static CSound* Create(HWND hWnd);						// ��������
	static CSound* GetInstance() { return m_pThisPtr; }		// �C���X�^���X�擾

private:

	//=============================
	// �����o�֐�
	//=============================
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);	// �`�����N�̃`�F�b�N
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);	// �`�����N�f�[�^�̓ǂݍ���

	//=============================
	// �����o�ϐ�
	//=============================
	IXAudio2 *m_pXAudio2;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[LABEL_MAX];						// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[LABEL_MAX];						// �I�[�f�B�I�f�[�^�T�C�Y
	DSBUFFERDESC SetVolume;								// ���ʒ���
	float m_fMasterVolume;								// ���ʁi�}�X�^�[�{�C�X�j
	float m_aVolume[TYPE_MAX];							// ���ʁi��ޕʁj
	static SOUNDINFO m_aSoundInfo[LABEL_MAX];			// �T�E���h�̏��
	WAVEFORMATEXTENSIBLE m_wfx[LABEL_MAX];
	static CSound* m_pThisPtr;							// ���g�̃|�C���^
};


#endif
