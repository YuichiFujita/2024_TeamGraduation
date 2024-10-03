//=============================================================================
// 
//  �X�R�A���� [resultscore.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "resultscore.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "gamemanager.h"
#include "game.h"
#include "game_pressenter.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE_NUMBER = "data\\TEXTURE\\number\\number_blackclover_01.png";		// �����̃e�N�X�`��
	const char* TEXTURE_TEXT = "data\\TEXTURE\\resultscore\\resultscore.png";		// �����̃e�N�X�`��
	const D3DXVECTOR2 SCORESIZE = D3DXVECTOR2(60.0f, 60.0f);
	const float TIME_SET = 0.2f;
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CResultScore::CResultScore(int nPriority) : CObject(nPriority)
{
	m_fStateTime = 0.0f;
	m_pScore = nullptr;
	m_pObj2D = nullptr;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CResultScore::~CResultScore()
{

}

//==========================================================================
// ��������
//==========================================================================
CResultScore* CResultScore::Create(int score)
{
	// �������̊m��
	CResultScore* pMarker = DEBUG_NEW CResultScore;

	if (pMarker != nullptr)
	{
		pMarker->m_nScore = score;

		// ����������
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CResultScore::Init()
{
	// ��������
	CreateScore();		// ��_���[�W����
	
	m_pObj2D = CObject2D::Create(GetPriority());
	m_pObj2D->SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_TEXT);
	m_pObj2D->BindTexture(nIdxTex);

	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(nIdxTex);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 640.0f);
	m_pObj2D->SetSize(size * 10.0f);
	m_pObj2D->SetSizeOrigin(size);

	m_pObj2D->SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));



	CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CResultScore::Uninit()
{
	if (m_pScore != nullptr)
	{
		m_pScore->Uninit();
		m_pScore = nullptr;
	}

	Release();
}

//==========================================================================
// �폜
//==========================================================================
void CResultScore::Kill()
{
	if (m_pScore != nullptr)
	{
		m_pScore->Kill();
		m_pScore = nullptr;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CResultScore::Update()
{
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_RESULT &&
		m_fStateTime >= TIME_SET * 1.5f)
	{
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_RESULT);
		CGame_PressEnter::Create(0.5f);
	}

	D3DXVECTOR2 size = m_pScore->GetSize();
	size.x = UtilFunc::Correction::EasingEaseInOut(SCORESIZE.x * 10.0f, SCORESIZE.x, 0.0f, TIME_SET, m_fStateTime);
	size.y = UtilFunc::Correction::EasingEaseInOut(SCORESIZE.y * 10.0f, SCORESIZE.y, 0.0f, TIME_SET, m_fStateTime);
	m_pScore->SetSize(size);

	D3DXVECTOR2 sizetext = m_pObj2D->GetSize();
	D3DXVECTOR2 sizetextOR = m_pObj2D->GetSizeOrigin();
	sizetext.x = UtilFunc::Correction::EasingEaseInOut(sizetextOR.x * 10.0f, sizetextOR.x, 0.0f, TIME_SET, m_fStateTime);
	sizetext.y = UtilFunc::Correction::EasingEaseInOut(sizetextOR.y * 10.0f, sizetextOR.y, 0.0f, TIME_SET, m_fStateTime);
	m_pObj2D->SetSize(sizetext);

	m_pScore->SetValue(m_nScore);
	m_pScore->Update();
}

//==========================================================================
// ���S�񐔐���
//==========================================================================
void CResultScore::CreateScore()
{
	// ����̕]�����擾
	m_pScore = CMultiNumber::Create(
		MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f),
		SCORESIZE,
		5,
		CNumber::EObjectType::OBJECTTYPE_2D,
		TEXTURE_NUMBER,
		true, GetPriority());
	if (m_pScore == nullptr)
	{
		return;
	}

	// �E�񂹂ɐݒ�
	m_pScore->SetAlignmentType(CMultiNumber::AlignmentType::ALIGNMENT_RIGHT);

	m_pScore->SetValue(m_nScore);
	m_pScore->SetSize(SCORESIZE * 10.0f);

	// �F�ݒ�
	m_pScore->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//==========================================================================
// �`�揈��
//==========================================================================
void CResultScore::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �I�u�W�F�N�g2D�̕`��
	m_pScore->Draw();

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}
