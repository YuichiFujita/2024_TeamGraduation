//=============================================================================
// 
//  �I�u�W�F�N�g3D�A�j���w�b�_�[ [object3D_Anim.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECT3D_ANIM_H_
#define _OBJECT3D_ANIM_H_	// ��d�C���N���[�h�h�~

#include "object3D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g3D�A�j���N���X��`
class CObject3DAnim : public CObject3D
{
public:

	CObject3DAnim(int nPriority = mylib_const::PRIORITY_DEF2D);
	virtual ~CObject3DAnim();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual HRESULT Init(const int nDivisionU, const int nDivisionV, const int nInterval);
	virtual void Update() override;
	virtual void SetVtx() override;

	void SetDesableAutoDeath() { m_bAutoDeath = false; }	// �����폜�̔���폜
	bool IsFinish() { return m_bFinish; }					// �I���̔���
	static CObject3DAnim *Create(MyLib::Vector3 pos, const int nDivisionU, const int nDivisionV, const int nInterval, bool bAutoDeath = true);
	CObject3DAnim *GetObject3DAnim() { return this; }
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