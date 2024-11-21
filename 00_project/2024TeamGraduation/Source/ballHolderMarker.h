//==========================================================================
// 
//  �{�[�������}�[�J�[�w�b�_�[ [ballHolderMarker.h]
//  Author : ���n�Ή�
// 
//==========================================================================

#ifndef _BALLHOLDERMARKER_H_
#define _BALLHOLDERMARKER_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "object.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayer;

//==========================================================================
// �N���X��`
//==========================================================================
// �{�[�������}�[�J�[�N���X
class CBallHolderMarker : public CObject
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CBallHolderMarker(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CBallHolderMarker();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill();	// �폜

	//=============================
	// �����o�֐�
	//=============================
	void BindPlyer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }	// �v���C���[���蓖��

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CBallHolderMarker* Create(CPlayer* pPlayer);	// ����

private:

	//=============================
	// �����o�ϐ�
	//=============================
	HRESULT CreateCircle();	// �~����
	HRESULT CreateArrow();		// ��󐶐�

	//=============================
	// �����o�ϐ�
	//=============================
	CObject3D* m_pCircle;	// �~
	CObject3D* m_pArrow;	// ���
	CPlayer* m_pPlayer;		// �{�[�����������Ă�v���C���[�̃|�C���^
	float m_fArrowTime;		// ���̃^�C�}�[
};

#endif
