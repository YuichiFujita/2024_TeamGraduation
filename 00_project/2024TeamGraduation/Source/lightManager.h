//============================================================
//
//	���C�g�}�l�[�W���[�w�b�_�[ [lightManager.h]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	��d�C���N���[�h�h�~
//************************************************************
#ifndef _LIGHT_MANAGER_H_
#define _LIGHT_MANAGER_H_

//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "lightDir.h"

//************************************************************
//	�N���X��`
//************************************************************
// ���C�g�}�l�[�W���[�N���X
class CLightManager
{
public:
	// �萔
	static constexpr int MAX_NUM = 3;	// �g�p���C�g��

	// �R���X�g���N�^
	CLightManager();

	// �f�X�g���N�^
	~CLightManager();

	// �����o�֐�
	HRESULT Init();	// ������
	void Uninit();	// �I��
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �X�V
	void SetEnableBright(const bool bBright, const float fChangeTime = 0.0f);			// ���邳�ݒ�
	inline D3DLIGHT9 GetLight(const int nIdx) { return m_apLight[nIdx]->GetLight(); }	// ���C�g�擾

	// �ÓI�����o�֐�
	static CLightManager* Create();	// ����
	static void Release(CLightManager*& prLightManager);	// �j��

private:
	// �����o�֐�
	MyLib::Vector3 ConvertColorToVector3(const D3DXCOLOR& rCol) { return MyLib::Vector3(rCol.r, rCol.g, rCol.b); }							// �F�̃x�N�g���ϊ�
	D3DXCOLOR ConvertVector3ToColor(const MyLib::Vector3& rVec, const float fAlpha) { return D3DXCOLOR(rVec.x, rVec.y, rVec.z, fAlpha); }	// �x�N�g���̐F�ϊ�

	// �����o�ϐ�
	CLightDir* m_apLight[MAX_NUM];	// ���C�g���
	D3DXCOLOR m_aColStart[MAX_NUM];	// ���C�g�J�n�F
	D3DXCOLOR m_aColEnd[MAX_NUM];	// ���C�g�I���F
	D3DXCOLOR m_colCamStart;		// �J�������C�g�J�n�F
	D3DXCOLOR m_colCamEnd;			// �J�������C�g�I���F
	float m_fStartTime;		// �J�n����
	float m_fCurTime;		// �o�ߎ���
	bool m_bChange;			// �ϓ��t���O
};

#endif	// _LIGHT_MANAGER_H_
