//============================================================
//
//	����UI�w�b�_�[ [valueUI.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _VALUE_UI_H_
#define _VALUE_UI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"
#include "value.h"

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X
class CMultiValue;	// �}���`�����N���X

//************************************************************
//	�N���X��`
//************************************************************
// ����UI�N���X
class CValueUI : public CObject
{
public:
	// �R���X�g���N�^
	CValueUI();

	// �f�X�g���N�^
	~CValueUI() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��
	void SetPriority(const int nPriority) override;	// �D�揇�ʐݒ�
	void SetEnableDraw(const bool bDraw) override;	// �`��󋵐ݒ�
	void SetVec3Position(const D3DXVECTOR3& rPos) override;				// �ʒu�ݒ�
	D3DXVECTOR3 GetVec3Position(void) const override { return m_pos; }	// �ʒu�擾

	// �ÓI�����o�֐�
	static CValueUI *Create	// ���� (�e�N�X�`���w��Ȃ�)
	( // ����
		const CValue::EType type,		// �������
		const int nDigit,				// ����
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rSpace,		// �s��
		const D3DXVECTOR3& rSpaceValue,	// �����s��
		const D3DXVECTOR3& rSizeTitle,	// �^�C�g���傫��
		const D3DXVECTOR3& rSizeValue,	// �����傫��
		const D3DXVECTOR3& rRotTitle = VEC3_ZERO,	// �^�C�g������
		const D3DXVECTOR3& rRotValue = VEC3_ZERO,	// ��������
		const D3DXCOLOR& rColTitle = XCOL_WHITE,	// �^�C�g���F
		const D3DXCOLOR& rColValue = XCOL_WHITE		// �����F
	);
	static CValueUI *Create	// ���� (�e�N�X�`���C���f�b�N�X�w��)
	( // ����
		const int nTextureID,			// �^�C�g���e�N�X�`���C���f�b�N�X
		const CValue::EType type,		// �������
		const int nDigit,				// ����
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rSpace,		// �s��
		const D3DXVECTOR3& rSpaceValue,	// �����s��
		const D3DXVECTOR3& rSizeTitle,	// �^�C�g���傫��
		const D3DXVECTOR3& rSizeValue,	// �����傫��
		const D3DXVECTOR3& rRotTitle = VEC3_ZERO,	// �^�C�g������
		const D3DXVECTOR3& rRotValue = VEC3_ZERO,	// ��������
		const D3DXCOLOR& rColTitle = XCOL_WHITE,	// �^�C�g���F
		const D3DXCOLOR& rColValue = XCOL_WHITE		// �����F
	);
	static CValueUI *Create	// ���� (�e�N�X�`���p�X�w��)
	( // ����
		const char *pTexturePass,		// �^�C�g���e�N�X�`���p�X
		const CValue::EType type,		// �������
		const int nDigit,				// ����
		const D3DXVECTOR3& rPos,		// �ʒu
		const D3DXVECTOR3& rSpace,		// �s��
		const D3DXVECTOR3& rSpaceValue,	// �����s��
		const D3DXVECTOR3& rSizeTitle,	// �^�C�g���傫��
		const D3DXVECTOR3& rSizeValue,	// �����傫��
		const D3DXVECTOR3& rRotTitle = VEC3_ZERO,	// �^�C�g������
		const D3DXVECTOR3& rRotValue = VEC3_ZERO,	// ��������
		const D3DXCOLOR& rColTitle = XCOL_WHITE,	// �^�C�g���F
		const D3DXCOLOR& rColValue = XCOL_WHITE		// �����F
	);

	// �����o�֐�
	void SetSpace(const D3DXVECTOR3& rSpace);			// �s�Ԑݒ�
	void BindTextureTitle(const int nTextureID);		// �^�C�g���e�N�X�`������ (�C���f�b�N�X)
	void BindTextureTitle(const char *pTexturePass);	// �^�C�g���e�N�X�`������ (�p�X)
	void SetRotationTitle(const D3DXVECTOR3& rRot);		// �^�C�g�������ݒ�
	void SetSizingTitle(const D3DXVECTOR3& rSize);		// �^�C�g���傫���ݒ�
	void SetColorTitle(const D3DXCOLOR& rCol);			// �^�C�g���F�ݒ�
	D3DXVECTOR3 GetSpace(void) const			{ return m_space; }						// �s�Ԏ擾
	D3DXVECTOR3 GetRotationTitle(void) const	{ return m_pTitle->GetVec3Rotation(); }	// �^�C�g�������擾
	D3DXVECTOR3 GetSizingTitle(void) const		{ return m_pTitle->GetVec3Sizing(); }	// �^�C�g���傫���擾
	D3DXCOLOR GetColorTitle(void) const			{ return m_pTitle->GetColor(); }		// �^�C�g���F�擾
	CMultiValue *GetMultiValue(void) const		{ return m_pValue; }					// �������擾

private:
	// �I�[�o�[���C�h�֐�
	void Release(void) override { CObject::Release(); }	// �j��

	// �����o�֐�
	void SetPositionRelative(void);	// ���Έʒu�ݒ�

	// �����o�ϐ�
	CObject2D *m_pTitle;	// �^�C�g�����
	CMultiValue *m_pValue;	// �������
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_space;	// �s��
};

#endif	// _VALUE_UI_H_