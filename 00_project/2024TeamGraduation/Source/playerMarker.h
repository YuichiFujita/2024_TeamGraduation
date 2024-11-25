//==========================================================================
// 
//  �v���C���[�}�[�J�[�w�b�_�[ [playerMarker.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _PLAYER_MARKER_H_
#define _PLAYER_MARKER_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "objectBillboard_Anim.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayer;

//==========================================================================
// �N���X��`
//==========================================================================
// �v���C���[�}�[�J�[�N���X
class CPlayerMarker : public CObjectBillboardAnim
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CPlayerMarker(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CPlayerMarker();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill() override;	// �폜

	//=============================
	// �����o�֐�
	//=============================
	void BindPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }	// �v���C���[����

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CPlayerMarker* Create(CPlayer* pPlayer);	// ����

private:

	//=============================
	// �����o�ϐ�
	//=============================
	CPlayer* m_pPlayer;	// �e�v���C���[���
};

#endif
