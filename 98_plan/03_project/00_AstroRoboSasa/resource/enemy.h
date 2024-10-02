//============================================================
//
//	�G�w�b�_�[ [enemy.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object3D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �G�N���X
class CEnemy : public CObject3D
{
public:
	// �R���X�g���N�^
	CEnemy();

	// �f�X�g���N�^
	~CEnemy();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CEnemy *Create(const D3DXVECTOR3& rPos);	// ����

	// �����o�֐�
	bool CollisionBullet(const D3DXVECTOR3& rPos);	// �e�Ƃ̓����蔻��
	bool CollisionPlayer(const D3DXVECTOR3& rPos);	// �v���C���[�Ƃ̓����蔻��

private:
	// �����o�ϐ�
	D3DXVECTOR3 m_move;	// �ړ���
	float m_fCurTime;	// �o�ߎ���
};

#endif	// _ENEMY_H_
