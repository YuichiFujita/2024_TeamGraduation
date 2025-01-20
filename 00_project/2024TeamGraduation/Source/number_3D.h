//=============================================================================
// 
//  ����(3D)�w�b�_�[ [number_3D.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _NUMBER_3D_H_
#define _NUMBER_3D_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "number.h"
#include "object3D_Anim.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObject3DAnim;

//==========================================================================
// �N���X��`
//==========================================================================
// ����(3D)�N���X��`
class CNumber3D : public CNumber
{
public:

	CNumber3D(int nPriority = 7, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CNumber3D();

	//--------------------------
	// �p�C�v���C��
	//--------------------------
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill();	// �폜

	//--------------------------
	// �ʒu
	//--------------------------
	virtual void SetPosition(const MyLib::Vector3& pos) override		{ m_pObj3D->SetPosition(pos); }				// �ʒu�ݒ�
	virtual MyLib::Vector3 GetPosition() override						{ return m_pObj3D->GetPosition(); }			// �ʒu�擾
	virtual void SetOldPosition(const MyLib::Vector3& pos) override		{ m_pObj3D->SetPosition(pos); }				// �ߋ��̈ʒu�ݒ�
	virtual MyLib::Vector3 GetOldPosition() override					{ return m_pObj3D->GetOldPosition(); }		// �ߋ��̈ʒu�擾
	virtual void SetOriginPosition(const MyLib::Vector3& pos) override	{ m_pObj3D->SetPosition(pos); }				// ���̈ʒu�ݒ�
	virtual MyLib::Vector3 GetOriginPosition() override					{ return m_pObj3D->GetOriginPosition(); }	// ���̈ʒu�擾

	//--------------------------
	// ����
	//--------------------------
	virtual void SetRotation(const MyLib::Vector3& rot) override		{ m_pObj3D->SetRotation(rot); }			// �����ݒ�
	virtual void SetOldRotation(const MyLib::Vector3& rot) override		{ m_pObj3D->SetOldRotation(rot); }		// �O��̌����ݒ�
	virtual void SetOriginRotation(const MyLib::Vector3& rot) override	{ m_pObj3D->SetOriginRotation(rot); }	// ���̌����ݒ�

	//--------------------------
	// �T�C�Y
	//--------------------------
	virtual void SetSize(const MyLib::Vector2& size) override		{ m_pObj3D->SetSize(size); }			// �T�C�Y�ݒ�
	virtual MyLib::Vector2 GetSize() const override					{ return m_pObj3D->GetSize(); }			// �T�C�Y�擾
	virtual void SetSizeOrigin(const MyLib::Vector2& size) override	{ m_pObj3D->SetSizeOrigin(size); }		// ���̃T�C�Y�ݒ�
	virtual MyLib::Vector2 GetSizeOrigin() const override			{ return m_pObj3D->GetSizeOrigin(); }	// ���̃T�C�Y�擾

	//--------------------------
	// �F
	//--------------------------
	virtual void SetColor(const D3DXCOLOR& col) override		{ m_pObj3D->SetColor(col); }			// �F�ݒ�
	virtual D3DXCOLOR GetColor() const override					{ return m_pObj3D->GetColor(); }		// �F�擾
	virtual void SetOriginColor(const D3DXCOLOR& col) override	{ m_pObj3D->SetColorOrigin(col); }		// ���̐F�ݒ�
	virtual D3DXCOLOR GetOriginColor() const override			{ return m_pObj3D->GetColorOrigin(); }	// ���̐F�擾
	virtual void SetAlpha(const float alpha) override			{ m_pObj3D->SetAlpha(alpha); }			// �s�����x�ݒ�
	virtual float GetAlpha() const override						{ return m_pObj3D->GetAlpha(); }		// �s�����x�擾

	//--------------------------
	// �l
	//--------------------------
	virtual void SetNum(int nNum) override	{ m_pObj3D->SetCurPtrn(nNum); }	// �l�ݒ�

	//--------------------------
	// ���̑�
	//--------------------------
	virtual void BindTexture(int nIdxTexture) override { m_pObj3D->BindTexture(nIdxTexture); }						// �e�N�X�`�����蓖��
	void SetVtx();	// ���_���ݒ菈��
	virtual void SetType(const TYPE type) override;		// ��ސݒ�

private:

	//=============================
	// �����o�ϐ�
	//=============================
	CObject3DAnim* m_pObj3D;	// �I�u�W�F�N�g3D
};

#endif