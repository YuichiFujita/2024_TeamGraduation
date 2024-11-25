//==========================================================================
// 
//  �v���C���[�}�[�J�[���� [playerMarker.cpp]
//  Author : ���c�E��
// 
//==========================================================================
#include "playerMarker.h"
#include "manager.h"
#include "renderer.h"
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
	const std::string TEXTURE = "data\\TEXTURE\\playerMarker000.png";	// �v���C���[�}�[�J�[�e�N�X�`��
	const MyLib::PosGrid2 PTRN = MyLib::PosGrid2(4, 1);	// �e�N�X�`��������
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
	HRESULT hr = CObjectBillboardAnim::Init(PTRN.x, PTRN.y, 0.0f, false);
	if (FAILED(hr)) { return E_FAIL; }

	// �����Đ���OFF�ɂ���
	SetEnableAutoPlay(false);

	// �e�N�X�`���̊���
	CTexture* pTexture = CTexture::GetInstance();
	int nTexID = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(nTexID);

	// �e�N�X�`���̑傫�����擾
	MyLib::Vector2 size = pTexture->GetImageSize(nTexID);

	// ���������ɃT�C�Y��ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 160.0f);
	size.x /= (float)PTRN.x;
	SetSize(size);
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
	// ���쌠�C���f�b�N�X�̎擾
	int nPadIdx = m_pPlayer->GetMyPlayerIdx();

	// ���쌠���Ȃ��ꍇ�͕`��OFF
	bool bDisp = (nPadIdx <= -1) ? false : true;
	SetEnableDisp(bDisp);

	// �`�悵�Ȃ��ꍇ�͔�����
	if (!bDisp) { return; }

	// �v���C���[�̓���Ɉʒu��ݒ�
	MyLib::Vector3 pos = m_pPlayer->GetPosition();
	pos.y += 200.0f;

	// ���݂̑��쌠�ɍ��킹���p�^�[���ɕύX
	SetPatternAnim(nPadIdx);

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
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;	// �f�o�C�X���

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �e�N���X�̕`��
	CObjectBillboardAnim::Draw();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
