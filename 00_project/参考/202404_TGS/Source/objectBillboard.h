//=============================================================================
// 
//  �r���{�[�h�w�b�_�[ [objectBillboard.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECTBILLBOARD_H_
#define _OBJECTBILLBOARD_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"
#include "constans.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �r���{�[�h�N���X��`
class CObjectBillboard : public CObject
{
public:

	CObjectBillboard(int nPriority = mylib_const::PRIORITY_ZSORT);
	~CObjectBillboard();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	virtual void SetVtx();
	void BindTexture(int nIdx);
	virtual void SetRotation(const MyLib::Vector3& rot) override;		// �����ݒ�

	virtual void SetWorldMtx(const MyLib::Matrix mtx);	// �}�g���b�N�X�ݒ�
	virtual MyLib::Matrix GetWorldMtx() const;			// �}�g���b�N�X�擾
	virtual void SetColor(const D3DXCOLOR col);			// �F�ݒ�
	virtual D3DXCOLOR GetColor() const;					// �F�擾
	virtual void SetAlpha(const float col);				// �s�����x�ݒ�
	virtual float GetAlpha() const;						// �s�����x�擾
	virtual void SetSize(const D3DXVECTOR2 size);		// �T�C�Y�̐ݒ�
	virtual D3DXVECTOR2 GetSize() const;				// �T�C�Y�̎擾
	virtual void SetSizeOrigin(const D3DXVECTOR2 size);	// ���̃T�C�Y�̐ݒ�
	virtual D3DXVECTOR2 GetSizeOrigin() const;			// ���̃T�C�Y�̎擾
	virtual void SetTex(D3DXVECTOR2 *tex);				// �e�N�X�`�����W�̐ݒ�
	virtual D3DXVECTOR2 *GetTex();						// �e�N�X�`�����W�̎擾

	static CObjectBillboard *Create(int nPriority = mylib_const::PRIORITY_ZSORT);
	static CObjectBillboard *Create(const MyLib::Vector3& pos, const MyLib::Vector3& rot);
	CObjectBillboard *GetObjectBillBoard();
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff() const;

protected:

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@
	float m_fLength;			// �Ίp���̒���
	float m_fAngle;				// �Ίp���̌���

private:
	MyLib::Matrix m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXCOLOR m_col;			// �F
	D3DXVECTOR2 m_fSize;		// �T�C�Y
	D3DXVECTOR2 m_sizeOrigin;	// ���̃T�C�Y
	D3DXVECTOR2 m_fTex[32];	// �e�N�X�`�����W
	int m_nTexIdx;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
};



#endif