//=============================================================================
// 
//  �T���v��_�L�����N�^�[�w�b�_�[ [sample_character.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_	// ��d�C���N���[�h�h�~

#include "objectChara.h"
#include "listmanager.h"

// �O���錾
class CShadow;

//==========================================================================
// �N���X��`
//==========================================================================
// �T���v��_�L�����N�^�[�N���X
class CSample_Character : public CObjectChara
{
public:
	
	//=============================
	// �񋓌^��`
	//=============================
	// ��Ԓ�`
	enum STATE
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_MAX
	};

	// ���[�V������`
	enum MOTION
	{
		MOTION_DEFAULT = 0,	// �f�t�H���g
		MOTION_MAX
	};

	
	CSample_Character(int nPriority = 2);
	~CSample_Character();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	//=============================
	// �����o�֐�
	//=============================
	// ���
	void SetState(STATE state)			{ m_state = state; }	// ��Ԑݒ�
	CSample_Character::STATE GetState() { return m_state; }		// ��Ԏ擾

	// ���̑�
	void Kill();	// �폜


	//=============================
	// �ÓI�֐�
	//=============================
	static CSample_Character* Create();	// ����
	static CListManager<CSample_Character> GetListObj() { return m_List; }	// ���X�g�擾

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CSample_Character::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	void UpdateState();		// ��ԍX�V
	void StateNone();		// �Ȃ�

	// ���̑��֐�

	// ���[�V�����n�֐�
	void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// �U��������
	void AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK) override;	// �U�����蒆����

	//=============================
	// �����o�ϐ�
	//=============================
	STATE m_state;			// ���
	CShadow* m_pShadow;		// �e�̏��
	D3DXCOLOR m_mMatcol;	// �}�e���A���̐F
	static CListManager<CSample_Character> m_List;	// ���X�g
};


#endif