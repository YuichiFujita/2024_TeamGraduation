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
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	virtual void SetVtx();

	//--------------------------
	// �ό`�p�����[�^
	//--------------------------
	virtual void SetWorldMtx(const MyLib::Matrix& mtx) { m_mtxWorld = mtx; }	// �}�g���b�N�X�ݒ�
	virtual MyLib::Matrix GetWorldMtx() const { return m_mtxWorld; }			// �}�g���b�N�X�擾

	//--------------------------
	// �F
	//--------------------------
	virtual void SetColor(const D3DXCOLOR& col) { m_col = col; }					// �F�ݒ�
	virtual D3DXCOLOR GetColor() const { return m_col; }							// �F�擾
	virtual void SetOriginColor(const D3DXCOLOR& col) { m_colOrigin = col; }		// ���̐F�ݒ�
	virtual D3DXCOLOR GetOriginColor() const { return m_colOrigin; }				// ���̐F�擾
	virtual void SetAlpha(const float alpha) { m_col.a = alpha; }					// �s�����x�ݒ�
	virtual float GetAlpha() const { return m_col.a; }								// �s�����x�擾
	
	//--------------------------
	// �T�C�Y�֘A
	//--------------------------
	virtual void SetSize(const D3DXVECTOR2& size);									// �T�C�Y�̐ݒ�
	virtual D3DXVECTOR2 GetSize() const { return m_size; }							// �T�C�Y�̎擾
	virtual void SetSizeOrigin(const D3DXVECTOR2& size) { m_sizeOrigin = size; }	// ���̃T�C�Y�̐ݒ�
	virtual D3DXVECTOR2 GetSizeOrigin() const { return m_sizeOrigin; }				// ���̃T�C�Y�̎擾

	// �Ίp��
	void SetLength(const float fLength) { m_fLength = fLength; }					// �Ίp���̒����ݒ�
	float GetLength() const { return m_fLength; }									// �Ίp���̒����擾
	void SetAngle(const float fAngle) { m_fAngle = fAngle; }						// �Ίp���̌����ݒ�
	float GetAngle() const { return m_fAngle; }										// �Ίp���̌����擾

	//--------------------------
	// ���_���
	//--------------------------
	// UV
	void SetUV(const std::vector<D3DXVECTOR2>& uv) { m_vecUV = uv; }		// �e�N�X�`�����W�ݒ�
	std::vector<D3DXVECTOR2> GetUV() { return m_vecUV; }					// �e�N�X�`�����W�擾

	void BindTexture(int nIdx) { m_nTexIdx = nIdx; }	// �e�N�X�`�����蓖��
	static CObjectBillboard *Create(int nPriority = mylib_const::PRIORITY_ZSORT);
	static CObjectBillboard *Create(const MyLib::Vector3& pos, const MyLib::Vector3& rot);
	CObjectBillboard* GetObjectBillBoard() { return this; }				// �I�u�W�F�N�g�r���{�[�h�擾
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff() const { return m_pVtxBuff; }	// ���_�o�b�t�@�擾

private:

	//=============================
	// �����o�ϐ�
	//=============================
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@
	MyLib::Matrix m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXCOLOR m_col;					// �F
	D3DXCOLOR m_colOrigin;				// ���̐F
	D3DXVECTOR2 m_size;					// �T�C�Y
	D3DXVECTOR2 m_sizeOrigin;			// ���̃T�C�Y
	float m_fLength;					// �Ίp���̒���
	float m_fAngle;						// �Ίp���̌���
	std::vector<D3DXVECTOR2> m_vecUV;	// �e�N�X�`�����W
	int m_nTexIdx;						// �e�N�X�`���̃C���f�b�N�X�ԍ�
};



#endif