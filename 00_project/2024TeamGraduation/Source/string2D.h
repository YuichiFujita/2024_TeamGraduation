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
	// �R���X�g���N�^
	CString2D(const int nPriority);

	// �f�X�g���N�^
	~CString2D() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Kill() override;		// �폜
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	void Draw() override;		// �`��
	void SetType(const TYPE type) override;					// ��ސݒ�
	void SetEnableDisp(const bool bDraw) override;			// �`��󋵐ݒ�
	void SetPosition(const MyLib::Vector3& pos) override;	// �ʒu�ݒ�
	void SetRotation(const MyLib::Vector3& rot) override;	// �����ݒ�

	// �ÓI�����o�֐�
	static CString2D* Create	// ���� (�}���`�o�C�g������)
	( // ����
		const std::string& rFilePath,	// �t�H���g�p�X
		const bool bItalic,				// �C�^���b�N
		const std::string& rStr,		// �w�蕶����
		const MyLib::Vector3& rPos,		// ���_�ʒu
		const float fHeight = 100.0f,					// �����c��
		const EAlignX alignX = XALIGN_CENTER,			// ���z�u
		const MyLib::Vector3& rRot = VEC3_ZERO,			// ���_����
		const D3DXCOLOR& rCol = MyLib::color::White(),	// �F
		const int nPriority = 6							// �D�揇��
	);
	static CString2D* Create	// ���� (���C�h������)
	( // ����
		const std::string& rFilePath,	// �t�H���g�p�X
		const bool bItalic,				// �C�^���b�N
		const std::wstring& rStr,		// �w�蕶����
		const MyLib::Vector3& rPos,		// ���_�ʒu
		const float fHeight = 100.0f,					// �����c��
		const EAlignX alignX = XALIGN_CENTER,			// ���z�u
		const MyLib::Vector3& rRot = VEC3_ZERO,			// ���_����
		const D3DXCOLOR& rCol = MyLib::color::White(),	// �F
		const int nPriority = 6							// �D�揇��
	);

	// ���z�֐�
	virtual HRESULT SetString(const std::string& rStr);		// ������̐ݒ� (�}���`�o�C�g������)
	virtual HRESULT SetString(const std::wstring& rStr);	// ������̐ݒ� (���C�h������)

	// �����o�֐�
	void SetFont	// �t�H���g�̐ݒ�
	( // ����
		const std::string& rFilePath,	// �t�H���g�p�X
		const bool bItalic = false		// �C�^���b�N
	);
	void SetAlpha(const float fAlpha);				// �����x�ݒ�
	void SetColor(const D3DXCOLOR& rCol);			// �F�ݒ�
	void SetCharHeight(const float fHeight);		// �����̏c���ݒ�
	void SetAlignX(const EAlignX align);			// ���z�u�ݒ�
	float GetStrWidth() const;						// ������̉����擾
	CChar2D* GetChar2D(const int nCharIdx) const;	// �����̎擾
	std::string GetStr() const;						// ������擾 (�}���`�o�C�g������)
	inline std::wstring GetWideStr() const	{ return m_wsStr; }				// ������擾 (���C�h������)
	inline float GetAlpha() const			{ return m_col.a; }				// �����x�擾
	inline D3DXCOLOR GetColor() const		{ return m_col; }				// �F�擾
	inline float GetCharHeight() const		{ return m_fCharHeight; }		// �����̏c���擾
	inline EAlignX GetAlignX() const		{ return m_alignX; }			// ���z�u�擾
	inline int GetNumChar() const			{ return (int)m_wsStr.size(); }	// �������擾

private:
	// �����o�֐�
	void SetPositionRelative();	// ���Έʒu�ݒ�

	// �����o�ϐ�
	std::vector<CChar2D*> m_vecChar;	// �����|���S���̏��
	CFontChar* m_pFontChar;	// �t�H���g����
	std::wstring m_wsStr;	// �w�蕶����
	D3DXCOLOR m_col;		// �F
	EAlignX m_alignX;		// ���z�u
	float m_fCharHeight;	// �����̏c��
};

#endif	// _STRING2D_H_
