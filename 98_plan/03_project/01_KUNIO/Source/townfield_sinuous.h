//=============================================================================
// 
//  �X�t�B�[���h(���˂�)�w�b�_�[ [townfield_sinuous.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TONWFIELD_SINUOUS_H_
#define _TONWFIELD_SINUOUS_H_	// ��d�C���N���[�h�h�~

#include "mapmesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �X�t�B�[���h(���˂�)�N���X
class CTownField_Sinuous : public CMapMesh
{

public:

	CTownField_Sinuous(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CTownField_Sinuous();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void BindVtxPosition() override;	// ���_���W���蓖��

private:

	virtual void SetVtxTexUV() override;	// �e���_UV���W�ݒ�
};



#endif