//=============================================================================
// 
//  �J�ڂȂ��t�F�[�h�w�b�_�[ [instantfade.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _INSTANTFADE_H_
#define _INSTANTFADE_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h
//==========================================================================
#include "object2D.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �J�ڂȂ��t�F�[�h�N���X
class CInstantFade
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum EState
	{
		STATE_NONE = 0,			// �������Ă��Ȃ�
		STATE_FADEOUT,			// �t�F�[�h�A�E�g
		STATE_FADEIN,			// �t�F�[�h�C��
		STATE_FADECOMPLETION,	// �t�F�[�h����
		STATE_MAX
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CInstantFade();
	~CInstantFade();

	//=============================
	// �����o�֐�
	//=============================
	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void Draw();

	bool IsCompletion() { return (m_state == EState::STATE_FADECOMPLETION); }	// �����t���O�擾
	EState GetState() { return m_state; }	// ��Ԏ擾
	void SetFade(const D3DXCOLOR& FadeColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), float fDuration = 0.5f);	// �t�F�[�h�ݒ�

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CInstantFade* Create();
private:

	float m_fDuration;			// �t�F�[�h�܂ł̎���
	float m_fTimerMove;			// �t�F�[�h����
	CObject2D *m_aObject2D;		// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	EState m_state;				// ���
};

#endif