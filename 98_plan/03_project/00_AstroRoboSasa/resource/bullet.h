//============================================================
//
//	�e�w�b�_�[ [bullet.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _BULLET_H_
#define _BULLET_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object3D.h"

//************************************************************
//	�N���X��`
//************************************************************
// �e�N���X
class CBullet : public CObject3D
{
public:
	// �R���X�g���N�^
	CBullet();

	// �f�X�g���N�^
	~CBullet();

	// �I�[�o�[���C�h�֐�
	HRESULT Init(void) override;	// ������
	void Uninit(void) override;		// �I��
	void Update(const float fDeltaTime) override;	// �X�V
	void Draw(CShader *pShader = nullptr) override;	// �`��

	// �ÓI�����o�֐�
	static CBullet *Create(const D3DXVECTOR3& rPos, const D3DXVECTOR3& rMove);	// ����
	static CListManager<CBullet> *GetList(void);	// ���X�g�擾

	// �����o�֐�
	D3DXVECTOR3 GetMove(void) const { return m_move; }	// �ړ��ʎ擾

private:
	// �ÓI�����o�ϐ�
	static CListManager<CBullet> *m_pList;	// �I�u�W�F�N�g���X�g

	// �����o�ϐ�
	CListManager<CBullet>::AIterator m_iterator;	// �C�e���[�^�[
	D3DXVECTOR3 m_move;	// �ړ���
	float m_fCurTime;	// �o�ߎ���
};

#endif	// _BULLET_H_
