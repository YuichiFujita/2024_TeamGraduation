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
#include "gameManager.h"
#include "charmManager.h"

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
		STATE_FADEIN = 0,	// �t�F�[�h�C��(���e�`�F�C���\)
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
	void SetCountUP(int nCnt) { m_nCntUp = nCnt; }	// �㏸�J�E���g�ݒ�
	void AddCountUP(int nCnt) { m_nCntUp += nCnt; }	// �㏸�J�E���g���Z
	void SetState(EState state);					// ��Ԑݒ�

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CCharmText* Create(CGameManager::ETeamSide side);	// ����

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
	
	// ����
	HRESULT CreateFace();	// ��A�C�R������
	HRESULT CreateText();	// ��������

protected:

	//=============================
	// �萔
	//=============================
	static constexpr float STATETIME_FADEIN = 0.2f;
	static constexpr float STATETIME_WAIT = CCharmManager::TIME_CHAIN - STATETIME_FADEIN;
	static constexpr float STATETIME_FADEOUT = 0.5f;
	static constexpr float DISTANCE_UP = 40.0f;	// �㏸�Ԋu

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	virtual void StateFadeIn();		// �t�F�[�h�C��
	virtual void StateWait();		// �ҋ@
	virtual void StateFadeOut();	// �t�F�[�h�A�E�g

	//=============================
	// �����o�ϐ�
	//=============================
	CObject2D* m_pFace;		// ��A�C�R��
	CObject2D* m_pText;		// ����
	EState m_state;			// ���
	float m_fStateTime;		// ��ԃ^�C�}�[
	int m_nCntUp;			// �㏸�J�E���g
};

#endif
