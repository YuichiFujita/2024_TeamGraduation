//==========================================================================
// 
//  �I�v�V�������j���[���� [optionMenu.cpp]
//  Author : ���n�Ή�
// 
//==========================================================================
#include "optionMenu.h"
#include "object2D.h"
#include "objectX.h"
#include "multinumber.h"
#include "number.h"
#include "manager.h"
#include "camera.h"
#include "instantfade.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_BOARD = "data\\TEXTURE\\title\\board.jpg";	// �{�[�h�̃e�N�X�`��
	const std::string TEXTURE_PAPER = "data\\TEXTURE\\title\\paper.png";	// ���̃e�N�X�`��
	const std::string TEXTURE_SOUND = "data\\TEXTURE\\title\\soundOption.png";	// �T�E���h���o���̃e�N�X�`��
	const std::string TEXTURE_PAD = "data\\TEXTURE\\title\\controllOption.png";	// �R���g���[���[���o���̃e�N�X�`��
	const std::string TEXTURE_SELECT[COptionMenu::ESelect::SELECT_MAX] =	// �I�����̃e�N�X�`��
	{
		"data\\TEXTURE\\title\\master.png",		// �}�X�^�[�{�����[��
		"data\\TEXTURE\\title\\SE.png"	,		// SE
		"data\\TEXTURE\\title\\BGM.png",		// BGM
		"data\\TEXTURE\\title\\vib.png",		// �o�C�u���[�V����
		"data\\TEXTURE\\title\\back.png",		// �߂�
		"data\\TEXTURE\\title\\reset.png",	// ���Z�b�g
	};
	const std::string TEXTURE_NUMBER = "data\\TEXTURE\\number\\school02.png";	// �����̃e�N�X�`��
	const std::string TEXTURE_PIN = "data\\TEXTURE\\title\\pin.png";		// �s���̃e�N�X�`��
	const std::string TEXTURE_VIB[2] =										// �o�C�u�t���O�̃e�N�X�`��
	{
		"data\\TEXTURE\\title\\vib_on.png",
		"data\\TEXTURE\\title\\vib_off.png",
	};
	const std::string TEXTURE_DESIGN01 = "data\\TEXTURE\\title\\design01.png";		// �f�U�C���̃e�N�X�`��
	const std::string TEXTURE_DESIGN02 = "data\\TEXTURE\\title\\design02.png";		// �f�U�C���̃e�N�X�`��
	const std::string TEXTURE_MARKER = "data\\TEXTURE\\title\\selectmarker.png";	// �I�����̃}�[�J�[�̃e�N�X�`��

	float SIZEHEIGHT_SELECT[COptionMenu::ESelect::SELECT_MAX] =	// �I�����̍����T�C�Y
	{
		45.0f,	// �}�X�^�[�{�����[��
		45.0f,	// SE
		45.0f,	// BGM
		45.0f,	// �o�C�u���[�V����
		55.0f,	// �߂�
		100.0f,	// ���Z�b�g
	};

	float SIZEHEIGHT_TITLE = 60.0f;	// ���o���̍����T�C�Y
	const MyLib::Vector3 POSITION_SOUNDTITLE = MyLib::Vector3(635.0f, 94.0f, 0.0f);	// �T�E���h���o���̍����T�C�Y
	const MyLib::Vector3 POSITION_PADTITLE = MyLib::Vector3(635.0f, 522.0f, 0.0f);	// �R���g���[���[���o���̍����T�C�Y
	const MyLib::Vector3 OFFSET_MARKER = MyLib::Vector3(-130.0f, 10.0f, 0.0f);	// �}�[�J�[�̃I�t�Z�b�g

	const MyLib::Vector3 POSITION_SELECT[COptionMenu::ESelect::SELECT_MAX] =	// �I�����̍����T�C�Y
	{
		MyLib::Vector3(500.0f, 215.0f, 0.0f),	// �}�X�^�[�{�����[��
		MyLib::Vector3(500.0f, 315.0f, 0.0f),	// SE
		MyLib::Vector3(500.0f, 415.0f, 0.0f),	// BGM
		MyLib::Vector3(500.0f, 620.0f, 0.0f),	// �o�C�u���[�V����
		MyLib::Vector3(150.0f, 653.0f, 0.0f),	// �߂�
		MyLib::Vector3(1100.0f, 615.0f, 0.0f),	// ���Z�b�g
	};

	const float NUMBER_POSITION_X = 730.0f;		// ������X��
	const float NUMBER_POSITION_Y = 12.0f;		// ������Y���Y��
	const D3DXCOLOR COLOR_NOTSELECT = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);	// �I�����ĂȂ��F
	const float MARKERTIME = 0.2f;	// �I���}�[�J�[�̎���
}

namespace StateTime
{
	const float SPAWN = 1.2f;	// �o��
	const float LOOP_ROTATION = 6.0f;	// ���[�v����
	const float LOOP = 1.0f;	// ���[�v����
	const float START = 1.0f;	// �J�n
}


