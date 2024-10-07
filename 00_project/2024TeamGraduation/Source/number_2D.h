//=============================================================================
// 
//  ����(2D)�w�b�_�[ [number_2D.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _NUMBER_2D_H_
#define _NUMBER_2D_H_	// ��d�C���N���[�h�h�~

#include "number.h"
#include "object2D.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObject2D;

//==========================================================================
// �N���X��`
//==========================================================================
// ����(2D)�N���X��`
class CNumber2D : public CNumber
{
public:

	CNumber2D(int nPriority = 7, const LAYER layer = LAYER::LAYER_2D);
	~CNumber2D();

	// �����o�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill() override;	// �폜����

	void SetPosition(const MyLib::Vector3& pos) override;	// �ʒu�ݒ�
	MyLib::Vector3 GetPosition() const override;		// �ʒu�擾
	void SetMove(const MyLib::Vector3& move) override;		// �ړ��ʐݒ�
	MyLib::Vector3 GetMove() const override;			// �ړ��ʎ擾
	void SetRotation(const MyLib::Vector3& rot) override;	// �����ݒ�
	MyLib::Vector3 GetRotation() const override;		// �����擾

	void SetColor(const D3DXCOLOR col) override;			// �F�ݒ�
	D3DXCOLOR GetColor() const override;				// �F�擾
	void SetSize(const D3DXVECTOR2 size) override;		// �T�C�Y�̐ݒ�
	D3DXVECTOR2 GetSize() const override;			// �T�C�Y�̎擾
	void SetSizeOrigin(const D3DXVECTOR2 size) override;	// ���̃T�C�Y�̐ݒ�
	D3DXVECTOR2 GetSizeOrigin() const override;		// ���̃T�C�Y�̎擾
	void SetTex(D3DXVECTOR2 *tex) override;				// �e�N�X�`�����W�̐ݒ�
	D3DXVECTOR2 *GetTex() override;					// �e�N�X�`�����W�̎擾

	void SetVtx() override;
	void BindTexture(int nIdx) override;
	void SetType(const CObject::TYPE type) override;
	CObject2D *GetObject2D() override;

private:
	CObject2D *m_aObject2D;				// �I�u�W�F�N�g2D�̃I�u�W�F�N�g
};

#endif