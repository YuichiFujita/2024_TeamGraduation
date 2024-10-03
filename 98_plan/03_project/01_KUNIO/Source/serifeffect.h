//=============================================================================
// 
//  �˗��l�Z���t�G�t�F�N�g [serifeffect.h]
//  Author : �Ό��D�n
// 
//=============================================================================

#ifndef _SERIFEFFECT_H_
#define _SERIFEFFECT_H_	// ��d�C���N���[�h�h�~

#include "object3D.h"

//==========================================================================
// �N���X��`
//==========================================================================
class CSerifEffect : public CObject3D
{
public:

	CSerifEffect(int nPriority = 11);
	~CSerifEffect();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Kill();	// �폜
	static CSerifEffect* Create(MyLib::Vector3 pos, MyLib::Vector3 rot, int life);	// ��������

private:

	//=============================
	// �����o�ϐ�
	//=============================
	int m_Life;
	int m_MaxLife;
};


#endif