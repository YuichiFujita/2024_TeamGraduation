//=============================================================================
// 
//  ����(2D)�w�b�_�[ [number_2D.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _NUMBER_2D_H_
#define _NUMBER_2D_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "number.h"
#include "object2D_Anim.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObject2D_Anim;

//==========================================================================
// �N���X��`
//==========================================================================
// ����(2D)�N���X��`
class CNumber2D : public CNumber
{
public:

	CNumber2D(int nPriority = 7, const LAYER layer = LAYER::LAYER_2D);
	~CNumber2D();

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
	virtual void SetPosition(const MyLib::Vector3& pos) override		{ m_pObj2D->SetPosition(pos); }	// �ʒu�ݒ�
	virtual void SetOldPosition(const MyLib::Vector3& pos) override		{ m_pObj2D->SetPosition(pos); }	// �ߋ��̈ʒu�ݒ�
	virtual void SetOriginPosition(const MyLib::Vector3& pos) override	{ m_pObj2D->SetPosition(pos); }	// ���̈ʒu�ݒ�

	//--------------------------
	// ����
	//--------------------------
	virtual void SetRotation(const MyLib::Vector3& rot) override		{ m_pObj2D->SetRotation(rot); }			// �����ݒ�
	virtual void SetOldRotation(const MyLib::Vector3& rot) override		{ m_pObj2D->SetOldRotation(rot); }		// �O��̌����ݒ�
	virtual void SetOriginRotation(const MyLib::Vector3& rot) override	{ m_pObj2D->SetOriginRotation(rot); }	// ���̌����ݒ�

	//--------------------------
	// �T�C�Y
	//--------------------------
	virtual void SetSize(const MyLib::Vector2& size) override		{ m_pObj2D->SetSize(size); }			// �T�C�Y�ݒ�
	virtual MyLib::Vector2 GetSize() const override					{ return m_pObj2D->GetSize(); }			// �T�C�Y�擾
	virtual void SetSizeOrigin(const MyLib::Vector2& size) override	{ m_pObj2D->SetSizeOrigin(size); }		// ���̃T�C�Y�ݒ�
	virtual MyLib::Vector2 GetSizeOrigin() const override			{ return m_pObj2D->GetSizeOrigin(); }	// ���̃T�C�Y�擾

	//--------------------------
	// �F
	//--------------------------
	virtual void SetColor(const D3DXCOLOR& col) override		{ m_pObj2D->SetColor(col); }			// �F�ݒ�
	virtual D3DXCOLOR GetColor() const override					{ return m_pObj2D->GetColor(); }		// �F�擾
	virtual void SetOriginColor(const D3DXCOLOR& col) override	{ m_pObj2D->SetOriginColor(col); }		// ���̐F�ݒ�
	virtual D3DXCOLOR GetOriginColor() const override			{ return m_pObj2D->GetOriginColor(); }	// ���̐F�擾
	virtual void SetAlpha(const float alpha) override			{ m_pObj2D->SetAlpha(alpha); }			// �s�����x�ݒ�
	virtual float GetAlpha() const override						{ return m_pObj2D->GetAlpha(); }		// �s�����x�擾

	//--------------------------
	// �l
	//--------------------------
	virtual void SetNum(int nNum) override	{ m_pObj2D->SetPatternAnim(nNum); }	// �l�ݒ�

	//--------------------------
	// ���̑�
	//--------------------------
	virtual void BindTexture(int nIdxTexture) override { m_pObj2D->BindTexture(nIdxTexture); }						// �e�N�X�`�����蓖��
	void SetVtx();	// ���_���ݒ菈��

private:

	//=============================
	// �����o�ϐ�
	//=============================
	CObject2D_Anim* m_pObj2D;	// �I�u�W�F�N�g2D
};

#endif