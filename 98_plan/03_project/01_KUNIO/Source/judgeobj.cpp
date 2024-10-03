//=============================================================================
// 
//  ����\�� [judgeobj.cpp]
//  Author : �Ό��D�n
// 
//=============================================================================
#include "judgeobj.h"
#include "judge.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "judgestatefunc.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::vector<std::string> TEXTURE =
	{
		"data\\TEXTURE\\judge\\judge_texture_01.png",
		"data\\TEXTURE\\judge\\judge_texture_02.png",
		"data\\TEXTURE\\judge\\judge_texture_03.png",
		"data\\TEXTURE\\judge\\judge_texture_04.png",
	};
}

//**************************************************************************
// ����I�u�W�F�N�g
//**************************************************************************
//==========================================================================
// �R���X�g���N�^
//==========================================================================
CJudgeObj::CJudgeObj(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	memset(m_RockOnInfo, 0, sizeof(m_RockOnInfo));
	m_pStateFunc = nullptr;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CJudgeObj::~CJudgeObj()
{
	if (m_pStateFunc != nullptr)
	{
		delete m_pStateFunc;
		m_pStateFunc = nullptr;
	}
}

//==========================================================================
// ��������
//==========================================================================
CJudgeObj* CJudgeObj::Create(const MyLib::Vector3& pos, const CJudge::JUDGE judge)
{
	// �������̊m��
	CJudgeObj* pMarker = DEBUG_NEW CJudgeObj;

	if (pMarker != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �ʒu�ݒ�
		pMarker->SetPosition(pos);
		pMarker->SetOriginPosition(pos);
		pMarker->m_Judge = judge;

		// ����������
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CJudgeObj::Init()
{
	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	// �e�N�X�`���ݒ�
	int pointTex = CTexture::GetInstance()->Regist(TEXTURE[m_Judge]);
	BindTexture(pointTex);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(pointTex);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 120.0f);
	SetSize(D3DXVECTOR2(size.x, 0.0f));
	SetSizeOrigin(size);

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// ��Ԋ֐�����
	CreateState();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CJudgeObj::Uninit()
{
	CObject2D::Uninit();
}

//==========================================================================
// �I������
//==========================================================================
void CJudgeObj::Kill()
{
	for (int nCntGauge = 0; nCntGauge < VTXTYPE_MAX; nCntGauge++)
	{
		if (m_RockOnInfo[nCntGauge].p2D != nullptr)
		{
			// �I������
			m_RockOnInfo[nCntGauge].p2D->Uninit();
			m_RockOnInfo[nCntGauge].p2D = nullptr;
		}
	}

	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CJudgeObj::Update()
{
	// ��Ԋ֐�����
	if (m_pStateFunc != nullptr)
	{
		// ��ԃ^�C�}�[���Z
		m_pStateFunc->AddStateTime(CManager::GetInstance()->GetDeltaTime());

		// ��ԕʏ���
		m_pStateFunc->ActionState();
	}
	else
	{//�Ȃ��i�������Ƃ��`�j
		assert(0 && "�������Ƃ��`");
	}

	if (IsDeath())
	{
		return;
	}

	CObject2D::Update();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CJudgeObj::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �I�u�W�F�N�g2D�̕`��
	CObject2D::Draw();

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==========================================================================
// ��Ԋ֐���������
//==========================================================================
void CJudgeObj::CreateState()
{
	if (m_Judge == CJudge::JUDGE::JUDGE_AAA)
	{
		m_pStateFunc = DEBUG_NEW CJudgeStateFunc_AAA(this);
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_AWESOME);
	}
	else if (m_Judge == CJudge::JUDGE::JUDGE_BBB)
	{
		m_pStateFunc = DEBUG_NEW CJudgeStateFunc_BBB(this);
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_GOOD);
	}
	else if (m_Judge == CJudge::JUDGE::JUDGE_CCC)
	{
		m_pStateFunc = DEBUG_NEW CJudgeStateFunc_CCC(this);
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_BAD);
	}
	else if (m_Judge == CJudge::JUDGE::JUDGE_DDD)
	{
		m_pStateFunc = DEBUG_NEW CJudgeStateFunc_DDD(this);
		CSound::GetInstance()->PlaySound(CSound::LABEL_SE_WORST);
	}
}
