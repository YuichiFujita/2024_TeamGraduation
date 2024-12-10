//=============================================================================
// 
//  �`�[���X�e�[�^�X�w�b�_�[ [teamStatus.h]
//  Author : Kai Takada
// 
//=============================================================================

#ifndef _TEAMSTATUS_H_
#define _TEAMSTATUS_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "gamemanager.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �`�[���X�e�[�^�X�N���X
//==========================================================================
class CTeamStatus
{
public:

	struct SCharmInfo
	{
		float fValue;
		float fValueMax;

		//�R���X�g���N�^
		SCharmInfo() : fValue(0.0f), fValueMax(0.0f) {}
	};

	struct SSpecialInfo
	{
		float fValue;
		float fValueMax;
		CObject2D* pGauge;

		//�R���X�g���N�^
		SSpecialInfo() : fValue(0.0f), fValueMax(0.0f), pGauge(nullptr) {}
	};

	// �R���X�g���N�^
	CTeamStatus();

	// �f�X�g���N�^
	~CTeamStatus();

	HRESULT Init();		// ������
	void Uninit();		// �I��

	//=============================
	// �����o�֐�
	//=============================
	void SetTeam(const CGameManager::ETeamSide team) { m_typeTeam = team; }		// �`�[���T�C�h�ݒ�
	CGameManager::ETeamSide GetTeam() { return m_typeTeam; }					// �`�[���T�C�h�擾
	void TeamSetting(const CGameManager::ETeamSide team);						// �`�[���T�C�h����Ȃ�ݒ�
	bool CheckAllDead();	// �S�łŏI��

	//-----------------------------
	// ���e���
	//-----------------------------
	void InitCharmInfo();													// ���e��񏉊���
	void AddCharmValue(float fValue);										// ���e�Q�[�W�l����
	void SubCharmValue(float fValue);										// ���e�Q�[�W�l����
	void SetCharmInfo(SCharmInfo sInfo) { m_sCharmInfo = sInfo; }			// ���e���ݒ�
	SCharmInfo GetCharmInfo() { return m_sCharmInfo; }						// ���e���擾

	//-----------------------------
	// �X�y�V�������
	//-----------------------------
	void InitSpecialInfo();													// �X�y�V������񏉊���
	void SetSpecialValue(float fValue);										// �X�y�V�����Q�[�W�l�ݒ�
	float GetSpecialValue() { return m_sSpecialInfo.fValue; }				// �X�y�V�����Q�[�W�l�擾
	void AddSpecialValue(float fValue);										// �X�y�V�����Q�[�W�l����
	void SubSpecialValue(float fValue);										// �X�y�V�����Q�[�W�l����
	void ZeroSpecialValue() { SetSpecialValue(0); }							// �X�y�V�����Q�[�W�l����
	void SetSpecialInfo(SSpecialInfo sInfo) { m_sSpecialInfo = sInfo; }		// �X�y�V�������ݒ�
	SSpecialInfo GetSpecialInfo() { return m_sSpecialInfo; }				// �X�y�V�������擾
	bool IsMaxSpecial() { return (m_sSpecialInfo.fValue == m_sSpecialInfo.fValueMax); }		//�}�b�N�X�t���O�擾

	void Debug();		//�f�o�b�O

	static CTeamStatus* Create();		//����

private:
	
	//=============================
	// �����o�ϐ�
	//=============================
	CGameManager::ETeamSide m_typeTeam;	// �`�[���T�C�h
	SCharmInfo m_sCharmInfo;			// ���e���
	SSpecialInfo m_sSpecialInfo;		// �X�y�V�������
};

#endif