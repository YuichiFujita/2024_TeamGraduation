//=============================================================================
// 
//  ���b�V���V�����_�[�w�b�_�[ [meshcylinder.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object3DMesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���b�V���V�����_�[�N���X��`
class CMeshCylinder : public CObject3DMesh
{
public:
	CMeshCylinder(int nPriority = 2);
	~CMeshCylinder();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetVtx();

	static CMeshCylinder *Create();
	static CMeshCylinder *Create(const char *aFileName = nullptr);
	CMeshCylinder *GetMyObject();

private:

};



#endif