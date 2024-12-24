//=============================================================================
// 
//  ���U���g�}�l�[�W���w�b�_�[ [resultmanager.h]
//  Author : Kai Takada
// 
//=============================================================================

#ifndef _RESULTMANAGER_H_
#define _RESULTMANAGER_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
//#include "constans.h"
//#include "instantfade.h"
//#include "scene.h"

#include "gameManager.h"

//==========================================================================
// ���O���
//==========================================================================
namespace ResultManager
{
	const std::string TEXTFILE = "data\\TEXT\\result\\teamStatus.txt";	// �`�[���X�e�[�^�Xtxt
}

//==========================================================================
// �O���錾
//==========================================================================
class CTeamStatus;
class CObject3D;
class CResultCrown;
class CEffekseerObj;

//==========================================================================
// �N���X��`
//==========================================================================
// ���U���g�}�l�[�W���N���X��`
class CResultManager
{
public:
	
	//=============================
	// �񋓒�`
	//=============================
	// ���
	enum EState
	{
		STATE_NONE = 0,				// �Ȃɂ��Ȃ�
		STATE_PRELUDE_READY,		// �O�����s����
		STATE_PRELUDE,				// �O�����s
		STATE_CONTEST_READY,		// ���e���s����
		STATE_CONTEST,				// ���e���s
		STATE_MAX
	};

	CResultManager();
	~CResultManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//=============================
	// �����o�֐�
	//=============================
	MyLib::Vector3 GetPosMid(CGameManager::ETeamSide team);		// �R�[�g�̒��S�ʒu�擾
	EState GetState() { return m_state; }		// ��Ԏ擾
	CGameManager::ETeamSide GetTeamPreludeWin() { return m_teamPreludeWin; }		// �����`�[��(�O��)
	CGameManager::ETeamSide GetTeamContestWin() { return m_teamContestWin; }		// �����`�[��(���e)

	static CResultManager* Create();							// ��������
	static CResultManager* GetInstance() { return m_pThisPtr; }	// �C���X�^���X�擾

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CResultManager::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];			// ��Ԋ֐�

	typedef void(CResultManager::* STATE_START_FUNC)();
	static STATE_START_FUNC m_StateStartFunc[];	// ��ԊJ�n�֐�

	typedef void(CResultManager::* STATE_END_FUNC)();
	static STATE_END_FUNC m_StateEndFunc[];		// ��ԏI���֐�

	//=============================
	// �����o�֐�
	//=============================
	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	void SetState(EState state);		// ��Ԑݒ�
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);				// ��ԍX�V
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);					// �Ȃ�
	void StatePreludeReady(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �O�����s����
	void StatePrelude(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);				// �O�����s
	void StateCharmContestReady(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ���e���s����
	void StateCharmContest(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ���e���s
	
	void StateStartPreludeReady();		// [�J�n]�O�����s����
	void StateStartPrelude();			// [�J�n]�O�����s
	void StateStartCharmContestReady();	// [�J�n]���e���s����
	void StateStartCharmContest();		// [�J�n]���e���s

	void StateEndPrelude();				// [�I��]�O�����s
	void StateEndCharmContest();		// [�I��]���e���s

	//-----------------------------
	// ���̑�
	//-----------------------------
	void CreateAudience();								// �ϋq����
	void CreateCrown(CGameManager::ETeamSide team);		// �������f������
	void CreatePolygon(EState state);					// ���s�|���S������
	void CreateParticle();								// �p�[�e�B�N������

	void Debug();		// �f�o�b�O
	void Load();		// �Q�[�����ǂݍ���
	void LoadTeam(std::ifstream* File, std::string line, int nTeam);		// �`�[�����ǂݍ���

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fTension;											// �e���V����
	CGameManager::ETeamSide m_teamPreludeWin;					// �����`�[��(�O��)
	CGameManager::ETeamSide m_teamContestWin;					// �����`�[��(���e)
	float m_fCharmValue[CGameManager::ETeamSide::SIDE_MAX];		// ���e�l
	CObject3D* m_pText;											// �����`�[���p3D�|���S��
	CResultCrown* m_pCrown;										// �������f��
	CEffekseerObj* m_pEfkConfetti;								// ������G�t�F�N�V�A

	//-----------------------------
	// ���
	//-----------------------------
	EState m_state;		// ���
	float m_fStateTime;	// ��Ԏ���
	bool m_bStateTrans;	// ��ԑJ�ډ\�t���O(t:�s��/f:��)

	static CResultManager* m_pThisPtr;							// ���g�̃|�C���^
};

#endif