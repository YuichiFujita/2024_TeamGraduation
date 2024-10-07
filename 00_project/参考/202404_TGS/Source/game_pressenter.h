//=============================================================================
// 
// �^�C�g���G���^�[ [game_pressenter.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _GAME_PRESSENTER_H_
#define _GAME_PRESSENTER_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "object2D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �^�C�g���G���^�[
class CGame_PressEnter : public CObject2D
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum STATE
	{
		STATE_NONE = 0,			// �Ȃɂ��Ȃ�
		STATE_FADEIN,			// �t�F�[�h�C��
		STATE_TUTORIAL_FADEOUT,	// �`���[�g���A���m�F�̃t�F�[�h�A�E�g
		STATE_MAX
	};

	CGame_PressEnter(float fadetime, int nPriority = 8);
	~CGame_PressEnter() {}
	
	//  �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Update() override;

	void SetState(STATE state);
	STATE GetState() { return m_state; }

	static CGame_PressEnter* Create(float fadetime);	// ��������

private:

	//=============================
	// �֐��|�C���^
	//=============================
	typedef void(CGame_PressEnter::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	void StateNone();		// �Ȃɂ��Ȃ�
	void StateFadeIn();		// �t�F�[�h�C��
	void StateTutorial_FadeOut();		// �`���[�g���A���m�F�̃t�F�[�h�A�E�g

	//=============================
	// �����o�ϐ�
	//=============================
	STATE m_state;		// ���
	float m_fStateTime;	// ��ԃJ�E���^�[
	const float m_fFadeOutTime;	// �t�F�[�h�ɂ����鎞��
};

#endif