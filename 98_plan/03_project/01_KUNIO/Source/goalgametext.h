//=============================================================================
// 
// �S�[���Q�[���e�L�X�g���� [goalgametext.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _GOALGAMETEXT_H_
#define _GOALGAMETEXT_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "object2D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �S�[���Q�[���e�L�X�g�N���X��`
class CGoalGameText : public CObject2D
{
public:

	CGoalGameText(int nPriority = 8);
	~CGoalGameText();

	
	//  �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

	static CGoalGameText* Create();

private:

	//=============================
	// ��ԗ�
	//=============================
	enum eState
	{
		STATE_EXPANSION = 0,
		STATE_EXPNONE,
		STATE_FADEOUT,
		STATE_MAX
	};

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CGoalGameText::* STATE_FUNC)();	// ��ԏ����̃��X�g
	static STATE_FUNC m_StateFuncList[];

	//=============================
	// �����o�֐�
	//=============================
	void UpdateState();		// ��ԍX�V
	void StateExpansion();	// �g��
	void StateExpNone();	// �g��㉽�����Ȃ�
	void StateFadeOut();	// �t�F�[�h�A�E�g���

	//=============================
	// �����o�ϐ�
	//=============================
	eState m_state;		// ���
	float m_fStateTimer;	// ��ԃ^�C�}�[

};

#endif