//==========================================================================
// �֐��|�C���^
//==========================================================================
COptionMenu::STATE_FUNC COptionMenu::m_StateFunc[] =	// ��Ԋ֐�
{
	&COptionMenu::StateNone,		// �Ȃ�
	&COptionMenu::StateSelect,	// �I��
	&COptionMenu::StateEdit,	// �ҏW
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
COptionMenu::COptionMenu(int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_state			(EState::STATE_NONE),	// ���
	m_fStateTime	(0.0f),					// ��ԃ^�C�}�[
	m_select		(ESelect::SELECT_MASTERVOLUME),		// �I����
	m_bBack			(false),				// �߂�t���O
	m_fMarkerTime	(0.0f),					// �}�[�J�[�̃^�C�}�[
	m_pBoard		(nullptr),				// �{�[�h
	m_pPaper		(nullptr),				// ��
	m_pTitleSound	(nullptr),				// �T�E���h�̌��o��
	m_pTitlePad		(nullptr),				// �R���g���[���[�̌��o��
	m_pNumberMaster	(nullptr),				// �}�X�^�[�{�����[��
	m_pNumberSE		(nullptr),				// SE
	m_pNumberBGM	(nullptr),				// BGM
	m_pVibFrag		(nullptr),				// �o�C�u�t���O
	m_pPin			(nullptr),				// �s��
	m_pDesign_01	(nullptr),				// �f�U�C��01
	m_pDesign_02	(nullptr),				// �f�U�C��02
	m_pSelectMarker	(nullptr)				// �I�����̃}�[�J�[
{
	memset(m_pSelectUI, 0, sizeof(m_pSelectUI));	// �I������UI
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
COptionMenu::~COptionMenu()
{

}

//==========================================================================
// ��������
//==========================================================================
COptionMenu* COptionMenu::Create()
{
	// �������̊m��
	COptionMenu* pLogo = DEBUG_NEW COptionMenu;

	if (pLogo != nullptr)
	{
		// �N���X�̏�����
		if (FAILED(pLogo->Init()))
		{ // �������Ɏ��s�����ꍇ
			SAFE_UNINIT(pLogo);
			return nullptr;
		}
	}

	return pLogo;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT COptionMenu::Init()
{
	// �I�u�W�F�N�g�̎�ސݒ�
	CObject::SetType(CObject::TYPE::TYPE_NONE);

	// �{�[�h����
	if (FAILED(CreateBoard()))
	{
		return E_FAIL;
	}

	// ������
	if (FAILED(CreatePaper()))
	{
		return E_FAIL;
	}

	// ���o������
	if (FAILED(CreateTitle(&m_pTitleSound, TEXTURE_SOUND, POSITION_SOUNDTITLE)))
	{
		return E_FAIL;
	}
	if (FAILED(CreateTitle(&m_pTitlePad, TEXTURE_PAD, POSITION_PADTITLE)))
	{
		return E_FAIL;
	}

	// �I��������
	if (FAILED(CreateSelect()))
	{
		return E_FAIL;
	}

	// ��������
	if (FAILED(CreateNumber()))
	{
		return E_FAIL;
	}

	// �o�C�u�t���O����
	if (FAILED(CreateVibFrag()))
	{
		return E_FAIL;
	}

	// �s������
	if (FAILED(CreatePin()))
	{
		return E_FAIL;
	}

	// �f�U�C������
	if (FAILED(CreateDesign(&m_pDesign_01, TEXTURE_DESIGN01, MyLib::Vector3(1000.0f, 330.0f, 0.0f), MyLib::Vector3(0.0f, 0.0f, -D3DX_PI * 0.05f))))
	{
		return E_FAIL;
	}
	if (FAILED(CreateDesign(&m_pDesign_02, TEXTURE_DESIGN02, MyLib::Vector3(200.0f, 400.0f, 0.0f), MyLib::Vector3(0.0f, 0.0f, D3DX_PI * 0.08f))))
	{
		return E_FAIL;
	}

	// �}�[�J�[����
	if (FAILED(CreateSelectMarker()))
	{
		return E_FAIL;
	}

	// �I�����X�V
	UpdateSelect(0.0f, 0.0f, 0.0f);

	// �F�X�V
	m_fMarkerTime = MARKERTIME;
	UpdateColor(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//==========================================================================
// �{�[�h����
//==========================================================================
HRESULT COptionMenu::CreateBoard()
{
	// ��������
	m_pBoard = CObject2D::Create(GetPriority());
	if (m_pBoard == nullptr) return E_FAIL;

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pBoard->CObject::SetType(CObject::TYPE::TYPE_NONE);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_BOARD);
	m_pBoard->BindTexture(texID);

	// �T�C�Y�ݒ�
	m_pBoard->SetSize(MyLib::Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
	m_pBoard->SetSizeOrigin(m_pBoard->GetSize());

	// �ʒu�ݒ�
	m_pBoard->SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	return S_OK;
}

//==========================================================================
// ������
//==========================================================================
HRESULT COptionMenu::CreatePaper()
{
	// ��������
	m_pPaper = CObject2D::Create(GetPriority());
	if (m_pPaper == nullptr) return E_FAIL;

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pPaper->CObject::SetType(CObject::TYPE::TYPE_NONE);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_PAPER);
	m_pPaper->BindTexture(texID);

	// �摜�T�C�Y�擾
	MyLib::Vector2 size = pTexture->GetImageSize(texID);

	// �c�������ɃT�C�Y�v�Z
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SCREEN_HEIGHT * 0.5f);
	m_pPaper->SetSize(size);
	m_pPaper->SetSizeOrigin(size);

	// �ʒu�ݒ�
	m_pPaper->SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
	return S_OK;
}

//==========================================================================
// ���o������
//==========================================================================
HRESULT COptionMenu::CreateTitle(CObject2D** ppObj2D, const std::string& texture, const MyLib::Vector3& pos)
{
	// ��������
	(*ppObj2D) = CObject2D::Create(GetPriority());
	if ((*ppObj2D) == nullptr) return E_FAIL;

	// �I�u�W�F�N�g�̎�ސݒ�
	(*ppObj2D)->CObject::SetType(CObject::TYPE::TYPE_NONE);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(texture);
	(*ppObj2D)->BindTexture(texID);


	// �摜�T�C�Y�擾
	MyLib::Vector2 size = pTexture->GetImageSize(texID);

	// �c�������ɃT�C�Y�v�Z
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZEHEIGHT_TITLE);
	(*ppObj2D)->SetSize(size);
	(*ppObj2D)->SetSizeOrigin(size);

	// �ʒu�ݒ�
	(*ppObj2D)->SetPosition(pos);
	return S_OK;
}

//==========================================================================
// �I��������
//==========================================================================
HRESULT COptionMenu::CreateSelect()
{
	for (int i = 0; i < ESelect::SELECT_MAX; i++)
	{
		// ��������
		m_pSelectUI[i] = CObject2D::Create(GetPriority());
		if (m_pSelectUI[i] == nullptr) return E_FAIL;

		// �I�u�W�F�N�g�̎�ސݒ�
		m_pSelectUI[i]->CObject::SetType(CObject::TYPE::TYPE_NONE);

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
// ��������
//==========================================================================
HRESULT COptionMenu::CreateNumber()
{
	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_NUMBER);

	// �摜�T�C�Y�̏c�������ɃT�C�Y�v�Z
	MyLib::Vector2 size = pTexture->GetImageSize(texID);
	size.x *= 0.1f;
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZEHEIGHT_SELECT[ESelect::SELECT_MASTERVOLUME] * 0.8f);

	std::vector<CNumber*> vecNumber;	// �����̃I�u�W�F�N�g

	//--------------------------
	// �}�X�^�[�{�����[��
	//--------------------------
	// �I�����̈ꎞ�|�C���^
	CObject2D* pSelect = m_pSelectUI[ESelect::SELECT_MASTERVOLUME];

	// ����
	m_pNumberMaster = CMultiNumber::Create(
		50,
		texID,
		MyLib::Vector3(NUMBER_POSITION_X, pSelect->GetPosition().y + NUMBER_POSITION_Y, 0.0f),
		size,
		3,
		CMultiNumber::EObjType::OBJTYPE_2D,
		true,
		GetPriority());

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pNumberMaster->SetType(CObject::TYPE::TYPE_NONE);

	// �����̃I�u�W�F�N�g�擾
	vecNumber = m_pNumberMaster->GetNumberObj();

	for (const auto& num : vecNumber)
	{
		// �I�u�W�F�N�g�̎�ސݒ�
		num->SetType(CObject::TYPE::TYPE_NONE);
	}


	//--------------------------
	// SE
	//--------------------------
	// �I�����̈ꎞ�|�C���^
	pSelect = m_pSelectUI[ESelect::SELECT_SE];

	// ����
	m_pNumberSE = CMultiNumber::Create(
		50,
		texID,
		MyLib::Vector3(NUMBER_POSITION_X, pSelect->GetPosition().y + NUMBER_POSITION_Y, 0.0f),
		size,
		3,
		CMultiNumber::EObjType::OBJTYPE_2D,
		true,
		GetPriority());

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pNumberSE->SetType(CObject::TYPE::TYPE_NONE);

	// �����̃I�u�W�F�N�g�擾
	vecNumber = m_pNumberSE->GetNumberObj();

	for (const auto& num : vecNumber)
	{
		// �I�u�W�F�N�g�̎�ސݒ�
		num->SetType(CObject::TYPE::TYPE_NONE);
	}


	//--------------------------
	// BGM
	//--------------------------
	// �I�����̈ꎞ�|�C���^
	pSelect = m_pSelectUI[ESelect::SELECT_BGM];

	// ����
	m_pNumberBGM = CMultiNumber::Create(
		50,
		texID,
		MyLib::Vector3(NUMBER_POSITION_X, pSelect->GetPosition().y + NUMBER_POSITION_Y, 0.0f),
		size,
		3,
		CMultiNumber::EObjType::OBJTYPE_2D,
		true,
		GetPriority());

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pNumberBGM->SetType(CObject::TYPE::TYPE_NONE);

	// �����̃I�u�W�F�N�g�擾
	vecNumber = m_pNumberBGM->GetNumberObj();

	for (const auto& num : vecNumber)
	{
		// �I�u�W�F�N�g�̎�ސݒ�
		num->SetType(CObject::TYPE::TYPE_NONE);
	}

	return S_OK;
}

//==========================================================================
// �s������
//==========================================================================
HRESULT COptionMenu::CreatePin()
{
	// ��������
	m_pPin = CObject2D::Create(GetPriority());
	if (m_pPin == nullptr) return E_FAIL;

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pPin->CObject::SetType(CObject::TYPE::TYPE_NONE);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_PIN);
	m_pPin->BindTexture(texID);


	// �摜�T�C�Y�擾
	MyLib::Vector2 size = pTexture->GetImageSize(texID);

	// �c�������ɃT�C�Y�v�Z
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, 80.0f);
	m_pPin->SetSize(size);
	m_pPin->SetSizeOrigin(size);

	// �ʒu�ݒ�
	m_pPin->SetPosition(MyLib::Vector3(1150.0f, 130.0f, 0.0f));
	return S_OK;
}

//==========================================================================
// �o�C�u�t���O����
//==========================================================================
HRESULT COptionMenu::CreateVibFrag()
{
	// ��������
	m_pVibFrag = CObject2D::Create(GetPriority());
	if (m_pVibFrag == nullptr) return E_FAIL;

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pVibFrag->CObject::SetType(CObject::TYPE::TYPE_NONE);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_VIB[0]);
	m_pVibFrag->BindTexture(texID);


	// �摜�T�C�Y�擾
	MyLib::Vector2 size = pTexture->GetImageSize(texID);

	// �c�������ɃT�C�Y�v�Z
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZEHEIGHT_SELECT[ESelect::SELECT_MASTERVOLUME] * 0.8f);
	m_pVibFrag->SetSize(size);
	m_pVibFrag->SetSizeOrigin(size);

	// �ʒu�ݒ�
	m_pVibFrag->SetPosition(MyLib::Vector3(
		NUMBER_POSITION_X,
		m_pSelectUI[ESelect::SELECT_VIB]->GetPosition().y + NUMBER_POSITION_Y,
		0.0f));
	return S_OK;
}

