//=============================================================================
// 
//  �������w�b�_�[ [meshsphere.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EXPLOSION_SPHERE_H_
#define _EXPLOSION_SPHERE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object3DMesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���b�V���X�t�B�A�N���X��`
class CMeshSphere : public CObject3DMesh
{
public:
	CMeshSphere(int nPriority = mylib_const::PRIORITY_DEF2D);
	~CMeshSphere();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void SetSizeDest(float fSize);	// �ڕW�̃T�C�Y�ݒ�
	static CMeshSphere *Create(MyLib::Vector3 pos, float fSize, const char *pFileName, int nPriority = mylib_const::PRIORITY_DEF2D);	// ��������
	static CMeshSphere *Create(MyLib::Vector3 pos, float fSize, int nTexIdx, int nPriority = mylib_const::PRIORITY_DEF2D,int nBlock = 16);	// ��������

private:

	// �����o�ϐ�
	int m_nTexIdx;		// �e�N�X�`���̃C���f�b�N�X�ԍ�
	float m_fSizeDest;	// �ڕW�̃T�C�Y

};



#endif