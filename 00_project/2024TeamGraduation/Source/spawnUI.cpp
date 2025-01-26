//============================================================
//
//	�o�ꉉ�o�`�[�����\��UI���� [spawnUI.cpp]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�C���N���[�h�t�@�C��
//************************************************************
#include "spawnUI.h"
#include "manager.h"
#include "object2D_Anim.h"
#include "scrollString2D.h"
#include "loadtext.h"
#include "entryRuleManager.h"

//************************************************************
//	�萔�錾
//************************************************************
namespace
{
	const int PRIORITY = 6;	// UI�̗D�揇��
	namespace gameset
	{
		const char* TEXTURE = "data\\TEXTURE\\teamNameGB000.png";	// �w�i�e�N�X�`��
		const MyLib::PosGrid2 PTRN = MyLib::PosGrid2(1, 2);			// �e�N�X�`��������
		const float WIDTH = 300.0f;		// ����
		const float MOVE_TIME = 0.25f;	// �ړ�����
		const float TRANS_TIME = 0.25f;	// �J�ڑҋ@����

		namespace l
		{
			const MyLib::Vector3 INIT_POS = MyLib::Vector3(-WIDTH, 630.0f, 0.0f);				// �����ʒu
			const MyLib::Vector3 DEST_POS = MyLib::Vector3(WIDTH - 10.0f, INIT_POS.y, 0.0f);	// �ڕW�ʒu
		}
		namespace r
		{
			const MyLib::Vector3 INIT_POS = MyLib::Vector3(VEC3_SCREEN_SIZE.x + WIDTH, 630.0f, 0.0f);				// �����ʒu
			const MyLib::Vector3 DEST_POS = MyLib::Vector3(VEC3_SCREEN_SIZE.x - WIDTH + 10.0f, INIT_POS.y, 0.0f);	// �ڕW�ʒu
		}

		const MyLib::Vector3 INIT_POS[] = { l::INIT_POS, r::INIT_POS };	// �����ʒu
		const MyLib::Vector3 DEST_POS[] = { l::DEST_POS, r::DEST_POS };	// �ڕW�ʒu
	}

	namespace str
	{
		const EAlignX ALIGN_X[] = { XALIGN_LEFT, XALIGN_RIGHT };	// ���z�u
		const char*	FONT	= "data\\FONT\\Kazesawa Bold.ttf";		// �t�H���g�p�X
		const bool	ITALIC	= false;	// �C�^���b�N
		const float	HEIGHT	= 40.0f;	// �����c��
		const float	OFFSET_X[] = { -gameset::WIDTH + 30.0f, gameset::WIDTH - 30.0f };	// �����c��

		namespace two
		{
			const float	OFFSET_Y = -30.0f;	// �I�t�Z�b�g
			const float	WAIT_TIIME = 0.02f;	// ��������̑ҋ@����
			const float	TRANS_TIME = 0.45f;	// �J�ڑҋ@����
		}
		namespace name
		{
			const float	OFFSET_Y = 30.0f;		// �I�t�Z�b�g
			const float	WAIT_TIIME = 0.035f;	// ��������̑ҋ@����
			const float	TRANS_TIME = 1.5f;		// �J�ڑҋ@����
		}
	}
}

//************************************************************
//	�֐��|�C���^
//************************************************************
// ��Ԋ֐�
CSpawnUI::STATE_FUNC CSpawnUI::m_StateFuncList[] =
{
	&CSpawnUI::UpdateNone,		// �������Ȃ��̍X�V
	&CSpawnUI::UpdateSpawn,		// �����̍X�V
	&CSpawnUI::UpdateHutatuna,	// ����̍X�V
	&CSpawnUI::UpdateTeamname,	// �`�[�����̍X�V
	&CSpawnUI::UpdateDespawn,	// �����̍X�V
};

//************************************************************
//	�q�N���X [CSpawnUI] �̃����o�֐�
//************************************************************
//============================================================
//	�R���X�g���N�^
//============================================================
CSpawnUI::CSpawnUI(const CGameManager::ETeamSide team) : CObject(PRIORITY, LAYER::LAYER_2D),
	m_pBG		(nullptr),		// �w�i���
	m_pHutatuna	(nullptr),		// ������
	m_pTeamName	(nullptr),		// �`�[�������
	m_team		(team),			// �`�[��
	m_state		(STATE_SPAWN),	// ���
	m_fCurTime	(0.0f)			// �o�ߎ���
{
	// �X�^�e�B�b�N�A�T�[�g
	static_assert(NUM_ARRAY(m_StateFuncList) == CSpawnUI::STATE_MAX, "ERROR : State Count Mismatch");
}

//============================================================
//	�f�X�g���N�^
//============================================================
CSpawnUI::~CSpawnUI()
{

}

