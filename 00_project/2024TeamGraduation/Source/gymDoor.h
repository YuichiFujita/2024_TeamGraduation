//==========================================================================
// 
//  �̈�كh�A�w�b�_�[ [gymDoor.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _GYM_DOOR_
#define _GYM_DOOR_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "object.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObjectX;

//==========================================================================
// �N���X��`
//==========================================================================
// �̈�كh�A�N���X
class CGymDoor : public CObject
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	// ��ԗ�
	enum EState
	{
		STATE_CLOSE = 0,	// ���Ă���
		STATE_MOVE,			// �J��
		STATE_OPEN,			// �J���Ă���
		STATE_MAX			// ���̗񋓌^�̑���
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CGymDoor(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CGymDoor();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill() override;	// �폜
	virtual void SetEnableDisp(bool bDisp) override;				// �`��󋵐ݒ�
	virtual void SetPosition(const MyLib::Vector3& pos) override;	// �ʒu�ݒ�

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CGymDoor* Create(const MyLib::Vector3& rPos);	// ����

	//=============================
	// �����o�֐�
	//=============================
	void SetEnableOpen(const bool bOpen, const float fMoveTime = 0.0f);		// �h�A�J���t���O�ݒ�
	inline bool IsOpen() const { return m_bOpen; }							// �h�A�J���t���O�擾
	inline bool IsMove() const { return (m_state == EState::STATE_MOVE); }	// �h�A�ғ����t���O�擾

private:

	//=============================
	// �񋓌^��`
	//=============================
	// �h�A��
	enum EDoor
	{
		DOOR_LEFT = 0,	// ���h�A
		DOOR_RIGHT,		// �E�h�A
		DOOR_MAX		// ���̗񋓌^�̑���
	};

	//=============================
	// �����o�֐�
	//=============================
	void SetPositionRelative(void);	// ���Έʒu�ݒ�

	//=============================
	// �����o�ϐ�
	//=============================
	CObjectX* m_apDoor[DOOR_MAX];	// �h�A���
	EState m_state;			// ���
	float m_fCurMoveTime;	// ���݂̃h�A�J�^�C�}�[
	float m_fEndMoveTime;	// �I���̃h�A�J�^�C�}�[
	float m_fCurOffset;		// ���݂̃h�A�z�u�I�t�Z�b�g
	float m_fInitOffset;	// �J�n�̃h�A�z�u�I�t�Z�b�g
	float m_fDestOffset;	// �ڕW�̃h�A�z�u�I�t�Z�b�g
	bool m_bOpen;			// �J�t���O
};

#endif
