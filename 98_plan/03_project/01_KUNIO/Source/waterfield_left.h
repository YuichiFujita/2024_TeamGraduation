//=============================================================================
// 
//  ���t�B�[���h�w�b�_�[ [waterfield_left.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _WATERFIELD_LEFT_H_
#define _WATERFIELD_LEFT_H_	// ��d�C���N���[�h�h�~

#include "waterfield.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���t�B�[���h�N���X
class CWaterField_Left : public CWaterField
{
public:

	CWaterField_Left();
	~CWaterField_Left();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;

};



#endif