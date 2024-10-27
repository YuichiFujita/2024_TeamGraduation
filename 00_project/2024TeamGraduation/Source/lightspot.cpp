#if 0
//==========================================================================
// 
//  �X�|�b�g���C�g���� [lightspot.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
#include "lightspot.h"
#include "manager.h"
#include "renderer.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CLightSpot::CLightSpot()
{
	// �X�|�b�g���C�g���̃N���A
	ZeroMemory(&m_light, sizeof(D3DLIGHT9));
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CLightSpot::~CLightSpot()
{

}

//==========================================================================
// �X�|�b�g���C�g�̏���������
//==========================================================================
HRESULT CLightSpot::Init()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X���
	MyLib::Vector3 vecDir;	// �ݒ�p�����x�N�g��

	for (int nCntLightSpot = 0; nCntLightSpot < TYPE_MAX; nCntLightSpot++)
	{
		// �X�|�b�g���C�g�̏����N���A����
		ZeroMemory(&m_aLightSpot[nCntLightSpot], sizeof(D3DLIGHT9));

		switch (nCntLightSpot)
		{
		case TYPE_DIRECTIONAL_01:

			// �X�|�b�g���C�g�̎�ނ�ݒ�
			m_aLightSpot[nCntLightSpot].Type = D3DLIGHT_DIRECTIONAL;

			// �X�|�b�g���C�g�̊g�U����ݒ�
			m_aLightSpot[nCntLightSpot].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �X�|�b�g���C�g�̕�����ݒ�
			vecDir = MyLib::Vector3(0.22f, -0.87f, 0.44f);
			break;

		case TYPE_DIRECTIONAL_02:

			// �X�|�b�g���C�g�̎�ނ�ݒ�
			m_aLightSpot[nCntLightSpot].Type = D3DLIGHT_DIRECTIONAL;

			// �X�|�b�g���C�g�̊g�U����ݒ�
			m_aLightSpot[nCntLightSpot].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

			// �X�|�b�g���C�g�̕�����ݒ�
			vecDir = MyLib::Vector3(-0.18f, 0.88f, -0.44f);
			break;

		case TYPE_DIRECTIONAL_03:

			// �X�|�b�g���C�g�̎�ނ�ݒ�
			m_aLightSpot[nCntLightSpot].Type = D3DLIGHT_DIRECTIONAL;

			// �X�|�b�g���C�g�̊g�U����ݒ�
			m_aLightSpot[nCntLightSpot].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

			// �X�|�b�g���C�g�̕�����ݒ�
			vecDir = MyLib::Vector3(0.89f, -0.11f, 0.44f);
			break;

		case TYPE_SPOT_01:

			// �X�|�b�g���C�g�̎�ނ��X�|�b�g�X�|�b�g���C�g�ɐݒ�
			m_aLightSpot[nCntLightSpot].Type = D3DLIGHT_DIRECTIONAL;

			// �X�|�b�g�X�|�b�g���C�g�̊g�U����ݒ�
			m_aLightSpot[nCntLightSpot].Diffuse = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);

			// �X�|�b�g�X�|�b�g���C�g�̕�����ݒ�
			vecDir = MyLib::Vector3(0.0f, 0.0f, 0.0f);
			break;
		}

		// �x�N�g���𐳋K������(1.0�ɂ���)
		D3DXVec3Normalize(&vecDir, &vecDir);
		m_aLightSpot[nCntLightSpot].Direction = vecDir;

		// �X�|�b�g���C�g��ݒ肷��
		pDevice->SetLightSpot(nCntLightSpot, &m_aLightSpot[nCntLightSpot]);

		// �X�|�b�g���C�g��L���ɂ���
		pDevice->LightSpotEnable(nCntLightSpot, TRUE);
	}

	return S_OK;
}

//==========================================================================
// �X�|�b�g���C�g�̏I������
//==========================================================================
void CLightSpot::Uninit()
{

}

//==========================================================================
// �X�|�b�g���C�g�̍X�V����
//==========================================================================
void CLightSpot::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//==========================================================================
// �X�|�b�g�X�|�b�g���C�g�̌����X�V
//==========================================================================
void CLightSpot::UpdateSpotLightSpotDirection(MyLib::Vector3 vec)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �X�|�b�g�X�|�b�g���C�g�̕�����ݒ�
	MyLib::Vector3 vecDir = vec;

	// �x�N�g���𐳋K������(1.0�ɂ���)
	D3DXVec3Normalize(&vecDir, &vecDir);
	m_aLightSpot[TYPE_SPOT_01].Direction = vecDir;

	// �X�|�b�g���C�g��ݒ肷��
	pDevice->SetLightSpot(TYPE_SPOT_01, &m_aLightSpot[TYPE_SPOT_01]);

	// �X�|�b�g���C�g��L���ɂ���
	pDevice->LightSpotEnable(TYPE_SPOT_01, TRUE);
}
#endif
