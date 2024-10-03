//=============================================================================
// 
//  ���b�V���E�H�[���w�b�_�[ [meshwall.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MESHWALL_H_
#define _MESHWALL_H_	// ��d�C���N���[�h�h�~

#include "object3DMesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���b�V���t�B�[���h�N���X��`
class CMeshWall : public CObject3DMesh
{
public:

	CMeshWall(int nPriority = 1);
	~CMeshWall();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void SetVtx();

	static CMeshWall *Create(MyLib::Vector3 pos, MyLib::Vector3 rot, float fWidthLen, float fHeightLen, int nWidth = 1, int nHeight = 1, int nPriority = 1, const char *aFileName = nullptr);
	virtual CMeshWall *GetMyObject();
protected:

private:
	int m_nTexIdx;	// �e�N�X�`���̃C���f�b�N�X�ԍ�
};



#endif