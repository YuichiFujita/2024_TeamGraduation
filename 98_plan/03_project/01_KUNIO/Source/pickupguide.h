//=============================================================================
// 
//  �s�b�N�A�b�v�K�C�h�w�b�_�[ [pickupguide.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PICKUPGUIDE_H_
#define _PICKUPGUIDE_H_	// ��d�C���N���[�h�h�~

#include "objectBillboard.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �s�b�N�A�b�v�K�C�h�N���X
class CPickupGuide : public CObjectBillboard
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	// ���
	enum TYPE
	{
		TYPE_TRANSFER = 0,		// �]��
		TYPE_SKILLTREE_BEGIN,	// �X�L���c���[�J�n
		TYPE_MAX
	};

	// ���
	enum STATE
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_FADEIN,	// �t�F�[�h�C��
		STATE_FADEOUT,	// �t�F�[�h�A�E�g
		STATE_MAX
	};

	CPickupGuide(int nPriority = 7);
	~CPickupGuide();


	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetState(STATE state);	// ��Ԑݒ�
	static CPickupGuide* Create(const MyLib::Vector3& pos, TYPE type);

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CPickupGuide::* STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// ��Ԋ֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐��n
	void StateNone() {}		// �Ȃɂ��Ȃ�
	void StateFadeIn();		// �t�F�[�h�C��
	void StateFadeOut();	// �t�F�[�h�A�E�g

	//=============================
	// �����o�ϐ�
	//=============================
	STATE m_state;	// ���
	float m_fStateTime;	// ��ԃJ�E���^�[
	TYPE m_type;		// ���
};


#endif