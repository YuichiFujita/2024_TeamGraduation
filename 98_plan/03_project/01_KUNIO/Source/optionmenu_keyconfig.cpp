//=============================================================================
// 
//  �T�E���h���j���[���� [optionmenu_keyconfig.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "optionmenu_keyconfig.h"
#include "manager.h"
#include "keyconfig.h"
#include "keyconfig_gamepad.h"
#include "keyconfig_keyboard.h"
#include "input_gamepad.h"
#include "controlkeydisp.h"
#include "selectdraw.h"


//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const MyLib::Vector3 DEFAULT_POS = MyLib::Vector3(640.0f, -50.0f, 0.0f);	// ��{���W
	const float DOWN_POSY = 100.0f;
	const float KEY_SIZE = 50.0f;
	const float FONT_WIDTH = 150.0f;
	const float TITLE_HEIGHT = KEY_SIZE * 1.3f;
	const D3DXCOLOR FRONT_COL = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.0f);
	const float ADD_ALPHA = (1.0f / 20.0f);
}

// �t�@�C����
namespace FILENAME
{
	const std::string CONFIG[INGAME::ACTION::ACT_MAX] =
	{
		"data\\TEXTURE\\ui_setting\\decide.png",
		"data\\TEXTURE\\ui_setting\\back.png",
		"data\\TEXTURE\\ui_setting\\updown.png",
		"data\\TEXTURE\\ui_setting\\air.png",
		"data\\TEXTURE\\ui_setting\\checkpoint.png",
		"data\\TEXTURE\\ui_setting\\retry.png",
		"data\\TEXTURE\\ui_setting\\pause.png",
	};

	const std::string FRONT = "data\\TEXTURE\\ui_setting\\front.png";
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
COptionMenu_Keyconfig::COptionMenu_Keyconfig(int nPriority) : COptionMenu(nPriority)
{
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		m_aKeyConfig[i].s_p2D = nullptr;
		m_aKeyConfig[i].s_pKeyDisp = nullptr;

		// �N���A
		m_aKeyConfig[i] = KEYINFO();
	}

	// �N���A
	m_checkconfig = CHENGEINFO();

	m_pTitle2D = nullptr;
	m_pSelect = nullptr;
	m_bNowChange = false;
	m_SelectKey = 0;
	m_Alpha = 0.0f;
	m_fTime = 0.0f;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
COptionMenu_Keyconfig::~COptionMenu_Keyconfig()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT COptionMenu_Keyconfig::Init()
{
	// ����������
	COptionMenu::Init();

	CKeyConfig* pConfigPad = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CTexture* pTexture = CTexture::GetInstance();
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ������������
	{
		MyLib::Vector3 pos = DEFAULT_POS;
		pos.y += DOWN_POSY;
		m_pTitle2D = CObject2D::Create(11);
		m_pTitle2D->SetType(CObject::TYPE_OBJECT2D);
		m_pTitle2D->SetPosition(pos);
		m_pTitle2D->BindTexture(pTexture->Regist("data\\TEXTURE\\ui_setting\\setting.png"));
		D3DXVECTOR2 texture = pTexture->GetImageSize(m_pTitle2D->GetIdxTexture());
		m_pTitle2D->SetSize(UtilFunc::Transformation::AdjustSizeByHeight(texture, TITLE_HEIGHT));
		m_pTitle2D->SetColor(col);
	}

	// �|���S���𐶐�����
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		if (i <= INGAME::ACT_BACK) { continue; }

		// �L�[�𐶐�
		MyLib::Vector3 pos = DEFAULT_POS;
		pos.y += i * DOWN_POSY;
		pos.x += FONT_WIDTH;
		m_aKeyConfig[i].s_pKeyDisp = CControlKeyDisp::Create(pos, 0.0f, KEY_SIZE, KEY_SIZE, pConfigPad->GetKey(i));
		m_aKeyConfig[i].s_pKeyDisp->SetColor(col);

		// �����𐶐�
		pos.x = DEFAULT_POS.x - FONT_WIDTH;
		m_aKeyConfig[i].s_p2D = CObject2D::Create(11);
		m_aKeyConfig[i].s_p2D->SetType(CObject::TYPE_OBJECT2D);
		m_aKeyConfig[i].s_p2D->SetPosition(pos);
		m_aKeyConfig[i].s_p2D->BindTexture(pTexture->Regist(FILENAME::CONFIG[i]));
		D3DXVECTOR2 texture = pTexture->GetImageSize(m_aKeyConfig[i].s_p2D->GetIdxTexture());

		// �T�C�Y�ݒ�
		D3DXVECTOR2 setsize = UtilFunc::Transformation::AdjustSizeByHeight(texture, KEY_SIZE);
		m_aKeyConfig[i].s_p2D->SetSize(setsize);
		m_aKeyConfig[i].s_p2D->SetSizeOrigin(setsize);
		m_aKeyConfig[i].s_p2D->SetColor(col);

		// �Â�����p�̃|���S������
		pos.x = DEFAULT_POS.x;
		
#if 1
		// �M�̃A�h���X�n��
		KEYINFO* pNowInfo = &m_aKeyConfig[i];
#else
		m_aKeyConfig[i].s_p2DFront = CObject2D::Create(10);
		m_aKeyConfig[i].s_p2DFront->SetSize(D3DXVECTOR2(0.0f, KEY_SIZE));
		m_aKeyConfig[i].s_p2DFront->SetSizeOrigin(D3DXVECTOR2(KEY_SIZE * 6, KEY_SIZE));
		m_aKeyConfig[i].s_p2DFront->SetColor(FRONT_COL);
		m_aKeyConfig[i].s_p2DFront->BindTexture(pTexture->Regist(FILENAME::FRONT));

		m_aKeyConfig[i].s_p2DFront->SetAnchorType(CObject2D::AnchorPoint::LEFT);
		m_aKeyConfig[i].s_p2DFront->SetPosition(pos - MyLib::Vector3(KEY_SIZE * 6.0f, 0.0f, 0.0f));
#endif
	}

