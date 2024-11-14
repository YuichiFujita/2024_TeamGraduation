//==========================================================================
// 
//  �����w�b�_�[ [number.h]
//  Author : ���n �Ή�
// 
//==========================================================================

#ifndef _NUMBER_H_
#define _NUMBER_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "multinumber.h"
#include "object.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �����N���X
class CNumber : public CObject
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CNumber(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	virtual ~CNumber();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
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
	virtual void SetPosition(const MyLib::Vector3& pos) = 0;		// �ʒu�ݒ�
	virtual void SetOldPosition(const MyLib::Vector3& pos) = 0;		// �ߋ��̈ʒu�ݒ�
	virtual void SetOriginPosition(const MyLib::Vector3& pos) = 0;	// ���̈ʒu�ݒ�

	//--------------------------
	// ����
	//--------------------------
	virtual void SetRotation(const MyLib::Vector3& rot) = 0;		// �����ݒ�
	virtual void SetOldRotation(const MyLib::Vector3& rot) = 0;		// �O��̌����ݒ�
	virtual void SetOriginRotation(const MyLib::Vector3& rot) = 0;	// ���̌����ݒ�

	//--------------------------
	// �T�C�Y
	//--------------------------
	virtual void SetSize(const MyLib::Vector2& size) = 0;			// �T�C�Y�ݒ�
	virtual MyLib::Vector2 GetSize() const = 0;						// �T�C�Y�擾
	virtual void SetSizeOrigin(const MyLib::Vector2& size) = 0;		// ���̃T�C�Y�ݒ�
	virtual MyLib::Vector2 GetSizeOrigin() const = 0;				// ���̃T�C�Y�擾

	//--------------------------
	// �F
	//--------------------------
	virtual void SetColor(const D3DXCOLOR& col) = 0;			// �F�ݒ�
	virtual D3DXCOLOR GetColor() const = 0;						// �F�擾
	virtual void SetOriginColor(const D3DXCOLOR& col) = 0;		// ���̐F�ݒ�
	virtual D3DXCOLOR GetOriginColor() const = 0;				// ���̐F�擾
	virtual void SetAlpha(const float alpha) = 0;				// �s�����x�ݒ�
	virtual float GetAlpha() const = 0;							// �s�����x�擾

	//--------------------------
	// �l
	//--------------------------
	virtual void SetNum(int nNum) = 0;	// �l�ݒ�

	//--------------------------
	// ���̑�
	//--------------------------
	virtual void BindTexture(int nIdxTexture) = 0;						// �e�N�X�`�����蓖��

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CNumber* Create(CMultiNumber::EObjType type, int nPriority);	// ����

private:

	
};

#endif
