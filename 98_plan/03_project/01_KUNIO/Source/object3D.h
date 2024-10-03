//=============================================================================
// 
//  �I�u�W�F�N�g3D�w�b�_�[ [object3D.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_	// ��d�C���N���[�h�h�~

#include "object.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g3D�N���X��`
class CObject3D : public CObject
{
public:

	CObject3D(int nPriority = 5, const LAYER layer = LAYER::LAYER_DEFAULT);
	virtual ~CObject3D();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	virtual void SetVtx();
	virtual void BindTexture(int nIdx);

	void SetWorldMtx(const MyLib::Matrix mtx);		// �}�g���b�N�X�ݒ�
	MyLib::Matrix GetWorldMtx() const;				// �}�g���b�N�X�擾
	void SetColor(const D3DXCOLOR col);				// �F�ݒ�
	D3DXCOLOR GetColor() const;						// �F�擾
	void SetColorOrigin(const D3DXCOLOR& col) { m_colOrigin = col; }				// �F�ݒ�
	D3DXCOLOR GetColorOrigin() const { return m_colOrigin; }						// �F�擾
	void SetAlpha(float a) { m_col.a = a; }			// �����x�ݒ�
	void SetSize(const MyLib::Vector3& size);		// �T�C�Y�̐ݒ�
	MyLib::Vector3 GetSize() const;					// �T�C�Y�̎擾
	void SetSizeOrigin(const MyLib::Vector3& size);	// ���̃T�C�Y�̐ݒ�
	MyLib::Vector3 GetSizeOrigin() const;			// ���̃T�C�Y�̎擾
	virtual void SetTex(D3DXVECTOR2 *tex);			// �e�N�X�`�����W�̐ݒ�
	virtual D3DXVECTOR2 *GetTex();					// �e�N�X�`�����W�̎擾

	static CObject3D *Create(int nPriority);
	static CObject3D *Create(MyLib::Vector3 pos, MyLib::Vector3 rot);
	CObject3D *GetObject3D();
	virtual CObject3DMesh *GetObject3DMesh();

protected:

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@

private:

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::Matrix m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXVECTOR2 m_fTex[32];		// �e�N�X�`�����W
	D3DXCOLOR m_col;			// �F
	D3DXCOLOR m_colOrigin;			// �F
	MyLib::Vector3 m_fSize;		// �T�C�Y
	int m_nTexIdx;				// �e�N�X�`���̃C���f�b�N�X�ԍ�
};



#endif