	{
		MyLib::Vector3 pos = DEFAULT_POS;
		pos.y += DOWN_POSY * (INGAME::ACT_BACK + 1);
		// �M�|���S������
		m_pSelect = CSelectDraw::Create(pos);
		m_pSelect->SetSizeOrigin(D3DXVECTOR2(KEY_SIZE * 3, KEY_SIZE));
		m_pSelect->SetSize(m_pSelect->GetSizeOrigin());

		// ������Ԑݒ�
		m_pSelect->SetState(CSelectDraw::State::STATE_NONE);
		m_pSelect->SetAlpha(0.0f);
	}

	// ����߂�ύX�p�|���S������
	{
		MyLib::Vector3 pos = DEFAULT_POS;
		pos.y += DOWN_POSY * INGAME::ACTION::ACT_MAX;

		// �����̐���
		m_checkconfig.s_p2D = CObject2D::Create(11);
		m_checkconfig.s_p2D->SetType(CObject::TYPE_OBJECT2D);
		m_checkconfig.s_p2D->SetPosition(pos);
		m_checkconfig.s_p2D->BindTexture(pTexture->Regist("data\\TEXTURE\\ui_setting\\change.png"));
		D3DXVECTOR2 texture = pTexture->GetImageSize(m_checkconfig.s_p2D->GetIdxTexture());
		m_checkconfig.s_p2D->SetSize(UtilFunc::Transformation::AdjustSizeByHeight(texture, KEY_SIZE * 0.8f));
		m_checkconfig.s_p2D->SetColor(col);

		// �L�[�R���t�B�O�𐶐�
		float size = KEY_SIZE * 0.75f;
		pos.x += (size * 3);

		// OK
		m_checkconfig.s_pKeyDispOK = CControlKeyDisp::Create(pos, 0.0f, size, size,
			pConfigPad->GetKey(INGAME::ACT_OK));
		pos.x += (size * 3);
		m_checkconfig.s_pKeyDispOK->SetColor(col);

		// NO
		m_checkconfig.s_pKeyDispNO = CControlKeyDisp::Create(pos, 0.0f, size, size,
			pConfigPad->GetKey(INGAME::ACT_BACK));
		m_checkconfig.s_pKeyDispNO->SetColor(col);

		// �Â�����p�̃|���S������
		pos = DEFAULT_POS;
		pos.y += DOWN_POSY * INGAME::ACTION::ACT_MAX;

#if 0
		m_checkconfig.s_p2DFront = CObject2D::Create(10);
		m_checkconfig.s_p2DFront->SetPosition(pos);
		m_checkconfig.s_p2DFront->SetSize(D3DXVECTOR2(KEY_SIZE * 6, KEY_SIZE));
		m_checkconfig.s_p2DFront->SetColor(FRONT_COL);
		m_checkconfig.s_p2DFront->BindTexture(pTexture->Regist(FILENAME::FRONT));
#else

		// �M�|���S������
		m_checkconfig.pSelect = CSelectDraw::Create(pos - MyLib::Vector3(KEY_SIZE * 6, 0.0f, 0.0f));
		m_checkconfig.pSelect->SetSizeOrigin(D3DXVECTOR2(KEY_SIZE * 6, KEY_SIZE));
		m_checkconfig.pSelect->SetSize(m_checkconfig.pSelect->GetSizeOrigin());

		// ������Ԑݒ�
		m_checkconfig.pSelect->SetState(CSelectDraw::State::STATE_NONE);
		m_checkconfig.pSelect->SetAlpha(0.0f);

#endif
	}

