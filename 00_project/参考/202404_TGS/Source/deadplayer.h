//=============================================================================
// 
//  ���S�v���C���[�w�b�_�[ [deadplayer.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _DEADPLAYER_H_
#define _DEADPLAYER_H_	// ��d�C���N���[�h�h�~

#include "listmanager.h"
#include "objectChara.h"
#include "motion.h"

// �O���錾
class CShadow;
class CEffekseerObj;

//==========================================================================
// �N���X��`
//==========================================================================
// �l�N���X
class CDeadPlayer : public CObjectChara
{
public:
	
	//=============================
	// ��ԗ�
	//=============================
	enum STATE
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_UP,		// �㏸
		STATE_FADEOUT,	// �t�F�[�h�A�E�g
		STATE_MAX
	};
	
	enum MOTION
	{
		MOTION_DEF = 0,		// �j���[�g����
		MOTION_DEAD,		// �ړ�
		MOTION_MAX
	};


	CDeadPlayer(int nPriority = mylib_const::ENEMY_PRIORITY);
	virtual ~CDeadPlayer();


	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	virtual void Kill();	// �폜

	void SetState(STATE state);		// ��Ԑݒ�
	STATE GetState() { return m_state; }
	void SetStateTime(float time) { m_fStateTime = time; }	// ��Ԏ��Ԑݒ�

	// ���[�V����
	void SetMotion(int motionIdx);	// ���[�V�����̐ݒ�

	HRESULT LoadText(const std::string& filename);

	static CListManager<CDeadPlayer> GetListObj() { return m_List; }	// ���X�g�擾
	static CDeadPlayer* Create(const MyLib::Vector3& pos);

protected:

	//=============================
	// �����o�֐�
	//=============================
	// ��ԍX�V�n
	virtual void StateNone();		// �����Ȃ����
	virtual void StateUP();			// �㏸
	virtual void StateFadeOut();	// �t�F�[�h�A�E�g

	// ���̑��֐�
	virtual void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// �U��������
	virtual void AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK) override;	// �U�����蒆����

	//=============================
	// �����o�ϐ�
	//=============================
	STATE m_state;							// ���
	STATE m_Oldstate;						// �O��̏��
	float m_fStateTime;						// ��ԃJ�E���^�[
	D3DXCOLOR m_mMatcol;					// �}�e���A���̐F

private:
	
	//=============================
	// �֐����X�g
	//=============================
	typedef void(CDeadPlayer::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];	// ��Ԋ֐����X�g

	void UpdateState();					// ��ԍX�V����
	void LimitArea(); // ��l�̕ǔ���

	//=============================
	// �����o�ϐ�
	//=============================
	CShadow *m_pShadow;			// �e�̏��
	CEffekseerObj* m_pEfkEffect;	// �G�t�F�N�V�A�I�u�W�F�N�g
	static CListManager<CDeadPlayer> m_List;	// ���X�g
};



#endif