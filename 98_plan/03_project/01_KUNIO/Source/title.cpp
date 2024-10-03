//=============================================================================
// 
//  �^�C�g������ [title.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "sound.h"
#include "particle.h"
#include "MyEffekseer.h"
#include "titlelogo.h"
#include "title_pressenter.h"
#include "camera.h"
#include "waterfield.h"
#include "course.h"
#include "mapmesh.h"
#include "stonewall.h"
#include "peoplemanager.h"
#include "environment.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float TIME_FADELOGO = 0.6f;	// ���S�̃t�F�[�h�A�E�g����
	const char* TEXTURE = "data\\TEXTURE\\title\\title.png";
}

namespace STARTCAMERA
{
	const MyLib::Vector3 POSV = (321.91f, 233.25f, -654.0f);
	const MyLib::Vector3 POSR = (321.91f, 160.74f, 296.27f);
	const MyLib::Vector3 ROT = (0.0f, 0.0f, -0.2f);
	const float LENGTH = 365.0f;
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CTitle* CTitle::m_pThisPtr = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �֐��|�C���^
//==========================================================================
CTitle::SCENE_FUNC CTitle::m_SceneFunc[] =
{
	&CTitle::SceneNone,			// �Ȃɂ��Ȃ�
	&CTitle::SceneFadeInLogo,	// ���S�t�F�[�h�C��
	&CTitle::SceneFadeOutLoGo,	// ���S�t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTitle::CTitle()
{
	// �l�̃N���A
	m_SceneType = SCENETYPE::SCENETYPE_NONE;	// �V�[���̎��
	m_fSceneTime = 0.0f;						// �V�[���J�E���^�[
	m_pLogo = nullptr;		// ���S�̃|�C���^
	m_pPressEnter = nullptr;	// �v���X�G���^�[
	m_pConfigSetting = nullptr;
	m_pSpawn_Leaf = nullptr;		// �~��t����
	m_pPeopleManager = nullptr;	// �l�}�l�[�W��
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTitle::~CTitle()
{

}

//==========================================================================
// ��������
//==========================================================================
CTitle* CTitle::Create()
{
	if (m_pThisPtr == nullptr)
	{// �܂��������Ă��Ȃ�������

		// �C���X�^���X����
		m_pThisPtr = DEBUG_NEW CTitle;
	}

	return m_pThisPtr;
}

//==========================================================================
// �C���X�^���X�擾
//==========================================================================
CTitle* CTitle::GetInstance()
{
	return m_pThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTitle::Init()
{

	// BGM�Đ�
	CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_TITLE);

	// ����������
	if (FAILED(CScene::Init()))
	{// ���s�����ꍇ
		return E_FAIL;
	}

	// �^�C�g�����S����
	m_pLogo = CTitleLogo::Create(1.0f);

	// �v���X�G���^�[
	m_pPressEnter = CTitle_PressEnter::Create(1.0f);

	// �V�[���̎��
	m_SceneType = SCENETYPE::SCENETYPE_NONE;

	// �J�����̏����l�ݒ�
	{
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		pCamera->SetStateCameraV(DEBUG_NEW CStateCameraV);
	}


	//=============================
	// �R�[�X
	//=============================
	CCourse::Create("", CScene::MODE::MODE_TITLE);

	//=============================
	// �Œ蕽�ʊX�t�B�[���h
	//=============================
	CMapMesh::Create(CMapMesh::MeshType::TYPE_TOWNFIELD_FIXEDPLANE);

	//=============================
	// �Ί_(��)
	//=============================
	{
		CStoneWall* pStoneWall = CStoneWall::Create();

		// ��_�n�_�ݒ�
		std::vector<MyLib::Vector3> vecpos =
		{
			MyLib::Vector3(-1500.0f, 0.0f, 3000.0f),
			MyLib::Vector3(0.0f, 0.0f, 3000.0f),
			MyLib::Vector3(500.0f, 0.0f, 3000.0f),
			MyLib::Vector3(2500.0f, 0.0f, 3000.0f),
		};
		pStoneWall->SetVecPosition(vecpos);
		pStoneWall->Reset();

		// �e���_���W
		std::vector<MyLib::Vector3> vecVtxpos =
		{
			MyLib::Vector3(20000.0f, 0.0f, 2000.0f),
			MyLib::Vector3(20010.0f, 0.0f, 2000.0f),
			MyLib::Vector3(90000.0f, 0.0f, 2000.0f),
			MyLib::Vector3(90010.0f, 0.0f, 2000.0f),
		};
		pStoneWall->SetVecVtxPosition(vecVtxpos);
		pStoneWall->BindVtxPosition();
	}

	//=============================
	// ���t�B�[���h
	//=============================
	CWaterField::Create(CWaterField::TYPE::TYPE_RIGHT);
	CWaterField::Create(CWaterField::TYPE::TYPE_LEFT);

	//=============================
	// �l�}�l�[�W��
	//=============================
	m_pPeopleManager = CPeopleManager::Create(CPeopleManager::Type::TYPE_TITLE);


	// �~��t�����N���X����
	m_pSpawn_Leaf = DEBUG_NEW CSpawn_Leaf_Title(0.0f, 0.6f);

	// ����
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTitle::Uninit()
{
	m_pThisPtr = nullptr;

	// �~��t�����N���X
	if (m_pSpawn_Leaf != nullptr)
	{
		delete m_pSpawn_Leaf;
		m_pSpawn_Leaf = nullptr;
	}

	// �l�}�l�[�W��
	if (m_pPeopleManager != nullptr)
	{
		m_pPeopleManager->Uninit();
		m_pPeopleManager = nullptr;
	}

	// �I������
	CScene::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTitle::Update()
{
	// �f���^�^�C���擾
	float deltaTime = CManager::GetInstance()->GetDeltaTime();

	// �~��t�����̍X�V
	if (m_pSpawn_Leaf != nullptr)
	{
		m_pSpawn_Leaf->Update(deltaTime);
	}

	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// �t�F�[�h���͔�����
		return;
	}

	// ��ԕʍX�V����
	(this->*(m_SceneFunc[m_SceneType]))();
}

//==========================================================================
// �Ȃɂ��Ȃ�
//==========================================================================
void CTitle::SceneNone()
{
	
}

//==========================================================================
// ���S�t�F�[�h�C��
//==========================================================================
void CTitle::SceneFadeInLogo()
{

}

//==========================================================================
// ���S�t�F�[�h�A�E�g
//==========================================================================
void CTitle::SceneFadeOutLoGo()
{
	// �V�[���J�E���^�[���Z
	m_fSceneTime -= CManager::GetInstance()->GetDeltaTime();

	// �s�����x�X�V
	float alpha = m_fSceneTime / TIME_FADELOGO;

	// �G���^�[�̐F
	m_pPressEnter->SetAlpha(alpha);

	if (m_fSceneTime <= 0.0f)
	{
		m_fSceneTime = 0.0f;
		m_SceneType = SCENETYPE_NONE;

		// �s�����x�X�V
		m_pLogo->Uninit();
		m_pLogo = nullptr;

		// �G���^�[�̐F
		m_pPressEnter->SetAlpha(1.0f);
		m_pPressEnter->Uninit();
		m_pPressEnter = nullptr;
		return;
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CTitle::Draw()
{

}