//============================================================
//	����������
//============================================================
HRESULT CSpawnUI::Init()
{
	// UI�̐���
	if (FAILED(CreateUI()))
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// ��ނ��I�u�W�F�N�g2D�ɂ���
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//============================================================
//	�I������
//============================================================
void CSpawnUI::Uninit()
{
	// ���ɔj���ς݂̏ꍇ������
	if (IsDeath()) { return; }

	// �w�i�̏I��
	SAFE_UNINIT(m_pBG);

	// �I�u�W�F�N�g��j��
	Release();
}

//============================================================
//	�폜����
//============================================================
void CSpawnUI::Kill()
{
	// ���g�̏I��
	CSpawnUI::Uninit();
}

//============================================================
//	�X�V����
//============================================================
void CSpawnUI::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (m_StateFuncList[m_state] != nullptr)
	{ // ��ԍX�V�֐�������ꍇ

		// ��ԕʏ���
		(this->*(m_StateFuncList[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
	}
}

//============================================================
//	�`�揈��
//============================================================
void CSpawnUI::Draw()
{

}

//============================================================
//	�ʒu�ݒ�̐ݒ菈��
//============================================================
void CSpawnUI::SetPosition(const MyLib::Vector3& rPos)
{
	// ���g�̈ʒu��ݒ�
	CObject::SetPosition(rPos);

	// ���Έʒu�̐ݒ�
	SetPositionRelative();
}

//============================================================
//	�`��󋵂̐ݒ菈��
//============================================================
void CSpawnUI::SetEnableDisp(const bool bDisp)
{
	// ���g�̕`��󋵂�ݒ�
	CObject::SetEnableDisp(bDisp);

	// �w�i�̕`��󋵂�ݒ�
	m_pBG->SetEnableDisp(bDisp);

	// ����̕`��󋵂�ݒ�
	m_pHutatuna->SetEnableDisp(bDisp);

	// �`�[�����̕`��󋵂�ݒ�
	m_pTeamName->SetEnableDisp(bDisp);
}

//============================================================
//	��������
//============================================================
CSpawnUI *CSpawnUI::Create(const CGameManager::ETeamSide team)
{
	// �C���X�^���X�̐���
	CSpawnUI* pTrans = DEBUG_NEW CSpawnUI(team);
	if (pTrans != nullptr)
	{ // �����ɐ��������ꍇ

		// �N���X�̏�����
		if (FAILED(pTrans->Init()))
		{ // �������Ɏ��s�����ꍇ

			// �N���X�̏I��
			SAFE_UNINIT(pTrans);
			return nullptr;
		}
	}

	// �C���X�^���X��Ԃ�
	return pTrans;
}

//============================================================
//	�������Ȃ��̍X�V����
//============================================================
void CSpawnUI::UpdateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{

}

//============================================================
//	�����̍X�V����
//============================================================
void CSpawnUI::UpdateSpawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �o�ߎ��Ԃ����Z
	m_fCurTime += fDeltaTime * fSlowRate;

	// �w�i�̈ʒu���ړ�
	MyLib::Vector3 posBG = UtilFunc::Correction::EasingCubicOut(gameset::INIT_POS[m_team], gameset::DEST_POS[m_team], 0.0f, gameset::MOVE_TIME, m_fCurTime);
	SetPosition(posBG);

	if (m_fCurTime >= gameset::MOVE_TIME + gameset::TRANS_TIME)
	{ // �o�߂��������ꍇ

		// �o�ߎ��Ԃ�������
		m_fCurTime = 0.0f;

		// ����̕�������J�n
		m_pHutatuna->SetEnableScroll(true);

		// �����Ԃɂ���
		m_state = STATE_HUTATUNA;
	}
}

//============================================================
//	����̍X�V����
//============================================================
void CSpawnUI::UpdateHutatuna(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (!m_pHutatuna->IsScroll())
	{ // �������肪�I�������ꍇ

		// �o�ߎ��Ԃ����Z
		m_fCurTime += fDeltaTime * fSlowRate;
		if (m_fCurTime >= str::two::TRANS_TIME)
		{ // �o�߂��������ꍇ

			// �o�ߎ��Ԃ�������
			m_fCurTime = 0.0f;

			// �`�[�����̕�������J�n
			m_pTeamName->SetEnableScroll(true);

			// �`�[������Ԃɂ���
			m_state = STATE_TEAMNAME;
		}
	}
}

//============================================================
//	�`�[�����̍X�V����
//============================================================
void CSpawnUI::UpdateTeamname(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	if (!m_pTeamName->IsScroll())
	{ // �������肪�I�������ꍇ

		// �o�ߎ��Ԃ����Z
		m_fCurTime += fDeltaTime * fSlowRate;
		if (m_fCurTime >= str::name::TRANS_TIME)
		{ // �o�߂��������ꍇ

			// �o�ߎ��Ԃ�������
			m_fCurTime = 0.0f;

			// ������Ԃɂ���
			m_state = STATE_DESPAWN;
		}
	}
}

//============================================================
//	�����̍X�V����
//============================================================
void CSpawnUI::UpdateDespawn(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �o�ߎ��Ԃ����Z
	m_fCurTime += fDeltaTime * fSlowRate;

	// �w�i�̈ʒu���ړ�
	MyLib::Vector3 posBG = UtilFunc::Correction::EasingCubicIn(gameset::DEST_POS[m_team], gameset::INIT_POS[m_team], 0.0f, gameset::MOVE_TIME, m_fCurTime);
	SetPosition(posBG);

	if (m_fCurTime >= gameset::MOVE_TIME)
	{ // �o�߂��������ꍇ

		// �o�ߎ��Ԃ�������
		m_fCurTime = 0.0f;

		// ���g�̏I��
		Uninit();
	}
}

//============================================================
//	���Έʒu�̐ݒ菈��
//============================================================
void CSpawnUI::SetPositionRelative()
{
	// ���g�̈ʒu���擾
	MyLib::Vector3 posThis = GetPosition();

	// �w�i�̈ʒu�ݒ�
	m_pBG->SetPosition(posThis);

	// ����̈ʒu�ݒ�
	m_pHutatuna->SetPosition(posThis + MyLib::Vector3(str::OFFSET_X[m_team], str::two::OFFSET_Y, 0.0f));

	// �`�[�����̈ʒu�ݒ�
	m_pTeamName->SetPosition(posThis + MyLib::Vector3(str::OFFSET_X[m_team], str::name::OFFSET_Y, 0.0f));
}

//============================================================
//	UI�̐�������
//============================================================
HRESULT CSpawnUI::CreateUI()
{
	// �w�i�̐���
	m_pBG = CObject2D_Anim::Create
	( // ����
		VEC3_ZERO,	// �ʒu
		gameset::PTRN.x,	// �e�N�X�`����������
		gameset::PTRN.y,	// �e�N�X�`���c������
		0.0f,		// �Đ�����
		false,		// �����j��
		PRIORITY	// �D�揇��
	);
	if (m_pBG == nullptr)
	{ // �����Ɏ��s�����ꍇ

		return E_FAIL;
	}

	// �����Đ���OFF�ɂ���
	m_pBG->SetEnableAutoPlay(false);

	// �e�N�X�`���p�^�[���̏�����
	m_pBG->SetPatternAnim((int)m_team);

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();	// �e�N�X�`�����
	int texID = pTexture->Regist(gameset::TEXTURE);
	m_pBG->BindTexture(texID);

	// ���������ɑ傫���ݒ�
	MyLib::Vector2 size = pTexture->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, gameset::WIDTH);
	size.y /= (float)gameset::PTRN.y;
	m_pBG->SetSize(size);

	// ����̐���
	m_pHutatuna = CScrollString2D::Create
	( // ����
		str::FONT,				// �t�H���g�p�X
		str::ITALIC,			// �C�^���b�N
		L"",					// �w�蕶����
		VEC3_ZERO,				// ���_�ʒu
		str::two::WAIT_TIIME,	// �����\���̑ҋ@����
		str::HEIGHT,			// �����c��
		str::ALIGN_X[m_team],	// ���z�u
		VEC3_ZERO,				// ���_����
		MyLib::color::White(),	// �F
		PRIORITY				// �D�揇��
	);
	if (m_pHutatuna == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	// �����_���ɓ��������
	loadtext::BindString(m_pHutatuna, loadtext::LoadText("data\\TEXT\\game\\hutatuna.txt", UtilFunc::Transformation::Random(0, 9)));

	// �`�[�����̎擾
	std::string sTeamName;
	CEntryRuleManager::LoadName(&sTeamName, m_team);

	// �`�[�����̐���
	m_pTeamName = CScrollString2D::Create
	( // ����
		str::FONT,				// �t�H���g�p�X
		str::ITALIC,			// �C�^���b�N
		sTeamName,				// �w�蕶����
		VEC3_ZERO,				// ���_�ʒu
		str::name::WAIT_TIIME,	// �����\���̑ҋ@����
		str::HEIGHT,			// �����c��
		str::ALIGN_X[m_team],	// ���z�u
		VEC3_ZERO,				// ���_����
		MyLib::color::Black(),	// �F
		PRIORITY				// �D�揇��
	);
	if (m_pTeamName == nullptr)
	{ // �����Ɏ��s�����ꍇ

		assert(false);
		return E_FAIL;
	}

	return S_OK;
}
