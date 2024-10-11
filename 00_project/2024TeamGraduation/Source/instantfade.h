//=============================================================================
// 
//  �J�ڂȂ��t�F�[�h�w�b�_�[ [instantfade.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _INSTANTFADE_H_
#define _INSTANTFADE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object2D.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �J�ڂȂ��t�F�[�h�N���X��`
class CInstantFade
{
public:

	// �񋓌^��`
	enum STATE
	{
		STATE_NONE = 0,			// �������Ă��Ȃ�
		STATE_FADEOUT,			// �t�F�[�h�A�E�g
		STATE_FADEIN,			// �t�F�[�h�C��
		STATE_FADECOMPLETION,	// �t�F�[�h����
		STATE_MAX
	};

	CInstantFade();
	~CInstantFade();

	// �����o�֐�
	HRESULT Init();
	void Uninit();
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void Draw();

	CObject2D *GetMyObject();
	static CInstantFade *Create();
	void SetFade(D3DXCOLOR FadeColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), float fDuration = 0.5f);	// �t�F�[�h�ݒ�
	STATE GetState();	// ��Ԏ擾

private:

	float m_fDuration;			// �t�F�[�h�܂ł̎���
	float m_fTimerMove;			// �t�F�[�h����
	CObject2D *m_aObject2D;		// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
	STATE m_state;				// ���
};

#endif