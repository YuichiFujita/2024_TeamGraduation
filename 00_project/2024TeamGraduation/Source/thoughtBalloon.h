//==========================================================================
// 
//  ���������o���w�b�_�[ [thoughtBalloon.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _THOUGHT_BALLOON_H_
#define _THOUGHT_BALLOON_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "objectBillboard.h"
#include "scrollText2D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���������o���N���X
class CThoughtBalloon : public CObjectBillboard
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum EState
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_MAX
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CThoughtBalloon(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CThoughtBalloon();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;	// ������
	virtual void Uninit() override;		// �I��
	virtual void Kill() override;		// �폜
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	virtual void Draw() override;		// �`��

	//=============================
	// �ÓI�����o�֐�
	//=============================
	// TODO : �����ɕ��� or ��ނ�������
	static CThoughtBalloon* Create();	// ����

	//=============================
	// �����o�֐�
	//=============================
	inline void SetEnableTextDisp(bool bDisp) { m_pText->SetEnableDisp(bDisp); }	// �e�L�X�g�\���ݒ�

private:

	//=============================
	// �֐����X�g
	//=============================
	// ��Ԋ֐�
	typedef void(CThoughtBalloon::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_stateFuncList[];	// �֐����X�g

	//=============================
	// �����o�֐�
	//=============================
	// �����֘A
	HRESULT CreateRenderTexture();	// �����_�[�e�N�X�`������
	HRESULT CreateTextureObject();	// �e�N�X�`���p�I�u�W�F�N�g����

	// �e�N�X�`���֘A
	void CreateTexture();	// �e�N�X�`���쐬

	// ��Ԋ֐�


	// ���̑��֐�


	//=============================
	// �����o�ϐ�
	//=============================
	// �e�N�X�`���֘A
	CRenderTexture* m_pRenderScene;	// �V�[�������_�[�e�N�X�`��
	CScrollText2D* m_pText;			// �e�L�X�g���

	// ��Ԍn
	EState m_state;		// ���
	float m_fStateTime;	// ��ԃJ�E���^�[

	// ���̑��ϐ�

};


#endif