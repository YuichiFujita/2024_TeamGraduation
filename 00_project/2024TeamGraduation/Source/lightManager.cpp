//============================================================
//
//	���C�g�}�l�[�W���[���� [lightManager.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "lightManager.h"
#include "manager.h"
#include "camera.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const MyLib::Color DIFFUSE[2][CLightManager::MAX_NUM] =	// �ݒ�p�g�U���J���[
	{
		{ // ���邢�Ƃ��̃��C�g
			MyLib::Color(1.0f, 1.0f, 1.0f, 1.0f),
			MyLib::Color(0.65f, 0.65f, 0.65f, 1.0f),
			MyLib::Color(0.15f, 0.15f, 0.15f, 1.0f),
		},

		{ // �Â��Ƃ��̃��C�g
			MyLib::Color(0.25f, 0.25f, 0.25f, 1.0f),
			MyLib::Color(0.25f, 0.25f, 0.25f, 1.0f),
			MyLib::Color(0.25f, 0.25f, 0.25f, 1.0f),
		}
	};

	const MyLib::Vector3 DIRECTION[] =	// �ݒ�p�����x�N�g��
	{
		MyLib::Vector3(0.22f, -0.87f, 0.44f),
		MyLib::Vector3(-0.18f, 0.88f, -0.44f),
		MyLib::Vector3(0.89f, -0.11f, 0.44f),
	};
}

//************************************************************
//	�X�^�e�B�b�N�A�T�[�g
//************************************************************
static_assert(NUM_ARRAY(DIRECTION) == CLightManager::MAX_NUM, "ERROR : Light Count Mismatch");

//************************************************************
//	�e�N���X [CLightManager] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CLightManager::CLightManager() :
	m_colCamStart	(MyLib::color::White()),	// �J�������C�g�J�n�F
	m_colCamEnd		(MyLib::color::White()),	// �J�������C�g�I���F
	m_fStartTime	(0.0f),	// �J�n����
	m_fCurTime		(0.0f),	// �o�ߎ���
	m_bChange		(false)	// �ϓ��t���O
{
	// �����o�ϐ����N���A
	memset(&m_apLight[0], 0, sizeof(m_apLight));		// ���C�g���
	memset(&m_aColStart[0], 0, sizeof(m_aColStart));	// ���C�g�J�n�F
	memset(&m_aColEnd[0], 0, sizeof(m_aColEnd));		// ���C�g�I���F

	D3DXCOLOR m_colCamStart;		// �J�������C�g�J�n�F
	D3DXCOLOR m_colCamEnd;			// �J�������C�g�I���F
}

//============================================================
//	�f�X�g���N�^
//============================================================
CLightManager::~CLightManager()
{

}

