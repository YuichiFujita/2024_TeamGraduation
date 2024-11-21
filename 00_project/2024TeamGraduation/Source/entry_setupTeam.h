//=============================================================================
//
// �`�[�����ݒ菈�� [entry_setupTeam.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _ENTRY_SETUPTEAM_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _ENTRY_SETUPTEAM_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "entryscene.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObjectChara;

//==========================================================================
// �N���X��`
//==========================================================================
// �`�[�����ݒ�N���X
class CEntry_SetUpTeam : public CEntryScene
{
public:

	CEntry_SetUpTeam();
	~CEntry_SetUpTeam();

	//=============================
	// �����o�֐�
	//=============================
	virtual HRESULT Init() override;		// ������
	virtual void Uninit() override;		// �I��
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V

	//--------------------------
	// ���̑�
	//--------------------------
	virtual void Debug() override;

private:

	//=============================
	// �����o�֐�
	//=============================
	void Save();

	//=============================
	// �����o�ϐ�
	//=============================
	std::vector<int> m_vecAddIdx[CGameManager::ETeamSide::SIDE_MAX];	// �ǉ����ꂽ�C���f�b�N�X
	CGameManager::ETeamSide m_TeamSide[mylib_const::MAX_PLAYER];		// �`�[���T�C�h
	int m_vecPlayerNum[CGameManager::ETeamSide::SIDE_MAX];				// �v���C���[�̐�
};

#endif