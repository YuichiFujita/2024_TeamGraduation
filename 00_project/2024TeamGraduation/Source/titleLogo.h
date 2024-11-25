//==========================================================================
// 
//  �^�C�g�����S�w�b�_�[ [titleLogo.h]
//  Author : Kai Takada
// 
//==========================================================================

#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "object.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �{�[�������}�[�J�[�N���X
class CTitleLogo : public CObject
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CTitleLogo(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CTitleLogo();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill();	// �폜
	virtual void SetEnableDisp(bool bDisp);	// �`��󋵐ݒ�

	//=============================
	// �����o�֐�
	//=============================

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CTitleLogo* Create();	// ����

private:

	//=============================
	// �����o�ϐ�
	//=============================
	HRESULT CreateMain();	// �吶��
	HRESULT CreateSub();	// ������

	void UpdateMain();		// ��X�V
	void UpdateSub();		// ���X�V

	//=============================
	// �����o�ϐ�
	//=============================
	CObject2D* m_pMain;		// ��
	CObject2D* m_pSub;		// ��
	float m_fTime;			// �^�C�}�[
};

#endif
