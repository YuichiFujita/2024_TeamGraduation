//=============================================================================
// 
// �f�o�b�O���͈͏��� [debug_bressrange.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _DEBUG_BRESSRANGE_H_
#define _DEBUG_BRESSRANGE_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "objectBillboard.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �f�o�b�O���͈̓N���X
class CDebugBressRange : public CObjectBillboard
{
public:

	CDebugBressRange(int nPriority = mylib_const::PRIORITY_ZSORT);
	~CDebugBressRange();

	static CDebugBressRange *Create();

	//  �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetVtx();

	void SetRange(const MyLib::Vector3& leftup, const MyLib::Vector3& rightup, const MyLib::Vector3& leftdw, const MyLib::Vector3& rightdw);

private:

	// �����o�֐�
	

	// �����o�ϐ�
	MyLib::Vector3 m_LeftUP;
	MyLib::Vector3 m_RightUP;
	MyLib::Vector3 m_LeftDW;
	MyLib::Vector3 m_RightDW;
};

#endif