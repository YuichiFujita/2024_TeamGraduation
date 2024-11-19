//============================================================
//
//	�X�N���[���w�b�_�[ [screen.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SCREEN_H_
#define _SCREEN_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object2D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �X�N���[���N���X
class CScreen : public CObject2D
{
public:
	// �R���X�g���N�^
	CScreen();

	// �f�X�g���N�^
	~CScreen();

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	void Draw() override;		// �`��

	// �ÓI�����o�֐�
	static CScreen* Create(const int nScreenTexIdx);	// ����

	// �����o�֐�
	void SetEnableShader(const bool bShader)	{ m_bShader = bShader; }	// �V�F�[�_�[�ݒ�
	bool IsShader() const						{ return m_bShader; }		// �V�F�[�_�[�擾

private:
	// �����o�ϐ�
	bool m_bShader;	// �V�F�[�_�[�t���O
};

#endif	// _SCREEN_H_
