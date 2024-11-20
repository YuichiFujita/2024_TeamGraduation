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

//==========================================================================
// �O���錾
//==========================================================================
class CPlayer;
class CBindKey;

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
	void DeletePlayer(CPlayer* pPlayer);			// �v���C���[�폜
	SOutInfo GetOutInfo(const EOutPos out);			// �O����擾
	CPlayer* GetOutPlayer(const EOutPos out);		// �O��v���C���[�擾
	EOutPos GetOutPosition(const CPlayer* pPlayer);	// �O��|�W�V�����擾
	void ChangeUser(CPlayer* pPlayer);				// AI�x�[�X�̃��[�U�[�ύX
	void ChangeAI(CPlayer* pPlayer);				// ���[�U�[�x�[�X��AI�ύX

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

	//=============================
	// �����o�ϐ�
	//=============================
	CListManager<CPlayer> m_listInLeft;		// ���썶�v���C���[
	CListManager<CPlayer> m_listInRight;	// ����E�v���C���[
	CPlayer* m_apOut[OUT_MAX];				// �O��v���C���[

	//=============================
	// �ÓI�����o�ϐ�
	//=============================
	static CPlayerManager* m_pInstance;	// ���g�̃C���X�^���X
};

#endif
