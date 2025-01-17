//==========================================================================
//
//	���UI�w�b�_�[ [arrowUI.h]
//	Author�F���n�Ή�
//
//==========================================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ARROW_UI_H_
#define _ARROW_UI_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object2D_Anim.h"

//************************************************************
// �O���錾
//************************************************************
class CDressup;			// �����ւ��N���X
class CObject2D;		// �I�u�W�F�N�g2D�N���X
class CObject2D_Anim;	// �I�u�W�F�N�g2D�A�j���N���X
class CEntry_Dressup;	// �h���X�A�b�v�ݒ�N���X

//************************************************************
//	�N���X��`
//************************************************************
// ���UI�N���X
class CArrowUI : public CObject2D_Anim
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	// ����
	enum EDirection
	{
		DIRECTION_L = 0,	// ��
		DIRECTION_R,		// �E
		DIRECTION_MAX
	};

	// ���
	enum EState
	{
		STATE_NONE = 0,		// NONE
		STATE_NORMAL,		// �ʏ�
		STATE_SELECTMOVE,	// �I�����ړ�
		STATE_MAX
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CArrowUI(int nPriority);
	~CArrowUI() override;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Kill() override;		// �폜
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V

	//=============================
	// �����o�֐�
	//=============================
	// ���
	void SetState(const EState state);					// ��Ԑݒ�
	inline EState GetState() const { return m_state; }	// ��Ԏ擾
	
	// ���̑�
	void SetSizeByWidth(const float width);								// ��������T�C�Y�ݒ�
	inline void SetValueFloat(float value)	{ m_fValueFloat = value; }	// �ӂ�ӂ�ʐݒ�
	EDirection GetDirection()				{ return m_direction; }		// ����

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CArrowUI* Create(EDirection dir, const MyLib::Vector3& pos, const float width, const MyLib::Color& color, int nPriority = mylib_const::PRIORITY_DEF2D);	// ����

private:

	//=============================
	// �֐����X�g
	//=============================
	// ��Ԋ֐�
	typedef void(CArrowUI::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// ��Ԋ֐�

	//=============================
	// �����o�֐�
	//=============================
	// ���
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ��ԍX�V
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) {}		// NONE
	void StateNormal(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �ʏ�
	void StateSelectMove(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �I�����ړ�

	//=============================
	// �����o�ϐ�
	//=============================
	// ���
	EState m_state;		// ���
	float m_fStateTime;	// ��ԃ^�C�}�[

	// ���̑�
	EDirection m_direction;	// ����
	float m_fValueFloat;	// �ӂ�ӂ��
};

#endif	// _ARROW_UI_H_
