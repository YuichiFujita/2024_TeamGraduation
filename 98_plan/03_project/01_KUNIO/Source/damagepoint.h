//=============================================================================
// 
//  �_���[�W�|�C���g�w�b�_�[ [damagepoint.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _DMGPOINT_H_
#define _DMGPOINT_H_	// ��d�C���N���[�h�h�~

#include "object.h"

//==========================================================================
// �O���錾
//==========================================================================
class CMultiNumber;

//==========================================================================
// �N���X��`
//==========================================================================
// �_���[�W�|�C���g�N���X
class CDamagePoint : CObject
{
public:

	CDamagePoint(int nPriority = 8);
	~CDamagePoint();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();

	static CDamagePoint* Create(MyLib::Vector3 pos, int nDamage);

private:
	
	//=============================
	// �����o�֐�
	//=============================
	void SetValue();

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fLife;	// ����
	CMultiNumber *m_apNumber;	// �����̃I�u�W�F�N�g
};



#endif