//==========================================================================
// 
//  �v���C���[�}�l�[�W���[�w�b�_�[ [playerManager.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _PLAYER_MANAGER_H_
#define _PLAYER_MANAGER_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "listmanager.h"
#include "gamemanager.h"
#include "player.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayer;
class CBindKey;

//==========================================================================
// ���O���
//==========================================================================
namespace PlayerManager
{
	const std::string TEXT_PLAYERINFO = "data\\TEXT\\playerManager\\playerInfo.txt";	// �v���C���[���̃e�L�X�g�t�@�C��
}

//==========================================================================
// �N���X��`
//==========================================================================
// �v���C���[�}�l�[�W���[�N���X
class CPlayerManager
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	// �O��|�W�V������
	enum EOutPos
	{
		OUT_LEFT_FAR = 0,	// ����
		OUT_LEFT,			// ��
		OUT_LEFT_NEAR,		// ����O
		OUT_RIGHT_FAR,		// �E��
		OUT_RIGHT,			// �E
		OUT_RIGHT_NEAR,		// �E��O
		OUT_MAX				// ���̗񋓌^�̑���
	};

	//=============================
	// �\���̒�`
	//=============================
	// �O����\����
	struct SOutInfo
	{
		MyLib::Vector3 posLeft;		// �ړ��\���ʒu
		MyLib::Vector3 posRight;	// �ړ��\�E�ʒu
		CBindKey* pKeyLeft;			// ���ړ��L�[����
		CBindKey* pKeyRight;		// �E�ړ��L�[����
	};

	struct LoadInfo		// �ǂݍ��ݏ��
	{
		int nControllIdx;				// ���삷��C���f�b�N�X�ԍ�
		CPlayer::EHandedness eHanded;	// ������
		CPlayer::EBody eBody;			// �̌^
		int nHair;						// ��
		int nAccessory;					// �A�N�Z�T���[
		int nFace;						// ��
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerManager();
	~CPlayerManager();

	//=============================
	// ���z�֐�
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	//=============================
	// �����o�֐�
	//=============================
	int RegistPlayer(CPlayer* pPlayer, const int nPosIdx = -1);	// �v���C���[�o�^
	void DeletePlayer(CPlayer* pPlayer);	// �v���C���[�폜

	CListManager<CPlayer> GetInList(const CGameManager::ETeamSide team);	// ����v���C���[���X�g�擾
	CPlayer* GetOutPlayer(const EOutPos out);			// �O��v���C���[�擾
	EOutPos GetOutPosition(const CPlayer* pPlayer);		// �O��|�W�V�����擾
	SOutInfo GetOutInfo(const EOutPos out);				// �O����擾
	void ChangeAIToUser(CPlayer* pPlayer);				// ���[�U�[�x�[�X��AI�ύX
	void ChangeUserToAI(CPlayer* pPlayer);				// AI�x�[�X�̃��[�U�[�ύX
	void SwapBase(CPlayer* pPlayer, CPlayer* pTarget);	// �x�[�X����

	void SetEnableUserChange(const bool bUserChange) { m_bUserChange = bUserChange; }	// ���[�U�[�ύX����t���O�ݒ�
	bool IsUserChange() const { return m_bUserChange; }	// ���[�U�[�ύX����t���O�擾
	
	// �t�@�C���֘A
	static void Save(const std::vector<LoadInfo>& LeftInfo, const std::vector<LoadInfo>& RightInfo);			// �Z�[�u����

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CPlayerManager* Create();	// ����
	static CPlayerManager* GetInstance() { return m_pInstance; }	// �C���X�^���X�擾

private:

	//=============================
	// �֐����X�g
	//=============================
	// �O����֐�
	typedef SOutInfo(CPlayerManager::*INFO_FUNC)();
	static INFO_FUNC m_InfoFuncList[];	// �֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ���O�쑀��
	int RegistOutPlayer(CPlayer* pPlayer, const int nPosIdx);	// �O��v���C���[�o�^ (�|�W�V�����w��)
	int RegistOutPlayer(CPlayer* pPlayer);	// �O��v���C���[�o�^
	void DeleteOutPlayer(CPlayer* pPlayer);	// �O��v���C���[�폜
	int RegistInPlayer(CPlayer* pPlayer);	// ����v���C���[�o�^
	void DeleteInPlayer(CPlayer* pPlayer);	// ����v���C���[�폜
	SOutInfo GetInfoLeftFar();		// �����̊O����擾
	SOutInfo GetInfoLeft();			// ���̊O����擾
	SOutInfo GetInfoLeftNear();		// ����O�̊O����擾
	SOutInfo GetInfoRightFar();		// �E���̊O����擾
	SOutInfo GetInfoRight();		// �E�̊O����擾
	SOutInfo GetInfoRightNear();	// �E��O�̊O����擾

	// ����
	HRESULT CreateLeftPlayer(const LoadInfo& info);	// ���̃v���C���[����
	HRESULT CreateRightPlayer(const LoadInfo& info);	// �E�̃v���C���[����

	// �t�@�C���֘A
	static void SavePlayerInfo(std::ofstream* File, const std::vector<LoadInfo>& Info);	// �v���C���[���Z�[�u
	static void Load();
	static void LoadPlayerInfo(std::ifstream* File, int nTeam, int nIdxPlayer);	// �v���C���[���ǂݍ���

	//=============================
	// �����o�ϐ�
	//=============================
	CListManager<CPlayer> m_listInLeft;		// ���썶�v���C���[
	CListManager<CPlayer> m_listInRight;	// ����E�v���C���[
	CPlayer* m_apOut[OUT_MAX];				// �O��v���C���[
	bool m_bUserChange;						// ���[�U�[�ύX����t���O

	// �t�@�C���֘A
	static std::vector<LoadInfo> m_vecLoadInfo[CGameManager::ETeamSide::SIDE_MAX];	// �ǂݍ��ݏ��

	//=============================
	// �ÓI�����o�ϐ�
	//=============================
	static CPlayerManager* m_pInstance;	// ���g�̃C���X�^���X
};

#endif
