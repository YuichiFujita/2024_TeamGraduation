//=============================================================================
// 
//  �X�t�B�[���h(���˂�)�w�b�_�[ [townfield_sinuous_front.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TONWFIELD_SINUOUS_FRONT_H_
#define _TONWFIELD_SINUOUS_FRONT_H_	// ��d�C���N���[�h�h�~

#include "townfield_sinuous.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �X�t�B�[���h(���˂�)�N���X
class CTownField_Sinuous_Front : public CTownField_Sinuous
{

public:

	CTownField_Sinuous_Front(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CTownField_Sinuous_Front();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual void BindVtxPosition() override;	// ���_���W���蓖��

private:

	virtual void SetVtxTexUV() override;	// �e���_UV���W�ݒ�
};



#endif