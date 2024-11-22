//==========================================================================
// 
//  �^�C�g�����S���� [titleLogo.cpp]
//  Author : Kai Takada
// 
//==========================================================================
#include "titleLogo.h"
#include "object2D.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_MAIN = "data\\TEXTURE\\title\\titlelogo4.png";	// ��̃e�N�X�`��
	const std::string TEXTURE_SUB = "data\\TEXTURE\\title\\player.png";			// ���̃e�N�X�`��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTitleLogo::CTitleLogo(int nPriority, const LAYER layer) : CObject(nPriority, layer),
	m_fTime(0.0f)	// ���̃^�C�}�[
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTitleLogo::~CTitleLogo()
{

}

//==========================================================================
// ��������
//==========================================================================
CTitleLogo* CTitleLogo::Create()
{
	// �������̊m��
	CTitleLogo* pLogo = DEBUG_NEW CTitleLogo;

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
HRESULT CTitleLogo::Init()
{
	// �I�u�W�F�N�g�̎�ސݒ�
	CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �吶��
	if (FAILED(CreateMain()))
	{
		return E_FAIL;
	}

	// ������
	if (FAILED(CreateSub()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �吶��
//==========================================================================
HRESULT CTitleLogo::CreateMain()
{
	// ��������
	m_pMain = CObject2D::Create(GetPriority());
	if (m_pMain == nullptr) return E_FAIL;

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pMain->CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_MAIN);
	m_pMain->BindTexture(texID);

	// �T�C�Y�ݒ�
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, SCREEN_WIDTH * 0.5f);
	m_pMain->SetSize(MyLib::Vector2(size.x, size.y));
	m_pMain->SetSizeOrigin(m_pMain->GetSize());
	
	// �ʒu�ݒ�
	m_pMain->SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	return S_OK;
}

//==========================================================================
// ������
//==========================================================================
HRESULT CTitleLogo::CreateSub()
{
	// ��������
	m_pSub = CObject2D::Create(GetPriority());
	if (m_pSub == nullptr) return E_FAIL;

	// �I�u�W�F�N�g�̎�ސݒ�
	m_pSub->CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���ݒ�
	CTexture* pTexture = CTexture::GetInstance();
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SUB);
	m_pSub->BindTexture(texID);

	// �T�C�Y�ݒ�
	MyLib::Vector2 size = CTexture::GetInstance()->GetImageSize(texID);

	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, SCREEN_WIDTH * 0.2f);
	m_pSub->SetSize(MyLib::Vector2(size.x, size.y));
	m_pSub->SetSizeOrigin(m_pSub->GetSize());

	// �ʒu�ݒ�
	m_pSub->SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.3f, 0.0f));

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTitleLogo::Uninit()
{
	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �폜����
//==========================================================================
void CTitleLogo::Kill()
{
	// ���g�̏I��
	Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTitleLogo::Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �|���S���X�V
	UpdateMain();
	UpdateSub();

	// �^�C�}�[�X�V
	m_fTime += fDeltaTime * fDeltaRate * fSlowRate;
	m_fTime = UtilFunc::Transformation::Clamp(m_fTime, 0.0f, 1.0f);
	if (m_fTime >= 1.0f)
	{
		m_fTime = 0.0f;
	}

#if _DEBUG
	ImGui::Text("Time: %f", m_fTime);
#endif
}

//==========================================================================
// ��X�V
//==========================================================================
void CTitleLogo::UpdateMain()
{
	//// �T�C�Y�g�k
	//MyLib::Vector2 size = MyLib::Vector2();
	//size = UtilFunc::Transformation::AdjustSizeByWidth(size, SCREEN_WIDTH * 0.2f);

	//// �T�C�Y�ݒ�
	//m_pMain->SetSize(MyLib::Vector2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f));
}

//==========================================================================
// ���X�V
//==========================================================================
void CTitleLogo::UpdateSub()
{
	// �ʒu�h��
	MyLib::Vector3 pos = m_pSub->GetPosition();
	//MyLib::Vector3 pos = m_pSub->GetOriginPosition();
	MyLib::Vector3 posDef = MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);

	pos = posDef * m_fTime;

	// �ʒu�ݒ�
	m_pSub->SetPosition(pos);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CTitleLogo::Draw()
{

}

//==========================================================================
// �`��󋵂̐ݒ菈��
//==========================================================================
void CTitleLogo::SetEnableDisp(bool bDisp)
{
	// ���N���X�̕`��󋵐ݒ�
	CObject::SetEnableDisp(bDisp);

	// ��ƕ��̕`��󋵐ݒ�
	m_pMain->SetEnableDisp(bDisp);
	m_pSub->SetEnableDisp(bDisp);
}
