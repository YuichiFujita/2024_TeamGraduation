//=============================================================================
// 
//  ���g���CUI���� [retryui.cpp]
//  Author : �x�씋��
// 
//=============================================================================
#include "retryui.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "player.h"
#include "keyconfig.h"
#include "keyconfig_gamepad.h"
#include "controlkeydisp.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_UI[3] = { "data\\TEXTURE\\dead\\restartbutton.png",
		"data\\TEXTURE\\dead\\retrybutton.png" ,
		"data\\TEXTURE\\dead\\posebutton.png" };	// �e�N�X�`���̃t�@�C��

	const MyLib::Vector3 DEST_POS[3] = { MyLib::Vector3(70.0f, 685.0f, 0.0f),
		MyLib::Vector3(640.0f, 685.0f, 0.0f),
		MyLib::Vector3(1210.0f, 685.0f, 0.0f) };	// �ڕW�̈ʒu

	const MyLib::Vector3 START_POS[3] = { MyLib::Vector3(-115.0f, 685.0f, 0.0f),
		MyLib::Vector3(640.0f, 900.0f, 0.0f),
		MyLib::Vector3(1415.0f, 685.0f, 0.0f) };	// �J�n�̈ʒu

	const int USE_CONFIG[CRetry_Ui::BUTTON_MAX] =	// �g�p����L�[�R���t�B�O
	{
		INGAME::ACT_RETRY,
		INGAME::ACT_CHECKPOINT,
		INGAME::ACT_PAUSE,
	};

	const float UI_HEIGHT[3] = { 75.0f,100.0f,75.0f };// Ui�̕�
	const float TIME_RETRY = 0.8f;		// ���g���C�Ń{�^�������������鎞��
	const float UI_MOVE_COEF = 0.04f;
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CRetry_Ui::CRetry_Ui(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	for (int cnt = 0; cnt < BUTTON_MAX; cnt++)
	{
		m_Button[cnt] = nullptr;
		m_apKeyDisp[cnt] = nullptr;
		m_DispPos[cnt] = 0.0f;
	}
	m_fRetryPushTime = 0.0f;	// ���g���C�̉�������
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CRetry_Ui::~CRetry_Ui()
{

}

//==========================================================================
// ��������
//==========================================================================
CRetry_Ui* CRetry_Ui::Create()
{
	// �������̊m��
	CRetry_Ui* pObj = DEBUG_NEW CRetry_Ui;

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CRetry_Ui::Init()
{
	// �C���v�b�g���擾
	CKeyConfigManager* pKeyConfig = CKeyConfigManager::GetInstance();
	CKeyConfig* pPad = pKeyConfig->GetConfig(CKeyConfigManager::Control::CONTROL_INPAD);
	//// �I�u�W�F�N�g2D�̏�����
	//CObject2D::Init();

	int texID;
	D3DXVECTOR2 size;
	for (int cnt = 0; cnt < BUTTON_MAX; cnt++)
	{
		m_Button[cnt] = CObject2D::Create();

		// �I�u�W�F�N�g2D�̏�����
		m_Button[cnt]->Init();

		// �e�N�X�`���ݒ�
		texID = CTexture::GetInstance()->Regist(TEXTURE_UI[cnt]);
		m_Button[cnt]->BindTexture(texID);

		// �T�C�Y�ݒ�
		size = CTexture::GetInstance()->GetImageSize(texID);

#if 1	// ���������ɃT�C�Y�ݒ�
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, UI_HEIGHT[cnt]);

#else	// �c�������ɃT�C�Y�ݒ�
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, 320.0f);
#endif

		m_Button[cnt]->SetSize(size);
		m_Button[cnt]->SetSizeOrigin(size);

		// �ʒu�̐ݒ�
		m_Button[cnt]->SetPosition(START_POS[cnt]);

		// ��ނ̐ݒ�
		m_Button[cnt]->SetType(CObject::TYPE::TYPE_OBJECT2D);

		// �L�[�R���t�B�O�𐶐�����
		if(m_apKeyDisp[cnt] == nullptr)
		{
			// �����n�_��ݒ�
			MyLib::Vector3 pos = START_POS[cnt];
			pos.x += size.x * 0.7f;
			float range = size.y * 0.45f;

			// �ڕW�n�_������
			m_DispPos[cnt] = DEST_POS[cnt];
			m_DispPos[cnt].x += size.x * 0.5f;

			// ����
			m_apKeyDisp[cnt] = CControlKeyDisp::Create(pos, 0.0f, range, range, pPad->GetKey(USE_CONFIG[cnt]));
		}
	}

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CRetry_Ui::Uninit()
{
	// �I������
	for (int cnt = 0; cnt < BUTTON_MAX; cnt++)
	{
		if (m_Button[cnt] != nullptr)
		{
			m_Button[cnt]->Uninit();
			m_Button[cnt] = nullptr;
		}

		if (m_apKeyDisp[cnt] != nullptr)
		{
			m_apKeyDisp[cnt]->Uninit();
			m_apKeyDisp[cnt] = nullptr;
		}
	}

	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CRetry_Ui::Update()
{
	// �C���v�b�g���擾
	CKeyConfigManager* pKeyConfig = CKeyConfigManager::GetInstance();
	CKeyConfig* pPad = pKeyConfig->GetConfig(CKeyConfigManager::Control::CONTROL_INPAD);
	CKeyConfig* pKey = pKeyConfig->GetConfig(CKeyConfigManager::Control::CONTROL_INKEY);

	// �t�F�[�h���擾
	CInstantFade* pFade = CManager::GetInstance()->GetInstantFade();
	CInstantFade::STATE fadestate = pFade->GetState();

	// �v���C���[�擾
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;
	playerList.ListLoop(&pPlayer);

	Moveui();
	if (pPad->GetTrigger(INGAME::ACT_CHECKPOINT) || pKey->GetTrigger(INGAME::ACT_CHECKPOINT))
	{// Y����͂��Ă���
		pPlayer->SetState(CPlayer::STATE::STATE_RETURN);
	}
	else if (pPad->GetPress(INGAME::ACT_RETRY) || pKey->GetTrigger(INGAME::ACT_RETRY))
	{// X����͂��Ă���
		
		pPlayer->SetState(CPlayer::STATE::STATE_RESTART);
	}
	else
	{// ������ĂȂ��̂ŃJ�E���g���Z�b�g
		m_fRetryPushTime = 0.0f;
	}


	// �X�V����
	for (int cnt = 0; cnt < BUTTON_MAX; cnt++)
	{
		m_Button[cnt]->Update();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CRetry_Ui::Draw()
{
	// �v���C���[�擾
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;

	// ���X�g���[�v
	while (playerList.ListLoop(&pPlayer))
	{
		if (pPlayer->GetState() == CPlayer::STATE_DEAD
			|| pPlayer->GetState() == CPlayer::STATE_RESPAWN
			|| pPlayer->GetState() == CPlayer::STATE_RETURN
			|| pPlayer->GetState() == CPlayer::STATE_RESTART)
		{
			// �`�揈��
			for (int cnt = 0; cnt < BUTTON_MAX; cnt++)
			{
				m_Button[cnt]->Draw();
			}
		}

	}
}

//==========================================================================
// �ړ�����
//==========================================================================
void CRetry_Ui::Moveui()
{
	MyLib::Vector3 deffpos, changepos;
	for (int cnt = 0; cnt < BUTTON_MAX; cnt++)
	{
		// �ڕW�ʒu�܂ł̍����v�Z
		deffpos.x = (DEST_POS[cnt].x - m_Button[cnt]->GetPosition().x) * UI_MOVE_COEF;
		deffpos.y = (DEST_POS[cnt].y - m_Button[cnt]->GetPosition().y) * UI_MOVE_COEF;

		// ���X�ɖڕW�ʒu�Ɉړ�
		MyLib::Vector3 changepos =
			MyLib::Vector3(m_Button[cnt]->GetPosition().x +
				deffpos.x, m_Button[cnt]->GetPosition().y +
				deffpos.y, m_Button[cnt]->GetPosition().z);

		// �ʒu��ݒ�
		m_Button[cnt]->SetPosition(changepos);

		// �L�[�R���t�B�O�\�����ړ�
		if (m_apKeyDisp[cnt] != nullptr)
		{
			// �ڕW�ʒu�܂ł̍����v�Z
			deffpos.x = (m_DispPos[cnt].x - m_apKeyDisp[cnt]->GetPosition().x) * UI_MOVE_COEF;
			deffpos.y = (m_DispPos[cnt].y - m_apKeyDisp[cnt]->GetPosition().y) * UI_MOVE_COEF;

			// �ڕW�ʒu��ݒ�
			changepos =
				MyLib::Vector3(m_apKeyDisp[cnt]->GetPosition().x +
					deffpos.x, m_apKeyDisp[cnt]->GetPosition().y +
					deffpos.y, m_apKeyDisp[cnt]->GetPosition().z);

			// �ʒu��ݒ�
			m_apKeyDisp[cnt]->SetPosition(changepos);
		}
	}
}