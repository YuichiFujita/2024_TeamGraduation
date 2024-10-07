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
	virtual HRESULT Init(const int nDivisionU, const int nDivisionV, const int nInterval, bool bAutoDeath = true);
	virtual void Update() override;
	virtual void SetVtx() override;

	void SetDesableAutoDeath() { m_bAutoDeath = false; }	// �����폜�̔���폜
	bool IsFinish() { return m_bFinish; }					// �I���̔���
	static CObjectBillboardAnim *Create(MyLib::Vector3 pos, const int nDivisionU, const int nDivisionV, const int nInterval, bool bAutoDeath = true);
	CObjectBillboardAnim *GetObject3DAnim() { return this; }
protected:

private:
	int m_nCntAnim;			// �A�j���[�V�����̃J�E���^�[
	int m_nPatternAnim;		// �A�j���[�V�����̃p�^�[��
	int m_nDivisionU;		// U�̕�����
	int m_nDivisionV;		// V�̕�����
	int m_nIntervalAnim;	// �A�j���[�V�����̃C���^�[�o��
	float m_fSplitValueU;	// U�̃X�v���C�g��
	float m_fSplitValueV;	// V�̃X�v���C�g��
	bool m_bAutoDeath;		// �����폜�̃t���O
	bool m_bFinish;			// �A�j���[�V�������I���������
};



#endif