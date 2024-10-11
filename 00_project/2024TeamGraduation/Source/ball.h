//=============================================================================
// 
//  �{�[���w�b�_�[ [ball.h]
//  Author : ���c�E��
// 
//=============================================================================

#ifndef _BALL_H_
#define _BALL_H_	// ��d�C���N���[�h�h�~

#include "objectX.h"
#include "listmanager.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �{�[���N���X
class CBall : public CObjectX
{
public:
	
	//=============================
	// �񋓌^��`
	//=============================
	enum EState
	{
		STATE_SPAWN = 0,	// ������� (�J�n���̃t���[�{�[��)
		STATE_CATCH,		// ������� (�v���C���[���ێ����Ă���)
		STATE_ATTACK,		// �U����� (�U������������Ă���)
		STATE_FALL,			// ������� (�n�ʂɓ]�����Ă���)
		STATE_MAX			// ���̗񋓌^�̑���
	};

	CBall(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CBall();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;

	//=============================
	// �����o�֐�
	//=============================
	void Kill();	// �폜

	//=============================
	// �ÓI�֐�
	//=============================
	/*
		@brief		��������
		@details	�K�v������Έ����ǉ�
	*/
	static CBall *Create();
	static CListManager<CBall> GetListObj() { return m_List; }	// ���X�g�擾

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CBall::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_SampleFuncList[];	// �֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	void UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ������Ԃ̍X�V
	void UpdateCatch(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ������Ԃ̍X�V
	void UpdateAttack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �U����Ԃ̍X�V
	void UpdateFall(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ������Ԃ̍X�V

	//=============================
	// �ÓI�����o�ϐ�
	//=============================
	static CListManager<CBall> m_List;	// ���X�g

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fStateTime;	// ��ԃJ�E���^�[
	EState m_state;		// ���

};


#endif