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
#include "thoughtBalloon.h"

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
	CCharmText(int nPriority = mylib_const::PRIORITY_ZSORT, const LAYER layer = LAYER::LAYER_2D);
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
	void SetEnableTextDisp(bool bDisp)					{ m_pThoughtBalloon->SetEnableTextDisp(bDisp); }	// �e�L�X�g�\���ݒ�
	void SetCountUP(int nCnt)							{ m_nCntUp = nCnt; }					// �㏸�J�E���g�ݒ�
	void AddCountUP(int nCnt)							{ m_nCntUp += nCnt; }					// �㏸�J�E���g���Z
	void SetEnablePossibleChain(bool bChain)			{ m_bPossibleChain = bChain; }			// �`�F�C���\�t���O�ݒ�
	bool IsPossibleChain()								{ return m_bPossibleChain; }			// �`�F�C���\�t���O�擾
	void SetMyChainIdx(int idx)							{ m_nMyChainIdx = idx; }				// �����̃`�F�C���C���f�b�N�X
	int GetMyChainIdx()									{ return m_nMyChainIdx; }				// �����̃`�F�C���C���f�b�N�X
	void SetChainIdx(const std::vector<int>& vecIdx)	{ m_nVecChainIdx = vecIdx; }			// �`�F�C�����Ă���C���f�b�N�X�ԍ��ݒ�
	std::vector<int> GetChainIdx()						{ return m_nVecChainIdx; }				// �`�F�C�����Ă���C���f�b�N�X�ԍ��擾
	void SetState(EState state);			// ��Ԑݒ�
	EState GetState() { return m_state; }	// ��Ԏ擾

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

	//--------------------------
	// �`�F�C��
	//--------------------------
	virtual void SetChain(int nChainIdx);	// �`�F�C���ݒ�
	virtual void CheckChain() = 0;			// �`�F�C���̊m�F
	//virtual void EffectChain();				// �`�F�C���G�t�F�N�g

	// ����
	HRESULT CreateText();	// ��������

protected:

	//=============================
	// �萔
	//=============================
	static constexpr float STATETIME_FADEIN = 0.3f;
	static constexpr float STATETIME_WAIT = CCharmManager::TIME_CHAIN - STATETIME_FADEIN;
	static constexpr float STATETIME_FADEOUT = 0.5f;
	static constexpr float DISTANCE_UP = 20.0f;	// �㏸�Ԋu
	static constexpr float DISTANCE_XZ = 80.0f;	// �㏸�Ԋu

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
	CThoughtBalloon* m_pThoughtBalloon;	// ���������o��
	EState m_state;			// ���
	float m_fStateTime;		// ��ԃ^�C�}�[
	int m_nCntUp;			// �㏸�J�E���g
	bool m_bPossibleChain;	// �`�F�C���\�t���O
	int m_nMyChainIdx;		// �����̃`�F�C���C���f�b�N�X
	CGameManager::ETeamSide m_TeamSide;		// �`�[���T�C�h
	std::vector<int> m_nVecChainIdx;	// �`�F�C�����Ă���C���f�b�N�X�ԍ�
};

#endif
