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
	// �`�[�����
	//--------------------------
	int GetPlayerNum(CGameManager::ETeamSide side) { return m_nPlayerNum[side]; }	// �v���C���[�̐��擾
	int GetAllPlayerNum() { return (m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT] + m_nPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT]); }	// �S�Ẵv���C���[�̐��擾
	int GetEntryIdx(int i);									// �G���g���[�̃C���f�b�N�X
	std::vector<int> GetIdxByTeam(CGameManager::ETeamSide side) { m_vecAddIdx[side]; }	// �`�[�����Ƃ̒ǉ����ꂽ�C���f�b�N�X�擾
	CGameManager::ETeamSide GetTeamSide(int i) { return m_TeamSide[i]; }		// �`�[���T�C�h

	//--------------------------
	// ���̑�
	//--------------------------
	virtual CEntry_SetUpTeam* GetSetupTeam() { return this; }	// �`�[�����̐ݒ�擾
	virtual void Debug() override;

private:

	//=============================
	// �����o�֐�
	//=============================
	void SelectTeam();		// �`�[���I��
	void ChangeMaxPlayer();	// �v���C���[�ő吔�ύX
	void Save();			// �Z�[�u
	void Load();			// ���[�h

	//=============================
	// �����o�ϐ�
	//=============================
	std::vector<int> m_vecAddIdx[CGameManager::ETeamSide::SIDE_MAX];	// �ǉ����ꂽ�C���f�b�N�X
	CGameManager::ETeamSide m_TeamSide[CGameManager::MAX_PLAYER];		// �`�[���T�C�h
	int m_nMaxChangeIdx[CGameManager::ETeamSide::SIDE_MAX];				// �ő吔�ύX����C���f�b�N�X
	int m_nPlayerNum[CGameManager::ETeamSide::SIDE_MAX];				// �v���C���[�̐�
	int m_nEntryIdx[mylib_const::MAX_PLAYER];							// �G���g���[�̃C���f�b�N�X
};

#endif