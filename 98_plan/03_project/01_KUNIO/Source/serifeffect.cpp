//=============================================================================
// 
//  �˗��l�Z���t�G�t�F�N�g [serifeffect.cpp]
//  Author : �Ό��D�n
// 
//=============================================================================
#include "serifeffect.h"
#include "texture.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string EFFECT_TEXTURE = "data\\TEXTURE\\effect\\voice_effect_00.png";
	const float EFFECT_DEF_SIZE = 20.0f;
	const float EFFECT_MAX_SIZE = EFFECT_DEF_SIZE * 1.8f;
	const float MOVE = 50.0f;
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CSerifEffect::CSerifEffect(int nPriority) : CObject3D(nPriority)
{
	m_Life = 0;
	m_MaxLife = 0;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CSerifEffect::~CSerifEffect()
{

}

//==========================================================================
// ��������
//==========================================================================
CSerifEffect* CSerifEffect::Create(MyLib::Vector3 pos, MyLib::Vector3 rot, int life)
{
	// �������̊m��
	CSerifEffect* pObject = DEBUG_NEW CSerifEffect;

	if (pObject != nullptr)
	{
		pObject->m_MaxLife = life;
		pObject->m_Life = pObject->m_MaxLife;
		pObject->SetPosition(pos);
		pObject->SetOriginPosition(pos);
		pObject->SetRotation(rot);

		// ����������
		pObject->Init();
	}

	return pObject;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CSerifEffect::Init()
{
	CObject3D::Init();

	SetType(CObject::TYPE::TYPE_OBJECT3D);

	BindTexture(CTexture::GetInstance()->Regist(EFFECT_TEXTURE));
	SetSize(MyLib::Vector3(EFFECT_DEF_SIZE, EFFECT_DEF_SIZE,0.0f));
	SetVtx();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CSerifEffect::Uninit()
{
	CObject3D::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CSerifEffect::Kill()
{
	
}

//==========================================================================
// �X�V����
//==========================================================================
void CSerifEffect::Update()
{
	// ���C�t�s���������
	m_Life--;
	if (m_Life <= 0)
	{
		Uninit();
		return;
	}

	// �i�s�x�v�Z
	float progress = 1.0f - static_cast<float>(m_Life) / static_cast<float>(m_MaxLife);

	// �ړ�
	MyLib::Vector3 rot = GetRotation();
	float move = UtilFunc::Correction::EasingEaseOut(0.0f, MOVE, progress);
	MyLib::Vector3 vecMove = MyLib::Vector3();
	vecMove.x = -move * cosf(rot.z);
	vecMove.y = -move * sinf(rot.z);

	// �g��
	float size = UtilFunc::Correction::EasingEaseOut(EFFECT_DEF_SIZE, EFFECT_MAX_SIZE, progress);

	// �t�F�[�h�A�E�g
	float alpha = UtilFunc::Correction::EasingEaseIn(1.0f, 0.0f, progress);

	
	// �ݒ�
	D3DXCOLOR col = GetColor();
	col.a = alpha;
	SetPosition(GetOriginPosition() + vecMove);
	SetSize(size);
	SetColor(col);

	CObject3D::Update();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CSerifEffect::Draw()
{
	CObject3D::Draw();
}
