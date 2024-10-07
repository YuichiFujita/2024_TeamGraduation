//=============================================================================
// 
//  �X�e���V���o�b�t�@�g�p�e[stencilshadow.h]
//  Author : �Ό��D�n
// 
//=============================================================================

#ifndef _STENCIL_SHADOW_H_
#define _STENCIL_SHADOW_H_		// ��d�C���N���[�h�h�~

#include "object2D.h"

//==========================================================================
// �N���X��`
//==========================================================================
//�`�F�b�N�|�C���g�N���X
class CStencilShadow : public CObject2D
{
public:

	CStencilShadow(int nPriority = 6);
	~CStencilShadow();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	// �����o�֐�
	void Kill();		// �폜

	//=============================
	// �ÓI�֐�
	//=============================
	static CStencilShadow* Create();

private:

};


#endif