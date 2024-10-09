//============================================================
//
//	�^�C��UI�w�b�_�[ [timeUI.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _TIME_UI_H_
#define _TIME_UI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "number.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace timeUI
{
	const int MAX_MIN	= 2;	// �^�C�}�[�̕��̌���
	const int MAX_SEC	= 2;	// �^�C�}�[�̕b�̌���
	const int MAX_MSEC	= 3;	// �^�C�}�[�̃~���b�̌���
	const int MAX_PART	= 2;	// ��؂�̍ő吔

	const int MAX_DIGIT = (MAX_MIN + MAX_SEC + MAX_MSEC);	// �^�C�}�[�̌��� (���E�b�E�~���b)
}

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �^�C��UI�N���X
class CTimeUI : public CObject
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

	// �c�z�u��
	enum EAlignY
	{
		YALIGN_TOP = 0,	// �㑵��
		YALIGN_CENTER,	// ��������
		YALIGN_BOTTOM,	// ������
		YALIGN_MAX,		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CTimeUI();

	// �f�X�g���N�^
	~CTimeUI() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Release(void) override;	// �j��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(void) override;						// �`��
	void SetEnableDisp(const bool bDisp) override;	// �`��󋵐ݒ�
	void SetPosition(const MyLib::Vector3& pos) override;	// �ʒu�ݒ�
	void SetRotation(const MyLib::Vector3& rot) override;	// �����ݒ�

	// �ÓI�����o�֐�
	static CTimeUI *Create	// ����
	( // ����
		const float fTime,				// �\������
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR2& rSizeValue,	// �����̑傫��
		const D3DXVECTOR2& rSizePart,	// ��؂�̑傫��
		const D3DXVECTOR2& rSpaceValue,	// �����̋�
		const D3DXVECTOR2& rSpacePart,	// ��؂�̋�
		const EAlignX alignX = XALIGN_CENTER,			// ���z�u
		const EAlignY alignY = YALIGN_CENTER,			// �c�z�u
		const D3DXVECTOR3& rRot = VEC3_ZERO,			// ����
		const D3DXCOLOR& rCol = MyLib::color::White()	// �F
	);

	// ���z�֐�
	virtual void SetTime(const float fTime);				// �\�����Ԑݒ�
	virtual float GetTime(void) const	{ return m_fTime; }	// �\�����Ԏ擾
	virtual int GetMin(void) const		{ return (int)((DWORD)(m_fTime * 1000.0f) / 60000); }		// ���擾
	virtual int GetSec(void) const		{ return (int)((DWORD)(m_fTime * 1000.0f) / 1000 % 60); }	// �b�擾
	virtual int GetMSec(void) const		{ return (int)((DWORD)(m_fTime * 1000.0f) % 1000); }		// �~���b�擾

	// �����o�֐�
	void SetSizeValue(const D3DXVECTOR2& rSize);	// ��؂�̑傫���ݒ�
	void SetSizePart(const D3DXVECTOR2& rSize);		// �����̑傫���ݒ�
	void SetSpaceValue(const D3DXVECTOR2& rSpace);	// ��؂�̋󔒐ݒ�
	void SetSpacePart(const D3DXVECTOR2& rSpace);	// �����̋󔒐ݒ�
	void SetColor(const D3DXCOLOR& rCol);			// �F�̐ݒ�
	void SetAlignX(const EAlignX align);			// ���z�u�ݒ�
	void SetAlignY(const EAlignY align);			// �c�z�u�ݒ�
	float GetTimeWidth(void) const;					// �^�C���S�̂̉����擾
	float GetTimeHeight(void) const;				// �^�C���S�̂̏c���擾
	D3DXVECTOR2 GetTimeSize(void) const;			// �^�C���S�̂̑傫���擾
	D3DXVECTOR2 GetSizingValue(void) const	{ return m_sizeValue; }		// ��؂�̑傫���擾
	D3DXVECTOR2 GetSizingPart(void) const	{ return m_sizePart; }		// �����̑傫���擾
	D3DXVECTOR2 GetSpaceValue(void) const	{ return m_spaceValue; }	// ��؂�̋󔒎擾
	D3DXVECTOR2 GetSpacePart(void) const	{ return m_spacePart; }		// �����̋󔒎擾
	D3DXCOLOR GetColor(void) const			{ return m_col;}			// �F�̐ݒ�
	EAlignX GetAlignX(void) const			{ return m_alignX; }		// ���z�u�擾
	EAlignY GetAlignY(void) const			{ return m_alignY; }		// �c�z�u�擾

private:
	// �����o�֐�
	void SetPositionRelative(void);	// ���Έʒu�ݒ�
	void SetTexNum(void);			// �����̃e�N�X�`�����W�ݒ�

	// �����o�ϐ�
	CNumber *m_apValue[timeUI::MAX_DIGIT];	// ���l�̏��
	CObject2D *m_apPart[timeUI::MAX_PART];	// ��؂�̏��
	D3DXVECTOR2 m_sizeValue;	// �����̑傫��
	D3DXVECTOR2 m_sizePart;		// ��؂�̑傫��
	D3DXVECTOR2 m_spaceValue;	// �����̋�
	D3DXVECTOR2 m_spacePart;	// ��؂�̋�
	D3DXCOLOR m_col;			// �F
	EAlignX m_alignX;			// ���z�u
	EAlignY m_alignY;			// �c�z�u
	float m_fTime;				// �\������
};

#endif	// _TIME_UI_H_
