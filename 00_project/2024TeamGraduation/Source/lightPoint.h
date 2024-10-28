//============================================================
//
//	�|�C���g���C�g�w�b�_�[ [lightPoint.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _LIGHT_POINT_H_
#define _LIGHT_POINT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "light.h"

//************************************************************
//	�N���X��`
//************************************************************
// �|�C���g���C�g�N���X
class CLightPoint : public CLight
{
public:
	// �R���X�g���N�^
	CLightPoint();

	// �f�X�g���N�^
	~CLightPoint() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Kill() override;		// �폜
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	void Draw() override;		// �`��

	// �ÓI�����o�֐�
	static CLightPoint* Create();	// ����
};

#endif	// _LIGHT_POINT_H_
