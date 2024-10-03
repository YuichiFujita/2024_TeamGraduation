//=============================================================================
// 
//  �e�w�b�_�[ [shadow.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SHADOW_H_
#define _SHADOW_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object3D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �e�N���X��`
class CShadow : public CObject3D
{
public:
	CShadow(int nPriority = 7);
	~CShadow();

	static CShadow *Create();
	static CShadow *Create(MyLib::Vector3 pos, float size = 50.0f);

	// �I�[�o�[���C�h���ꂽ�֐�����
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	static int GetNumAll();

private:

	CObject *m_pObject;		// �I�u�W�F�N�g�̃|�C���^
	static int m_nNumAll;	// ����
	int m_nTexIdx;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
};


#endif