//==========================================================================
// 
//  ���e�����w�b�_�[ [charmText.h]
//  Author : ���n�Ή�
// 
//==========================================================================

#ifndef _CHARMTEXT_H_
#define _CHARMTEXT_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "object.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// ���e�����N���X
class CCharmText : public CObject
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum EState
	{
		STATE_FADEIN = 0,	// �t�F�[�h�C��
		STATE_WAIT,			// �ҋ@(���e�`�F�C���\)
		STATE_FADEOUT,		// �t�F�[�h�A�E�g
		STATE_MAX
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CCharmText(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_2D);
	~CCharmText();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill() override;						// �폜
	virtual void SetEnableDisp(bool bDisp) override;	// �`��󋵐ݒ�

	//=============================
	// �����o�֐�
	//=============================
	void SetState(EState state);	// ��Ԑݒ�

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CCharmText* Create();	// ����

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CCharmText::*STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];	// ��Ԋ֐�

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ��ԍX�V
	void StateFadeIn();		// �t�F�[�h�C��
	void StateWait();		// �ҋ@
	void StateFadeOut();	// �t�F�[�h�A�E�g
	
	// ����
	HRESULT CreateFace();	// ��A�C�R������
	HRESULT CreateText();	// ��������

	//=============================
	// �����o�ϐ�
	//=============================
	CObject2D* m_pFace;		// ��A�C�R��
	CObject2D* m_pText;		// ����
	EState m_state;			// ���
	float m_fStateTime;		// ��ԃ^�C�}�[
};

#endif
