//==========================================================================
// 
//  �I�v�V�����̌f���w�b�_�[ [option_BBS.h]
//  Author : ���n�Ή�
// 
//==========================================================================

#ifndef _OPTION_BBS_H_
#define _OPTION_BBS_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "objectX.h"

//==========================================================================
// �O���錾
//==========================================================================
class CBalloonFrame;
class COptionMenu;

//==========================================================================
// �N���X��`
//==========================================================================
// ���������o���N���X
class COption_BBS : public CObjectX
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
	COption_BBS(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~COption_BBS();

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
	static COption_BBS* Create();	// ����

	//=============================
	// �����o�֐�
	//=============================

private:

	//=============================
	// �֐����X�g
	//=============================
	// ��Ԋ֐�
	typedef void(COption_BBS::*STATE_FUNC)(const float, const float, const float);
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

	// ��Ԍn
	EState m_state;		// ���
	float m_fStateTime;	// ��ԃJ�E���^�[

	// ���̑��ϐ�
	COptionMenu* m_pOptionMenu;	// �I�v�V�������j���[
};


#endif