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
	enum EType
	{
		TYPE_BGM,
		TYPE_SE,
		TYPE_MAX
	};

	// �T�E���h���x��
	enum ELabel
	{
		LABEL_NONE = -1,		// �w��Ȃ�
		LABEL_BGM_TITLE,		// �^�C�g��
		LABEL_BGM_TITLE_SUSURU,	// �^�C�g��(SUSURU)
		LABEL_BGM_ENTRY,		// �G���g���[
		LABEL_BGM_GAME,			// �Q�[��
		LABEL_BGM_SPAWN,		// �o�ꎞ
		LABEL_BGM_RESULT,		// ���U���g
		LABEL_BGM_TUTORIAL,		// �`���[�g���A��
		LABEL_BGM_RANKING,		// �����L���O
		LABEL_SE_SPAWNEND,		// �o�ꎞ���o�I����
		LABEL_SE_WALK,			// ����
		LABEL_SE_RUN01,			// ����
		LABEL_SE_RUN02,			// ����
		LABEL_SE_GRIP01,		// �O���b�v
		LABEL_SE_THROW_NORMAL,	// �ʏ퓊��
		LABEL_SE_THROW_JUMP,	// �W�����v����
		LABEL_SE_CATCH,			// �L���b�`
		LABEL_SE_JUSTCATCH,		// �W���X�g�L���b�`
		LABEL_SE_PUSH,			// �����߂�
		LABEL_SE_BOUND_HIGH,	// �o�E���h(����)
		LABEL_SE_BOUND_MEDIUM,	// �o�E���h(����)
		LABEL_SE_BOUND_LOW,		// �o�E���h(���)
		LABEL_SE_BOUND_REPEAT,	// �o�E���h(�J��Ԃ�)
		LABEL_SE_HIT_NORMAL,	// �q�b�g(�ʏ�)
		LABEL_SE_HIT_JUMP,		// �q�b�g(�W�����v)
		LABEL_SE_WHISTLE,		// �z�C�b�X��
		LABEL_SE_AI,			// AI�ǂ݂���
		LABEL_MAX
	};

	//=============================
	// �\���̒�`
	//=============================
	struct SOUNDINFO
	{
		CSound::EType type;	// �����
		std::string file;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	};

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit();
	HRESULT PlaySound(ELabel label, bool stop = false);		// �T�E���h�Đ�
	void StopSound();										// �T�E���h��~
	void StopSound(ELabel label);							// �T�E���h��~(���x���w��)
	void VolumeChange(ELabel label, float volume);			// ���ʐݒ�
	void VolumeChange(float fVolume);						// �}�X�^�[�{�����[���ݒ�
	void VolumeChange(EType type, float fVolume);			// ��ޕʃ{�����[���ݒ�
	float GetVolume() { return m_fMasterVolume; }			// ���ʎ擾�i�}�X�^�[�{�����[���j
	float GetVolume(EType type) { return m_aVolume[type]; }	// ���ʎ擾�i��ޕʁj
	void SetFrequency(ELabel label, float fValue);			// ���g���ݒ�

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
	IXAudio2 *m_pXAudio2;										// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;					// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[ELabel::LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[ELabel::LABEL_MAX];						// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[ELabel::LABEL_MAX];						// �I�[�f�B�I�f�[�^�T�C�Y
	DSBUFFERDESC SetVolume;										// ���ʒ���
	float m_fMasterVolume;										// ���ʁi�}�X�^�[�{�C�X�j
	float m_aVolume[ELabel::LABEL_MAX];							// ���ʁi��ޕʁj
	static SOUNDINFO m_aSoundInfo[ELabel::LABEL_MAX];			// �T�E���h�̏��
	WAVEFORMATEXTENSIBLE m_wfx[ELabel::LABEL_MAX];
	static CSound* m_pThisPtr;									// ���g�̃|�C���^
};


#endif
