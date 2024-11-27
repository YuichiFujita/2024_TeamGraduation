//==========================================================================
// 
//  �ϋq�w�b�_�[ [audience.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _AUDIENCE_H_
#define _AUDIENCE_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "object.h"
#include "gamemanager.h"
#include "listmanager.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �ϋq�N���X
class CAudience : public CObject
{
public:

	//=============================
	// �萔
	//=============================
	static constexpr float MAX_LEFT_LINE = -2100.0f;	// ���̐����ʒu���
	static constexpr float MAX_RIGHT_LINE = 2100.0f;	// �E�̐����ʒu���
	static constexpr float SPAWN_SIDE_LINE = 2500.0f;	// �������鉡�ʒu
	static constexpr int MAX_WATCH = 500;				// �ϐ킷��ő�l��

	//=============================
	// �񋓌^��`
	//=============================
	// �I�u�W�F�N�g���
	enum EObjType
	{
		OBJTYPE_NONE = 0,	// �Ȃ�
		OBJTYPE_ANIM,		// 3D�|���S��
		OBJTYPE_LOWPOLY,	// ���[�|���L�����N�^�[
		OBJTYPE_HIGHPOLY,	// �n�C�|���L�����N�^�[
		OBJTYPE_MAX			// ���̗񋓌^�̑���
	};

	// ���
	enum EState
	{
		STATE_SPAWN = 0,	// ������
		STATE_NORMAL,		// �ʏ���
		STATE_JUMP,			// ����オ����
		STATE_SPECIAL,		// �X�y�V�������
		STATE_DESPAWN,		// �ޏ���
		STATE_MAX			// ���̗񋓌^�̑���
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CAudience(EObjType type, CGameManager::ETeamSide team, int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CAudience();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill();	// �폜

	//=============================
	// �������z�֐�
	//=============================
	virtual void SetPosition(const MyLib::Vector3& pos) override = 0;	// �ʒu�ݒ�
	virtual MyLib::Vector3 GetPosition() const override = 0;			// �ʒu�擾
	virtual void SetMove(const MyLib::Vector3& move) override = 0;		// �ړ��ʐݒ�
	virtual MyLib::Vector3 GetMove() const override = 0;				// �ړ��ʎ擾
	virtual void SetRotation(const MyLib::Vector3& rot) override = 0;	// �����ݒ�
	virtual MyLib::Vector3 GetRotation() const override = 0;			// �����擾

	//=============================
	// ���z�֐�
	//=============================


	//=============================
	// �����o�֐�
	//=============================
	void SetEnableJump(const bool bJump);	// ����オ��ݒ�
	void SetSpecial();						// �X�y�V�����ݒ�
	virtual bool SetDespawn(EObjType type = OBJTYPE_NONE);	// �ޏ�ݒ�

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CAudience* Create(EObjType type, CGameManager::ETeamSide team);				// ����
	static HRESULT SetNumWatch(const int nNumWatch, CGameManager::ETeamSide team);		// �ϐ풆�̐l���ݒ�
	static int GetNumWatchAll(CGameManager::ETeamSide team);								// �S�ϐ풆�̐l���擾
	static void SetEnableJumpAll(const bool bJump, CGameManager::ETeamSide team);		// �S����オ��ݒ�
	static void SetSpecialAll(CGameManager::ETeamSide team);								// �S�X�y�V�����ݒ�
	static void SetDespawnAll(CGameManager::ETeamSide team, const int nNumDespawn = -1);	// �S�ޏ�ݒ�

protected:

	//=============================
	// �������z�֐�
	//=============================
	// ��Ԋ֐�
	virtual int UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;		// �����Ԃ̍X�V
	virtual int UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// �ʏ��Ԃ̍X�V
	virtual int UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;		// ����オ���Ԃ̍X�V
	virtual int UpdateSpecial(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// �X�y�V������Ԃ̍X�V
	virtual int UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) = 0;	// �ޏ��Ԃ̍X�V

	// �Q�b�^�[/�Z�b�^�[
	virtual void SetMotion(const int nMotion) = 0;	// ���[�V�����ݒ�
	virtual void EndSettingSpawn() = 0;				// �X�|�[���I�����̐ݒ�

	//=============================
	// �����o�֐�
	//=============================
	// �Q�b�^�[/�Z�b�^�[
	inline void SetSpawnPosition(const MyLib::Vector3& rPos)	{ m_posSpawn = rPos;}	// ����ʒu�ݒ�
	inline MyLib::Vector3 GetSpawnPosition() const				{ return m_posSpawn; }	// ����ʒu�擾
	inline void SetWatchPosition(const MyLib::Vector3& rPos)	{ m_posWatch = rPos;}	// �ϐ�ʒu�ݒ�
	inline MyLib::Vector3 GetWatchPosition() const				{ return m_posWatch; }	// �ϐ�ʒu�擾
	inline CGameManager::ETeamSide GetTeam() const				{ return m_team; }		// �����`�[���擾

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef int(CAudience::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFuncList[];	// �֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// �ėp�֐�
	bool UpdateGravity(MyLib::Vector3* pPos, MyLib::Vector3* pMove, const float fDeltaTime, const float fDeltaRate, const float fSlowRate, const float fGravityRate = 1.0f);	// �d�͍X�V

	//=============================
	// �ÓI�����o�ϐ�
	//=============================
	static CListManager<CAudience> m_list;	// ���X�g
	static int m_aNumWatchAll[CGameManager::ETeamSide::SIDE_MAX];			// �ϐ풆�̐l��

	//=============================
	// �����o�ϐ�
	//=============================
	const CGameManager::ETeamSide m_team;	// �����`�[��
	const EObjType m_type;		// �I�u�W�F�N�g���
	const float m_fJumpLevel;	// �W�����v��
	MyLib::Vector3 m_posSpawn;	// ����ʒu
	MyLib::Vector3 m_posWatch;	// �ϐ�ʒu
	MyLib::Vector3 m_posDespawnStart;	// �ޏ�J�n�ʒu
	EState m_state;		// ���
	float m_fTimeState;	// ��ԊǗ�����
};

#endif
