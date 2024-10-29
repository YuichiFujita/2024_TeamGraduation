//============================================================
//
//	���C�g�w�b�_�[ [light.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _LIGHT_H_
#define _LIGHT_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "object.h"

//************************************************************
//	�N���X��`
//************************************************************
// ���C�g�N���X
class CLight : public CObject
{
public:
	// �R���X�g���N�^
	CLight();

	// �f�X�g���N�^
	~CLight() override;

	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;	// ������
	void Uninit() override;		// �I��
	void Kill() override;		// �폜
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V
	void Draw() override;		// �`��

	// �����o�֐�
	void SetLightType(const D3DLIGHTTYPE type);		// ��ސݒ�
	void SetPosition(const MyLib::Vector3& rPos);	// �ʒu�ݒ�
	void SetDirection(const MyLib::Vector3& rDir);	// �����ݒ�
	void SetRange(const float fRange);				// �����͈͐ݒ�
	void SetDiffuse(const MyLib::Color& rCol);		// �g�U���ݒ�
	void SetEnableLight(const bool bLight);			// �L���ݒ�
	inline D3DLIGHT9 GetLight() { return m_light; }	// ���C�g�擾

private:
	// �ÓI�����o�ϐ�
	static int m_nNumLight;	// ���C�g�̑���

	// �����o�ϐ�
	D3DLIGHT9 m_light;		// ���C�g���
	const int m_nLightIdx;	// ���g�̃��C�g�C���f�b�N�X
};

#endif	// _LIGHT_H_
