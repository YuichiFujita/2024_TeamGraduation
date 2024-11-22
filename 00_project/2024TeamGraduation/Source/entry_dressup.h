//=============================================================================
//
// �h���X�A�b�v�ݒ菈�� [entry_dressup.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _ENTRY_DRESSUP_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _ENTRY_DRESSUP_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "entryscene.h"
#include "player.h"

//==========================================================================
// �O���錾
//==========================================================================
class CDressup;

//==========================================================================
// �N���X��`
//==========================================================================
// �h���X�A�b�v�ݒ�N���X
class CEntry_Dressup : public CEntryScene
{
public:

	//=============================
	// �\���̒�`
	//=============================
	struct SEntryDressup
	{
		CPlayer* pPlayer;		// �v���C���[
		CDressup* pHair;		// �������ւ�
		CDressup* pAccessory;	// �A�N�Z�����ւ�
	};

	CEntry_Dressup();
	~CEntry_Dressup();

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
	void Save();					// �Z�[�u
	void Load();					// ���[�h
	void ChangeBodyType(int i);		// �̌^�ύX
	void ChangeHandedness(int i);	// ������ύX
	void ReCreatePlayer(int i, CPlayer::EHandedness handedness, CPlayer::EBody body);		// �v���C���[�Đ���

	//=============================
	// �����o�ϐ�
	//=============================
	int m_nPlayerNum[CGameManager::ETeamSide::SIDE_MAX];	// �v���C���[�̐�
	std::vector<SEntryDressup> m_vecDressupInfo;			// �����ւ����
};

#endif