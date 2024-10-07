//=============================================================================
// 
// �ړ��n���h������ [handle_Move.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _HANDLE_MOVE_H_
#define _HANDLE_MOVE_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "objectX.h"
#include "edithandle.h"
class CObjectLine;

//==========================================================================
// �N���X��`
//==========================================================================
// �ړ��n���h���N���X
class CHandle_Move : public CHandle
{
public:

	CHandle_Move(int nPriority = 8);
	~CHandle_Move();

	//  �I�[�o�[���C�h�֐�
	HRESULT Init() override;

	SEditHandleInfo GetDiff(HandleAngle angle);	// �����擾
	void Grab() override;	// �͂ݒ�����

private:

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::Vector3 m_DiffMove;				// �ړ��̍���
};
#endif