//=============================================================================
// 
//  �t���Ϗ��� [leaf_flow.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "leaf_flow.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "game.h"
#include "course.h"
#include "spline.h"

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
	const float FADEOUT = 0.3f;	// �t�F�[�h�A�E�g
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CLeafFlow::STATE_FUNC CLeafFlow::m_StateFunc[] =
{
	&CLeafFlow::StateFlow,		// ����
	&CLeafFlow::StateFadeOut,	// �t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CLeafFlow::CLeafFlow(int nPriority) : CLeaf(nPriority)
{
	// �l�̃N���A
	m_fStateTimer = 0.0f;			// ��ԃJ�E���^�[
	m_state = CLeafFlow::State::STATE_FLOW;	// ���
	m_fMoveLen = 0.0f;	// �ړ�����
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CLeafFlow::~CLeafFlow()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CLeafFlow::Init()
{

	// �t���ς̏�����
	CLeaf::Init();

	// ����
	MyLib::Vector3 rot;
	rot.y = UtilFunc::Transformation::Random(-31, 31) * 0.1f;
	rot.x = D3DX_PI * 0.5f;
	SetRotation(rot);

	m_fMoveLen = 0.0f;	// �ړ�����

	return S_OK;
}

//==========================================================================
// �X�V����
//==========================================================================
void CLeafFlow::Update()
{
	// ��ԍX�V
	UpdateState();
	if (IsDeath()) return;

	// �X�V����
	CObject3D::Update();
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CLeafFlow::UpdateState()
{
	// ��ԃ^�C�}�[
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// ����
//==========================================================================
void CLeafFlow::StateFlow()
{
	MyLib::Vector3 pos = GetPosition();

	// �R�[�X�擾
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	int courseIdx = pCourse->GetMyVtxIndex(pos);

	// �ʒu�v�Z
	m_fMoveLen += 5.0f;

	MyLib::Vector3 calpos;
	calpos = MySpline::GetSplinePosition_NonLoop(pCourse->GetVecPosition(), m_fMoveLen);
	pos.x = calpos.x;
	pos.z = calpos.z;

	pos.z += GetOriginPosition().z;

	// ���ʔ���
	MyLib::Vector3 colPos[3];
	bool bHit[3] = {};

	bHit[0] = pCourse->CollisionVtxQuad(courseIdx, MyLib::Vector3(pos.x, 2000.0f, pos.z), &colPos[0]);
	/*bHit[1] = pCourse->CollisionVtxQuad(courseIdx - 1, MyLib::Vector3(pos.x, 2000.0f, pos.z), &colPos[1]);
	bHit[2] = pCourse->CollisionVtxQuad(courseIdx + 1, MyLib::Vector3(pos.x, 2000.0f, pos.z), &colPos[2]);*/

	float minHeight = 2000.0f;
	bool bAllHit = false;
	for (int i = 0; i < 3; i++)
	{
		if (bHit[i] &&
			minHeight >= colPos[i].y)
		{
			minHeight = colPos[i].y;

			bAllHit = true;
		}
	}

	if (bAllHit)
	{
		pos.y = minHeight + 2.0f;
	}
	SetPosition(pos);

	if (pos.x >= GetOriginPosition().x + 20000.0f)
	{// ���Ԍo��

		m_state = State::STATE_FADEOUT;
		m_fStateTimer = 0.0f;
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CLeafFlow::StateFadeOut()
{
	float alpha = 1.0f - (m_fStateTimer / StateTime::FADEOUT);
	SetAlpha(alpha);

	if (m_fStateTimer >= StateTime::FADEOUT)
	{// ���Ԍo��

		Uninit();
	}
}

//==========================================================================
// �ړ������ݒ�
//==========================================================================
void CLeafFlow::SetMoveLen(float len)
{

	// �ړ������ݒ�
	m_fMoveLen = len;

	// �R�[�X�擾
	CCourse* pCourse = CGame::GetInstance()->GetCourse();

	// �ʒu�␳
	MyLib::Vector3 calpos;
	MyLib::Vector3 pos = GetPosition();
	calpos = MySpline::GetSplinePosition_NonLoop(pCourse->GetVecPosition(), m_fMoveLen);
	pos.x = calpos.x;
	pos.z = calpos.z;
	pos.z += GetOriginPosition().z;
	SetPosition(pos);
}
