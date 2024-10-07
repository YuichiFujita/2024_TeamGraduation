//=============================================================================
// 
// �g�k�n���h������ [handle_Scale.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _HANDLE_SCALE_H_
#define _HANDLE_SCALE_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "edithandle.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �g�k�n���h���N���X
class CHandle_Scale : public CHandle
{
public:

	CHandle_Scale(int nPriority = 8);
	~CHandle_Scale();

	//  �I�[�o�[���C�h�֐�
	HRESULT Init() override;

	SEditHandleInfo GetDiff(HandleAngle angle) override;	// �ړ������擾
	void Grab() override;	// �͂ݒ�����


private:

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::Vector3 m_DiffMove;	// �ړ��̍���
	MyLib::Vector3 m_DiffScale;	// �X�P�[���̍���
};
#endif