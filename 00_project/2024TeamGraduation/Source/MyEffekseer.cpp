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
	"data/Effekseer/Laser01.efkefc",					// �T���v���̃��[�U�[
	"data/Effekseer/ball.efkefc",						// �{�[��
	"data/Effekseer/throwLine_normal.efkefc",			// ���������̐�(�ʏ�)
	"data/Effekseer/throwLine_fast.efkefc",				// ���������̐�(����)
	"data/Effekseer/throwLine_pass.efkefc",				// ���������̐�(�p�X)
	"data/Effekseer/kamehame_chargeStart.efkefc",		// ���߂͂ߔg(�J�n)
	"data/Effekseer/kamehame_aura.efkefc",				// ���߂͂ߔg(�`���[�W)
	"data/Effekseer/kamehame_chargeWind.efkefc",		// ���߂͂ߔg(�`���[�W)(��)
	"data/Effekseer/kamehame_chargeThunder.efkefc",		// ���߂͂ߔg(�`���[�W)(��)
	"data/Effekseer/kamehame_atmosphere.efkefc",		// ���߂͂ߔg(��ԃI�[��)
	"data/Effekseer/kamehame_brust.efkefc",				// ���߂͂ߔg(����)
	"data/Effekseer/kamehame_Flash.efkefc",				// ���߂͂ߔg(�t���b�V��)
	"data/Effekseer/kamehame_ballast.efkefc",			// ���߂͂ߔg(��Ԃ��ꂫ)
	"data/Effekseer/hit.efkefc",						// �q�b�g
	"data/Effekseer/bound.efkefc",						// �o�E���h
	"data/Effekseer/sweat.efkefc",						// ��
	"data/Effekseer/walk.efkefc",						// ����
	"data/Effekseer/run.efkefc",						// ����
	"data/Effekseer/blink.efkefc",						// �u�����N
	"data/Effekseer/jump.efkefc",						// �W�����v
	"data/Effekseer/land.efkefc",						// ���n
	"data/Effekseer/cantch_stance.efkefc",				// �L���b�`(�\��)
	"data/Effekseer/cantch_normal.efkefc",				// �L���b�`(�ʏ�)
	"data/Effekseer/cantch_just.efkefc",				// �L���b�`(�W���X�g)
	"data/Effekseer/confetti.efkefc",					// ������
	"data/Effekseer/crown.efkefc",						// ����
	"data/Effekseer/logo.efkefc",						// ���S
	"data/Effekseer/logo_always.efkefc",				// ���S(��ɏo��)
	"data/Effekseer/sandsmoke.efkefc",					// ����
};
CMyEffekseer* CMyEffekseer::m_pMyEffekseer = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMyEffekseer::CMyEffekseer()
{
	// �ϐ��̃N���A
	fTime = 0.0f;
	efkHandle = 0;
	m_vecLoadInfo.clear();	// �ǂݍ��ݏ��
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
	fTime = 0.0f;
	efkHandle = 0;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

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

	// �ǂݍ��ݏ��
	m_vecLoadInfo.clear();

	return S_OK;
}

//==========================================================================
// �ǂݍ��ݏ���
//==========================================================================
Effekseer::EffectRef CMyEffekseer::LoadEffect(std::string efkpath)
{
	// char16_t�ɕϊ�
	std::u16string string16t = UtilFunc::Transformation::ConvertUtf8ToUtf16(efkpath);

	return LoadProcess(string16t);
}

//==========================================================================
// �G�t�F�N�g�ǂݍ���
//==========================================================================
Effekseer::EffectRef CMyEffekseer::LoadEffect(EEfkLabel label)
{
	// char16_t�ɕϊ�
	std::u16string string16t = UtilFunc::Transformation::ConvertUtf8ToUtf16(m_EffectName[label]);

	return LoadProcess(string16t);
}

