//=============================================================================
//
// �R�}���h�w�b�_�[ [command.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _COMMAND_H_
#define _COMMAND_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �N���X��`
//==========================================================================
// �L�[�R���t�B�O
class CCommand
{
public:

	// ���
	enum TYPE
	{
		TYPE_ROTATION = 0,	// ������]
		TYPE_UP,			// �㏸
		TYPE_FLY,			// ���
		TYPE_MAX
	};

protected:

	// ���͏��
	struct Info
	{
		int nButton;	// ���͇�
		bool bActive;	// ���͂��ꂽ���ǂ���

		// �R���X�g���N�^
		Info() 
		{ 
			nButton = 1; 
			bActive = false; 
		}
	};

public:

	CCommand();
	virtual ~CCommand() {}	// �f�X�g���N�^

	// ��{����
	void Uninit();	// �I������
	virtual bool GetCommand() = 0;	// ���͊m�F
	void Load(std::string& string);
	void SetId(const int nId) { m_nId = nId; }
	int GetId() { return m_nId; }
	void SetType(const int nType) { m_nType = nType; }
	int GetType() { return m_nType; }

protected:

	// �����o�֐�
	void SetCommand(int nKey);	// �R�}���h�L�[�ݒ�
	void Reset();

	// �擾�n�����o�֐�
	int GetNowFlame() { return m_nNowFlame; }	// ���݂̃t���[���擾
	int GetMaxFlame() { return m_nMaxFlame; }	// �ő�t���[���擾

	// �ݒ�n�����o�֐�
	void SetNowFlame(int nFlame);	// ���݂̃t���[���ݒ�	
	void SetMaxFlame(int nFlame) { m_nMaxFlame = nFlame; }	// �ő�t���[���ݒ�

	// �����o�ϐ�
	std::vector<Info> m_Info;	// ���

private:

	// �����o�ϐ�
	int m_nType;		// ���
	int m_nId;			// �ԍ�
	int m_nNowFlame;	// ���݂̃t���[��
	int m_nMaxFlame;	// ���͎�t�ő�t���[��
};

//==========================================================================
// �h���N���X��`
//==========================================================================
// �Q�[���p�b�h
class CCommandPad : public CCommand
{
public:

	// ���͕�����
	enum DIRECTION
	{
		STICK_NONE = 0,		// �����ĂȂ�
		STICK_UP,			// ��
		STICK_DOWN,			// ��
		STICK_LEFT,			// ��
		STICK_RIGHT,		// �E
		STICK_LEFTUP,		// ����	
		STICK_RIGHTUP,		// �E��
		STICK_LEFTDOWN,		// ����
		STICK_RIGHTDOWN,	// �E��
		STICK_MAX
	};

public:

	// �R���X�g���N�^
	CCommandPad() {}
	~CCommandPad() override {}	// �f�X�g���N�^

	// �����o�֐�
	bool GetCommand() override;	// ���͊m�F
	DIRECTION GetDirection();	// ���͕����擾

private:
};

#endif