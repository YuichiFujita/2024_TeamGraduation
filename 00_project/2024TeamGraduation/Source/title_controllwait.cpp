//=============================================================================
// 
// ����ҋ@���� [title_controllwait.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "title_controllwait.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "titleLogo.h"
#include "titlestudent.h"
#include "fade.h"
#include "option_BBS.h"
#include "camera.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int PLAYER_MIN = 10;	// �v���C���[�ŏ��l��
	const int PLAYER_MAX = 14;	// �v���C���[�ő�l��

	const std::string TEXTURE_SELECT[CTitle_ControllWait::ESelect::SELECT_MAX] =	// �I�����̃e�N�X�`��
	{
		"data\\TEXTURE\\title\\battle.png",	// �����J�n
		"data\\TEXTURE\\title\\more.png",	// �F�X
	};
	const std::string TEXTURE_BG[CTitle_ControllWait::ESelect::SELECT_MAX] =	// �I�����̃e�N�X�`��
	{
		"data\\TEXTURE\\title\\battle_BG.png",	// �����J�n
		"data\\TEXTURE\\title\\more_BG.png",	// �F�X
	};

	const float SIZEHEIGHT_SELECT[CTitle_ControllWait::ESelect::SELECT_MAX] =	// �I�����̃e�N�X�`��
	{
		100.0f,	// �����J�n
		140.0f,	// �F�X
	};
	const float SIZEHEIGHT_BG = 200.0f;	// �w�i�̃T�C�Y

	const MyLib::Vector3 POSITION_SELECT[COptionMenu::ESelect::SELECT_MAX] =	// �I�����̈ʒu
	{
		MyLib::Vector3(340.0f, 500.0f, 0.0f),	// �����J�n
		MyLib::Vector3(940.0f, 500.0f, 0.0f),	// �F�X
	};
	const D3DXCOLOR COLOR_NOTSELECT = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);	// �I�����ĂȂ��F
	const float MARKERTIME = 0.2f;	// �I���}�[�J�[�̎���
	const float SCALE_SELECT = 0.7f;	// �I���}�[�J�[�̃X�P�[��
}

namespace Timer
{
	namespace State
	{
		const float WAIT = 15.0f;	// �ҋ@
		const float TRANSITION = 0.2f;	// �J��
		const float TRANSITION_WAIT = 0.5f;		// �J�ڑҋ@
		const float TRANSITION_SECOND = 0.4f;	// �J��
		const float TRANSITION_LAST = 0.5f;		// �J��(���X�g)
		const float SCALE = 1.0f;		// �g�嗦
	}

	namespace BG
	{
		const float SPAWN = 0.3f;	// �o��
		const float ROTATE = 2.0f;	// �o��
	}

