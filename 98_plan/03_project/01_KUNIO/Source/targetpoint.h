//=============================================================================
// 
//  �ڕW�n�_�w�b�_�[ [targetpoint.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TARGETPOINT_H_
#define _TARGETPOINT_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object3DMesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �ڕW�n�_�N���X��`
class CTargetPoint : public CObject3DMesh
{
public:
	CTargetPoint(int nPriority = 6);
	~CTargetPoint();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetVtx();

	void Control();	// ����
	static CTargetPoint *Create(MyLib::Vector3 pos, float fWidthLen, float fHeightLen);
	CTargetPoint *GetObject3DMesh();

private:

	int m_nTexIdx;	// �e�N�X�`���̃C���f�b�N�X�ԍ�
	int m_nAlphaCnt;	// �s�����x�̃J�E���g
};



#endif