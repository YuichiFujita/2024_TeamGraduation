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

	// �񋓌^��`
	enum TYPE
	{
		TYPE_BGM,
		TYPE_SE,
		TYPE_MAX
	};
	enum LABEL
	{
		LABEL_BGM_TITLE = 0,			// �^�C�g��
		LABEL_BGM_GAME,					// �Q�[��
		LABEL_BGM_WATER_FLOW,			// ����
		LABEL_BGM_RESULT,				// ���U���g
		LABEL_BGM_TUTORIAL,				// �ϋq
		LABEL_BGM_RANKING,				// �ϋq
		LABEL_SE_WALK1,					// ���s1
		LABEL_SE_CURSOR_MOVE,			// �J�[�\���ړ�
		LABEL_SE_CURSOR_END,			// �J�[�\����
		LABEL_SE_CANCEL1,				// �L�����Z��1
		LABEL_SE_CANCEL2,				// �L�����Z��2
		LABEL_SE_DROWN,				// �M��
		LABEL_SE_HIT,				// �q�b�g
		LABEL_SE_CRACK_GRASS,				// ��ʊ���
		LABEL_SE_BRESS_STREAM,				// �����[�v
		LABEL_SE_OKSD_000,
		LABEL_SE_OKSD_001,
		LABEL_SE_OKSD_002,
		LABEL_SE_SEGA_000,
		LABEL_SE_SEGA_001,
		LABEL_SE_SEGA_002,
		LABEL_SE_HNSW_000,
		LABEL_SE_HNSW_001,
		LABEL_SE_HNSW_002,
		LABEL_SE_KARAKURI,		// ���炭��T�[�J�X
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
		LABEL_SE_PLAYERMOVE01,	// �v���C���[�ړ�01
		LABEL_SE_PLAYERMOVE02,	// �v���C���[�ړ�02
		LABEL_SE_PLAYERMOVE03,	// �v���C���[�ړ�03
		LABEL_SE_PLAYERMOVE04,	// �v���C���[�ړ�04
		LABEL_SE_PLAYERMOVE05,	// �v���C���[�ړ�05
		LABEL_SE_PLAYERMOVE06,	// �v���C���[�ړ�06
		LABEL_SE_PLAYERMOVE07,	// �v���C���[�ړ�07
		LABEL_SE_PLAYERMOVE08,	// �v���C���[�ړ�08
		LABEL_SE_TORIDASU,
		LABEL_SE_COIN,			// �R�C���擾
		LABEL_MAX
	};

	typedef struct
	{
		CSound::TYPE type;	// �����
		std::string file;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} SOUNDINFO;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit();
	HRESULT PlaySound(LABEL label, bool stop = false);
	void StopSound();
	void StopSound(LABEL label);
	void VolumeChange(LABEL label, float volume);	// ���ʐݒ�
	void VolumeChange(float fVolume);				// �}�X�^�[�{�����[���ݒ�
	void VolumeChange(TYPE type, float fVolume);	// ��ޕʃ{�����[���ݒ�
	int GetVolumeNum();								// ���ʎ擾�i�}�X�^�[�{�����[���j
	int GetVolumeNum(TYPE type);					// ���ʎ擾�i��ޕʁj
	float GetVolume() { return m_fMasterVolume; }			// ���ʎ擾�i�}�X�^�[�{�����[���j
	float GetVolume(TYPE type) { return m_aVolume[type]; }	// ���ʎ擾�i��ޕʁj
	void SetFrequency(LABEL label, float fValue);	// ���g���ݒ�

	static CSound* Create(HWND hWnd);	// ��������
	static CSound* GetInstance() { return m_pThisPtr; }

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

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
	static CSound* m_pThisPtr;	// ���g�̃|�C���^
};


#endif
