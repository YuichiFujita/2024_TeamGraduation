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
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_PRELUDE,		// �O�����s
		STATE_CONTEST,		// ���e���s
		STATE_MAX
	};

	CResultManager();
	~CResultManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CResultManager::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];	// ��Ԋ֐�

	//=============================
	// �����o�֐�
	//=============================
	EState GetState() { return m_state; }		// ��Ԏ擾
	CGameManager::ETeamSide GetTeamPreludeWin() { return m_teamPreludeWin; }		// �����`�[��(�O��)
	CGameManager::ETeamSide GetTeamContestWin() { return m_teamContestWin; }		// �����`�[��(���e)

	static CResultManager* Create();							// ��������
	static CResultManager* GetInstance() { return m_pThisPtr; }	// �C���X�^���X�擾

private:

	//=============================
	// �����o�֐�
	//=============================
	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	void SetState(EState state);		// ��Ԑݒ�
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ��ԍX�V
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);				// �Ȃ�
	void StatePrelude(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �O�����s
	void StateCharmContest(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ���e���s

	void CreatePrelude();				// �O�����s����
	void CreateCharmContest();			// ���e���s����
	void CreateAudience();				// �ϋq����

	void Debug();		// �f�o�b�O
	void Load();		// �Q�[�����ǂݍ���
	void LoadTeam(std::ifstream* File, std::string line, int nTeam);						// �`�[�����ǂݍ���
	void LoadPlayer(std::ifstream* File, std::string line, int nTeam, int nIdxPlayer);		// �v���C���[���ǂݍ���

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fTension;											// �e���V����
	CGameManager::ETeamSide m_teamPreludeWin;					// �����`�[��(�O��)
	CGameManager::ETeamSide m_teamContestWin;					// �����`�[��(���e)
	float m_fCharmValue[CGameManager::ETeamSide::SIDE_MAX];		// ���e�l

	//-----------------------------
	// ���
	//-----------------------------
	EState m_state;		// ���
	float m_fStateTime;	// ��Ԏ���

	static CResultManager* m_pThisPtr;							// ���g�̃|�C���^
};

#endif