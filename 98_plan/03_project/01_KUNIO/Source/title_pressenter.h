//=============================================================================
// 
// �^�C�g���G���^�[ [title_pressenter.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TITLE_PRESSENTER_H_
#define _TITLE_PRESSENTER_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "object2D.h"

class CTitle_Select;

//==========================================================================
// �N���X��`
//==========================================================================
// �^�C�g���G���^�[
class CTitle_PressEnter : public CObject2D
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum STATE
	{
		STATE_NONE = 0,			// �Ȃɂ��Ȃ�
		STATE_FADEIN,			// �t�F�[�h�C��
		STATE_FADEOUT,		// �t�F�[�h�A�E�g
		STATE_NOACTIVE,			// �������Ȃ�
		STATE_MAX
	};

	CTitle_PressEnter(float fadetime, int nPriority = 8);
	~CTitle_PressEnter() {}
	
	//  �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	void SetState(STATE state);
	STATE GetState() { return m_state; }
	CTitle_Select* GetSelect() { return m_pSelect; }

	static CTitle_PressEnter* Create(float fadetime);	// ��������

private:

	//=============================
	// �֐��|�C���^
	//=============================
	typedef void(CTitle_PressEnter::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	void StateNone();		// �Ȃɂ��Ȃ�
	void StateFadeIn();		// �t�F�[�h�C��
	void StateFadeOut();		// �t�F�[�h�A�E�g
	void StateNoActive();		// �������Ȃ�

	//=============================
	// �����o�ϐ�
	//=============================
	STATE m_state;		// ���
	float m_fStateTime;	// ��ԃJ�E���^�[
	const float m_fFadeOutTime;	// �t�F�[�h�ɂ����鎞��
	CTitle_Select* m_pSelect;
};

#endif