//=============================================================================
// 
// �X�e�[�W���w�b�_�[ [stagename.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _STAGENAME_H_
#define _STAGENAME_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "object2D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �X�e�[�W���N���X
class CStageName : public CObject2D
{
public:

	CStageName(int nPriority = 10);
	~CStageName();
	
	//  �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CStageName* Create();

private:

	//=============================
	// �����o�֐�
	//=============================

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fAlphaTime;	// �s�����x�^�C�}�[
	static CStageName* m_pThisPtr;	// ���g�̃|�C���^
};

#endif