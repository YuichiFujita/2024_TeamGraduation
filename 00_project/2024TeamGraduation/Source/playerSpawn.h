//==========================================================================
// 
//  �o�ꉉ�o�p�v���C���[�w�b�_�[ [playerSpawn.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _PLAYER_SPAWN_
#define _PLAYER_SPAWN_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "player.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �o�ꉉ�o�p�v���C���[�N���X
class CPlayerSpawn : public CPlayer
{
public:
	
	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerSpawn(const CGameManager::ETeamSide typeTeam, const CPlayer::EFieldArea typeArea, const CPlayer::EBaseType typeBase, int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CPlayerSpawn();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill() override;	// ���I�폜
};

#endif
