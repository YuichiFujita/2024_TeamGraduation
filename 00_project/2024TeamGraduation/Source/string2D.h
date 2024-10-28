//============================================================
//
//	������2D�w�b�_�[ [string2D.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _STRING2D_H_
#define _STRING2D_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CFontChar;	// �t�H���g�����N���X
class CChar2D;		// ����2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ������2D�N���X
class CString2D : public CObject
{
public:
	// ���z�u��
	enum EAlignX
	{
		XALIGN_LEFT = 0,	// ������
		XALIGN_CENTER,		// ��������
		XALIGN_RIGHT,		// �E����
		XALIGN_MAX,			// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CString2D();

	// �f�X�g���N�^
	~CString2D() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Kill() override;			// ���I�폜����
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	void Draw(void) override;						// �`��
	void SetType(const TYPE type) override;			// ��ސݒ�
	void SetEnableDisp(const bool bDraw) override;	// �`��󋵐ݒ�
	void SetPosition(const MyLib::Vector3& pos) override;	// �ʒu�ݒ�
	void SetRotation(const MyLib::Vector3& rot) override;	// �����ݒ�

	// �ÓI�����o�֐�
	static CString2D *Create	// ����
	( // ����
		const std::string &rFilePass,	// �t�H���g�p�X
		const bool bItalic,				// �C�^���b�N
		const std::wstring &rStr,		// �w�蕶����
		const D3DXVECTOR3 &rPos,		// ���_�ʒu
		const float fHeight = 100.0f,			// �����c��
		const EAlignX alignX = XALIGN_CENTER,	// ���z�u
		const D3DXVECTOR3& rRot = VEC3_ZERO,	// ���_����
		const D3DXCOLOR& rCol = MyLib::color::White()	// �F
	);

	// ���z�֐�
	virtual HRESULT SetString(const std::wstring& rStr);	// ������̐ݒ�

	// �����o�֐�
	void SetFont	// �t�H���g�̐ݒ�
	( // ����
		const std::string &rFilePass,	// �t�H���g�p�X
		const bool bItalic = false		// �C�^���b�N
	);
	void SetColor(const D3DXCOLOR& rCol);			// �F�ݒ�
	void SetAlpha(const float fAlpha);				// �����x�ݒ�
	void SetCharHeight(const float fHeight);		// �����̏c���ݒ�
	void SetAlignX(const EAlignX align);			// ���z�u�ݒ�
	float GetStrWidth(void) const;					// ������̉����擾
	CChar2D *GetChar2D(const int nCharID) const;	// �����̎擾
	D3DXCOLOR GetColor(void) const	{ return m_col; }				// �F�擾
	float GetAlpha(void) const		{ return m_col.a; }				// �����x�擾
	float GetCharHeight(void) const	{ return m_fCharHeight; }		// �����̏c���擾
	EAlignX GetAlignX(void) const	{ return m_alignX; }			// ���z�u�擾
	int GetNumChar(void) const		{ return (int)m_wsStr.size(); }	// �������擾
	std::wstring GetStr(void) const { return m_wsStr; }				// ������擾

private:
	// �����o�֐�
	void SetPositionRelative(void);	// ���Έʒu�ݒ�

	// �����o�ϐ�
	CChar2D **m_ppChar;		// �����|���S���̏��
	CFontChar *m_pFontChar;	// �t�H���g����
	std::wstring m_wsStr;	// �w�蕶����
	D3DXCOLOR m_col;		// �F
	EAlignX m_alignX;		// ���z�u
	float m_fCharHeight;	// �����̏c��
};

#endif	// _STRING2D_H_
