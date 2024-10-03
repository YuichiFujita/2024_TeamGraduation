//=============================================================================
// 
//  �Ί_�w�b�_�[ [stonewall.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _STONEWALL_H_
#define _STONEWALL_H_	// ��d�C���N���[�h�h�~

#include "mapmesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �Ί_�N���X
class CStoneWall : public CMapMesh
{

public:

	CStoneWall(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CStoneWall();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void BindVtxPosition() override;	// ���_���W���蓖��

	std::vector<MyLib::Vector3> GetVecTopPosition() { return m_vecTopPosition; }	// ����̈ʒu�擾
	virtual void DispPoint();	// �_�\��

	static CStoneWall *Create();	// ����

protected:

	std::vector<MyLib::Vector3> m_vecTopPosition;	// ����̈ʒu

private:

	//=============================
	// �����o�֐�
	//=============================
	virtual void SetVtxTexUV() override;	// �e���_UV���W�ݒ�


};



#endif