//==========================================================================
// �ǂݍ��ݏ���
//==========================================================================
Effekseer::EffectRef CMyEffekseer::LoadProcess(const std::u16string& efkpath)
{
	// �\���̂̒��̕������T��
	auto itr = std::find_if(m_vecLoadInfo.begin(), m_vecLoadInfo.end(),
		[&efkpath](const CMyEffekseer::SLoadInfo& info) {return info.filename == efkpath; });

	// ���������ꍇ
	if (itr != m_vecLoadInfo.end())
	{// �ǂݍ��ݍςݏ���Ԃ�
		return (*itr).ref;
	}

	// �G�t�F�N�g�̓Ǎ�
	Effekseer::EffectRef effect = Effekseer::Effect::Create(m_efkManager, efkpath.c_str());

	onLostDevice = [effect]() -> void
	{
		// �ǂݍ��񂾃G�t�F�N�g�̃��\�[�X�͑S�Ĕj������B
		if (effect != nullptr)
		{
			assert(false);
			effect->UnloadResources();
		}
	};

	onResetDevice = [effect]() -> void
	{
		// �G�t�F�N�g�̃��\�[�X���ēǂݍ��݂���B
		if (effect != nullptr)
		{
			assert(false);
			effect->ReloadResources();
		}
	};

	// �ǂݍ��ݏ��ǉ�
	m_vecLoadInfo.emplace_back();
	m_vecLoadInfo.back().filename = efkpath;	// �t�@�C����
	m_vecLoadInfo.back().ref = effect;			// ���t�@�����X

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

	// ��Q���̃��X�g�擾
	CListManager<CEffekseerObj> list = CEffekseerObj::GetListObj();

	// �擪��ۑ�
	std::list<CEffekseerObj*>::iterator itr = list.GetEnd();
	CEffekseerObj* pObj = nullptr;

	// ���X�g���[�v
	while (list.ListLoop(itr))
	{
		(*itr)->Uninit();
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CMyEffekseer::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	bool bPause = CManager::GetInstance()->GetPause()->IsPause();
	if (!bPause)
	{
		// �S�X�V
		UpdateAll(fDeltaTime, fDeltaRate, fSlowRate);

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
		fTime += fDeltaTime * fSlowRate;
	}

#if _DEBUG
	// �f�o�b�O
	Debug();
#endif
}

//==========================================================================
// �S�X�V
//==========================================================================
void CMyEffekseer::UpdateAll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��Q���̃��X�g�擾
	CListManager<CEffekseerObj> list = CEffekseerObj::GetListObj();

	// �擪��ۑ�
	std::list<CEffekseerObj*>::iterator itr = list.GetEnd();
	CEffekseerObj* pObj = nullptr;

	// ���X�g���[�v
	while (list.ListLoop(itr))
	{
		(*itr)->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CMyEffekseer::Draw()
{
	// ���Ԃ��X�V����
	efkRenderer->SetTime(fTime);

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

//==========================================================================
// �f�o�b�O
//==========================================================================
void CMyEffekseer::Debug()
{

	if (ImGui::TreeNode("Effekseer"))
	{
		//--------------------------
		// ���x���؂�ւ�
		//--------------------------
		int label = m_debugInfo.label;
		ImGui::SliderInt("Label", &label, 0, static_cast<int>(EEfkLabel::EFKLABEL_MAX) - 1, "%d");
		ImGui::Text("%s", magic_enum::enum_name(m_debugInfo.label));
		m_debugInfo.label = static_cast<EEfkLabel>(label);

		ImGui::Dummy(ImVec2(0.0f, 5.0f));

		//--------------------------
		// ����
		//--------------------------
		if (ImGui::Button("Create"))
		{
			if (m_debugInfo.pEfkObj != nullptr)
			{
				m_debugInfo.pEfkObj->Uninit();
				m_debugInfo.pEfkObj = nullptr;
			}

			m_debugInfo.pEfkObj = CEffekseerObj::Create(m_debugInfo.label,
				m_debugInfo.pos,
				m_debugInfo.rot,
				MyLib::Vector3(),
				m_debugInfo.scale, false);
		}

		//--------------------------
		// �g���K�[���M
		//--------------------------
		if (ImGui::Button("Trigger00") &&
			m_debugInfo.pEfkObj != nullptr)
		{
			m_debugInfo.pEfkObj->SetTrigger(0);
		}
		ImGui::SameLine();

		if (ImGui::Button("Trigger01") &&
			m_debugInfo.pEfkObj != nullptr)
		{
			m_debugInfo.pEfkObj->SetTrigger(1);
		}
		ImGui::SameLine();

		if (ImGui::Button("Trigger02") &&
			m_debugInfo.pEfkObj != nullptr)
		{
			m_debugInfo.pEfkObj->SetTrigger(2);
		}
		ImGui::SameLine();

		if (ImGui::Button("Trigger03") &&
			m_debugInfo.pEfkObj != nullptr)
		{
			m_debugInfo.pEfkObj->SetTrigger(3);
		}

		//--------------------------
		// �ʒu
		//--------------------------
		if (ImGui::Button("pos Reset"))
		{// ���Z�b�g
			m_debugInfo.pos = MyLib::Vector3();
		}
		ImGui::SameLine();

		ImGui::DragFloat3("pos", (float*)&m_debugInfo.pos, 1.0f, 0.0f, 0.0f, "%.2f");

		//--------------------------
		// ����
		//--------------------------
		if (ImGui::Button("rot Reset"))
		{// ���Z�b�g
			m_debugInfo.rot = MyLib::Vector3();
		}
		ImGui::SameLine();

		ImGui::DragFloat3("rot", (float*)&m_debugInfo.rot, D3DX_PI * 0.01f, 0.0f, 0.0f, "%.2f");
		UtilFunc::Transformation::RotNormalize(m_debugInfo.rot);


		//--------------------------
		// �X�P�[��
		//--------------------------
		if (ImGui::Button("scale Reset"))
		{// ���Z�b�g
			m_debugInfo.scale = 10.0f;
		}
		ImGui::SameLine();

		ImGui::DragFloat("scale", (float*)&m_debugInfo.scale, 0.1f, 0.0f, 100.0f, "%.2f");


		ImGui::TreePop();
	}


	// ��񔽉f
	if (m_debugInfo.pEfkObj == nullptr) return;

	m_debugInfo.pEfkObj->SetPosition(m_debugInfo.pos);
	m_debugInfo.pEfkObj->SetRotation(m_debugInfo.rot);
	m_debugInfo.pEfkObj->SetScale(m_debugInfo.scale);

}
