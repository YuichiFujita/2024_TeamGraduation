//=============================================================================
//
// �J�����f�o�b�O���� [camera_debug.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _CAMERA_DEBUG_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _CAMERA_DEBUG_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "camera.h"	// �J����

//==========================================================================
// �N���X��`
//==========================================================================
// �J�����N���X��`
class CCamera_Debug
{
public:

	CCamera_Debug(CCamera* pCamera) : m_pCamera(pCamera), m_move() {}
	CCamera_Debug(CCamera* pCamera, const MyLib::Vector3& move) : m_pCamera(pCamera), m_move(move) {}
	~CCamera_Debug() {}

	void Update();	// �X�V����

private:


	//=============================
	// �֐����X�g
	//=============================

	//=============================
	// �����o�֐�
	//=============================
	void UpdateWASD();	// WASD�̍X�V
	void UpdateDistance();	// �����̍X�V

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::Vector3 m_move;		// �ړ���
	MyLib::Vector3 m_moveRot;	// ��]�ړ���
	CCamera* m_pCamera;		// �J�����̃|�C���^
};

#endif