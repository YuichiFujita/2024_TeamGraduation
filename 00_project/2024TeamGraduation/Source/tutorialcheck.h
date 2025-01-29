//=============================================================================
//
//  �`���[�g���A���m�F��ʃw�b�_�[ [tutorialcheck.h]
//  Author�F���n�Ή�
//
//=============================================================================
//==========================================================================
// ��d�C���N���[�h�h�~
//==========================================================================
#ifndef _TUTORIAL_CHECK_H_
#define _TUTORIAL_CHECK_H_

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "object2D.h"

//==========================================================================
// �O���錾
//==========================================================================
class CTitle_ControllWait;	// ����ҋ@�V�[��

//==========================================================================
// �N���X��`
//==========================================================================
// �`���[�g���A���m�F��ʃN���X
class CTutorialCheck : public CObject2D
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	// ���
	enum EState
	{
		STATE_NONE = 0,	// �Ȃɂ��Ȃ�
		STATE_FADEIN,	// �t�F�[�h�C��
		STATE_FADEOUT,	// �t�F�[�h�A�E�g
		STATE_MAX
	};

	// �I����
	enum ESelect
	{
		SELECT_YES = 0,	// �`���[�g���A������
		SELECT_NO,		// ���Ȃ�
		SELECT_MAX
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CTutorialCheck(int controllIdx, int nPriority = mylib_const::PRIORITY_DEF2D);
	~CTutorialCheck() override;

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Kill() override;		// �폜
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V

	// �����o�֐�
	void SetState(EState state);	// ��Ԑݒ�

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CTutorialCheck* Create(int controllIdx, CTitle_ControllWait* pControllWait);	// ����

private:

	//=============================
	// �֐��|�C���^
	//=============================
	typedef void(CTutorialCheck::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	// ���
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ��ԍX�V
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �����Ȃ�
	void StateFadeIn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �t�F�[�h�C��
	void StateFadeOut(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �t�F�[�h�A�E�g

	// ����
	HRESULT CreateBG();		// �w�i����
	HRESULT CreateSelect();	// �I��������

	// ���̑�
	void UpdateColor();	// �F�̍X�V
	void Decide();		// ����
	void Cancel();		// �L�����Z��

	//=============================
	// �����o�ϐ�
	//=============================
	// ���
	EState m_state;		// ���
	float m_fStateTime;	// ��ԃ^�C�}�[

	// �I�u�W�F�N�g
	CObject2D* m_pSelect[ESelect::SELECT_MAX];	// �I�����̏��
	CTitle_ControllWait* m_pControllWait;		// ����ҋ@���

	// ���̑�
	ESelect m_select;			// �I����
	const int m_nControllIdx;	// ����C���f�b�N�X
};

#endif	// _ARROW_UI_H_
