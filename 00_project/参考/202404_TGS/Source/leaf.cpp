//=============================================================================
// 
//  �t���Ϗ��� [leaf.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "leaf.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "game.h"
#include "course.h"

// �h���N���X
#include "leaf_flow.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\effect\\leaf.png";	// �e�N�X�`���̃t�@�C��
}

namespace StateTime	// ��ԕʎ���
{
	const float WAIT = 0.2f;	// �ҋ@
	const float FADEIN = 0.5f;	// �t�F�[�h�C��
	const float FADEOUT = 0.3f;	// �t�F�[�h�A�E�g
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CLeaf::STATE_FUNC CLeaf::m_StateFunc[] =
{
	&CLeaf::StateFall,		// ����
	&CLeaf::StateWait,		// �ҋ@
	&CLeaf::StateFadeIn,	// �t�F�[�h�C��
	&CLeaf::StateFadeOut,	// �t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CLeaf::CLeaf(int nPriority, const LAYER layer) : CObject3D(nPriority, layer)
{
	// �l�̃N���A
	m_fStateTimer = 0.0f;			// ��ԃJ�E���^�[
	m_state = State::STATE_FALL;	// ���
	m_rotStart = MyLib::Vector3();	// �J�n���̌���
	m_rotDest = MyLib::Vector3();	// �ڕW�̌���
	m_fFallWidth = 0.0f;			// ������
	m_fWidthMoveTimer = 0.0f;		// ���ړ��̃^�C�}�[
	m_fRotateTimer = 0.0f;			// ��]�܂ł̎���
	m_fRotateInterval = 0.0f;		// ��]�܂ł̊Ԋu
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CLeaf::~CLeaf()
{

}

//==========================================================================
// ��������
//==========================================================================
CLeaf* CLeaf::Create(const MyLib::Vector3& pos, const Type& type)
{
	// �������̊m��
	CLeaf* pObj = nullptr;

	switch (type)
	{
	case Type::TYPE_FALL:
		pObj = DEBUG_NEW CLeaf;
		break;

	case Type::TYPE_FLOW:
		pObj = DEBUG_NEW CLeafFlow;
		break;

	default:
		return nullptr;
		break;
	}

	if (pObj == nullptr)
	{// ���s
		return nullptr;
	}

	// �������ݒ�
	pObj->SetPosition(pos);
	pObj->SetOriginPosition(pos);

	// ����������
	pObj->Init();

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CLeaf::Init()
{

	// �I�u�W�F�N�g3D�̏�����
	CObject3D::Init();

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// �c�������ɃT�C�Y�ݒ�
	float randSize = UtilFunc::Transformation::Random(200, 400) * 0.1f;
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, randSize);

	MyLib::Vector3 setsize = MyLib::Vector3(size.x, size.y, 0.0f);
	SetSize(setsize);
	SetSizeOrigin(setsize);

	// ����
	MyLib::Vector3 rot;
	rot.y = UtilFunc::Transformation::Random(-31, 31) * 0.1f;
	rot.x = UtilFunc::Transformation::Random(-31, 31) * 0.01f;
	rot.z = UtilFunc::Transformation::Random(-31, 31) * 0.01f;
	SetRotation(rot);

	// �ڕW�̌���
	CalDestRotation();

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);


	// ���
	m_fRotateInterval = UtilFunc::Transformation::Random(60, 90) * 0.01f;
	m_fFallWidth = static_cast<float>(UtilFunc::Transformation::Random(80, 150));	// ������
	m_state = State::STATE_FADEIN;	// ���

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CLeaf::Uninit()
{
	// �I������
	CObject3D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CLeaf::Update()
{
	// ��ԍX�V
	UpdateState();
	if (IsDeath()) return;

	// �X�V����
	CObject3D::Update();
}

//==========================================================================
// �ڕW�̌����v�Z
//==========================================================================
void CLeaf::CalDestRotation()
{
	// �����擾
	MyLib::Vector3 rot = GetRotation();

	// �ڕW�̌���
	m_rotDest.y = rot.y + UtilFunc::Transformation::Random(11, 31) * 0.01f;
	m_rotDest.x = rot.x + UtilFunc::Transformation::Random(21, 51) * 0.01f;
	m_rotDest.z = rot.z + UtilFunc::Transformation::Random(21, 51) * 0.01f;
	UtilFunc::Transformation::RotNormalize(m_rotDest);

	// �J�n���̌���
	m_rotStart = rot;
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CLeaf::UpdateState()
{
	// ��ԃ^�C�}�[
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// ����
//==========================================================================
void CLeaf::StateFall()
{
	MyLib::Vector3 pos = GetPosition(), posOrigin = GetOriginPosition();
	MyLib::Vector3 rot = GetRotation();

	// ��]�܂ł̎��ԉ��Z
	m_fRotateTimer += CManager::GetInstance()->GetDeltaTime();

	// ���ړ��̃^�C�}�[
	m_fWidthMoveTimer += CManager::GetInstance()->GetDeltaTime();


	// �ړ�
	float ratio = sinf(D3DX_PI * (1.0f * (m_fWidthMoveTimer / m_fRotateInterval)));
	float len = ratio * m_fFallWidth;
	pos.x = posOrigin.x + sinf(rot.y) * len;
	pos.z = posOrigin.z + cosf(rot.y) * len;


	// ��]���`���
	rot = UtilFunc::Correction::EasingEaseOut(m_rotStart, m_rotDest, 0.0f, m_fRotateInterval, m_fRotateTimer);
	UtilFunc::Transformation::RotNormalize(rot);
	SetRotation(rot);

	// ����
	pos.y -= UtilFunc::Transformation::Random(150, 190) * 0.01f;

	// ���Ԍo�߁A�V�����ڕW�����Z�o
	if (m_fRotateTimer >= m_fRotateInterval)
	{
		m_fRotateTimer = 0.0f;

		// �ڕW�̌���
		CalDestRotation();

		// ������
		m_fFallWidth = static_cast<float>(UtilFunc::Transformation::Random(80, 150));
	}

	// ���ݒ�
	SetPosition(pos);

	// ���n����
	CheckLand();
}

//==========================================================================
// ���n�m�F
//==========================================================================
void CLeaf::CheckLand()
{
	bool IsLand = false;


	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	// �Q�[���擾
	CGame* pGame = CGame::GetInstance();

	float height = 0.0f;
	if (pGame != nullptr)
	{
		// �R�[�X�擾
		CCourse* pCourse = pGame->GetCourse();
		if (pCourse == nullptr) return;

		// �����擾
		height = pCourse->GetHeight(pos, &IsLand);
	}

	if (pos.y <= 0.0f)
	{
		// ���ݒ�
		pos.y = height;
		SetPosition(pos);

		// �t�F�[�h�A�E�g�ɐݒ�
		m_state = State::STATE_FADEOUT;
		m_fStateTimer = 0.0f;
	}
}

//==========================================================================
// �ҋ@
//==========================================================================
void CLeaf::StateWait()
{
	MyLib::Vector3 pos = GetPosition(), posOrigin = GetOriginPosition();
	pos = UtilFunc::Correction::EasingEaseOut(posOrigin, posOrigin + MyLib::Vector3(0.0f, -10.0f, 0.0f), 0.0f, StateTime::WAIT, m_fStateTimer);
	SetPosition(pos);

	if (m_fStateTimer >= StateTime::WAIT)
	{// ���Ԍo��

		m_state = State::STATE_FALL;
		m_fStateTimer = 0.0f;

		SetOriginPosition(pos);
	}
}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CLeaf::StateFadeIn()
{
	// �s�����x�ݒ�
	float alpha = m_fStateTimer / StateTime::FADEIN;
	SetAlpha(alpha);

	// �~���͍s��
	StateFall();

	if (m_fStateTimer >= StateTime::FADEIN)
	{// ���Ԍo��

		// �s�����x�ݒ�
		SetAlpha(1.0f);

		// ���
		m_state = State::STATE_FALL;
		m_fStateTimer = 0.0f;
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CLeaf::StateFadeOut()
{
	float alpha = 1.0f - (m_fStateTimer / StateTime::FADEOUT);
	SetAlpha(alpha);

	if (m_fStateTimer >= StateTime::FADEOUT)
	{// ���Ԍo��

		Uninit();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CLeaf::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �w�ʂ̃J�����O�Ȃ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �`�揈��
	CObject3D::Draw();

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �J�����O�f�t�H���g
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