//==========================================================================
// �f�U�C������
//==========================================================================
HRESULT COptionMenu::CreateDesign(CObject2D** ppObj2D, const std::string& texture, const MyLib::Vector3& pos, const MyLib::Vector3& rot)
{
	// ��������
	(*ppObj2D) = CObject2D::Create(GetPriority());
	if ((*ppObj2D) == nullptr) return E_FAIL;

	// �I�u�W�F�N�g�̎�ސݒ�
	(*ppObj2D)->CObject::SetType(CObject::TYPE::TYPE_NONE);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(texture);
	(*ppObj2D)->BindTexture(texID);


	// �摜�T�C�Y�擾
	MyLib::Vector2 size = pTexture->GetImageSize(texID);

	// �c�������ɃT�C�Y�v�Z
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, 200.0f);
	(*ppObj2D)->SetSize(size);
	(*ppObj2D)->SetSizeOrigin(size);

	// �ʒu�ݒ�
	(*ppObj2D)->SetPosition(pos);

	// �����ݒ�
	(*ppObj2D)->SetRotation(rot);
	return S_OK;
}

//==========================================================================
// �I�����̃}�[�J�[����
//==========================================================================
HRESULT COptionMenu::CreateSelectMarker()
{
	// ��������
	m_pSelectMarker = CObject2D::Create(GetPriority());
	if (m_pSelectMarker == nullptr) return E_FAIL;

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pSelectMarker->CObject::SetType(CObject::TYPE::TYPE_NONE);

	// �A���J�[�|�C���g�ݒ�
	m_pSelectMarker->SetAnchorType(CObject2D::AnchorPoint::LEFT);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_MARKER);
	m_pSelectMarker->BindTexture(texID);


	// �摜�T�C�Y�擾
	MyLib::Vector2 size = pTexture->GetImageSize(texID);

	// �c�������ɃT�C�Y�v�Z
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZEHEIGHT_SELECT[ESelect::SELECT_MASTERVOLUME] * 1.5f);
	m_pSelectMarker->SetSize(MyLib::Vector2(0.0f, size.y));
	m_pSelectMarker->SetSizeOrigin(size);

	// �ʒu�ݒ�
	m_pSelectMarker->SetPosition(m_pSelectUI[0]->GetPosition());
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void COptionMenu::Uninit()
{
	// �{�[�h�폜
	SAFE_UNINIT(m_pBoard);

	// ��
	SAFE_UNINIT(m_pPaper);

	// �}�[�J�[
	SAFE_UNINIT(m_pSelectMarker);

	// ���o��
	SAFE_UNINIT(m_pTitleSound);
	SAFE_UNINIT(m_pTitlePad);

	// �I����UI�폜
	for (int i = 0; i < ESelect::SELECT_MAX; i++)
	{
		SAFE_UNINIT(m_pSelectUI[i]);
	}

	// �����폜
	SAFE_KILL(m_pNumberMaster);
	SAFE_KILL(m_pNumberSE);
	SAFE_KILL(m_pNumberBGM);

	// �s��
	SAFE_UNINIT(m_pPin);

	// �f�U�C��
	SAFE_UNINIT(m_pDesign_01);
	SAFE_UNINIT(m_pDesign_02);

	// �o�C�u�̃t���O
	SAFE_UNINIT(m_pVibFrag);

	// �I�����̃}�[�J�[
	SAFE_UNINIT(m_pSelectMarker);

	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �폜����
//==========================================================================
void COptionMenu::Kill()
{
	// �{�[�h�폜
	SAFE_KILL(m_pBoard);

	// ��
	SAFE_KILL(m_pPaper);

	// �}�[�J�[
	SAFE_KILL(m_pSelectMarker);

	// ���o��
	SAFE_KILL(m_pTitleSound);
	SAFE_KILL(m_pTitlePad);

	// �I����UI�폜
	for (int i = 0; i < ESelect::SELECT_MAX; i++)
	{
		SAFE_KILL(m_pSelectUI[i]);
	}

	// �����폜
	SAFE_KILL(m_pNumberMaster);
	SAFE_KILL(m_pNumberSE);
	SAFE_KILL(m_pNumberBGM);

	// �s��
	SAFE_KILL(m_pPin);

	// �f�U�C��
	SAFE_KILL(m_pDesign_01);
	SAFE_KILL(m_pDesign_02);

	// �o�C�u�̃t���O
	SAFE_KILL(m_pVibFrag);

	// �I�����̃}�[�J�[
	SAFE_KILL(m_pSelectMarker);

	// ���g�̏I��
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void COptionMenu::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ��ԍX�V
	UpdateState(fDeltaTime, fDeltaRate, fSlowRate);

	// �}�[�J�[�X�V
	UpdateMarker(fDeltaTime, fDeltaRate, fSlowRate);

	// �I�u�W�F�N�g�X�V
	UpdateObj(fDeltaTime, fDeltaRate, fSlowRate);

#if _DEBUG
	// �f�o�b�O����
	Debug();
#endif
}

//==========================================================================
// �I�����X�V
//==========================================================================
void COptionMenu::UpdateSelect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// �p�b�h���
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();	// �L�[�{�[�h���

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
		pKey->GetTrigger(DIK_W))
	{// ��(�t���[�v)
		int nSelect = (m_select + (ESelect::SELECT_BACK - 1)) % ESelect::SELECT_BACK;
		m_select = (ESelect)nSelect;
		m_oldSelect = m_select;

		// �}�[�J�[�^�C�}�[���Z�b�g
		m_fMarkerTime = 0.0f;

		// ������
		WriteSound();
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
		pKey->GetTrigger(DIK_S))
	{// ��(���[�v)
		int nSelect = (m_select + 1) % ESelect::SELECT_BACK;
		m_select = (ESelect)nSelect;
		m_oldSelect = m_select;

		// �}�[�J�[�^�C�}�[���Z�b�g
		m_fMarkerTime = 0.0f;

		// ������
		WriteSound();
	}

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
		pKey->GetTrigger(DIK_A))
	{// ��

		// ���Z�b�g�Ȃ�ߋ��ցA����ȊO�͖߂��
		ESelect old = m_select;
		m_select = (m_select == ESelect::SELECT_RESET) ? m_oldSelect : ESelect::SELECT_BACK;

		if (old != m_select)
		{
			m_oldSelect = old;

			// �}�[�J�[�^�C�}�[���Z�b�g
			m_fMarkerTime = 0.0f;
		}
	}
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
		pKey->GetTrigger(DIK_D))
	{// �E

		// �߂�Ȃ�ߋ��ցA����ȊO�̓��Z�b�g��
		ESelect old = m_select;
		m_select = (m_select == ESelect::SELECT_BACK) ? m_oldSelect : ESelect::SELECT_RESET;
		
		if (old != m_select)
		{
			m_oldSelect = old;

			// �}�[�J�[�^�C�}�[���Z�b�g
			m_fMarkerTime = 0.0f;
		}
	}

	// �}�[�J�[
	m_pSelectMarker->SetPosition(OFFSET_MARKER + m_pSelectUI[m_select]->GetPosition());

	// �߂�t���O���Z�b�g
	m_bBack = false;

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0) ||
		pKey->GetTrigger(DIK_RETURN))
	{// ����

		// �߂�t���O�ݒ�
		m_bBack = m_select == ESelect::SELECT_BACK;

		switch (m_select)
		{
		case COptionMenu::SELECT_MASTERVOLUME:
		case COptionMenu::SELECT_SE:
		case COptionMenu::SELECT_BGM:
		case COptionMenu::SELECT_VIB:

			// �ҏW��Ԑݒ�
			SetState(EState::STATE_EDIT);
			break;

		case COptionMenu::SELECT_BACK:

			// �T�E���h�̍Đ�
			PLAY_SOUND(CSound::ELabel::LABEL_SE_OPTION_BACK);
			break;

		case COptionMenu::SELECT_RESET:

			// ���Z�b�g
			Reset();		
			break;

		default:
			assert(false);
			break;
		}
	}

}

