//==========================================================================
// 
//  �J�b�g�C���w�b�_�[ [cutin.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _CUT_IN_H_
#define _CUT_IN_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "object2D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �J�b�g�C���N���X
class CCutIn : public CObject2D
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum EState
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ����
		STATE_TIME,		// ���Ԍo�ߏ��
		STATE_END,		// �I�����
		STATE_MAX
	};

	CCutIn(int nPriority = 7);
	~CCutIn();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Kill() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CCutIn* Create();

	//=============================
	// �����o�֐�
	//=============================
	bool IsEnd() const { return (m_state == STATE_END); }	// �I���t���O�擾

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CCutIn::*STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFuncList[];	// �֐����X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	void UpdateTime(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���Ԍo�ߏ�Ԃ̍X�V

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fStateTime;	// ��ԃJ�E���^�[
	EState m_state;		// ���
};


#endif