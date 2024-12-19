//=============================================================================
// 
//  ����(Billboard)�w�b�_�[ [number_Billboard.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _NUMBER_BILLBOARD_H_
#define _NUMBER_BILLBOARD_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "number.h"
#include "objectBillboard_Anim.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObjectBillboardAnim;

//==========================================================================
// �N���X��`
//==========================================================================
// ����(Billboard)�N���X��`
class CNumberBillboard : public CNumber
{
public:

	CNumberBillboard(int nPriority = 7, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CNumberBillboard();

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
	virtual void SetPosition(const MyLib::Vector3& pos) override		{ m_pObjBillboard->SetPosition(pos); }	// �ʒu�ݒ�
	virtual void SetOldPosition(const MyLib::Vector3& pos) override		{ m_pObjBillboard->SetPosition(pos); }	// �ߋ��̈ʒu�ݒ�
	virtual void SetOriginPosition(const MyLib::Vector3& pos) override	{ m_pObjBillboard->SetPosition(pos); }	// ���̈ʒu�ݒ�

	//--------------------------
	// ����
	//--------------------------
	virtual void SetRotation(const MyLib::Vector3& rot) override		{ m_pObjBillboard->SetRotation(rot); }			// �����ݒ�
	virtual void SetOldRotation(const MyLib::Vector3& rot) override		{ m_pObjBillboard->SetOldRotation(rot); }		// �O��̌����ݒ�
	virtual void SetOriginRotation(const MyLib::Vector3& rot) override	{ m_pObjBillboard->SetOriginRotation(rot); }	// ���̌����ݒ�

	//--------------------------
	// �T�C�Y
	//--------------------------
	virtual void SetSize(const MyLib::Vector2& size) override		{ m_pObjBillboard->SetSize(size); }			// �T�C�Y�ݒ�
	virtual MyLib::Vector2 GetSize() const override					{ return m_pObjBillboard->GetSize(); }			// �T�C�Y�擾
	virtual void SetSizeOrigin(const MyLib::Vector2& size) override	{ m_pObjBillboard->SetSizeOrigin(size); }		// ���̃T�C�Y�ݒ�
	virtual MyLib::Vector2 GetSizeOrigin() const override			{ return m_pObjBillboard->GetSizeOrigin(); }	// ���̃T�C�Y�擾

	//--------------------------
	// �F
	//--------------------------
	virtual void SetColor(const D3DXCOLOR& col) override		{ m_pObjBillboard->SetColor(col); }			// �F�ݒ�
	virtual D3DXCOLOR GetColor() const override					{ return m_pObjBillboard->GetColor(); }		// �F�擾
	virtual void SetOriginColor(const D3DXCOLOR& col) override	{ m_pObjBillboard->SetOriginColor(col); }		// ���̐F�ݒ�
	virtual D3DXCOLOR GetOriginColor() const override			{ return m_pObjBillboard->GetOriginColor(); }	// ���̐F�擾
	virtual void SetAlpha(const float alpha) override			{ m_pObjBillboard->SetAlpha(alpha); }			// �s�����x�ݒ�
	virtual float GetAlpha() const override						{ return m_pObjBillboard->GetAlpha(); }		// �s�����x�擾

	//--------------------------
	// �l
	//--------------------------
	virtual void SetNum(int nNum) override	{ m_pObjBillboard->SetPatternAnim(nNum); }	// �l�ݒ�

	//--------------------------
	// ���̑�
	//--------------------------
	virtual void BindTexture(int nIdxTexture) override { m_pObjBillboard->BindTexture(nIdxTexture); }						// �e�N�X�`�����蓖��
	void SetVtx();	// ���_���ݒ菈��
	virtual void SetType(const TYPE type) override;		// ��ސݒ�

private:

	//=============================
	// �����o�ϐ�
	//=============================
	CObjectBillboardAnim* m_pObjBillboard;	// �I�u�W�F�N�gBillboard
};

#endif