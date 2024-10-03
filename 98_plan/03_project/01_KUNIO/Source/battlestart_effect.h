//=============================================================================
// 
//  �퓬�J�n�w�b�_�[ [battlestart_effect.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _BATTLESTART_EFFECT_H_
#define _BATTLESTART_EFFECT_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �퓬�J�n�N���X
class CBattleStart_Effect : public CObject2D
{
public:

	CBattleStart_Effect(int nPriority = 10);
	~CBattleStart_Effect();


	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CBattleStart_Effect* Create(const MyLib::Vector3& pos);

private:

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fLife;	// ����
};


#endif