//==========================================================================
// �}�[�J�[�X�V
//==========================================================================
void COptionMenu::UpdateMarker(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �}�[�J�[�̃^�C�}�[���Z
	m_fMarkerTime += fDeltaTime * fSlowRate;

	// �T�C�Y�擾
	MyLib::Vector2 size = m_pSelectMarker->GetSize();

	// �T�C�Y�ݒ�
	size.x = UtilFunc::Correction::EasingEaseOut(0.0f, m_pSelectMarker->GetSizeOrigin().x, 0.0f, MARKERTIME, m_fMarkerTime);
	m_pSelectMarker->SetSize(size);

	// UV�ݒ�
	std::vector<D3DXVECTOR2> uv = m_pSelectMarker->GetVecTexUV();
	uv[1].x = uv[3].x = size.x / m_pSelectMarker->GetSizeOrigin().x;
	m_pSelectMarker->SetTexUV(uv);
}

//==========================================================================
// �T�E���h�X�V
//==========================================================================
void COptionMenu::UpdateSound(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	CMultiNumber* pMultiNum = nullptr;
	switch (m_select)
	{
	case COptionMenu::SELECT_MASTERVOLUME:
		pMultiNum = m_pNumberMaster;
		break;

	case COptionMenu::SELECT_SE:
		pMultiNum = m_pNumberSE;
		break;

	case COptionMenu::SELECT_BGM:
		pMultiNum = m_pNumberBGM;
		break;
	}
	if (pMultiNum == nullptr) return;

	// ���͏��擾
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// �p�b�h���
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();	// �L�[�{�[�h���

	// �l�擾
	int num = pMultiNum->GetNum();

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
		pKey->GetTrigger(DIK_A))
	{// ��, ����

		// �l�ݒ�
		pMultiNum->SetNum(UtilFunc::Transformation::Clamp(num - 5, 0, 100));

		// �T�E���h�̍Đ�
		PLAY_SOUND(CSound::ELabel::LABEL_SE_ARROW_DOWN);
	}
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
		pKey->GetTrigger(DIK_D))
	{// �E, ����

		// �l�ݒ�
		pMultiNum->SetNum(UtilFunc::Transformation::Clamp(num + 5, 0, 100));

		// �T�E���h�̍Đ�
		PLAY_SOUND(CSound::ELabel::LABEL_SE_ARROW_UP);
	}
	num = pMultiNum->GetNum();

	// �ύX�����l�𔽉f
	CSound* pSound = CSound::GetInstance();
	float volume = ((float)num / 100.0f) * 2.0f;
	switch (m_select)
	{
	case COptionMenu::SELECT_MASTERVOLUME:
		pSound->VolumeChange(volume);
		break;

	case COptionMenu::SELECT_SE:
		pSound->VolumeChange(CSound::EType::TYPE_SE, volume);
		break;

	case COptionMenu::SELECT_BGM:
		pSound->VolumeChange(CSound::EType::TYPE_BGM, volume);
		break;
	}
}

