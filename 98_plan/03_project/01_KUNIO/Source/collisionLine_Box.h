//=============================================================================
// 
// �����蔻��{�b�N�X���� [collisionLine_Box.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _COLLISIONLINE_BOX_H_
#define _COLLISIONLINE_BOX_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "objectLine.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �����蔻��{�b�N�X�N���X
class CCollisionLine_Box : public CObject
{
public:

	CCollisionLine_Box(int nPriority = mylib_const::PRIORITY_DEF2D, const LAYER layer = LAYER::LAYER_LINE);
	~CCollisionLine_Box();

	//  �I�[�o�[���C�h�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void SetPosition(const MyLib::Vector3& pos) override;	// �ʒu�ݒ�

	void SetColor(const D3DXCOLOR& col);			// �F�ݒ�

	void SetAABB(MyLib::AABB aabb, float scale = 1.0f);	// AAbB���ݒ�
	MyLib::AABB GetAABB() { return m_AABB; }
	void Kill();

	static CCollisionLine_Box* Create(const MyLib::AABB& aabb, const D3DXCOLOR& col);

private:

	//=============================
	// �����o�ϐ�
	//=============================
	MyLib::AABB m_AABB;
	D3DXCOLOR m_LineColor;
	CObjectLine* m_pLine[12];	// ���C���̃I�u�W�F�N�g
};
#endif