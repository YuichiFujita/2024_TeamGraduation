//============================================================
//
//	�X�L�b�v����w�b�_�[ [skip.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _SKIP_H_
#define _SKIP_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�O���錾
//************************************************************
class CObject2D;	// �I�u�W�F�N�g2D�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �X�L�b�v����N���X
class CSkip
{
public:
	// ��ԗ�
	enum EState
	{
		STATE_NONE = 0,	// �������Ȃ�
		STATE_FADEOUT,	// �t�F�[�h�A�E�g
		STATE_DISP,		// �\��
		STATE_FADEIN,	// �t�F�[�h�C��
		STATE_MAX		// ���̗񋓌^�̑���
	};

	// �R���X�g���N�^
	CSkip();

	// �f�X�g���N�^
	~CSkip();

	// �����o�֐�
	HRESULT Init();	// ������
	void Uninit();	// �I��
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �X�V
	void Draw();	// �`��
	void SetDisp();	// �\���ݒ�
	bool IsDisp();	// �\���擾

	// �ÓI�����o�֐�
	static CSkip *Create();					// ����
	static void Release(CSkip *&prSkip);	// �j��

private:
	// �����o�ϐ�
	CObject2D *m_pControl;	// ����̏��
	EState m_state;			// ���
	float m_fCurTime;		// ��ԊǗ��J�E���^�[
};

#endif	// _SKIP_H_
