//=============================================================================
// 
//  �X�t�B�[���h(�Œ蕽��)�w�b�_�[ [townfield_fixedplane.h]
//  Author : ���n �Ή�
// 
//=============================================================================

#ifndef _TONWFIELD_FIXED_PLANE_H_
#define _TONWFIELD_FIXED_PLANE_H_	// ��d�C���N���[�h�h�~

#include "mapmesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �X�t�B�[���h(�Œ蕽��)�N���X
class CTownField_FixedPlane : public CMapMesh
{

public:

	CTownField_FixedPlane(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CTownField_FixedPlane();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void BindVtxPosition() override;	// ���_���W���蓖��

private:

	virtual void SetVtxTexUV() override;	// �e���_UV���W�ݒ�
};


// �X�t�B�[���h(�Œ蕽��/���U���g)�N���X
class CTownField_FixedPlane_Result : public CTownField_FixedPlane
{
public:

	CTownField_FixedPlane_Result();
	~CTownField_FixedPlane_Result() {}

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
};


// �X�t�B�[���h(�Œ蕽��/�����L���O)�N���X
class CTownField_FixedPlane_Ranking : public CTownField_FixedPlane
{
public:

	CTownField_FixedPlane_Ranking();
	~CTownField_FixedPlane_Ranking() {}

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
};

#endif