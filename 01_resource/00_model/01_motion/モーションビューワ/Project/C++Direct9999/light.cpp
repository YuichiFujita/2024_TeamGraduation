//=============================================================================
// 
//  ���C�g���� [light.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "light.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"


//==========================================================================
// �R���X�g���N�^
//==========================================================================
CLight::CLight()
{
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ���C�g�̏����N���A����
		ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));
	}
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CLight::~CLight()
{

}

//==================================================================================
// ���C�g�̏���������
//==================================================================================
HRESULT CLight::Init(void)
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir;	// �ݒ�p�����x�N�g��

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ���C�g�̏����N���A����
		ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));

		// ���C�g�̎�ނ�ݒ�
		m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		switch (nCntLight)
		{
		case 0:
			// ���C�g�̊g�U����ݒ�
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
			break;

		case 1:
			// ���C�g�̊g�U����ݒ�
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

			// ���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
			break;

		case 2:
			// ���C�g�̊g�U����ݒ�
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

			// ���C�g�̕�����ݒ�
			vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
			break;
		}

		// �x�N�g���𐳋K������(1.0�ɂ���)
		D3DXVec3Normalize(&vecDir, &vecDir);
		m_aLight[nCntLight].Direction = vecDir;

		// ���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &m_aLight[nCntLight]);

		// ���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}

	return S_OK;
}

//==================================================================================
// ���C�g�̏I������
//==================================================================================
void CLight::Uninit(void)
{

}

//==================================================================================
// ���C�g�̍X�V����
//==================================================================================
void CLight::Update(void)
{
	
}
