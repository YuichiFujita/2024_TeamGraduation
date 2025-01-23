//=============================================================================
// 
//  �v���C���[�X�e�[�^�X�w�b�_�[ [playerStatus.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PLAYERSTATUS_H_
#define _PLAYERSTATUS_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "gamemanager.h"
#include "listManager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayer;
class CObjectCircleGauge2D;	// �~�̃Q�[�W

//==========================================================================
// �v���C���[�X�e�[�^�X�N���X
//==========================================================================
class CPlayerStatus
{
public:

	// �R���X�g���N�^
	CPlayerStatus(CPlayer* pPlayer);

	// �f�X�g���N�^
	~CPlayerStatus();

	//=============================
	// �����o�֐�
	//=============================
	void Init();	// ����������
	void Kill();	// �폜����
	void BindPlayer(CPlayer* pPlayer)	{ m_pPlayer = pPlayer; }	// �v���C���[����
	void LifeDamage(const int nDmg);	// �̗͌��Z
	void LifeHeal(const int nHeal);		// �̗͉��Z

private:

	//=============================
	// �����o�֐�
	//=============================
	void CreateLifeGuge(CObjectCircleGauge2D** pGauge, bool bAddList);	// �̗̓Q�[�W����
	void CreateGaugeFrame();	// �Q�[�W�t���[������

	//=============================
	// �����o�ϐ�
	//=============================
	CPlayer* m_pPlayer;					// �v���C���[�̃|�C���^
	CObjectCircleGauge2D* m_pLifeGauge;	// �̗̓Q�[�W
	CObjectCircleGauge2D* m_pLifeGauge_Ground;	// �̗̓Q�[�W�̉��n
	CObject2D* m_pGaugeFrame;			// �Q�[�W�t���[��
	static CListManager<CObjectCircleGauge2D> m_LifeGaugeListLeft;	// ���̗̓Q�[�W�̃��X�g
	static CListManager<CObjectCircleGauge2D> m_LifeGaugeListRight;	// �E�̗̓Q�[�W�̃��X�g

};

#endif