	m_SelectKey = INGAME::ACT_BACK + 1;


	// �����I�����̕M��Ԑݒ�
	m_pSelect->SetState(CSelectDraw::State::STATE_FADEIN);

	// �s�����x�����l1.0f
	m_Alpha = 1.0f;


	// �I�������Z�b�g
	ResetSelect();
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void COptionMenu_Keyconfig::Uninit()
{
	// �I������
	COptionMenu::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void COptionMenu_Keyconfig::Kill()
{
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		// 2D�|���S���̏I��
		if (m_aKeyConfig[i].s_p2D != nullptr)
		{
			m_aKeyConfig[i].s_p2D->Uninit();
			m_aKeyConfig[i].s_p2D = nullptr;
		}

		// 2D�|���S���̏I��
#if 0
		if (m_aKeyConfig[i].s_p2DFront != nullptr)
		{
			m_aKeyConfig[i].s_p2DFront->Uninit();
			m_aKeyConfig[i].s_p2DFront = nullptr;
		}
#else
		
#endif

		// �L�[�R���t�B�O�\���̏I��
		if (m_aKeyConfig[i].s_pKeyDisp != nullptr)
		{
			m_aKeyConfig[i].s_pKeyDisp->Uninit();
			m_aKeyConfig[i].s_pKeyDisp = nullptr;
		}
	}

	{
		// 2D�|���S���̏I��
		if (m_checkconfig.s_p2D != nullptr)
		{
			m_checkconfig.s_p2D->Uninit();
			m_checkconfig.s_p2D = nullptr;
		}

		// 2D�|���S���̏I��
		// �I�����̕M�̏I��
		if (m_checkconfig.pSelect != nullptr)
		{
			m_checkconfig.pSelect->Uninit();
			m_checkconfig.pSelect = nullptr;
		}

		// �L�[�R���t�B�O�\���̏I��
		if (m_checkconfig.s_pKeyDispOK != nullptr)
		{
			m_checkconfig.s_pKeyDispOK->Uninit();
			m_checkconfig.s_pKeyDispOK = nullptr;
		}

		if (m_checkconfig.s_pKeyDispNO != nullptr)
		{
			m_checkconfig.s_pKeyDispNO->Uninit();
			m_checkconfig.s_pKeyDispNO = nullptr;
		}
	}

	// �^�C�g��
	if (m_pTitle2D != nullptr)
	{
		m_pTitle2D->Uninit();
		m_pTitle2D = nullptr;
	}

	if (m_pSelect != nullptr)
	{
		m_pSelect->Uninit();
		m_pSelect = nullptr;
	}

	// �폜����
	COptionMenu::Kill();
}

//==========================================================================
// �X�V����
//==========================================================================
void COptionMenu_Keyconfig::Update()
{
	// �X�V����
	COptionMenu::Update();
}

//==========================================================================
// �ҏW
//==========================================================================
void COptionMenu_Keyconfig::StateEdit()
{
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();
	CKeyConfig* pKeyConfig = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);

	// �s�����x�ݒ�
	//SetAlpha();


	// ���̃T�C�Y�ɖ߂�
	for (const auto& key : m_aKeyConfig)
	{
		if (key.s_pKeyDisp == nullptr) continue;
		key.s_pKeyDisp->SetSize(key.s_pKeyDisp->GetSizeOrigin());
	}

