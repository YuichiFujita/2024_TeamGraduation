//=============================================================================
// 
//  ���b�N�I���}�[�J�[���� [rockon_marker.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "rockon_marker.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "camera.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\rockon\\rockon_marker2.png";	// �e�N�X�`���̃t�@�C��
	const char* POINT_TEXTURE = "data\\TEXTURE\\rockon\\rockon_point2.png";	// �e�N�X�`���̃t�@�C��
	const float DIV_ANGLE = (D3DX_PI * 2.0f) * 0.25f;	// 1����������̌���
	const float ORIGIN_DISTANCE = 35.0f;
	const float TIME_SCALE = 5.0f;	// �g�k�̎���
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CRockOnMarker::CRockOnMarker(int nPriority) : CObjectBillboard(nPriority)
{
	// �l�̃N���A
	memset(m_RockOnInfo, 0, sizeof(m_RockOnInfo));
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CRockOnMarker::~CRockOnMarker()
{

}

//==========================================================================
// ��������
//==========================================================================
CRockOnMarker* CRockOnMarker::Create(const MyLib::Vector3& pos)
{
	// �������̊m��
	CRockOnMarker* pMarker = DEBUG_NEW CRockOnMarker;

	if (pMarker != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �ʒu�ݒ�
		pMarker->SetPosition(pos);

		// ����������
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CRockOnMarker::Init()
{
	// �r���{�[�h�̏�����
	CObjectBillboard::Init();
	int pointTex = CTexture::GetInstance()->Regist(POINT_TEXTURE);
	BindTexture(pointTex);
	SetSize(D3DXVECTOR2(30.0f, 30.0f));

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECTBILLBOARD);

	int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE);

	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nTexIdx);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 30.0f);

	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		// ��������
		m_RockOnInfo[nCntGauge].pBillboard = CObjectBillboard::Create();
		if (m_RockOnInfo[nCntGauge].pBillboard == nullptr)
		{
			return E_FAIL;
		}

		CObjectBillboard* pObj = m_RockOnInfo[nCntGauge].pBillboard;

		// �e�N�X�`�����蓖��
		pObj->BindTexture(nTexIdx);

		// ��ނ̐ݒ�
		pObj->SetType(CObject::TYPE_NONE);

		// ���ݒ�
		pObj->SetSize(size);

		float rot = D3DX_PI * 0.25f + nCntGauge * DIV_ANGLE;
		UtilFunc::Transformation::RotNormalize(rot);

		pObj->SetRotation(rot);
		pObj->SetOriginPosition(GetPosition());

		m_RockOnInfo[nCntGauge].Angle = 1;
		m_RockOnInfo[nCntGauge].HeightAngle = 1;
		if (nCntGauge >= VTXTYPE_MAX / 2)
		{
			m_RockOnInfo[nCntGauge].Angle = -1;
		}

		if (nCntGauge == VTXTYPE_RIGHTDW ||
			nCntGauge == VTXTYPE_LEFTDW)
		{
			m_RockOnInfo[nCntGauge].HeightAngle = -1;
		}
	}


	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CRockOnMarker::Uninit()
{
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		m_RockOnInfo[nCntGauge].pBillboard->Uninit();
		m_RockOnInfo[nCntGauge].pBillboard = nullptr;
	}

	CObjectBillboard::Uninit();
}

//==========================================================================
// �I������
//==========================================================================
void CRockOnMarker::Kill()
{
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_RockOnInfo[nCntGauge].pBillboard != nullptr)
		{
			// �I������
			m_RockOnInfo[nCntGauge].pBillboard->Uninit();
			m_RockOnInfo[nCntGauge].pBillboard = nullptr;
		}
	}

	CObjectBillboard::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CRockOnMarker::Update()
{
	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// �J�����̌����擾
	MyLib::Vector3 camerarot = CManager::GetInstance()->GetCamera()->GetRotation();

	float deltaTime = CManager::GetInstance()->GetDeltaTime();

	for (auto& info : m_RockOnInfo)
	{
		info.distancetimer += deltaTime;

		// �g�k
		info.distance = ORIGIN_DISTANCE + fabsf(sinf(D3DX_PI * (info.distancetimer / TIME_SCALE))) * 10.0f;

		MyLib::Vector3 setpos = GetPosition();

		setpos = MyLib::Vector3(
			pos.x + sinf(D3DX_PI * 0.5f + camerarot.y) * (info.distance * info.Angle),
			pos.y + info.distance * info.HeightAngle,
			pos.z + cosf(D3DX_PI * 0.5f + camerarot.y) * (info.distance * info.Angle));	// �ʒu

		info.pBillboard->SetPosition(setpos);
		info.pBillboard->Update();
	}

	CObjectBillboard::Update();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CRockOnMarker::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �r���{�[�h�̕`��
	CObjectBillboard::Draw();

	// �`�揈��
	for (const auto& info : m_RockOnInfo)
	{
		info.pBillboard->Draw();
	}

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