	namespace SandSmoke
	{
		const float SPAWN = 3.5f;	// ����
		const int RANDRANGE = 8;	// �����_���͈�(10����1�b)
	}
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CTitle_ControllWait::STATE_FUNC CTitle_ControllWait::m_StateFunc[] =	// ��Ԋ֐�
{
	&CTitle_ControllWait::StateWait,					// �ҋ@
	&CTitle_ControllWait::StateControll,				// ����
	&CTitle_ControllWait::StateTransitionMoreFirst,		// �J��
	&CTitle_ControllWait::StateTransitionWait,			// �J�ڑҋ@
	&CTitle_ControllWait::StateTransitionMoreSecond,	// �J��
	&CTitle_ControllWait::StateTransitionMoreLast,		// �J��(���X�g)
	&CTitle_ControllWait::StateTansitionBack,			// �߂�
};

CTitle_ControllWait::STATEBG_FUNC CTitle_ControllWait::m_StateBGFunc[] =	// ��Ԋ֐�
{
	&CTitle_ControllWait::StateBGSpawn,					// �ҋ@
	&CTitle_ControllWait::StateBGLoop,					// �ҋ@
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTitle_ControllWait::CTitle_ControllWait() : CTitleScene()
{
	// ���
	m_state = EState::STATE_WAIT;		// ���
	m_fStateTime = 0.0f;				// ��Ԏ���

	// �w�i���
	m_stateBG = EStateBG::STATEBG_SPAWN;		// ���
	m_fStateTimeBG = 0.0f;	// ��Ԏ���

	// �I����
	m_select = ESelect::SELECT_BATTLE;

	// ���̑�
	m_pLogo = nullptr;			// ���S
	m_pOptionBBS = nullptr;		// �f����
	m_fTimeMarker = 0.0f;		// �}�[�J�[�̃^�C�}�[
	m_fTimeSandSmoke = 0.0f;		// �����̃^�C�}�[
	m_fIntervalSandSmoke = 0.0f;	// �����̃C���^�[�o��
	m_vecTitleStudent.clear();	// �^�C�g���̐��k

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTitle_ControllWait::~CTitle_ControllWait()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTitle_ControllWait::Init()
{
	// ����������
	if (FAILED(CTitleScene::Init())) return E_FAIL;

	// ���S�̐���
	m_pLogo = CTitleLogo::Create();
	if (m_pLogo == nullptr) return E_FAIL;

	// �v���C���[����
	int num = UtilFunc::Transformation::Random(PLAYER_MIN, PLAYER_MAX);
	for (int i = 0; i < num; i++)
	{
		// �^�C�g���̐��k����
		m_vecTitleStudent.push_back(CTitleStudent::Create());
		if (m_vecTitleStudent.back() == nullptr) return E_FAIL;
	}

	// �w�i����
	if (FAILED(CreateBG()))
	{
		return E_FAIL;
	}

	// �I��������
	if (FAILED(CreateSelect()))
	{
		return E_FAIL;
	}

	// BGM�Đ�
	CSound::GetInstance()->PlaySound(CSound::ELabel::LABEL_BGM_TITLE);

	// �f������
	m_pOptionBBS = COption_BBS::Create();

	// ����
	m_fTimeSandSmoke = 0.0f;		// �����̃^�C�}�[
	m_fIntervalSandSmoke = Timer::SandSmoke::SPAWN;	// �����̃C���^�[�o��
	return S_OK;
}

//==========================================================================
// �I��������
//==========================================================================
HRESULT CTitle_ControllWait::CreateSelect()
{
	for (int i = 0; i < ESelect::SELECT_MAX; i++)
	{
		// ��������
		m_pSelectUI[i] = CObject2D::Create(1);
		if (m_pSelectUI[i] == nullptr) return E_FAIL;

		// �I�u�W�F�N�g�̎�ސݒ�
		m_pSelectUI[i]->CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

		// �e�N�X�`���ݒ�
		CTexture* pTexture = CTexture::GetInstance();
		int texID = CTexture::GetInstance()->Regist(TEXTURE_SELECT[i]);
		m_pSelectUI[i]->BindTexture(texID);


		// �摜�T�C�Y�擾
		MyLib::Vector2 size = pTexture->GetImageSize(texID);

		// �c�������ɃT�C�Y�v�Z
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZEHEIGHT_SELECT[i]);
		m_pSelectUI[i]->SetSize(MyLib::Vector2(size.x, size.y));
		m_pSelectUI[i]->SetSizeOrigin(m_pSelectUI[i]->GetSize());

		// �ʒu�ݒ�
		m_pSelectUI[i]->SetPosition(POSITION_SELECT[i]);
	}

	return S_OK;
}

//==========================================================================
// �w�i����
//==========================================================================
HRESULT CTitle_ControllWait::CreateBG()
{
	for (int i = 0; i < ESelect::SELECT_MAX; i++)
	{
		// ��������
		m_pBG[i] = CObject2D::Create(0);
		if (m_pBG[i] == nullptr) return E_FAIL;

		// �I�u�W�F�N�g�̎�ސݒ�
		m_pBG[i]->CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

		// �e�N�X�`���ݒ�
		CTexture* pTexture = CTexture::GetInstance();
		int texID = CTexture::GetInstance()->Regist(TEXTURE_BG[i]);
		m_pBG[i]->BindTexture(texID);


		// �摜�T�C�Y�擾
		MyLib::Vector2 size = pTexture->GetImageSize(texID);

		// �c�������ɃT�C�Y�v�Z
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZEHEIGHT_BG);
		m_pBG[i]->SetSize(MyLib::Vector2(size.x, size.y));
		m_pBG[i]->SetSizeOrigin(m_pBG[i]->GetSize());

		// �ʒu�ݒ�
		m_pBG[i]->SetPosition(POSITION_SELECT[i]);
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTitle_ControllWait::Uninit()
{
	delete this;
}

//==========================================================================
// �폜����
//==========================================================================
void CTitle_ControllWait::Kill()
{
	// ���S
	SAFE_KILL(m_pLogo);

	// ���k
	for (int i = 0; i < static_cast<int>(m_vecTitleStudent.size()); i++)
	{
		SAFE_KILL(m_vecTitleStudent[i]);
	}

	// �f���폜
	SAFE_KILL(m_pOptionBBS);

	// �w�i�폜
	for (int i = 0; i < ESelect::SELECT_MAX; i++)
	{
		SAFE_KILL(m_pBG[i]);
	}

	// �I����UI�폜
	for (int i = 0; i < ESelect::SELECT_MAX; i++)
	{
		SAFE_KILL(m_pSelectUI[i]);
	}

	// �I������
	Uninit();

	// �G�t�F�N�g�S�폜
	CMyEffekseer::GetInstance()->StopAll();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTitle_ControllWait::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �e�̍X�V
	CTitleScene::Update(fDeltaTime, fDeltaRate, fSlowRate);

	// ��ԍX�V
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// �F�X�V
	UpdateColor(fDeltaTime, fDeltaRate, fSlowRate);

	// �����X�V
	UpdateSandSmoke(fDeltaTime, fDeltaRate, fSlowRate);

#if _DEBUG
	ImGui::Text("%s", magic_enum::enum_name(m_select));
#endif // _DEBUG


}

//==========================================================================
// �����X�V
//==========================================================================
void CTitle_ControllWait::UpdateSandSmoke(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����̃^�C�}�[���Z
	m_fTimeSandSmoke += fDeltaTime * fSlowRate;;
	if (m_fTimeSandSmoke < m_fIntervalSandSmoke) return;

	// �C���^�[�o���Ē��I
	m_fTimeSandSmoke = 0.0f;
	m_fIntervalSandSmoke = 
		Timer::SandSmoke::SPAWN + 
		UtilFunc::Transformation::Random(-Timer::SandSmoke::RANDRANGE, Timer::SandSmoke::RANDRANGE) * 0.1f;

	// �����ʒu�Z�o
	MyLib::Vector3 pos;
	pos.x = UtilFunc::Transformation::Random(-6, 6) * 100.0f;
	pos.z = 100.0f + UtilFunc::Transformation::Random(-6, 2) * 100.0f;

	// �j�u�C�`�ŋt�]
	float rotY = 0.0f;
	if (rand() % 2 == 0)
	{
		rotY += D3DX_PI;
	}

	// ��������
	CEffekseerObj::Create(CMyEffekseer::EEfkLabel::EFKLABEL_SANDSMOKE,
		pos,
		MyLib::Vector3(0.0f, rotY, 0.0f),	// ����
		MyLib::Vector3(),
		10.0f);
}

//==========================================================================
// �I�����X�V
//==========================================================================
void CTitle_ControllWait::UpdateSelect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (GET_MANAGER->GetInstantFade()->GetState() == CInstantFade::EState::STATE_FADEIN)
	{// �t�F�[�h���͔�����
		return;
	}

	// �C���v�b�g���擾
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// �p�b�h���
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();	// �L�[�{�[�h���

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
		pKey->GetTrigger(DIK_A))
	{// ��(�t���[�v)

		int nSelect = (m_select + (ESelect::SELECT_MAX - 1)) % ESelect::SELECT_MAX;
		m_select = (ESelect)nSelect;

		// �^�C�}�[���Z�b�g
		m_fStateTime = 0.0f;

		// �}�[�J�[�^�C�}�[���Z�b�g
		m_fTimeMarker = 0.0f;
		SetStateBG(EStateBG::STATEBG_SPAWN);
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
		pKey->GetTrigger(DIK_D))
	{// �E(���[�v)

		int nSelect = (m_select + 1) % ESelect::SELECT_MAX;
		m_select = (ESelect)nSelect;

		// �^�C�}�[���Z�b�g
		m_fStateTime = 0.0f;

		// �}�[�J�[�^�C�}�[���Z�b�g
		m_fTimeMarker = 0.0f;
		SetStateBG(EStateBG::STATEBG_SPAWN);
	}

	// �J��
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0) ||
		pKey->GetTrigger(DIK_RETURN))
	{
		// ���莞����
		Decide();
	}
}

