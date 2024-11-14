//=============================================================================
// 
//  �I�u�W�F�N�g�r���{�[�h�A�j���w�b�_�[ [objectBillboard_Anim.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECTBILLBOARD_ANIM_H_
#define _OBJECTBILLBOARD_ANIM_H_	// ��d�C���N���[�h�h�~

#include "objectBillboard.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g�r���{�[�h�A�j���N���X��`
class CObjectBillboardAnim : public CObjectBillboard
{
public:

	CObjectBillboardAnim(int nPriority = mylib_const::PRIORITY_DEF2D);
	virtual ~CObjectBillboardAnim();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual HRESULT Init(const int nDivisionU, const int nDivisionV, const float fInterval, bool bAutoDeath = true);
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void SetVtx() override;

	//--------------------------
	// �ݒ�
	//--------------------------
	void SetDesableAutoDeath() { m_bAutoDeath = false; }		// �����폜�̔���폜
	void SetEnableAutoPlay(bool bAuto) { m_bAutoPlay = bAuto; }	// �����Đ��̔���ݒ�
	bool IsFinish() { return m_bFinish; }						// �I���̔���

	//--------------------------
	// �A�j���[�V�����֘A
	//--------------------------
	void SetPatternAnim(int nPattern) { m_nPatternAnim = nPattern; }	// �A�j���[�V�����̃p�^�[��

	//--------------------------
	// ���̑�
	//--------------------------
	static CObjectBillboardAnim *Create(const MyLib::Vector3& pos, const int nDivisionU, const int nDivisionV, const float fInterval, bool bAutoDeath = true, int nPriority = mylib_const::PRIORITY_DEF2D);
	CObjectBillboardAnim *GetObject3DAnim() { return this; }

private:
	float m_fTimerAnim;		// �A�j���[�V�����̃J�E���^�[
	float m_fIntervalAnim;	// �A�j���[�V�����̃C���^�[�o��
	int m_nPatternAnim;		// �A�j���[�V�����̃p�^�[��
	int m_nDivisionU;		// U�̕�����
	int m_nDivisionV;		// V�̕�����
	float m_fSplitValueU;	// U�̃X�v���C�g��
	float m_fSplitValueV;	// V�̃X�v���C�g��
	bool m_bAutoDeath;		// �����폜�̃t���O
	bool m_bAutoPlay;		// �����Đ��̃t���O
	bool m_bFinish;			// �A�j���[�V�������I���������
};



#endif