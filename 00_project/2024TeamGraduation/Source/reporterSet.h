//=============================================================================
// 
//  �����҃Z�b�g�w�b�_�[ [reporterSet.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _REPORTERSET_H_
#define _REPORTERSET_H_		// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "gamemanager.h"
#include "objectChara.h"
#include "listmanager.h"
#include "objectX.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �����҃Z�b�g�N���X
class CReporterSet : public CObjectX
{
public:
	
	//=============================
	// �񋓌^��`
	//=============================
	// ��Ԓ�`
	enum EState
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_GO,			// ������
		STATE_COLLECT,		// ���
		STATE_BACK,			// �߂�
		STATE_MAX
	};

	CReporterSet(int nPriority = 6);
	~CReporterSet();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	void Kill() override;		// �폜

	//=============================
	// �����o�֐�
	//=============================
	void SetState(EState state);							// ��Ԑݒ�

	//=============================
	// �ÓI�֐�
	//=============================
	// ��������
	static CReporterSet* Create(const MyLib::Vector3& pos, const CGameManager::ETeamSide& side);

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CReporterSet::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];							// ��Ԋ֐�

	//=============================
	// �����o�֐�
	//=============================
	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	virtual void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ��ԍX�V
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �Ȃ�
	void StateGo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ������
	void StateCollect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���
	void StateBack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �߂�

	//-----------------------------
	// ���̑�
	//-----------------------------
	void CreateChair();	// �֎q����
	void CreateMic();	// �}�C�N����

	//-----------------------------
	// ���̑�
	//-----------------------------

	//=============================
	// �����o�ϐ�
	//=============================
	//-----------------------------
	// ���
	//-----------------------------
	EState m_Oldstate;		// �O��̏��
	EState m_state;			// ���
	float m_fStateTime;		// ��Ԏ���

	//-----------------------------
	// ���f��
	//-----------------------------
	CObjectX* m_pChair;		// �֎q
	CObjectX* m_pMic;		// �}�C�N

	//-----------------------------
	// ���̑��ϐ�
	//-----------------------------
	CShadow* m_pShadow;		// �e
	CGameManager::ETeamSide m_TeamSide;		// �`�[���T�C�h
};


#endif