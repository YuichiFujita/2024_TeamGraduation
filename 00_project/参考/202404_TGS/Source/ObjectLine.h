//=============================================================================
// 
//  �I�u�W�F�N�g���C���w�b�_�[ [objectLine.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECT3D_ANIM_H_
#define _OBJECT3D_ANIM_H_	// ��d�C���N���[�h�h�~

#include "object.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g���C���N���X
class CObjectLine : public CObject
{
public:

	//=============================
	// �\���̒�`
	//=============================
	struct SLineInfo
	{
		MyLib::Vector3 left;
		MyLib::Vector3 right;
		D3DXCOLOR col;

		SLineInfo() : left(0.0f), right(0.0f), col(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)) {}
	};

	CObjectLine(int nPriority = 8, const LAYER layer = LAYER::LAYER_LINE);
	virtual ~CObjectLine();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void SetVtx() override;

	void SetLeftPosition(const MyLib::Vector3& pos) { m_Info.left = pos; }
	void SetRightPosition(const MyLib::Vector3& pos) { m_Info.right = pos; }
	void SetColor(const D3DXCOLOR& col) { m_Info.col = col; }			// �F�ݒ�
	void SetWorldMtx(const MyLib::Matrix mtx) { m_mtxWorld = mtx; }		// �}�g���b�N�X�ݒ�
	MyLib::Matrix GetWorldMtx() const { return m_mtxWorld; }			// �}�g���b�N�X�擾

	void SetEnableZSort(bool bSort) { m_bZSort = bSort; }

	static CObjectLine *Create(MyLib::Vector3 left, MyLib::Vector3 right, D3DXCOLOR col);

private:

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@
	MyLib::Matrix m_mtxWorld;			// ���[���h�}�g���b�N�X
	SLineInfo m_Info;
	bool m_bZSort;				// Z�\�[�g�̃t���O
};



#endif