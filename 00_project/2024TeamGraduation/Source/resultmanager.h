//=============================================================================
// 
//  ���U���g�}�l�[�W���w�b�_�[ [resultmanager.h]
//  Author : ���n�Ή�
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
// �O���錾
//==========================================================================
class CBall;
class CCollisionLine_Box;
class CTeamStatus;
class CGymWallManager;
class CTimerUI;
class CCharmManager;

//==========================================================================
// �N���X��`
//==========================================================================
// �Q�[���}�l�[�W���N���X��`
class CResultManager
{
public:

	CResultManager();
	~CResultManager();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);

	void Debug();			// �f�o�b�O

	static CResultManager* Create(CScene::MODE mode);				// ��������
	static CResultManager* GetInstance() { return m_pThisPtr; }	// �C���X�^���X�擾

private:
	
	//=============================
	// �����o�֐�
	//=============================


	//=============================
	// �����o�ϐ�
	//=============================


	//--------------------------
	// ���������I�u�W�F�N�g
	//--------------------------
	
	float f[CGameManager::ETeamSide::SIDE_MAX];		// ���e�l
	CGameManager::ETeamSide m_teamWin;				// �����`�[��

	static CResultManager* m_pThisPtr;				// ���g�̃|�C���^
};


#endif