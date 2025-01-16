//============================================================
//
//	��������̓w�b�_�[ [inputKeyButton.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _INPUT_KEY_BUTTON_H_
#define _INPUT_KEY_BUTTON_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X
class CString2D;	// ������2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ��������̓N���X
class CInputKeyButton : public CObject
{
public:
	// ������ޗ�
	enum ETypeChar
	{
		TYPECHAR_HIRAGANA = 0,	// �Ђ炪��
		TYPECHAR_KATAKANA,		// �J�^�J�i
		TYPECHAR_ALPHABET,		// �A���t�@�x�b�g
		TYPECHAR_MAX			// ���̗񋓌^�̑���
	};

	// ��I���
	enum ESelectX
	{
		XSELECT_LEFT = 0,	// ��
		XSELECT_CENTER,		// ����
		XSELECT_RIGHT,		// �E
		XSELECT_MAX			// ���̗񋓌^�̑���
	};

	// �s�I���
	enum ESelectY
	{
		YSELECT_CHAR_CHANGE,	// �����ύX
		YSELECT_CHAR_DECIDE,	// ��������
		YSELECT_MAX				// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CInputKeyButton(const int nPadIdx, const std::string& rOrigin, CString2D* pChange);

	// �f�X�g���N�^
	~CInputKeyButton() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Kill() override;		// �폜
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	void Draw() override;		// �`��

	// �ÓI�����o�֐�
	static CInputKeyButton* Create	// ����
	( // ����
		const int nPadIdx,			// ���쌠�C���f�b�N�X
		const std::string& rOrigin,	// �ύX�O�̕�����
		CString2D* pChange			// �ύX���镶����
	);
	static CInputKeyButton* GetInstance();	// �C���X�^���X�擾

private:
	// �����o�֐�
	void ControlSelect();	// �I�𑀍�
	void UpdateSelect();	// �I���X�V
	void UpdateDecide();	// ����X�V
	HRESULT ChangeChar(const ETypeChar typeChar);	// �����ύX
	HRESULT LoadArray(const ETypeChar typeChar);	// �z�u�Ǎ�

	// �ÓI�����o�ϐ�
	static CInputKeyButton* m_pInstance;	// ���g�̃C���X�^���X

	// �����o�ϐ�
	std::vector<std::vector<CString2D*>> m_vecSelect;	// �I�𕶎�
	const int m_nPadIdx;			// ���쌠�C���f�b�N�X
	const std::string m_sOrigin;	// �ύX�O�̕�����
	CString2D* m_pChange;			// �ύX���镶����
	CObject2D* m_pBG;				// �w�i
	CString2D* m_pTitle;			// �^�C�g��
	CString2D* m_pName;				// ���O
	ETypeChar m_typeChar;			// �������
	MyLib::PosGrid2 m_curSelect;	// ���݂̑I����
	MyLib::PosGrid2 m_oldSelect;	// �O��̑I����
};

#endif	// _INPUT_KEY_BUTTON_H_
