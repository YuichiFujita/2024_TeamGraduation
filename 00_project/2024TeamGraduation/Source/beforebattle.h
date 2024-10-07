//=============================================================================
// 
//  �퓬�����w�b�_�[ [beforebattle.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _BEFOREBATTLE_H_
#define _BEFOREBATTLE_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �퓬�����N���X
class CBeforeBattle : public CObject2D
{
public:

	CBeforeBattle(int nPriority = 7);
	~CBeforeBattle();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CBeforeBattle* Create(const MyLib::Vector3& pos);

private:

	//=============================
	// �����o�ϐ�
	//=============================
	CObject2D* m_pAnyButton;	// �u�L�[�����āv�̃e�L�X�g
};


#endif