//============================================================
//	����������
//============================================================
HRESULT CLightManager::Init()
{
	for (int i = 0; i < MAX_NUM; i++)
	{ // �g�p���郉�C�g�����J��Ԃ�

		// ���C�g�̐���
		m_apLight[i] = CLightDir::Create();
		if (m_apLight[i] == nullptr)
		{ // �����Ɏ��s�����ꍇ

			assert(false);
			return E_FAIL;
		}

		// ���C�g�̃I�u�W�F�N�g��ނ�ύX
		m_apLight[i]->SetType(CObject::TYPE::TYPE_NONE);	// �����j��/�����X�V���~

		// ���C�g�̊g�U����ݒ�
		m_apLight[i]->SetDiffuse(DIFFUSE[0][i]);

		// ���C�g�̕�����ݒ�
		m_apLight[i]->SetDirection(DIRECTION[i]);
	}

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CLightManager::Uninit()
{
	for (int i = 0; i < MAX_NUM; i++)
	{ // �g�p���郉�C�g�����J��Ԃ�

		// ���C�g�̏I��
		SAFE_UNINIT(m_apLight[i]);
	}
}

//============================================================
//	�X�V����
//============================================================
void CLightManager::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_bChange)
	{
		CCamera* pCamera = GET_MANAGER->GetCamera();	// �J�������

		// �ϓ����Ԃ����Z
		m_fCurTime -= fDeltaTime * fSlowRate;

		// �o�ߏI�������ꍇ�I��
		if (m_fCurTime <= 0.0f) { m_bChange = false; }

		// �J�������C�g�̊g�U����ۑ�
		MyLib::Vector3 vecCamStart	= ConvertColorToVector3(m_colCamStart);
		MyLib::Vector3 vecCamEnd	= ConvertColorToVector3(m_colCamEnd);
		MyLib::Vector3 vecCamCur	= UtilFunc::Correction::EasingQuintIn(vecCamStart, vecCamEnd, m_fStartTime, 0.0f, m_fCurTime);
		D3DXCOLOR	   colCamCur	= ConvertVector3ToColor(vecCamCur, m_colCamEnd.a);
		pCamera->SetLightDiffuse(colCamCur);

		for (int i = 0; i < MAX_NUM; i++)
		{ // �g�p���郉�C�g�����J��Ԃ�

			// ���C�g�̊g�U����ۑ�
			MyLib::Vector3 vecStart	= ConvertColorToVector3(m_aColStart[i]);
			MyLib::Vector3 vecEnd	= ConvertColorToVector3(m_aColEnd[i]);
			MyLib::Vector3 vecCur	= UtilFunc::Correction::EasingQuintIn(vecStart, vecEnd, m_fStartTime, 0.0f, m_fCurTime);
			D3DXCOLOR	   colCur	= ConvertVector3ToColor(vecCur, m_colCamEnd.a);
			m_apLight[i]->SetDiffuse(colCur);
		}
	}

	for (int i = 0; i < MAX_NUM; i++)
	{ // �g�p���郉�C�g�����J��Ԃ�

		// ���C�g�̍X�V
		m_apLight[i]->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//============================================================
//	���邳�̐ݒ菈��
//============================================================
void CLightManager::SetEnableBright(const bool bBright, const float fChangeTime)
{
	CCamera* pCamera = GET_MANAGER->GetCamera();	// �J�������
	int nBright = (bBright) ? 0 : 1;		// ���C�g�z��C���f�b�N�X
	const D3DXCOLOR aEndCol[] =		// �J�������C�g�z��
	{
		pCamera->GetDestLightDiffuse(),		// ���邢�Ƃ��̐F
		MyLib::color::Black(aEndCol[0].a),	// �Â��Ƃ��̐F
	};

	if (fChangeTime > 0.0f)
	{ // �ϓ����Ԃ��ݒ肳��Ă���ꍇ

		// �ϓ����Ԃ�ۑ�
		m_fStartTime = m_fCurTime = fChangeTime;

		for (int i = 0; i < MAX_NUM; i++)
		{ // �g�p���郉�C�g�����J��Ԃ�

			// ���C�g�̊g�U����ۑ�
			m_aColStart[i] = m_apLight[i]->GetLight().Diffuse;
			m_aColEnd[i] = DIFFUSE[nBright][i];
		}

		// �J�������C�g�̊g�U����ۑ�
		m_colCamStart = pCamera->GetLightDiffuse();
		m_colCamEnd = aEndCol[nBright];

		// �ϓ����ɂ���
		m_bChange = true;
	}
	else
	{ // �ϓ����Ԃ����ݒ�̏ꍇ

		// ���Ԃ̏�����
		m_fStartTime = m_fCurTime = 0.0f;

		for (int i = 0; i < MAX_NUM; i++)
		{ // �g�p���郉�C�g�����J��Ԃ�

			// ���C�g�̊g�U����ݒ�
			m_apLight[i]->SetDiffuse(DIFFUSE[nBright][i]);
		}

		// �J�������C�g�̊g�U����ݒ�
		pCamera->SetLightDiffuse(aEndCol[nBright]);

		// �ϓ��I��
		m_bChange = false;
	}
}

//============================================================
//	��������
//============================================================
CLightManager* CLightManager::Create()
{
	// ���C�g�}�l�[�W���[�̐���
	CLightManager* pLightManager = DEBUG_NEW CLightManager;
	if (pLightManager == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return nullptr;
	}
	else
	{ // �����ɐ��������ꍇ

		// ���C�g�}�l�[�W���[�̏�����
		if (FAILED(pLightManager->Init()))
		{ // �������Ɏ��s�����ꍇ

			// ���C�g�}�l�[�W���[�̔j��
			SAFE_DELETE(pLightManager);
			return nullptr;
		}

		// �m�ۂ����A�h���X��Ԃ�
		return pLightManager;
	}
}

//============================================================
//	�j������
//============================================================
void CLightManager::Release(CLightManager*& prLightManager)
{
	// ���C�g�}�l�[�W���[�̏I��
	assert(prLightManager != nullptr);
	prLightManager->Uninit();

	// �������J��
	SAFE_DELETE(prLightManager);
}
