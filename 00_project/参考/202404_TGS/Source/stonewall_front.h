//=============================================================================
// 
//  �Ί_�w�b�_�[ [stonewall_front.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _STONEWALL_FRONT_H_
#define _STONEWALL_FRONT_H_	// ��d�C���N���[�h�h�~

#include "stonewall.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �Ί_�N���X
class CStoneWall_Front : public CStoneWall
{

public:

	CStoneWall_Front(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CStoneWall_Front();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual void BindVtxPosition() override;	// ���_���W���蓖��

	// �����o�֐�
	virtual void DispPoint() override;	// �_�\��

	static CStoneWall_Front *Create();	// ����

private:

	//=============================
	// �����o�֐�
	//=============================
	virtual void SetVtxTexUV() override;	// �e���_UV���W�ݒ�

};



#endif