//=============================================================================
//
// �L�[�R���t�B�O�w�b�_�[ [keyconfig.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _KEYCONFIG_H_
#define _KEYCONFIG_H_	// ��d�C���N���[�h�h�~

// �C���Q�[��
namespace INGAME
{
	enum ACTION
	{
		ACT_OK = 0,		// ����
		ACT_BACK,		// �߂�
		ACT_UPDOWN,		// ����
		ACT_AIR,		// ��C
		ACT_CHECKPOINT,	// �`�F�b�N�|�C���g
		ACT_RETRY,		// ���g���C
		ACT_PAUSE,		// �|�[�Y
		ACT_MAX
	};
}

// �A�E�g�Q�[��
namespace OUTGAME
{
	enum ACTION
	{
		ACT_OK = 0,		// ����
		ACT_BACK,		// �߂�
		ACT_MAX
	};
}

//==========================================================================
// �N���X��`
//==========================================================================
// �L�[�R���t�B�O
class CKeyConfig
{
public:

	// �R���X�g���N�^
	CKeyConfig() {}
	virtual ~CKeyConfig(){}

	//=============================
	// �����o�֐�
	//============================= 
	virtual void Uninit() = 0;
	virtual bool GetPress(const int type, const int nId = 0) = 0;
	virtual bool GetTrigger(const int type, const int nId = 0) = 0;
	virtual bool GetRelease(const int type, const int nId = 0) = 0;
	virtual bool GetRepeat(const int type, const int nId = 0) = 0;
	virtual void Load(const std::string& file) = 0;
	virtual void Setting(const int type, const int nId = 0) = 0;
	virtual void Join(const int type, const int nKey, const int nId = 0) = 0;
	virtual int GetKey(const int type, const int nId = 0) = 0;
};

//==========================================================================
// �L�[�R���t�B�O�}�l�[�W���[
//==========================================================================
class CKeyConfigManager
{
public:
	// �����ޗ�
	enum Control
	{
		CONTROL_INPAD = 0,	// �C���Q�[���p�b�h
		CONTROL_OUTPAD,		// �A�E�g�Q�[���p�b�h
		CONTROL_INKEY,		// �C���Q�[���L�[�{�[�h
		CONTROL_OUTKEY,		// �A�E�g�Q�[���L�[�{�[�h
		CONTROL_MAX
	};

private:

	// �R���X�g���N�^
	CKeyConfigManager();
	virtual ~CKeyConfigManager() {}

public:

	// �����o�֐�
	void Uninit();

	// �ÓI�����o�֐�
	static CKeyConfigManager* GetInstance() { return m_pInstance; }
	static CKeyConfigManager* Create();
	static void ConfigTextureLoad();
	bool Bind(CKeyConfig* pConfig, const int type);
	CKeyConfig* GetConfig(const Control type) { return m_apKeyConfig[type]; }

private:

	// �����o�֐�
	void ConfigCreate();

	// �ÓI�����o�ϐ�
	static CKeyConfigManager* m_pInstance;

	// �����o�ϐ�
	CKeyConfig* m_apKeyConfig[Control::CONTROL_MAX];	// ���i�[�ꏊ
};

#endif