	// �I�𒆂̂��̂������g��
	if (m_bNowChange &&
		m_SelectKey < INGAME::ACTION::ACT_MAX &&
		m_aKeyConfig[m_SelectKey].s_pKeyDisp != nullptr)
	{
		m_aKeyConfig[m_SelectKey].s_pKeyDisp->SetSize(m_aKeyConfig[m_SelectKey].s_pKeyDisp->GetSizeOrigin() * 1.15f);
	}

	if (m_bNowChange) { return; }

	// ���͑҂���Ԃɂ���
	if (pKeyConfig->GetTrigger(INGAME::ACT_OK))
	{
		if (m_SelectKey == INGAME::ACTION::ACT_MAX)
		{
			// �L�[���擾
			int oktype = pKeyConfig->GetKey(INGAME::ACT_OK);
			int backtype = pKeyConfig->GetKey(INGAME::ACT_BACK);

			// �L�[�̓���ւ�
			pKeyConfig->Join(INGAME::ACT_OK, backtype);
			pKeyConfig->Join(INGAME::ACT_BACK, oktype);

			// �e�N�X�`����ނ�ݒ�
			m_checkconfig.s_pKeyDispOK->SetType(backtype);
			m_checkconfig.s_pKeyDispNO->SetType(oktype);

			return;
		}
		// ����L�[�Ɩ߂�L�[�ȊO
		else if (m_SelectKey != INGAME::ACT_OK && m_SelectKey != INGAME::ACT_BACK)
		{
			// �ύX��Ԃ�
			m_bNowChange = true;

			// SE�Đ�
			CSound::GetInstance()->PlaySound(CSound::LABEL_SE_DICTION);

			// �}���`�X���b�h�œ��͎�t
			std::thread th(&COptionMenu_Keyconfig::Chenge, this);
			th.detach();

			return;
		}
	}

	int oldkey = m_SelectKey;

	// �I���ꏊ�ύX
	if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_Y) && pPad->GetStickMoveL(0).y > 0) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
		pKey->GetTrigger(DIK_W))
	{
		// ��Ɉړ�
		m_SelectKey = (m_SelectKey + INGAME::ACTION::ACT_MAX) % (INGAME::ACTION::ACT_MAX + 1);

		if (m_SelectKey < 2)
		{
			m_SelectKey = INGAME::ACTION::ACT_MAX;
		}

		// �T�E���h�Đ�
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING_FINISH);
	}
	else if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_Y) && pPad->GetStickMoveL(0).y < 0) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
		pKey->GetTrigger(DIK_S))
	{
		// ���Ɉړ�
		m_SelectKey = (m_SelectKey + 1) % (INGAME::ACTION::ACT_MAX + 1);
		m_SelectKey = UtilFunc::Transformation::Clamp(m_SelectKey, 2, static_cast<int>(INGAME::ACTION::ACT_MAX) + 1);

		// �T�E���h�Đ�
		CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_WRITING_FINISH);
	}

	// �ύX���Ă���
	if (oldkey != m_SelectKey)
	{
		if (m_SelectKey < INGAME::ACT_MAX)
		{// �I����
			m_pSelect->SetState(CSelectDraw::State::STATE_DRAWING);
			MyLib::Vector3 pos = m_aKeyConfig[m_SelectKey].s_pKeyDisp->GetPosition();
			m_pSelect->SetPosition(pos);
			m_pSelect->SetAlpha(1.0f);
			m_checkconfig.pSelect->SetAlpha(0.0f);
		}
		else
		{// ����߂�؂�ւ�
			m_checkconfig.pSelect->SetState(CSelectDraw::State::STATE_DRAWING);
			m_checkconfig.pSelect->SetAlpha(1.0f);
			m_pSelect->SetAlpha(0.0f);
		}
	}


	// �I�������Z�b�g
	ResetSelect();
}