//==========================================================================
// �o�C�u�t���O�X�V
//==========================================================================
void COptionMenu::UpdateVib(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���͏��擾
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// �p�b�h���
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();	// �L�[�{�[�h���

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
		pKey->GetTrigger(DIK_A) ||
		pKey->GetTrigger(DIK_D))
	{// ���E, �؂�ւ�

		// �o�C�u�t���O
		bool bVib = pPad->IsEnableVibration();

		// ���]
		pPad->SetEnableVibration(!bVib);

		// �U��������
		if (!bVib)
		{
			pPad->SetVibration(CInputGamepad::EVibType::VIBTYPE_THROW_FAST, 0);
		}

		// �T�E���h�̍Đ�
		PLAY_SOUND(CSound::ELabel::LABEL_SE_ARROW);
	}
	
	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_VIB[!pPad->IsEnableVibration()]);
	m_pVibFrag->BindTexture(texID);
}

//==========================================================================
// �F�X�V
//==========================================================================
void COptionMenu::UpdateColor(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	float color = UtilFunc::Correction::EasingLinear(COLOR_NOTSELECT.r, 1.0f, 0.0f, MARKERTIME, m_fMarkerTime);
	D3DXCOLOR selectColor = D3DXCOLOR(color, color, color, 1.0f);

	// �I������UI
	for (const auto& select : m_pSelectUI)
	{
		select->SetColor(COLOR_NOTSELECT);
	}
	m_pSelectUI[m_select]->SetColor(selectColor);

	// �}�[�J�[
	m_pSelectMarker->SetColor(selectColor);

	// �o�C�u�t���O
	m_pVibFrag->SetColor(COLOR_NOTSELECT);

	// ����
	m_pNumberMaster->SetColor(COLOR_NOTSELECT);
	m_pNumberSE->SetColor(COLOR_NOTSELECT);
	m_pNumberBGM->SetColor(COLOR_NOTSELECT);

	switch (m_select)
	{
	case COptionMenu::SELECT_MASTERVOLUME:
		m_pNumberMaster->SetColor(selectColor);
		break;

	case COptionMenu::SELECT_SE:
		m_pNumberSE->SetColor(selectColor);
		break;

	case COptionMenu::SELECT_BGM:
		m_pNumberBGM->SetColor(selectColor);
		break;

	case ESelect::SELECT_VIB:
		m_pVibFrag->SetColor(selectColor);
		break;

	default:
		m_pSelectMarker->SetColor(D3DXCOLOR(MyLib::color::White(0.0f)));
		break;
	}
}

