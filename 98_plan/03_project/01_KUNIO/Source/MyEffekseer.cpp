//=============================================================================
// 
//  �G�t�F�N�V�A���� [MyEffekseer.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "MyEffekseer.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"
#include "pause.h"
#include <Effekseer.h>
#include "EffekseerObj.h"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
std::string CMyEffekseer::m_EffectName[CMyEffekseer::EFKLABEL_MAX] =	// �G�t�F�N�g�̃t�@�C����
{
	"data/Effekseer/Laser01.efkefc",		// �T���v���̃��[�U�[
	"data/Effekseer/bress.efkefc",			// ��
	"data/Effekseer/sample_river.efkefc",	// ��T���v��
	"data/Effekseer/impact.efkefc",			// �Ռ�
	"data/Effekseer/spraywater.efkefc",		// �Ռ�
	"data/Effekseer/spraywater_mini.efkefc",// �Ռ�
	"data/Effekseer/waterjump.efkefc",		// ������
	"data/Effekseer/playermoveLine.efkefc",	// �v���C���[�̈ړ���
	"data/Effekseer/photon.efkefc",			// ���S
	"data/Effekseer/ItemGet.efkefc",		// �ו��l��
	"data/Effekseer/air.efkefc",			// ��C
	"data/Effekseer/crowd.efkefc",			// �Q�O
	"data/Effekseer/teleport.efkefc",		// �e���|�[�g
	"data/Effekseer/hit.efkefc",			// �q�b�g
	"data/Effekseer/wing.efkefc",			// �H�΂���
	"data/Effekseer/smash.efkefc",			// �ׂ��
	"data/Effekseer/aura.efkefc",			// �I�[��
};
CMyEffekseer* CMyEffekseer::m_pMyEffekseer = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMyEffekseer::CMyEffekseer()
{
	// �ϐ��̃N���A
	time = 0;
	efkHandle = 0;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMyEffekseer::~CMyEffekseer()
{

}

//==========================================================================
// ��������
//==========================================================================
CMyEffekseer* CMyEffekseer::Create()
{
	if (m_pMyEffekseer == nullptr)
	{// �܂��������Ă��Ȃ�������

		// �C���X�^���X����
		m_pMyEffekseer = DEBUG_NEW CMyEffekseer;

		// ����������
		m_pMyEffekseer->Init();
	}
	else
	{
		// �C���X�^���X�擾
		m_pMyEffekseer->GetInstance();
	}

	return m_pMyEffekseer;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMyEffekseer::Init()
{
	time = 0;
	efkHandle = 0;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// Effekseer�̃I�u�W�F�N�g�̓X�}�[�g�|�C���^�ŊǗ������B�ϐ����Ȃ��Ȃ�Ǝ����I�ɍ폜�����B

	// �G�t�F�N�g�̃}�l�[�W���[�̍쐬
	m_efkManager = ::Effekseer::Manager::Create(12800);

	// Effekseer�̃��W���[�����Z�b�g�A�b�v����
	SetupEffekseerModules(m_efkManager);
	auto efkRenderer = GetEffekseerRenderer();

	// ���W�n��ݒ肷��B�A�v���P�[�V�����ƈ�v������K�v������B
	m_efkManager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	// ���_�ʒu���m��
	viewerPosition = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

	// ���e�s���ݒ�
	projectionMatrix.PerspectiveFovRH(90.0f / 180.0f * 3.14f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 1.0f, 500.0f);

	// �J�����s���ݒ�
	cameraMatrix.LookAtRH(viewerPosition, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f));

	return S_OK;
}

//==========================================================================
// �ǂݍ��ݏ���
//==========================================================================
Effekseer::EffectRef CMyEffekseer::LoadEffect(std::string efkpath)
{
	// char16_t�ɕϊ�
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	std::u16string string16t = converter.from_bytes(efkpath);

	return LoadProcess(string16t);
}

//==========================================================================
// �G�t�F�N�g�ǂݍ���
//==========================================================================
Effekseer::EffectRef CMyEffekseer::LoadEffect(EFKLABEL label)
{
	// char16_t�ɕϊ�
	std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
	std::u16string string16t = converter.from_bytes(m_EffectName[label]);

	return LoadProcess(string16t);
}

//==========================================================================
// �ǂݍ��ݏ���
//==========================================================================
Effekseer::EffectRef CMyEffekseer::LoadProcess(const std::u16string& efkpath)
{
	// �G�t�F�N�g�̓Ǎ�
	auto effect = Effekseer::Effect::Create(m_efkManager, efkpath.c_str());

	onLostDevice = [effect]() -> void
	{
		// �ǂݍ��񂾃G�t�F�N�g�̃��\�[�X�͑S�Ĕj������B
		if (effect != nullptr)
		{
			effect->UnloadResources();
		}
	};

	onResetDevice = [effect]() -> void
	{
		// �G�t�F�N�g�̃��\�[�X���ēǂݍ��݂���B
		if (effect != nullptr)
		{
			effect->ReloadResources();
		}
	};

	return effect;
}

//==========================================================================
// �I������
//==========================================================================
void CMyEffekseer::Uninit()
{
	// �S��~
	m_efkManager->StopAllEffects();

	// �擪��ۑ�
	CListManager<CEffekseerObj> list = CEffekseerObj::GetListObj();
	std::list<CEffekseerObj*>::iterator itr = list.GetEnd();
	CEffekseerObj* pObj = nullptr;

	std::vector<CEffekseerObj*> deletelist;

	// ���X�g���[�v
	while (list.ListLoop(itr))
	{
		deletelist.push_back((*itr));
	}

	for (const auto& obj : deletelist)
	{
		obj->Uninit();
	}

	delete m_pMyEffekseer;
	m_pMyEffekseer = nullptr;
}

//==========================================================================
// �S�Ē�~
//==========================================================================
void CMyEffekseer::StopAll()
{
	// �S�Ē�~
	m_efkManager->StopAllEffects();

	
}

//==========================================================================
// �X�V����
//==========================================================================
void CMyEffekseer::Update()
{
	bool bPause = CManager::GetInstance()->GetPause()->IsPause();
	if (!bPause)
	{
		// �S�X�V
		UpdateAll();

		// ���C���[�p�����[�^�̐ݒ�
		Effekseer::Manager::LayerParameter layerParameter;
		::Effekseer::Matrix44 invEfkCameraMatrix;
		::Effekseer::Matrix44::Inverse(invEfkCameraMatrix, cameraMatrix);
		layerParameter.ViewerPosition = ::Effekseer::Vector3D(invEfkCameraMatrix.Values[3][0], invEfkCameraMatrix.Values[3][1], invEfkCameraMatrix.Values[3][2]);
		m_efkManager->SetLayerParameter(0, layerParameter);

		// �}�l�[�W���[�̍X�V
		Effekseer::Manager::UpdateParameter updateParameter;
		m_efkManager->Update(updateParameter);
	}

	// �`�揈��
	Draw();

	if (!bPause)
	{
		time++;
	}
}

//==========================================================================
// �S�X�V
//==========================================================================
void CMyEffekseer::UpdateAll()
{
	// ��Q���̃��X�g�擾
	CListManager<CEffekseerObj> list = CEffekseerObj::GetListObj();

	// �擪��ۑ�
	std::list<CEffekseerObj*>::iterator itr = list.GetEnd();
	CEffekseerObj* pObj = nullptr;

	// ���X�g���[�v
	while (list.ListLoop(itr))
	{
		(*itr)->Update();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CMyEffekseer::Draw()
{
	// ���Ԃ��X�V����
	efkRenderer->SetTime(time / 60.0f);

	// ���e�s���ݒ�
	efkRenderer->SetProjectionMatrix(projectionMatrix);

	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	D3DXMATRIX appProjectionMatrix = pCamera->GetMtxProjection();

	// Effekseer��CameraMatrix�́A�J�����̎p���s��̋t�s����w���B�J�����s�񂪃J�����̎p���s��ł���ꍇ�́A�t�s�񉻂��Ă����B
	auto invAppCameraMatrix = pCamera->GetMtxView();

	// �����̊���Effekseer�́A���e(�ˉe)�s��ƃJ�����s��𓯊�
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			projectionMatrix.Values[i][j] = appProjectionMatrix.m[i][j];
			cameraMatrix.Values[i][j] = invAppCameraMatrix.m[i][j];
		}
	}

	// �J�����s���ݒ�
	efkRenderer->SetCameraMatrix(cameraMatrix);

	// �G�t�F�N�g�̕`��J�n�������s���B
	efkRenderer->BeginRendering();

	// �G�t�F�N�g�̕`����s���B
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 1.0f;
	drawParameter.ViewProjectionMatrix = efkRenderer->GetCameraProjectionMatrix();
	m_efkManager->Draw(drawParameter);

	// �G�t�F�N�g�̕`��I������
	efkRenderer->EndRendering();
}

//==========================================================================
// ���W���[���̃Z�b�g�A�b�v
//==========================================================================
void CMyEffekseer::SetupEffekseerModules(::Effekseer::ManagerRef m_efkManager)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �`��f�o�C�X�̍쐬
	auto graphicsDevice = ::EffekseerRendererDX9::CreateGraphicsDevice(pDevice);

	// �G�t�F�N�g�̃����_���[�̍쐬
	efkRenderer = ::EffekseerRendererDX9::Renderer::Create(graphicsDevice, 8000);

	// �`�惂�W���[���̐ݒ�
	m_efkManager->SetSpriteRenderer(efkRenderer->CreateSpriteRenderer());
	m_efkManager->SetRibbonRenderer(efkRenderer->CreateRibbonRenderer());
	m_efkManager->SetRingRenderer(efkRenderer->CreateRingRenderer());
	m_efkManager->SetTrackRenderer(efkRenderer->CreateTrackRenderer());
	m_efkManager->SetModelRenderer(efkRenderer->CreateModelRenderer());

	// �e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	m_efkManager->SetTextureLoader(efkRenderer->CreateTextureLoader());
	m_efkManager->SetModelLoader(efkRenderer->CreateModelLoader());
	m_efkManager->SetMaterialLoader(efkRenderer->CreateMaterialLoader());
	m_efkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}