//==========================================================================
// �ύX
//==========================================================================
void COptionMenu_Keyconfig::Chenge()
{
	CKeyConfig* pKeyConfig = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);

	while (1)
	{
		if (pKeyConfig->GetRelease(INGAME::ACT_OK))
		{
			break;
		}
	}

	// �ύX�O��ۑ�
	int oldkey = pKeyConfig->GetKey(m_SelectKey);

	// �ύX
	pKeyConfig->Setting(m_SelectKey);

	// ���̃T�C�Y�ɖ߂�
	m_aKeyConfig[m_SelectKey].s_pKeyDisp->SetSize(m_aKeyConfig[m_SelectKey].s_pKeyDisp->GetSizeOrigin());

	if (m_aKeyConfig[m_SelectKey].s_pKeyDisp != nullptr)
	{
		m_aKeyConfig[m_SelectKey].s_pKeyDisp->SetType(pKeyConfig->GetKey(m_SelectKey));
	}

	// �ύX���ۑ�
	int selectkey = pKeyConfig->GetKey(m_SelectKey);

	// �����m�F����	
	for (int i = 0; i < INGAME::ACT_MAX; i++)
	{
		// �I��
		if (m_SelectKey == i) { continue; }

		// ����߂�̏ꍇ
		if (i <= INGAME::ACT_BACK) { continue; }

		// �L�[�擾
		int key = pKeyConfig->GetKey(i);

		// �����L�[�ɂ��Ă��܂��Ă���ꍇ
		if (selectkey == key)
		{
			// �ύX�O�̂Ɠ���ւ���
			pKeyConfig->Join(i, oldkey);
			if (m_aKeyConfig[i].s_pKeyDisp != nullptr)
			{
				m_aKeyConfig[i].s_pKeyDisp->SetType(pKeyConfig->GetKey(i));
			}
		}
	}

	while (1)
	{
		if (!pKeyConfig->GetPress(m_SelectKey))
		{
			break;
		}
	}

	m_bNowChange = false;
}

//==========================================================================
// �����x����
//==========================================================================
void COptionMenu_Keyconfig::SetAlpha()
{
	if (m_Alpha >= 1.0f) { return; }

	m_fTime += ADD_ALPHA;
	m_Alpha = UtilFunc::Correction::EaseInOutExpo(0.0f, 1.0f, 0, 2.0f, m_fTime);
	m_Alpha = UtilFunc::Transformation::Clamp(m_Alpha, 0.0f, 1.0f);

	// �F�𒲐�����

	// �^�C�g��
	D3DXCOLOR col = m_pTitle2D->GetColor();
	col.a = m_Alpha;
	m_pTitle2D->SetColor(col);

	// �F�𒲐�����
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		// ����
		col = m_aKeyConfig[i].s_p2D->GetColor();
		col.a = m_Alpha;
		m_aKeyConfig[i].s_p2D->SetColor(col);

		// �L�[�R���t�B�O
		col = m_aKeyConfig[i].s_pKeyDisp->GetColor();
		col.a = m_Alpha;
		m_aKeyConfig[i].s_pKeyDisp->SetColor(col);
	}

	// �����̐���
	col = m_checkconfig.s_p2D->GetColor();
	col.a = m_Alpha;
	m_checkconfig.s_p2D->SetColor(col);

	// �L�[�R���t�B�O�𐶐�
	col = m_checkconfig.s_pKeyDispOK->GetColor();
	col.a = m_Alpha;
	m_checkconfig.s_pKeyDispOK->SetColor(col);
	col = m_checkconfig.s_pKeyDispNO->GetColor();
	col.a = m_Alpha;
	m_checkconfig.s_pKeyDispNO->SetColor(col);
}


//==========================================================================
// �I�������Z�b�g
//==========================================================================
void COptionMenu_Keyconfig::ResetSelect()
{
	// �I���󋵂ɉ������F�ύX
	for (int i = 0; i < INGAME::ACT_MAX; i++)
	{
		if (m_aKeyConfig[i].s_p2D == nullptr) continue;
		if (m_aKeyConfig[i].s_pKeyDisp == nullptr) continue;

		// �I���O�͈Â�
		D3DXCOLOR setcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		if (i != m_SelectKey)
		{
			setcol = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
		}
		m_aKeyConfig[i].s_p2D->SetColor(setcol);
		m_aKeyConfig[i].s_pKeyDisp->SetColor(setcol);
	}

	// ����߂�ύX��
	D3DXCOLOR setcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	if (m_SelectKey != INGAME::ACT_MAX)
	{
		setcol = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	}
	m_checkconfig.s_p2D->SetColor(setcol);
	m_checkconfig.s_pKeyDispNO->SetColor(setcol);
	m_checkconfig.s_pKeyDispOK->SetColor(setcol);

}
