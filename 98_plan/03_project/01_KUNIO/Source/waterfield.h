//=============================================================================
// 
//  ���t�B�[���h�w�b�_�[ [waterfield.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _WATERFIELD_H_
#define _WATERFIELD_H_	// ��d�C���N���[�h�h�~

#include "object3DMesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���t�B�[���h�N���X
class CWaterField : public CObject3DMesh
{
public:

	enum TYPE
	{
		TYPE_NORMAL = 0,
		TYPE_RIGHT,
		TYPE_LEFT,
		TYPE_MAX
	};

	CWaterField(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CWaterField();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void SetVtx() override;

	static CWaterField *Create(TYPE type);

protected:

	float m_fTexU;	// U�X�N���[���p
	float m_fTexV;	// V�X�N���[���p
};



#endif