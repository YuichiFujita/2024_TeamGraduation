//============================================================
//
//	�v���C���[�w�b�_�[ [player.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object3D.h"

//************************************************************
//	�O���錾
//************************************************************
class CGun;	// �e�N���X

//************************************************************
//	�N���X��`
//************************************************************
// �v���C���[�N���X
class CPlayer : public CObject3D
{
public:
	// �R���X�g���N�^
	CPlayer();

	// �f�X�g���N�^
	~CPlayer();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CPlayer *Create(void);	// ����
	static CListManager<CPlayer> *GetList(void);	// ���X�g�擾

	// �����o�֐�
	void MoveGunRecoil(const float fRecScalar);	// �����ړ�
	D3DXVECTOR3 GetMove(void) const { return m_move; }	// �ړ��ʎ擾
	D3DXVECTOR3 GetGunRotation(void) const { return m_rotGun; }	// �e�̌����擾

private:
	// �ÓI�����o�ϐ�
	static CListManager<CPlayer> *m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CPlayer>::AIterator m_iterator;	// �C�e���[�^�[
	CGun* m_pGun;			// �e�̏��
	D3DXVECTOR3 m_rotGun;	// �e�̌���
	D3DXVECTOR3 m_move;		// �ړ���
};

#endif	// _PLAYER_H_
