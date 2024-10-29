//=============================================================================
// 
//  �e�w�b�_�[ [shadow.h]
//  Author : ���n�Ή�
//  Adder  : ���c�E��
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

	// �R���X�g���N�^/�f�X�g���N�^
	CShadow(int nPriority = 7);
	~CShadow();

	// �ÓI�����o�֐�
	static CShadow *Create(CObject* pObject, float fRadius = 50.0f);

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

	// �����o�֐�
	void BindObject(CObject* pObject) { m_pObject = pObject; }	// �Ǐ]�I�u�W�F�N�g����
	void SetPositionRelative();	// ���Έʒu�ݒ�

private:

	// �����o�ϐ�
	CObject *m_pObject;	// �I�u�W�F�N�g�̃|�C���^
	int m_nTexIdx;		// �e�N�X�`���̃C���f�b�N�X�ԍ�
};


#endif