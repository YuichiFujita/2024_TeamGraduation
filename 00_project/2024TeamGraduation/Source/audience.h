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
	static constexpr float MAX_LEFT_LINE = -1000.0f;	// ���̐����ʒu���
	static constexpr float MAX_RIGHT_LINE = 1000.0f;	// �E�̐����ʒu���

	//=============================
	// �񋓌^��`
	//=============================
	// �I�u�W�F�N�g���
	enum EObjType
	{
		OBJTYPE_ANIM = 0,	// 3D�|���S��
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
		STATE_DESPAWN,		// �ޏ���
		STATE_MAX			// ���̗񋓌^�̑���
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CAudience(EObjType type, int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
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


	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CAudience* Create(EObjType type);

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CAudience::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFuncList[];	// �֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	void UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �����Ԃ̍X�V
	void UpdateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �ʏ��Ԃ̍X�V
	void UpdateJump(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ����オ���Ԃ̍X�V
	void UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �ޏ��Ԃ̍X�V

	//=============================
	// �ÓI�����o�ϐ�
	//=============================
	static CListManager<CAudience> m_list;	// ���X�g

	//=============================
	// �����o�ϐ�
	//=============================
	const EObjType m_type;		// �I�u�W�F�N�g���
	const float m_fJumpLevel;	// �W�����v��
	EState m_state;	// ���
};

#endif