//==========================================================================
// �I�u�W�F�N�g�X�V
//==========================================================================
void COptionMenu::UpdateObj(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �{�[�h�̍X�V
	assert(m_pBoard != nullptr);
	m_pBoard->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// ��
	m_pPaper->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �}�[�J�[
	m_pSelectMarker->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// ���o��
	m_pTitleSound->Update(fDeltaTime, fDeltaRate, fSlowRate);
	m_pTitlePad->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �I������UI
	for (const auto& select : m_pSelectUI)
	{
		assert(select != nullptr);
		select->Update(fDeltaTime, fDeltaRate, fSlowRate);
	}

	// ����
	m_pNumberMaster->Update(fDeltaTime, fDeltaRate, fSlowRate);
	m_pNumberSE->Update(fDeltaTime, fDeltaRate, fSlowRate);
	m_pNumberBGM->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �o�C�u�̃t���O
	m_pVibFrag->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �s��
	m_pPin->Update(fDeltaTime, fDeltaRate, fSlowRate);

	// �f�U�C��
	m_pDesign_01->Update(fDeltaTime, fDeltaRate, fSlowRate);
	m_pDesign_02->Update(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void COptionMenu::UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �^�C�}�[�X�V
	m_fStateTime += fDeltaTime * fDeltaRate * fSlowRate;

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �I��
//==========================================================================
void COptionMenu::StateSelect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �I�����X�V
	UpdateSelect(fDeltaTime, fDeltaRate, fSlowRate);

	// �F�X�V
	UpdateColor(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// �ҏW
//==========================================================================
void COptionMenu::StateEdit(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// ���͏��擾
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// �p�b�h���
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();	// �L�[�{�[�h���

	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_B, 0) ||
		pKey->GetTrigger(DIK_BACKSPACE))
	{// �L�����Z��

		// �ҏW��Ԑݒ�
		SetState(EState::STATE_SELECT);
		return;
	}

	// �T�E���h�X�V
	switch (m_select)
	{
	case COptionMenu::SELECT_MASTERVOLUME:
	case COptionMenu::SELECT_SE:
	case COptionMenu::SELECT_BGM:
		UpdateSound(fDeltaTime, fDeltaRate, fSlowRate);
		break;

	case COptionMenu::SELECT_VIB:
		UpdateVib(fDeltaTime, fDeltaRate, fSlowRate);
		break;

	default:
		break;
	}

	// �F�X�V
	UpdateColor(fDeltaTime, fDeltaRate, fSlowRate);
}

//==========================================================================
// ������
//==========================================================================
void COptionMenu::WriteSound()
{
	if (m_select == ESelect::SELECT_BACK) return;

	// �T�E���h�̍Đ�
	int soundLabel = UtilFunc::Transformation::Random(CSound::ELabel::LABEL_SE_WRITE01, CSound::ELabel::LABEL_SE_WRITE02);
	PLAY_SOUND((CSound::ELabel)soundLabel);
}

//==========================================================================
// ���Z�b�g
//==========================================================================
void COptionMenu::Reset()
{
	// ���͏��擾
	CInputGamepad* pPad = CInputGamepad::GetInstance();		// �p�b�h���

	// �o�C�uON
	pPad->SetEnableVibration(true);
	pPad->SetVibration(CInputGamepad::EVibType::VIBTYPE_THROW_FAST, 0);

	// �\���ؑ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_VIB[!pPad->IsEnableVibration()]);
	m_pVibFrag->BindTexture(texID);

	// �������Z�b�g
	m_pNumberMaster->SetNum(50);
	m_pNumberSE->SetNum(50);
	m_pNumberBGM->SetNum(50);

	// 1.0f�ɂ���
	CSound* pSound = CSound::GetInstance();
	pSound->VolumeChange(1.0f);
	pSound->VolumeChange(CSound::EType::TYPE_SE, 1.0f);
	pSound->VolumeChange(CSound::EType::TYPE_BGM, 1.0f);
}

//==========================================================================
// �`�揈��
//==========================================================================
void COptionMenu::Draw()
{
	// �{�[�h�̕`��
	assert(m_pBoard != nullptr);
	m_pBoard->Draw();

	// ��
	m_pPaper->Draw();

	// �}�[�J�[
	m_pSelectMarker->Draw();

	// ���o���`��
	m_pTitleSound->Draw();
	m_pTitlePad->Draw();

	// �I������UI
	for (const auto& select : m_pSelectUI)
	{
		assert(select != nullptr);
		select->Draw();
	}

	// ����
	m_pNumberMaster->Draw();
	m_pNumberSE->Draw();
	m_pNumberBGM->Draw();

	// �o�C�u�̃t���O
	m_pVibFrag->Draw();

	// �s��
	m_pPin->Draw();

	// �f�U�C��
	m_pDesign_01->Draw();
	m_pDesign_02->Draw();
}

//==========================================================================
// �f�o�b�O����
//==========================================================================
void COptionMenu::Debug()
{
	if (ImGui::TreeNode("OptionMenu"))
	{
		// �I������UI
		for (int i = 0; i < ESelect::SELECT_MAX; i++)
		{
			if (ImGui::TreeNode(magic_enum::enum_name((ESelect)i).data()))
			{
				//--------------------------
				// �T�C�Y�ݒ�
				//--------------------------
				// �摜�T�C�Y�擾
				CTexture* pTexture = CTexture::GetInstance();
				MyLib::Vector2 size = pTexture->GetImageSize(m_pSelectUI[i]->GetIdxTexture());

				// �c�������ɃT�C�Y�v�Z
				std::string str = "SIZEHEIGHT_SELECT[" + std::to_string(i) + "]";
				ImGui::DragFloat(str.c_str(), &SIZEHEIGHT_SELECT[i], 0.1f, 0.0f, 0.0f, "%.2f");

				size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZEHEIGHT_SELECT[i]);
				m_pSelectUI[i]->SetSize(MyLib::Vector2(size.x, size.y));

				//--------------------------
				// �ʒu�ݒ�
				//--------------------------
				str = "SIZEHEIGHT_SELECT[" + std::to_string(i) + "]";
				ImGui::DragFloat3(str.c_str(), (float*)&POSITION_SELECT[i], 1.0f, 0.0f, 0.0f, "%.2f");
				m_pSelectUI[i]->SetPosition(POSITION_SELECT[i]);


				//--------------------------
				// �F
				//--------------------------
				str = "Color[" + std::to_string(i) + "]";

				D3DXCOLOR col = m_pSelectUI[i]->GetColor();
				ImVec4 editCol = ImVec4(col.r, col.g, col.b, col.a);
				ImGui::ColorEdit4(str.c_str(), (float*)&editCol);
				m_pSelectUI[i]->SetColor(D3DXCOLOR(editCol.x, editCol.y, editCol.z, editCol.w));

				ImGui::TreePop();
			}
		}

		// ���o��
		if (ImGui::TreeNode("itle"))
		{
			//--------------------------
			// �T�C�Y�ݒ�
			//--------------------------
			// �摜�T�C�Y�擾
			CTexture* pTexture = CTexture::GetInstance();
			MyLib::Vector2 size = pTexture->GetImageSize(m_pTitleSound->GetIdxTexture());

			// �c�������ɃT�C�Y�v�Z
			ImGui::DragFloat("SIZEHEIGHT_SOUNDTITLE]", &SIZEHEIGHT_TITLE, 0.1f, 0.0f, 0.0f, "%.2f");
			size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZEHEIGHT_TITLE);
			m_pTitleSound->SetSize(size);
			m_pTitlePad->SetSize(size);


			if (ImGui::TreeNode("SoundTitle"))
			{
				//--------------------------
				// �ʒu�ݒ�
				//--------------------------
				ImGui::DragFloat3("pos", (float*)&POSITION_SOUNDTITLE, 1.0f, 0.0f, 0.0f, "%.2f");
				m_pTitleSound->SetPosition(POSITION_SOUNDTITLE);

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("PadTitle"))
			{
				//--------------------------
				// �ʒu�ݒ�
				//--------------------------
				ImGui::DragFloat3("pos", (float*)&POSITION_PADTITLE, 1.0f, 0.0f, 0.0f, "%.2f");
				m_pTitlePad->SetPosition(POSITION_PADTITLE);

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}


		ImGui::TreePop();
	}

	
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void COptionMenu::SetState(EState state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// �I�����ݒ�
//==========================================================================
void COptionMenu::SetSelect(ESelect select)
{
	m_oldSelect = m_select;
	m_select = select;
}
