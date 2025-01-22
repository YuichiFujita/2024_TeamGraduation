//==========================================================================
// 
//  �v���C���[�}�l�[�W���[(���U���g)�w�b�_�[ [playerManager_result.h]
//  Author : ���n�Ή�
// 
//==========================================================================

#ifndef _PLAYER_MANAGER_RESULT_H_
#define _PLAYER_MANAGER_RESULT_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "playerManager.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �v���C���[�}�l�[�W���[(���U���g)�N���X
class CPlayerManager_Result : public CPlayerManager
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerManager_Result();
	~CPlayerManager_Result() override;

	//=============================
	// ���z�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;

private:

	//=============================
	// �����o�֐�
	//=============================
	// ����
	virtual HRESULT CreatePlayer() override;	// �v���C���[����
	virtual HRESULT CreateOutPlayer(CGameManager::ETeamSide team, const LoadInfo& info) override;	// �O��v���C���[����
	virtual HRESULT CreateLeftPlayer(int i, const LoadInfo& info) override;		// ���̃v���C���[����
	virtual HRESULT CreateRightPlayer(int i, const LoadInfo& info) override;	// �E�̃v���C���[����

	// ���̑�
	void InitPlayer();	// �v���C���[������
};

#endif
