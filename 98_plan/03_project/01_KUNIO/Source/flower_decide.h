//=============================================================================
// 
//  �����ԃw�b�_�[ [flower_decide.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _FLOWER_DECIDE_H_
#define _FLOWER_DECIDE_H_	// ��d�C���N���[�h�h�~

#include "objectX.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �����ԃN���X��`
class CDecideFlower : public CObjectX
{
public:

	CDecideFlower(int nPriority = 6);
	~CDecideFlower();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;

	static CDecideFlower* Create(const MyLib::Vector3& pos, const MyLib::Vector3& move);	// ����

private:

	//=============================
	// �֐����X�g
	//=============================

	//=============================
	// �����o�֐�
	//=============================

	//=============================
	// �����o�ϐ�
	//=============================
};


#endif