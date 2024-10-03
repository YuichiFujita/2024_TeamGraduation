//=============================================================================
// 
//  �����w�b�_�[ [subtitle.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SUBTITLE_H_
#define _SUBTITLE_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �����N���X
class CSubTitle : public CObject2D
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_FADEIN,		// �t�F�[�h�C��
		STATE_FADEOUT,		// �t�F�[�h�A�E�g
		STATE_MAX
	};

	CSubTitle(int nPriority = 7);
	~CSubTitle();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void BindSubtitle(const std::string& texfile);	// �������蓖��
	void SetSizeByHeight(const float height);		// ��������ɃT�C�Y�ݒ�
	void SetState(State state);	// ��Ԑݒ�

	//=============================
	// �ÓI�֐�
	//=============================
	static CSubTitle* Create(const MyLib::Vector3& pos, float life = 1.0f);	// ��������

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CSubTitle::*STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// �֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void UpdateState();		// ��ԍX�V
	void StateNone();		// �Ȃ�
	void StateFadeIn();		// �t�F�[�h�C��
	void StateFadeout();	// �t�F�[�h�A�E�g

	// ���̑��֐�

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	float m_fLifeTimer;		// ��������
	float m_fStateTime;		// ��ԃJ�E���^�[
	State m_state;			// ���

};


#endif