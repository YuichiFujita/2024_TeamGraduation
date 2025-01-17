//==========================================================================
// 
//  �`�[�����ݒ菈�� [entry_setupTeam.h]
//  Author : ���n�Ή�
//  Adder  : ���c�E
// 
//==========================================================================

#ifndef _ENTRY_SETUPTEAM_H_
#define _ENTRY_SETUPTEAM_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "entryscene.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObjectChara;
class CObject2D;
class CObject2D_Anim;
class CNumber;
class CTransUI;

//==========================================================================
// �N���X��`
//==========================================================================
// �`�[�����ݒ�N���X
class CEntry_SetUpTeam : public CEntryScene
{
public:

	//=============================
	// �\���̒�`
	//=============================
	// �`�[���ۑ��\����
	struct STeam
	{
		CGameManager::ETeamSide team;	// �`�[���T�C�h
		int nPadIdx;	// ���쌠�C���f�b�N�X
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CEntry_SetUpTeam();
	~CEntry_SetUpTeam();

	//=============================
	// ���z�֐�
	//=============================
	virtual HRESULT Init() override;	// ������
	virtual void Uninit() override;		// �I��
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V

	//=============================
	// �����o�֐�
	//=============================
	//-----------------------------
	// �`�[�����
	//-----------------------------
	int PadIdxToEntryIdx(int nPadIdx);		// �p�b�h�C���f�b�N�X�̃G���g���[�C���f�b�N�X�ϊ�
	int PlayerIdxToPadIdx(int nPlayerIdx);	// �v���C���[�C���f�b�N�X�̃p�b�h�C���f�b�N�X�ϊ�
	CGameManager::ETeamSide GetTeamSide(int nPlayerIdx);	// �`�[���T�C�h�擾
	CGameManager::ETeamSide GetTeamSideAI(int nOccurIdx);	// �`�[���T�C�h�擾 (AI)
	inline int GetAllPlayerNum() { return (m_nPlayerNum[CGameManager::ETeamSide::SIDE_LEFT] + m_nPlayerNum[CGameManager::ETeamSide::SIDE_RIGHT]); }	// �S�Ẵv���C���[�̐��擾
	inline int GetPlayerNum(CGameManager::ETeamSide side)				{ return m_nPlayerNum[side]; }	// �v���C���[�̐��擾
	inline std::vector<int> GetIdxByTeam(CGameManager::ETeamSide side)	{ m_vecAddIdx[side]; }			// �`�[�����Ƃ̒ǉ����ꂽ�C���f�b�N�X�擾

	//-----------------------------
	// ���̑�
	//-----------------------------
	virtual inline CEntry_SetUpTeam* GetSetupTeam() { return this; }	// �`�[�����̐ݒ�擾
	virtual void Debug() override;

private:

	//=============================
	// �����o�֐�
	//=============================
	//-----------------------------
	// ����
	//-----------------------------
	HRESULT CreateNumInTeam();	// �`�[���l���̐���
	HRESULT CreatePadUI();		// �R���g���[���[UI�̐���
	HRESULT CreateTransUI();	// �J��UI�̐���

	//-----------------------------
	// �폜
	//-----------------------------
	void FillAI();	// �󂫃����o�[��AI����
	void KillUI();	// UI�폜

	//-----------------------------
	// �ʒu�␳
	//-----------------------------
	void PosAdjUI(const bool bAllReady, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// UI�ʒu�␳
	void PosAdjPadUI();	// �R���g���[���[UI�ʒu�␳

	//-----------------------------
	// �v���C���[����
	//-----------------------------
	bool SelectTeam();		// �`�[���I��
	void ChangeMaxPlayer();	// �v���C���[�ő吔�ύX
	void TransDressUp(const bool bAllReady);	// �����ւ��J��
	void DeleteEntry(int* pEntryIdx);			// �G���g���[����

	//-----------------------------
	// �v���C���[�t���O
	//-----------------------------
	bool IsUserReady(const int nUserIdx, std::vector<int>::iterator* pItr = nullptr);	// ���������t���O�擾
	bool IsUserTeamSelect(const int nUserIdx);	// �`�[���I�𒆃t���O�擾
	bool IsUserMaxChange(const int nUserIdx);	// �ő吔�ύX�t���O�擾

	//-----------------------------
	// �O���t�@�C������
	//-----------------------------
	void Save();	// �Z�[�u
	void Load();	// ���[�h

	//=============================
	// �����o�ϐ�
	//=============================
	//-----------------------------
	// UI���
	//-----------------------------
	CObject2D_Anim* m_apPadUI[CGameManager::MAX_PLAYER];		// �R���g���[���[UI���
	CNumber* m_apNumInTeam[CGameManager::ETeamSide::SIDE_MAX];	// �`�[���l�����
	CTransUI* m_pTransUI;	// �J��UI���

	//-----------------------------
	// �Z�b�g�A�b�v���
	//-----------------------------
	std::vector<int> m_vecAddIdx[CGameManager::ETeamSide::SIDE_MAX];	// �ǉ����ꂽ�C���f�b�N�X
	STeam m_TeamSide[CGameManager::MAX_PLAYER];				// �`�[���T�C�h
	int m_nMaxChangeIdx[CGameManager::ETeamSide::SIDE_MAX];	// �ő吔�ύX����C���f�b�N�X
	int m_nPlayerNum[CGameManager::ETeamSide::SIDE_MAX];	// �v���C���[�̐�
	int m_nEntryIdx[mylib_const::MAX_PLAYER];				// �G���g���[�̃C���f�b�N�X
};

#endif