//=============================================================================
//
// SUSURU�V�[������ [title_susuru.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _ENTRY_DRESSUP_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _ENTRY_DRESSUP_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "titlescene.h"
#include "player.h"

//==========================================================================
// �O���錾
//==========================================================================
class CDressup;

//==========================================================================
// �N���X��`
//==========================================================================
// SUSURU�V�[���N���X
class CTitle_SUSURU : public CTitleScene
{
public:

	//=============================
	// �萔
	//=============================
	static constexpr int NUM_TEXT = 10;		// �e�L�X�g�̐�

	//=============================
	// �񋓌^��`
	//=============================
	enum EState
	{
		STATE_WAIT = 0,	// �ҋ@
		STATE_SCROLL,	// ��������
		STATE_MAX
	};

	CTitle_SUSURU();
	~CTitle_SUSURU();

	//=============================
	// �����o�֐�
	//=============================
	virtual HRESULT Init() override;		// ������
	virtual void Uninit() override;		// �I��
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V

	//--------------------------
	// ���̑�
	//--------------------------

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CTitle_SUSURU::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// ��Ԋ֐�

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	//-----------------------------
	virtual void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ��ԍX�V
	void StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �ҋ@
	void StateScroll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ��������

	//=============================
	// �����o�ϐ�
	//=============================
	//-----------------------------
	// ���
	//-----------------------------
	EState m_state;			// ���
	float m_fStateTime;		// ��Ԏ���

	//-----------------------------
	// ���̑�
	//-----------------------------
	int m_nIdxScroll;	// ���镶���C���f�b�N�X
	CObject2D* m_apText[NUM_TEXT];	// ����
};

#endif