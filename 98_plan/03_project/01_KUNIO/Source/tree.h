//=============================================================================
// 
//  tree�w�b�_�[ [tree.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TREE_H_
#define _TREE_H_	// ��d�C���N���[�h�h�~

#include "object3D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// tree�N���X
class CTree : public CObject3D
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_MAX
	};

	CTree(int nPriority = 7, const LAYER layer = LAYER::LAYER_MAP);
	~CTree();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//=============================
	// �ÓI�֐�
	//=============================
	static CTree* Create(MyLib::Vector3 pos);

private:

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn

	// ���̑��֐�

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	float m_fStateTime;		// ��ԃJ�E���^�[
	State m_state;			// ���

	// ���̑��ϐ�

};


#endif