//==========================================================================
// ����
//==========================================================================
void CTitle_ControllWait::Decide()
{
	switch (m_select)
	{
	case CTitle_ControllWait::SELECT_BATTLE:

		// �J��
		GET_MANAGER->GetFade()->SetFade(CScene::MODE::MODE_ENTRY);
		break;

	case CTitle_ControllWait::SELECT_MORE:
	{
		// �J�����̏����ʒu�����݂̒l�ɐݒ�
		CCamera* pCamera = GET_MANAGER->GetCamera();
		pCamera->SetPositionROrigin(pCamera->GetPositionR());

		// ���������݂̂ɂ���
		pCamera->SetOriginRotation(pCamera->GetRotation());

		// ���S��ҋ@������
		m_pLogo->SetState(CTitleLogo::EState::STATE_WAIT);

		// �I�v�V�������j���[�̏��NONE
		m_pOptionBBS->GetOptionMenu()->SetState(COptionMenu::EState::STATE_NONE);

		// ��ԑJ��
		SetState(EState::STATE_TRANSITION_MORE_1st);

		// ���̑��֑J��
		GET_MANAGER->GetInstantFade()->SetFade(MyLib::color::White(), Timer::State::TRANSITION);
	}
		break;

	default:
		break;
	}
}

//==========================================================================
// �F�X�V
//==========================================================================
void CTitle_ControllWait::UpdateColor(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �}�[�J�[�̃^�C�}�[���Z
	m_fTimeMarker += fDeltaTime * fSlowRate;

	// �����x�X�V
	float color = UtilFunc::Correction::EasingQuintOut(COLOR_NOTSELECT.r, 1.0f, 0.0f, MARKERTIME, m_fTimeMarker);
	D3DXCOLOR selectColor = D3DXCOLOR(color, color, color, 1.0f);

	// �I������UI
	for (const auto& select : m_pSelectUI)
	{
		select->SetColor(MyLib::color::White() - (selectColor - COLOR_NOTSELECT));
		select->SetAlpha(1.0f);
	}
	m_pSelectUI[m_select]->SetColor(selectColor);

	// �g�嗦
	float scale = UtilFunc::Correction::EasingQuintOut(0.5f, 1.0f, 0.0f, MARKERTIME, m_fTimeMarker);
	float scaleRate = (SCALE_SELECT + (1.0f - SCALE_SELECT) * scale);

	for (const auto& select : m_pSelectUI)
	{
		select->SetSize(select->GetSizeOrigin() * (1.0f - (scaleRate - SCALE_SELECT)));
	}
	m_pSelectUI[m_select]->SetSize(m_pSelectUI[m_select]->GetSizeOrigin() * scaleRate);

	// �I�����̃T�C�Y�X�V
	UpdateSelectSize(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �I�����̃T�C�Y�X�V
//==========================================================================
void CTitle_ControllWait::UpdateSelectSize(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �g�k
	float scale = sinf(D3DX_PI * (m_fTimeMarker / Timer::State::SCALE * 0.5f)) * 0.025f;

	MyLib::Vector2 size = m_pSelectUI[m_select]->GetSize();

	// ���̃T�C�Y�ɉ��Z
	size += m_pSelectUI[m_select]->GetSizeOrigin() * scale;
	m_pSelectUI[m_select]->SetSize(size);
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CTitle_ControllWait::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԃ^�C�}�[���Z
	m_fStateTime += fDeltaTime * fSlowRate;
	m_fStateTimeBG += fDeltaTime * fSlowRate;

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	(this->*(m_StateBGFunc[m_stateBG]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �ҋ@
//==========================================================================
void CTitle_ControllWait::StateWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �I�����X�V
	UpdateSelect(fDeltaTime, fDeltaRate, fSlowRate);

	if (m_fStateTime >= Timer::State::WAIT)
	{
		// �V�[���؂�ւ�
		CTitle::GetInstance()->ChangeScene(CTitle::ESceneType::SCENETYPE_SUSURU);
	}
}

//==========================================================================
// ����
//==========================================================================
void CTitle_ControllWait::StateControll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	m_fStateTime = 0.0f;

	// �I�v�V�������j���[�Ŗ߂��Ă���J�ڂ���
	if (m_pOptionBBS->GetOptionMenu()->IsBack())
	{
		SetState(EState::STATE_TRANSITION_BACK);
	}
}

//==========================================================================
// �F�X�ɑJ��(�O��)
//==========================================================================
void CTitle_ControllWait::StateTransitionMoreFirst(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �J�����擾
	CCamera* pCamera = GET_MANAGER->GetCamera();

	// ���֍����ړ�
	MyLib::Vector3 posR = UtilFunc::Correction::EaseInExpo(pCamera->GetPositionROrigin(), MyLib::Vector3(0.0f, 0.0f, 1500.0f), 0.0f, Timer::State::TRANSITION, m_fStateTime);
	pCamera->SetPositionR(posR);

	if (m_fStateTime >= Timer::State::TRANSITION)
	{// �㔼�֑J��

		// �J�����̏����ʒu�����݂̒l�ɐݒ�
		CCamera* pCamera = GET_MANAGER->GetCamera();
		pCamera->SetPositionROrigin(pCamera->GetPositionR());

		// �������Z�b�g
		pCamera->SetRotation(MyLib::Vector3(0.0f, D3DX_PI * 0.2f, pCamera->GetOriginRotation().z));
		pCamera->SetOriginRotation(pCamera->GetRotation());

		// �w�i����
		for (const auto& bg : m_pBG)
		{
			bg->SetEnableDisp(false);
		}

		// �I������UI����
		for (const auto& select : m_pSelectUI)
		{
			select->SetEnableDisp(false);
		}

		// �J�ڑҋ@
		SetState(EState::STATE_TRANSITION_WAIT);
	}

}

//==========================================================================
// �J�ڑҋ@
//==========================================================================
void CTitle_ControllWait::StateTransitionWait(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_fStateTime >= Timer::State::TRANSITION_WAIT)
	{// �㔼�֑J��
		SetState(EState::STATE_TRANSITION_MORE_2nd);
	}
}

//==========================================================================
// �F�X�ɑJ��(�㔼)
//==========================================================================
void CTitle_ControllWait::StateTransitionMoreSecond(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �J�����擾
	CCamera* pCamera = GET_MANAGER->GetCamera();

	// �f���̈ʒu�ֈړ�
	MyLib::Vector3 posR = UtilFunc::Correction::EasingQuintInOut(pCamera->GetPositionROrigin(), m_pOptionBBS->GetPosition(), 0.0f, Timer::State::TRANSITION_SECOND, m_fStateTime);
	pCamera->SetPositionR(posR);

	// ����
	MyLib::Vector3 rot = UtilFunc::Correction::EasingQuintInOut(pCamera->GetOriginRotation(), MyLib::Vector3(0.0f, 0.0f, pCamera->GetOriginRotation().z), 0.0f, Timer::State::TRANSITION_SECOND, m_fStateTime);
	pCamera->SetRotation(rot);

	if (m_fStateTime >= Timer::State::TRANSITION_SECOND)
	{// ���X�g�֑J��

		// �J�����̏����ʒu�����݂̒l�ɐݒ�
		CCamera* pCamera = GET_MANAGER->GetCamera();
		pCamera->SetPositionROrigin(pCamera->GetPositionR());

		// �������Z�b�g
		pCamera->SetOriginRotation(pCamera->GetRotation());

		SetState(EState::STATE_TRANSITION_MORE_LAST);
	}
}

//==========================================================================
// �F�X�ɑJ��(���X�g)
//==========================================================================
void CTitle_ControllWait::StateTransitionMoreLast(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �J�����擾
	CCamera* pCamera = GET_MANAGER->GetCamera();

	// ����
	MyLib::Vector3 rot = UtilFunc::Correction::EasingQuintInOut(pCamera->GetOriginRotation(), MyLib::Vector3(), 0.0f, Timer::State::TRANSITION_SECOND, m_fStateTime);
	pCamera->SetRotation(rot);

	// ����
	float distance = UtilFunc::Correction::EasingQuintInOut(pCamera->GetDistanceOrigin(), 150.0f, 0.0f, Timer::State::TRANSITION_LAST, m_fStateTime);
	pCamera->SetDistance(distance);

	if (m_fStateTime >= Timer::State::TRANSITION_SECOND)
	{// ����֑J��

		// �J�����̏����ʒu�����݂̒l�ɐݒ�
		CCamera* pCamera = GET_MANAGER->GetCamera();
		pCamera->SetPositionROrigin(pCamera->GetPositionR());

		// �I�v�V�������j���[�I���ł���悤�ɂ���
		m_pOptionBBS->GetOptionMenu()->SetState(COptionMenu::EState::STATE_SELECT);

		SetState(EState::STATE_CONTROLL);
	}
}

//==========================================================================
// �߂�
//==========================================================================
void CTitle_ControllWait::StateTansitionBack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �J�����擾
	CCamera* pCamera = GET_MANAGER->GetCamera();

	// ���֍����ړ�
	MyLib::Vector3 posR = UtilFunc::Correction::EaseInExpo(pCamera->GetPositionROrigin(), MyLib::Vector3(0.0f, 0.0f, -1500.0f), 0.0f, Timer::State::TRANSITION, m_fStateTime);
	pCamera->SetPositionR(posR);

	if (m_fStateTime >= Timer::State::TRANSITION)
	{// �ҋ@�֑J��
		SetState(EState::STATE_WAIT);

		// ���S����
		m_pLogo->SetState(CTitleLogo::EState::STATE_LOOP);

		// �w�i����
		for (const auto& bg : m_pBG)
		{
			bg->SetEnableDisp(true);
		}

		// �I������UI����
		for (const auto& select : m_pSelectUI)
		{
			select->SetEnableDisp(true);
		}

		// ���Z�b�g
		pCamera->ResetByMode(CScene::MODE::MODE_TITLE);

		// �I�v�V�������j���[�I�������Z�b�g
		m_pOptionBBS->GetOptionMenu()->SetSelect(COptionMenu::ESelect::SELECT_MASTERVOLUME);
	}
}

//==========================================================================
// �o��
//==========================================================================
void CTitle_ControllWait::StateBGSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �����x�X�V
	float alpha = UtilFunc::Correction::EasingQuintOut(0.0f, 1.0f, 0.0f, Timer::BG::SPAWN, m_fStateTimeBG);
	D3DXCOLOR selectColor = MyLib::color::White(alpha);

	// �g�k
	float scale = UtilFunc::Correction::EasingQuintInOut(0.0f, 1.0f, 0.0f, Timer::BG::SPAWN, m_fStateTimeBG);

	// ��]���ēo��
	float rotZ = UtilFunc::Correction::EasingQuintInOut(D3DX_PI * 4.0f, 0.0f, 0.0f, Timer::BG::SPAWN, m_fStateTimeBG);

	for (const auto& select : m_pBG)
	{
		// �F
		select->SetColor(MyLib::color::White(1.0f - UtilFunc::Correction::EasingQuintOut(0.0f, 0.4f, 0.0f, 1.0f, alpha)));

		// �g�k
		select->SetSize(select->GetSizeOrigin() * UtilFunc::Correction::EasingQuintOut(1.0f, SCALE_SELECT, 0.0f, 1.0f, scale));

		// ��]
		select->SetRotation(MyLib::Vector3());
	}
	m_pBG[m_select]->SetColor(selectColor);
	m_pBG[m_select]->SetSize(m_pBG[m_select]->GetSizeOrigin() * scale);
	m_pBG[m_select]->SetRotation(MyLib::Vector3(0.0f, 0.0f, rotZ));

	// ��ԑJ��
	if (m_fStateTimeBG >= Timer::BG::SPAWN)
	{
		m_stateBG = EStateBG::STATEBG_LOOP;
		m_fStateTimeBG = 0.0f;
	}
}

//==========================================================================
// ���[�v
//==========================================================================
void CTitle_ControllWait::StateBGLoop(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��]
	float rotZ = sinf(D3DX_PI * (m_fStateTimeBG / Timer::BG::ROTATE)) * (D3DX_PI * 0.01f);
	m_pBG[m_select]->SetRotation(MyLib::Vector3(0.0f, 0.0f, rotZ));

	// �g�k
	float scale = 1.0f + sinf(D3DX_PI * (m_fStateTimeBG / Timer::BG::ROTATE * 0.5f)) * 0.05f;
	m_pBG[m_select]->SetSize(m_pBG[m_select]->GetSizeOrigin() * scale);

	if (m_fStateTimeBG >= Timer::BG::ROTATE * 2.0f)
	{
		m_fStateTimeBG = 0.0f;
	}
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CTitle_ControllWait::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CTitle_ControllWait::SetStateBG(EStateBG state)
{
	m_stateBG = state;
	m_fStateTimeBG = 0.0f;
}