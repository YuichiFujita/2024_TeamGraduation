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
	
	CResultManager();
	~CResultManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void CreatePrelude();							// �O�����s����
	void CreateCharmContest();						// ���e���s����

	void Debug();		// �f�o�b�O
	void Load();		// �`�[���X�e�[�^�X�ǂݍ���

	static CResultManager* Create();							// ��������
	static CResultManager* GetInstance() { return m_pThisPtr; }	// �C���X�^���X�擾

private:
	
	//=============================
	// �����o�ϐ�
	//=============================
	float m_fTension;											// �e���V����
	CGameManager::ETeamSide m_teamWin;							// �����`�[��
	float m_fCharmValue[CGameManager::ETeamSide::SIDE_MAX];		// ���e�l
	float m_fTime;												// ���Ԍo��

	static CResultManager* m_pThisPtr;							// ���g�̃|�C���^
};

#endif