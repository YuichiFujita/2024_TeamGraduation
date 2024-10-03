//=============================================================================
// 
//  �����}�[�N�w�b�_�[ [discovery.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _DISCOVERY_H_
#define _DISCOVERY_H_	// ��d�C���N���[�h�h�~

#include "objectBillboard.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �����}�[�N�N���X
class CDiscovery : public CObjectBillboard
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum State
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_APPEARANCE,	// �o��
		STATE_WAIT,			// �ҋ@
		STATE_FADEOUT,		// �t�F�[�h�A�E�g
		STATE_MAX
	};

	CDiscovery(int nPriority = 7);
	~CDiscovery();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();

	static CDiscovery* Create(const MyLib::Vector3& pos);

private:
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CDiscovery::* STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// �֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void UpdateState();		// ��ԍX�V
	void SetState(State state);		// ��Ԑݒ�
	void StateNone();		// �ʏ�
	void StateAppearance();	// �o��
	void StateWait();		// �ҋ@
	void StateFadeOut();	// �t�F�[�h�A�E�g

	//=============================
	// �����o�ϐ�
	//=============================
	State m_state;			// ���
	float m_fStateTime;		// ��ԃJ�E���^�[
};


#endif