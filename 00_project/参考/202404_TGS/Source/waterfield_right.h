//=============================================================================
// 
//  ���t�B�[���h�w�b�_�[ [waterfield_right.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _WATERFIELD_RIGHT_H_
#define _WATERFIELD_RIGHT_H_	// ��d�C���N���[�h�h�~

#include "waterfield.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���t�B�[���h�N���X
class CWaterField_Right : public CWaterField
{
public:

	CWaterField_Right();
	~CWaterField_Right();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;

};



#endif