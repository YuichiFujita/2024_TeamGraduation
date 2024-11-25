//==========================================================================
// 
//  �v���C���[�}�[�J�[���� [playerMarker.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerMarker.h"
#include "gameManager.h"
#include "player.h"

// TODO�F�v���C���[�}�[�J�[�\��
#if 1
#define DISP
#endif

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\ballmarker\\circle.png";	// �~�̃e�N�X�`��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CPlayerMarker::CPlayerMarker(int nPriority) : CObjectBillboardAnim(nPriority),
	m_pPlayer	(nullptr)	// �e�v���C���[���
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CPlayerMarker::~CPlayerMarker()
{

}

//==========================================================================
// ��������
//==========================================================================
CPlayerMarker* CPlayerMarker::Create(CPlayer* pPlayer)
{
	// �������̊m��
	CPlayerMarker* pMarker = DEBUG_NEW CPlayerMarker;
	if (pMarker != nullptr)
	{
		// �v���C���[����
		pMarker->m_pPlayer = pPlayer;

		// �N���X�̏�����
		if (FAILED(pMarker->Init()))
		{ // �������Ɏ��s�����ꍇ

			SAFE_UNINIT(pMarker);
			return nullptr;
		}
	}

	return pMarker;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CPlayerMarker::Init()
{
	// �I�u�W�F�N�g�̎�ސݒ�
	CObject::SetType(CObject::TYPE::TYPE_OBJECTBILLBOARD);

	// �e�N���X�̏�����
	HRESULT hr = CObjectBillboardAnim::Init();
	if (FAILED(hr)) { return E_FAIL; }

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(nTexID);

	// �e�N�X�`���̑傫�����擾
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);

	// ���������ɃT�C�Y��ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 50.0f);
	SetSize(MyLib::Vector2(size.x, size.y));
	SetSizeOrigin(GetSize());

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CPlayerMarker::Uninit()
{
	// �e�N���X�̏I��
	CObjectBillboardAnim::Uninit();
}

//==========================================================================
// �폜����
//==========================================================================
void CPlayerMarker::Kill()
{
	// ���g�̏I��
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CPlayerMarker::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
#ifdef DISP
	m_pPlayer->GetMyPlayerIdx();

	// �v���C���[���Ȃ��ƕ`��؂�
	bool bDisp = (m_pPlayer == nullptr) ? false : true;
	SetEnableDisp(bDisp);

	// �`�悵�Ȃ��ꍇ�͔�����
	if (!bDisp) { return; }

	// �v���C���[�̓���Ɉʒu��ݒ�
	MyLib::Vector3 pos = m_pPlayer->GetPosition();
	pos.y += 160.0f;

	// �ʒu�̔��f
	SetPosition(pos);

	// �e�N���X�̍X�V
	CObjectBillboardAnim::Update(fDeltaTime, fDeltaRate, fSlowRate);
#else
	SetEnableDisp(false);
#endif
}

//==========================================================================
// �`�揈��
//==========================================================================
void CPlayerMarker::Draw()
{
	// �e�N���X�̕`��
	CObjectBillboardAnim::Draw();
}
