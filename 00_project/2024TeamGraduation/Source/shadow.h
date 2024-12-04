//==========================================================================
// 
//  �e�w�b�_�[ [shadow.h]
//  Author : ���n�Ή�
//  Adder  : ���c�E��
// 
//==========================================================================

#ifndef _SHADOW_H_
#define _SHADOW_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object3D.h"
#include "gamemanager.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �e�N���X��`
class CShadow : public CObject3D
{
public:

	// �萔
	static constexpr float MIN_ALPHA = 0.25f;	// ���l�̍ŏ��l
	static constexpr float MAX_ALPHA = 0.55f;	// ���l�̍ő�l

	// �R���X�g���N�^/�f�X�g���N�^
	CShadow(int nPriority = 7);
	~CShadow();

	// �ÓI�����o�֐�
	static CShadow *Create
	(
		CObject* pObject,				// �e�I�u�W�F�N�g
		float fRadius = 50.0f,			// ���̔��a
		float fMinAlpha = MIN_ALPHA,	// �ŏ������x
		float fMaxAlpha = MAX_ALPHA,	// �ő哧���x
		float fLandY = CGameManager::FIELD_LIMIT	// �\��Y���W
	);

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
	CObject *m_pObject;		// �I�u�W�F�N�g�̃|�C���^
	float m_fOriginRadius;	// ���̔��a
	float m_fLandY;			// �\��Y���W
	float m_fMinAlpha;		// �����x�̍ŏ��l
	float m_fMaxAlpha;		// �����x�̍ő�l
	int m_nTexIdx;			// �e�N�X�`���̃C���f�b�N�X�ԍ�
};


#endif