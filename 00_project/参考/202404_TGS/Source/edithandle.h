//=============================================================================
// 
// �n���h������ [edithandle.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EDITHANDLE_H_
#define _EDITHANDLE_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "objectX.h"
class CObjectLine;

//==========================================================================
// �N���X��`
//==========================================================================
// �n���h���N���X
class CHandle : public CObject
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum HandleType
	{
		TYPE_MOVE = 0,	// �ړ�
		TYPE_SCALE,		// �T�C�Y
		TYPE_MAX
	};

	enum HandleAngle
	{
		ANGLE_X = 0,
		ANGLE_Y,
		ANGLE_Z,
		ANGLE_MAX
	};

	enum State
	{
		STATE_NONE = 0,	// �����Ȃ�
		STATE_GRAB,		// �͂�
		STATE_MAX
	};

	//=============================
	// �\���̒�`
	//=============================
	struct SEditHandleInfo
	{
		MyLib::Vector3 pos;		// �ʒu
		MyLib::Vector3 rot;		// ����
		MyLib::Vector3 scale;	// �X�P�[��

		SEditHandleInfo() : pos(0.0f), rot(0.0f), scale(0.0f) {}
	};


	CHandle(int nPriority = 8, const LAYER layer = LAYER::LAYER_LINE);
	~CHandle();

	//  �I�[�o�[���C�h�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetPosition(const MyLib::Vector3& pos) override;		// �ʒu�ݒ�

	float GetScale() { return m_scale; }						// �}�b�v�ɑ΂��ẴX�P�[���擾
	void SetAngle(HandleAngle angle) { m_moveAngle = angle; }	// �����ݒ�
	void Kill();
	bool IsHoverHandle() { return m_bHoverHandle; }				// �n���h���Ƃ̃z�o�[����
	HandleAngle GetHoverAngle() { return m_moveAngle; }			// �z�o�[���Ă�������擾
	void SetState(State state) { m_state = state; }				// ��Ԑݒ�

	virtual SEditHandleInfo GetDiff(HandleAngle angle) = 0;		// �����擾
	virtual void Grab() = 0;	// �͂ݒ�����

	static CHandle* Create(const HandleType type, const MyLib::Vector3& pos);
	static CListManager<CObjectX> GetList() { return m_List; }

protected:

	//=============================
	// ���L�ϐ�
	//=============================
	float m_scale;							// �}�b�v�ɑ΂��ẴX�P�[��

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CHandle::*STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	void StateNone();	// �Ȃɂ��Ȃ�
	void StateGrab();	// �͂�

	// ���̑�
	void ReSizeToCamera();	// �J�����ɑ΂��ă��T�C�Y
	void CollisionHandle();	// �n���h���Ƃ̔���

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::AABB m_AABB;
	CObjectX* m_pArrow[ANGLE_MAX];			// ���C���̃I�u�W�F�N�g
	CObjectLine* m_pLine[ANGLE_MAX];		// ���C���̃I�u�W�F�N�g
	CObjectX* m_pCenterSphere;				// ���S
	HandleAngle m_moveAngle;				// �ړ��̌���
	HandleType m_HandleType;				// �n���h���̎��
	bool m_bHoverHandle;					// �n���h���̃z�o�[����
	State m_state;							// ���
	static CListManager<CObjectX> m_List;	// �ړ��n���h�����X�g
};
#endif