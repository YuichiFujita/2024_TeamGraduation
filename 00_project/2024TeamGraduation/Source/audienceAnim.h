//==========================================================================
// 
//  �ϋq_�A�j���[�V����3D�w�b�_�[ [audienceAnim.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _AUDIENCE_ANIM_H_
#define _AUDIENCE_ANIM_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "audience.h"
#include "gamemanager.h"
#include "objectCharaAnim.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObjectX;

//==========================================================================
// �N���X��`
//==========================================================================
// �ϋq_�A�j���[�V����3D�N���X��`
class CAudienceAnim : public CAudience
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	// ���[�V������
	enum EMotion
	{
		MOTION_IDOL_U = 0,	// ��ҋ@���[�V����
		MOTION_IDOL_D,		// ���ҋ@���[�V����
		MOTION_JUMP_U,		// ��W�����v���[�V����
		MOTION_JUMP_D,		// ���W�����v���[�V����
		MOTION_MOVE_L,		// ���ړ����[�V����z 
		MOTION_MOVE_R,		// �E�ړ����[�V����
		MOTION_MAX			// ���̗񋓌^�̑���
	};

	// �ϐ�G���A��
	enum EArea
	{
		AREA_FAR = 0,	// ��
		AREA_UP,		// ��
		AREA_NEAR,		// ��O
		AREA_MAX		// ���̗񋓌^�̑���
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CAudienceAnim(EObjType type, CGameManager::ETeamSide team);
	~CAudienceAnim();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	void Kill() override;	// �폜����
	void SetPosition(const MyLib::Vector3& pos) override;	// �ʒu�ݒ�
	void SetMove(const MyLib::Vector3& move) override;		// �ړ��ʐݒ�
	void SetRotation(const MyLib::Vector3& rot) override;	// �����ݒ�
	inline MyLib::Vector3 GetPosition() const override		{ return m_pFrontAnimChara->GetPosition(); }	// �ʒu�擾
	inline MyLib::Vector3 GetMove() const override			{ return m_pFrontAnimChara->GetMove(); }		// �ړ��ʎ擾
	inline MyLib::Vector3 GetRotation() const override		{ return m_pFrontAnimChara->GetRotation(); }	// �����擾

protected:
	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	// ��Ԋ֐�
	int UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �����Ԃ̍X�V
	int UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �ʏ��Ԃ̍X�V
	int UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;		// ����オ���Ԃ̍X�V
	int UpdateSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�y�V������Ԃ̍X�V
	int UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �ޏ��Ԃ̍X�V

	// �Q�b�^�[/�Z�b�^�[
	void SetMotion(const int nMotion) override;	// ���[�V�����ݒ�
	void EndSettingSpawn() override {}			// �X�|�[���I�����̐ݒ�
	void SetIdolMotion(const EMotion motion) { m_idolMotion = motion; }		// �ҋ@���[�V����
	void SetJumpMotion(const EMotion motion) { m_jumpMotion = motion; }		// �W�����v���[�V����
	void SetMoveMotion(const EMotion motion) { m_moveMotion = motion; }		// �ړ����[�V����
	virtual bool SetNTR(CGameManager::ETeamSide team) override;									// NTR�ݒ�

private:

	//=============================
	// �֐����X�g
	//=============================
	// �ϐ�ʒu�v�Z���X�g
	typedef void(CAudienceAnim::*WATCH_POS_FUNC)();
	static WATCH_POS_FUNC m_CalcWatchPositionFunc[];

	//=============================
	// �����o�֐�
	//=============================
	HRESULT CreateAnimCharacter(const MyLib::Vector3& rPos);	// �L�����N�^�[����
	HRESULT CreatePenLight();		// �y�����C�g����
	virtual void CalcWatchPositionFar();	// �ϐ�ʒu�v�Z (��)
	virtual void CalcWatchPositionUp();		// �ϐ�ʒu�v�Z (��)
	virtual void CalcWatchPositionNear();	// �ϐ�ʒu�v�Z (��O)

	//=============================
	// �����o�ϐ�
	//=============================
	CObjectCharaAnim* m_pFrontAnimChara;	// �\�ʃL�����N�^�[���
	CObjectCharaAnim* m_pBackAnimChara;		// ���ʃL�����N�^�[���
	CObjectX* m_pLight;		// �y�����C�g���
	EMotion m_idolMotion;	// �ҋ@���[�V����
	EMotion m_jumpMotion;	// �W�����v���[�V����
	EMotion m_moveMotion;	// �ړ����[�V����
};

#endif
