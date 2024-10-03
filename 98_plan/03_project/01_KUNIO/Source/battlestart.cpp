//=============================================================================
// 
//  �퓬�J�n���� [battlestart.cpp]
//  Author : ���n�Ή�, �Ό��D�n
// 
//=============================================================================
#include "battlestart.h"
#include "battlestart_effect.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "calculation.h"
#include "particle.h"
#include "game.h"
#include "camera.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string SWORD_TEXTURE = "data\\TEXTURE\\battlestart\\sword.png";				// �e�N�X�`���̃t�@�C��
	const std::string SWORD_TEXTURE_COMPLETE = "data\\TEXTURE\\battlestart\\sword_complete.png";				// �e�N�X�`���̃t�@�C��
	const std::string TEXT_TEXTURE_COMPLETE = "data\\TEXTURE\\battlestart\\don2.png";				// �e�N�X�`���̃t�@�C��
	const std::string TEXT_TEXTURE = "data\\TEXTURE\\battlestart\\don.png";	// �e�N�X�`���̃t�@�C��
	const float TIME_EXPANSION = 1.0f;	// �g�厞��
	const float TIME_WAIT = 0.4f;		// �҂�����
	const float TIME_DROP = 0.5f;		// ���Ƃ�����
	const float TIME_FADEOUT = 0.6f;	// �t�F�[�h�A�E�g����
	const MyLib::Vector3 DESTROTATION_GETTOGETHER = MyLib::Vector3(0.0f, 0.0f, D3DX_PI * 0.75f);
	const MyLib::Vector3 DESTPOSITION_GETTOGETHER = MyLib::Vector3(130.0f, 230.0f, 0.0f);
	const MyLib::Vector3 DESTPOSITION_STINGS = MyLib::Vector3(0.0f, 360.0f, 0.0f);
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CBattleStart::STATE_FUNC CBattleStart::m_StateFuncList[] =
{
	&CBattleStart::StateExpansion,		//�g��
	&CBattleStart::StateWait,			//�҂�
	&CBattleStart::StateDrop,			//���Ƃ�
	&CBattleStart::StateFadeOut,		//�t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBattleStart::CBattleStart(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_state = STATE::STATE_EXPANTION;		// ���
	m_fStateTime = 0.0f;	// ��ԃ^�C�}�[
	memset(m_RockOnInfo, 0, sizeof(m_RockOnInfo));
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBattleStart::~CBattleStart()
{

}

//==========================================================================
// ��������
//==========================================================================
CBattleStart* CBattleStart::Create(const MyLib::Vector3& pos)
{
	// �������̊m��
	CBattleStart* pMarker = DEBUG_NEW CBattleStart;

	if (pMarker != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// �ʒu�ݒ�
		pMarker->SetPosition(pos);
		pMarker->SetOriginPosition(pos);

		// ����������
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CBattleStart::Init()
{

	CTexture::GetInstance()->Regist(TEXT_TEXTURE);
	CTexture::GetInstance()->Regist(TEXT_TEXTURE_COMPLETE);

	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();
	int pointTex = CTexture::GetInstance()->Regist(TEXT_TEXTURE);
	BindTexture(pointTex);
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(pointTex);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
	SetSize(D3DXVECTOR2(size.x, 0.0f));
	SetSizeOrigin(size);

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	int nTexIdx = CTexture::GetInstance()->Regist(SWORD_TEXTURE);

	size = CTexture::GetInstance()->GetImageSize(nTexIdx);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 120.0f);

	MyLib::Vector3 pos = GetPosition();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CBattleStart::Uninit()
{
	CObject2D::Uninit();
}

//==========================================================================
// �I������
//==========================================================================
void CBattleStart::Kill()
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
void CBattleStart::Update()
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// ��ԕʏ���
	(this->*(m_StateFuncList[m_state]))();

	if (IsDeath())
	{
		return;
	}

	CObject2D::Update();
}

//==========================================================================
// �g��
//==========================================================================
void CBattleStart::StateExpansion()
{
	if (m_fStateTime >= TIME_EXPANSION)
	{
		m_fStateTime = 0.0f;
		m_state = STATE_WAIT;
		//CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_BATTLESTART_CHARGE);
		return;
	}

	float ratio = m_fStateTime / TIME_EXPANSION;

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = GetSize();
	size.x = UtilFunc::Correction::EasingEaseIn(0.0f, GetSizeOrigin().x * 1.6f, ratio);
	size.y = UtilFunc::Correction::EasingEaseIn(0.0f, GetSizeOrigin().y * 1.6f, ratio);
	SetSize(size);

}

//==========================================================================
// ���Ƃ�
//==========================================================================
void CBattleStart::StateWait()
{
	if (m_fStateTime >= TIME_WAIT)
	{
		m_fStateTime = 0.0f;
		m_state = STATE_DROP;
	}
}

//==========================================================================
// ���Ƃ�
//==========================================================================
void CBattleStart::StateDrop()
{
	if (m_fStateTime >= TIME_DROP)
	{
		m_fStateTime = 0.0f;
		m_state = STATE_FADEOUT;
		//CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_BATTLESTART_CHARGE);

		SetSize(GetSizeOrigin());

		// ������̃e�N�X�`���ɐؑ�
		int nTexIdx = CTexture::GetInstance()->Regist(TEXT_TEXTURE_COMPLETE);
		BindTexture(nTexIdx);
 
		// �h����p�[�e�B�N������
		my_particle::Create(GetPosition(), my_particle::TYPE::TYPE_BATTLESTART);

		// �����G�t�F�N�g����
		CBattleStart_Effect::Create(GetPosition());
		return;
	}

	float ratio = m_fStateTime / TIME_DROP;

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = GetSize();
	size.x = UtilFunc::Correction::EasingEaseIn(GetSizeOrigin().x * 1.6f, GetSizeOrigin().x, ratio);
	size.y = UtilFunc::Correction::EasingEaseIn(GetSizeOrigin().y * 1.6f, GetSizeOrigin().y, ratio);
	SetSize(size);
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CBattleStart::StateFadeOut()
{
	if (m_fStateTime >= TIME_FADEOUT)
	{
		m_fStateTime = 0.0f;
		Uninit();

		// �퓬�J�n�ɑJ��
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_MAIN);
		return;
	}
	float ratio = 1.0f - m_fStateTime / TIME_FADEOUT;

	// �s�����x�ݒ�
	SetAlpha(ratio);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CBattleStart::Draw()
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
