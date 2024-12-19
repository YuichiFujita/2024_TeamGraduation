//==========================================================================
// 
//  �ϋq_���[�|���L�����w�b�_�[ [audienceLowPoly.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _AUDIENCE_LOWPOLY_H_
#define _AUDIENCE_LOWPOLY_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "audience.h"
#include "gamemanager.h"
#include "objectChara.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObjectX;

//==========================================================================
// �N���X��`
//==========================================================================
// �ϋq_���[�|���L�����N���X��`
class CAudienceLowPoly : public CAudience
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	// ���[�V������
	enum EMotion
	{
		MOTION_DEF = 0,	// �ʏ���
		MOTION_SPAWN,	// ������
		MOTION_JUMP,	// ����オ����
		MOTION_SPECIAL,	// �X�y�V�������
		MOTION_DESPAWN,	// �ޏ���
		MOTION_MAX		// ���̗񋓌^�̑���
	};

	// �ϐ�G���A��
	enum EArea
	{
		AREA_FAR = 0,	// ��
		AREA_SIDE,		// ��
		AREA_NEAR,		// ��O
		AREA_MAX		// ���̗񋓌^�̑���
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CAudienceLowPoly(EObjType type, CGameManager::ETeamSide team);
	~CAudienceLowPoly();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	void Kill() override;	// �폜����
	inline void SetPosition(const MyLib::Vector3& pos) override	{ m_pChara->SetPosition(pos); }		// �ʒu�ݒ�
	inline MyLib::Vector3 GetPosition() const override			{ return m_pChara->GetPosition(); }	// �ʒu�擾
	inline void SetMove(const MyLib::Vector3& move) override	{ m_pChara->SetMove(move); }		// �ړ��ʐݒ�
	inline MyLib::Vector3 GetMove() const override				{ return m_pChara->GetMove(); }		// �ړ��ʎ擾
	inline void SetRotation(const MyLib::Vector3& rot) override	{ m_pChara->SetRotation(rot); }		// �����ݒ�
	inline MyLib::Vector3 GetRotation() const override			{ return m_pChara->GetRotation(); }	// �����擾

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
	void EndSettingSpawn() override;			// �X�|�[���I�����̐ݒ�
	virtual bool SetDespawn(EObjType type = OBJTYPE_NONE) override;	// �ޏ�ݒ�
	virtual bool SetNTR(CGameManager::ETeamSide team) override;									// NTR�ݒ�

private:

	//=============================
	// �֐����X�g
	//=============================
	// �ϐ�ʒu�v�Z���X�g
	typedef void(CAudienceLowPoly::*WATCH_POS_FUNC)();
	static WATCH_POS_FUNC m_CalcWatchPositionFunc[];

	//=============================
	// �����o�֐�
	//=============================
	HRESULT CreateCharacter(const MyLib::Vector3& rPos, const MyLib::Vector3& rRot);	// �L�����N�^�[����
	HRESULT CreatePenLight();		// �y�����C�g����
	virtual void CalcWatchPositionFar();	// �ϐ�ʒu�v�Z (��)
	virtual void CalcWatchPositionSide();	// �ϐ�ʒu�v�Z (��)
	virtual void CalcWatchPositionNear();	// �ϐ�ʒu�v�Z (��O)

	//=============================
	// �����o�ϐ�
	//=============================
	CObjectChara* m_pChara;	// �L�����N�^�[���
	CObjectX* m_pLight;		// �y�����C�g���
};

#endif
