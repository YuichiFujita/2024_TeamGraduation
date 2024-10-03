//=============================================================================
// 
//  �I�u�W�F�N�g3D�w�b�_�[ [object3DMesh.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECT3DMESH_H_
#define _OBJECT3DMESH_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object3D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g3D�N���X��`
class CObject3DMesh : public CObject3D
{
public:

	typedef enum
	{
		TYPE_FIELD = 0,	// �n��
		TYPE_WALL,		// ��
		TYPE_CYLINDER,	// �V�����_�[
		TYPE_DOME,		// �h�[��
		TYPE_DONUTS,	// �h�[�i�c
		TYPE_SPHERE,	// ��
		TYPE_MAX
	}TYPE;

	CObject3DMesh(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CObject3DMesh();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	HRESULT Init(TYPE type);
	void Uninit();
	void Update();
	void Draw();
	void SetVtx();
	void BindTexture(int nIdx);

	int GetNumIndex();		// �C���f�b�N�X��
	void SetNumIndex(int nWidth, int nHeight);
	int GetNumVertex();		// ���_��
	void SetNumVertex(int nWidth, int nHeight);
	MyLib::Vector3 *GetVtxPos();		// ���_���W�擾
	void SetVtxPos(MyLib::Vector3 *pos);	// ���_���W�擾
	MyLib::Vector3 *GetVtxNor();		// ���_�x�N�g���擾
	void SetVtxNor(MyLib::Vector3 *nor);	// ���_�x�N�g��
	D3DXCOLOR *GetVtxCol();			// ���_�J���[�擾
	void SetVtxCol(D3DXCOLOR *col);		// ���_�J���[�ݒ�
	D3DXVECTOR2 *GetVtxTex();		// ���_�e�N�X�`���擾
	void SetVtxTex(D3DXVECTOR2 *tex);	// ���_�e�N�X�`���ݒ�
	int GetWidthBlock();			// ��������
	void SetWidthBlock(int nWidth);		// ��������
	int GetHeightBlock();			// �c������
	void SetHeightBlock(int nHeight);	// �c������
	float GetWidthLen();			// ���̒���
	void SetWidthLen(float fLen);		// ���̒���
	float GetHeightLen();			// �c�̒���
	void SetHeightLen(float fLen);		// �c�̒���
	int GetIdxTex();				// �e�N�X�`���̃C���f�b�N�X�ԍ��擾

	static CObject3DMesh *Create();
	static CObject3DMesh *Create(MyLib::Vector3 pos, MyLib::Vector3 rot, int nPriority = mylib_const::PRIORITY_DEFAULT);
	static CObject3DMesh *Create(MyLib::Vector3 pos, MyLib::Vector3 rot, float fWidthLen, float fHeightLen, int nWidth, int nHeight, TYPE type, const char *pFileName, int nPriority = mylib_const::PRIORITY_DEFAULT);
	CObject3DMesh *GetObject3DMesh();
protected:

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff();
private:
	HRESULT CreateVertex();
	HRESULT CreateIndex();
	void SetInitVtxField();
	void SetVtxField();
	void SetVtxWall();
	void SetVtxCylinder();
	void SetVtxDome();
	void SetVtxDonuts();
	void SetVtxSphere();
	void UPVtxField(MyLib::Vector3 pos);
	void CalWorldMtx();	// ���[���h�}�g���b�N�X�̌v�Z����

	MyLib::Vector3 *m_pVtxPos;	// ���_���W
	MyLib::Vector3 *m_pVtxNor;	// ���_�@��
	D3DXCOLOR *m_pVtxCol;	// ���_�J���[
	D3DXVECTOR2 *m_pVtxTex;	// ���_�e�N�X�`�����W
	int m_nNumIndex;		// �C���f�b�N�X��
	int m_nNumVertex;		// ���_��
	int m_nWidth;			// ��������
	int m_nHeight;			// �c������
	float m_fWidthLen;		// ���̒���
	float m_fHeightLen;		// �c�̒���
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	int m_nTexIdx;						// �e�N�X�`���̃C���f�b�N�X�ԍ�
	TYPE m_type;						// ���b�V���̃^�C�v
};



#endif