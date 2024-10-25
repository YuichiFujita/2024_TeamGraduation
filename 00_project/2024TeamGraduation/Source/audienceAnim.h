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
// �N���X��`
//==========================================================================
// �ϋq_�A�j���[�V����3D�N���X��`
class CAudienceAnim : public CAudience
{
public:

	//=============================
	// �萔
	//=============================
	static constexpr float NEAR_LINE = 1350.0f;	// ��O�̐����ʒu���
	static constexpr float FAR_LINE = 1750.0f; 	// ���̐����ʒu���

	//=============================
	// �񋓌^��`
	//=============================
	// ���[�V������
	enum EMotion
	{
		MOTION_IDOL = 0,	// �ҋ@���[�V����
		MOTION_JUMP,		// �W�����v���[�V����
		MOTION_MOVE_L,		// ���ړ����[�V����
		MOTION_MOVE_R,		// �E�ړ����[�V����
		MOTION_MAX			// ���̗񋓌^�̑���
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CAudienceAnim(EObjType type, CGameManager::TeamSide team);
	~CAudienceAnim();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	void Kill() override;	// �폜����
	inline void SetPosition(const MyLib::Vector3& pos) override	{ m_pAnimChara->SetPosition(pos); }		// �ʒu�ݒ�
	inline MyLib::Vector3 GetPosition() const override			{ return m_pAnimChara->GetPosition(); }	// �ʒu�擾
	inline void SetMove(const MyLib::Vector3& move) override	{ m_pAnimChara->SetMove(move); }		// �ړ��ʐݒ�
	inline MyLib::Vector3 GetMove() const override				{ return m_pAnimChara->GetMove(); }		// �ړ��ʎ擾
	inline void SetRotation(const MyLib::Vector3& rot) override	{ m_pAnimChara->SetRotation(rot); }		// �����ݒ�
	inline MyLib::Vector3 GetRotation() const override			{ return m_pAnimChara->GetRotation(); }	// �����擾

protected:
	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	// ��Ԋ֐�
	int UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �����Ԃ̍X�V
	int UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �ʏ��Ԃ̍X�V
	int UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ����オ���Ԃ̍X�V
	int UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �ޏ��Ԃ̍X�V

	// �Q�b�^�[/�Z�b�^�[
	void SetMotion(const int nMotion) override;	// ���[�V�����ݒ�

private:

	//=============================
	// �����o�ϐ�
	//=============================
	CObjectCharaAnim* m_pAnimChara;	// �L�����N�^�[�A�j���[�V�������
	EMotion m_moveMotion;	// �ړ����[�V����
};

#endif
