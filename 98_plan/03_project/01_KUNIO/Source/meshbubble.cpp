//=============================================================================
// 
//  �e�������� [meshbubble.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "meshbubble.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\FIELD\\bubble.png";
	const int BLOCK = 12;
	const float TIME_BURST = 0.3f;
	const float TIME_FADEOUT = 0.15f;
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CMeshBubble::STATE_FUNC CMeshBubble::m_StateFunc[] =
{
	&CMeshBubble::StateMove,	// �Ȃ�
	&CMeshBubble::StateBurst,	// �͂�
	&CMeshBubble::StateFadeOut,	// �͂�
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMeshBubble::CMeshBubble(int nPriority) : CMeshSphere(nPriority)
{
	// �l�̃N���A
	m_fOriginSize = 0.0f;		// ���̃T�C�Y
	m_fDestSize = 0.0f;			// �ڕW�̃T�C�Y
	m_fRepeatCycle = 0.0f;		// ��������
	m_fStateTime = 0.0f;		// ��ԃ^�C�}�[
	m_state = STATE::STATE_MOVE;	// ���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMeshBubble::~CMeshBubble()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CMeshBubble* CMeshBubble::Create(const MyLib::Vector3& pos, const MyLib::Vector3& move, const float radius, const float destRadius, float repeatCycle)
{

	// �������̊m��
	CMeshBubble* pBullet = DEBUG_NEW CMeshBubble;

	if (pBullet != nullptr)
	{// �������̊m�ۂ��o���Ă�����

		// ���蓖��
		pBullet->SetPosition(pos);
		pBullet->SetMove(move);

		// �T�C�Y�ݒ�
		pBullet->m_fOriginSize = radius;		// ���̃T�C�Y
		pBullet->m_fDestSize = destRadius;		// �ڕW�̃T�C�Y
		pBullet->m_fRepeatCycle = repeatCycle;	// ��������
		pBullet->SetWidthLen(radius);
		pBullet->SetHeightLen(radius);
		pBullet->SetWidthBlock(BLOCK);
		pBullet->SetHeightBlock(BLOCK);

		// ����������
		pBullet->Init();
	}

	return pBullet;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMeshBubble::Init(void)
{
	// �e��ϐ��̏�����

	float rg = UtilFunc::Transformation::Random(-10, 10) * 0.01f;
	float b = UtilFunc::Transformation::Random(-20, 0) * 0.01f;

	SetColor(D3DXCOLOR(
		1.0f,
		1.0f,
		1.0f, 0.8f));
	SetColorOrigin(GetColor());

	// �e�N�X�`���̊��蓖��
	int texIdx = CTexture::GetInstance()->Regist("");
	BindTexture(texIdx);

	// ����������
	CMeshSphere::Init();

	// ��ނ̐ݒ�
	CObject::SetType(CObject::TYPE::TYPE_MESHSPHERE);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMeshBubble::Uninit(void)
{
	// �I������
	CMeshSphere::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CMeshBubble::Update(void)
{
	// �e�����ԉ��Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))();

	if (IsDeath())
		return;
	
	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �ړ�����
//==========================================================================
void CMeshBubble::StateMove()
{
	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();
	bool bLand = false;

	// �����擾
	float height = CManager::GetInstance()->GetScene()->GetElevation()->GetHeight(pos, &bLand);

	// �ړ��ʎ擾
	MyLib::Vector3 move = GetMove();

	// �ʒu�X�V
	pos.y += move.y;

	// �����ňʒu�X�V
	pos.x += sinf(D3DX_PI * (m_fStateTime / m_fRepeatCycle)) * move.x;
	pos.z += cosf(D3DX_PI * (m_fStateTime / m_fRepeatCycle)) * move.x;

	// ��������
	if (pos.y >= height)
	{
		m_state = STATE::STATE_BURST;
		m_fStateTime = 0.0f;
	}

	// �ʒu�ݒ�
	SetPosition(pos);



	float radius = GetWidthLen();

	radius += m_fDestSize * 0.001f;
	m_fOriginSize = radius;

	SetWidthLen(radius);
	SetHeightLen(radius);
}

//==========================================================================
// �e������
//==========================================================================
void CMeshBubble::StateBurst()
{
	// ���X�ɉ���
	float radius = UtilFunc::Correction::EasingEaseIn(m_fOriginSize, m_fDestSize, 0.0f, TIME_BURST, m_fStateTime);

	SetWidthLen(radius);
	SetHeightLen(radius);

	if (m_fStateTime >= TIME_BURST)
	{
		for (int i = 0; i < 3; i++)
		{
			MyLib::Vector3 move;
			float fMove = 8.0f + UtilFunc::Transformation::Random(-50, 50) * 0.1f;
			float fMoveY = UtilFunc::Transformation::Random(80, 120) * 0.1f;
			float fRot = UtilFunc::Transformation::Random(-31, 31) * 0.1f;

			// �ړ��ʂ̐ݒ�
			move.x = sinf(fRot) * fMove;
			move.y = fMoveY;
			move.z = cosf(fRot) * fMove;

			float radius = 5.0f + UtilFunc::Transformation::Random(-20, 20) * 0.1f;

			CEffect3D* pEffect = CEffect3D::Create(
				GetPosition(),
				move,
				D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f),
				radius, 30,
				CEffect3D::MOVEEFFECT::MOVEEFFECT_GENSUI,
				CEffect3D::TYPE::TYPE_NORMAL);
			pEffect->SetEnableGravity();
			pEffect->SetGravityValue(0.5f);
		}

		m_state = STATE::STATE_FADEOUT;
		m_fStateTime = 0.0f;
	}
}

//==========================================================================
// �t�F�[�h�A�E�g����
//==========================================================================
void CMeshBubble::StateFadeOut()
{
	
	D3DXCOLOR col = GetColor();

	// �s�����x�X�V
	col.a = GetColorOrigin().a * (1.0f - (m_fStateTime / TIME_FADEOUT));
	SetColor(col);

	if (m_fStateTime >= TIME_FADEOUT)
	{

		Uninit();
	}

}

//==========================================================================
// �`�揈��
//==========================================================================
void CMeshBubble::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �`�揈��
	CMeshSphere::Draw();

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CMeshBubble::SetVtx(void)
{
	// ���_�ݒ�
	CMeshSphere::SetVtx();
}
