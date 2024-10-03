//=============================================================================
// 
//  ���U���g���� [result.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "result.h"
#include "resultManager.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "player.h"
#include "sound.h"
#include "game.h"

#include "result_people.h"
#include "mapmesh.h"
#include "peoplemanager.h"
#include "blackframe.h"
#include "camera.h"

//=============================================================================
// �萔��`
//=============================================================================
namespace
{
	const D3DXCOLOR WIN_COLOR = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
	const D3DXCOLOR LOSE_COLOR = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	const float START_LENGTH = 300.0f; // ��������
	const float END_LENGTH = 2000.0f; // �ڕW����

	//���f���̔z�u�ʒu
	const MyLib::Vector3 CENTER = MyLib::Vector3(0.0f, 0.0f, -2000.0f);
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CResultScore *CResult::m_pResultScore = nullptr;	// ���U���g�X�N���[���̃I�u�W�F�N�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CResult::CResult() : m_clear(false)
{
	// �l�̃N���A
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CResult::~CResult()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CResult::Init()
{
	//�v���C���[�������Z�b�g
	CManager::GetInstance()->SetNumPlayer(0);

	// ����������
	if (FAILED(CScene::Init()))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	// BGM�Đ�
	CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_RESULT);

	//=============================
	// �Œ蕽�ʊX�t�B�[���h
	//=============================
	CMapMesh::Create(CMapMesh::MeshType::TYPE_TOWNFIELD_FIXEDPLANE_RESULT);

	//=============================
	// �l�}�l�[�W��
	//=============================
	m_pPeopleManager = CPeopleManager::Create(CPeopleManager::Type::TYPE_RESULT);


	// ���t���[���C��
	CBlackFrame::GetInstance()->SetState(CBlackFrame::STATE::STATE_INCOMPLETION);

	// �J�������[�V�����Đ�
	CManager::GetInstance()->GetCamera()->GetCameraMotion()->SetMotion(CCameraMotion::MOTION::MOTION_RESULT, CCameraMotion::EASING::Linear);

	// ���U���g�̐l����
	CResultPeople::Create(MyLib::Vector3(76325.0f, 300.0f, 3060.0f));

	// ����
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CResult::Uninit()
{
	m_pResultScore = nullptr;

	// �l�}�l�[�W��
	if (m_pPeopleManager != nullptr)
	{
		m_pPeopleManager->Uninit();
		m_pPeopleManager = nullptr;
	}

	// ���U���g�}�l�[�W���̃��Z�b�g
	CResultManager* pResultManager = CResultManager::GetInstance();
	pResultManager->Reset();

	// �I������
	CScene::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CResult::Update()
{
	// �V�[���̍X�V����
	CScene::Update();

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	// �Q�[���p�b�h���擾
	CInputGamepad *pInputGamepad = CInputGamepad::GetInstance();

	// ���U���g���
	CResultManager* pResultManager = CResultManager::GetInstance();
	pResultManager->Update();

	if (pResultManager->GetState() != CResultManager::State::STATE_PRESSENTER) return;

	// ��ʑJ��
	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{
		// ���[�h�ݒ�
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RANKING);
	}

}

//==========================================================================
// �`�揈��
//==========================================================================
void CResult::Draw()
{

}

//==========================================================================
// ���U���g�X�N���[���̏��擾
//==========================================================================
CResultScore *CResult::GetResultScore()
{
	return m_pResultScore;
}

