//=============================================================================
//
// �����ւ�(�A�N�Z)���� [dressup_accessory.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _DRESSUP_ACCESSORY_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _DRESSUP_ACCESSORY_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "dressup.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �����ւ�(��)�N���X
class CDressup_Accessory : public CDressup
{
public:

	CDressup_Accessory();
	~CDressup_Accessory();

	//=============================
	// �����o�֐�
	//=============================
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override; // �X�V
	virtual void ReRegist() override;	// �Ċ��蓖��

private:

	//=============================
	// �����o�֐�
	//=============================
	// ���̑�
	void Debug() override;

};

#endif