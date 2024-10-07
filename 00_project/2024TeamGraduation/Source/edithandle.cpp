//=============================================================================
// 
// �n���h������ [edithandle.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "edithandle.h"
#include "manager.h"
#include "calculation.h"
#include "3D_Effect.h"
#include "camera.h"
#include "objectLine.h"

// �h���N���X
#include "handle_Move.h"
#include "handle_Scale.h"


//==========================================================================
// �萔��`
//==========================================================================
namespace 
{
	const std::string MODEL[CHandle::HandleType::TYPE_MAX][CHandle::HandleAngle::ANGLE_MAX] =
	{
		{// �ړ�
			"data\\MODEL\\handle\\arrow_red.x",
			"data\\MODEL\\handle\\arrow_green.x",
			"data\\MODEL\\handle\\arrow_blue.x",
		},
		{// �X�P�[��
			"data\\MODEL\\handle\\scale_red.x",
			"data\\MODEL\\handle\\scale_green.x",
			"data\\MODEL\\handle\\scale_blue.x",
		},
	};

	const std::string MODEL_CENTER = "data\\MODEL\\handle\\centersphere.x";

	const float LENGTH = 200.0f;

	const D3DXCOLOR COLOR[] =
	{
		{1.0f, 0.0f, 0.0f, 1.0f},
		{0.0f, 1.0f, 0.0f, 1.0f},
		{0.0f, 0.0f, 1.0f, 1.0f},
	};
}
CListManager<CObjectX> CHandle::m_List = {};	// �ړ��n���h�����X�g


//==========================================================================
// �֐��|�C���^
//==========================================================================
CHandle::STATE_FUNC CHandle::m_StateFunc[] =
{
	&CHandle::StateNone,	// �Ȃ�
	&CHandle::StateGrab,	// �͂�
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CHandle::CHandle(int nPriority, const LAYER layer) : CObject(nPriority, layer)
{
	// �l�̃N���A
	memset(m_pArrow, 0, sizeof(m_pArrow));
	m_pCenterSphere = nullptr;				// ���S
	m_moveAngle = HandleAngle::ANGLE_X;		// �ړ��̌���
	m_HandleType = HandleType::TYPE_MOVE;	// �n���h���̎��
	m_bHoverHandle = false;					// �n���h���̃z�o�[����
	m_state = State::STATE_NONE;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CHandle::~CHandle()
{

}

//==========================================================================
// ��������
//==========================================================================
CHandle* CHandle::Create(const HandleType type, const MyLib::Vector3& pos)
{
	CHandle* pBox = nullptr;

	// �������m��
	switch (type)
	{
	case HandleType::TYPE_MOVE:
		pBox = DEBUG_NEW CHandle_Move;
		break;

	case HandleType::TYPE_SCALE:
		pBox = DEBUG_NEW CHandle_Scale;
		break;

	default:
		return nullptr;
		break;
	}

	if (pBox != nullptr){

		// ���ݒ�
		pBox->m_HandleType = type;
		pBox->SetPosition(pos);

		// ����������
		pBox->Init();
	}

	return pBox;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CHandle::Init()
{

	// ��ނ̐ݒ�
	SetType(TYPE_OBJECT2D);

	MyLib::Vector3 pos[HandleAngle::ANGLE_MAX], rot[HandleAngle::ANGLE_MAX];

	pos[0] = MyLib::Vector3(LENGTH, 0.0f, 0.0f);
	pos[1] = MyLib::Vector3(0.0f, LENGTH, 0.0f);
	pos[2] = MyLib::Vector3(0.0f, 0.0f, -LENGTH);

	rot[0] = MyLib::Vector3(0.0f, -D3DX_PI * 0.5f, 0.0f);
	rot[1] = MyLib::Vector3(D3DX_PI * 0.5f, 0.0f, 0.0f);
	rot[2] = MyLib::Vector3(0.0f, 0.0f, 0.0f);

	int type = static_cast<int>(m_HandleType);
	for (int i = 0; i < HandleAngle::ANGLE_MAX; i++)
	{
		m_pArrow[i] = CObjectX::Create(MODEL[type][i], pos[i], rot[i], false);
		m_pArrow[i]->SetOriginPosition(pos[i]);
		m_List.Regist(m_pArrow[i]);
	}

	for (int i = 0; i < HandleAngle::ANGLE_MAX; i++)
	{
		m_pLine[i] = CObjectLine::Create(0.0f, pos[i], COLOR[i]);	// ���C���̃I�u�W�F�N�g
		m_pLine[i]->SetEnableZSort(true);
	}

	// ���S�̃I�u�W�F�N�g����
	m_pCenterSphere = CObjectX::Create(MODEL_CENTER, 0.0f, 0.0f, false);

	Update();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CHandle::Uninit()
{
	for (int i = 0; i < HandleAngle::ANGLE_MAX; i++)
	{
		if (m_pArrow[i] == nullptr){
			continue;
		}
		m_List.Delete(m_pArrow[i]);
		m_pArrow[i] = nullptr;
	}

	// �I������
	Release();
}

//==========================================================================
// �폜����
//==========================================================================
void CHandle::Kill()
{

	for (int i = 0; i < HandleAngle::ANGLE_MAX; i++)
	{
		if (m_pArrow[i] == nullptr) {
			continue;
		}
		m_List.Delete(m_pArrow[i]);
		m_pArrow[i]->Kill();
		m_pArrow[i] = nullptr;
	}

	for (int i = 0; i < HandleAngle::ANGLE_MAX; i++)
	{
		if (m_pLine[i] == nullptr) {
			continue;
		}
		m_pLine[i]->Uninit();
		m_pLine[i] = nullptr;
	}

	if (m_pCenterSphere != nullptr) {
		m_pCenterSphere->Kill();
		m_pCenterSphere = nullptr;
	}

	// �I������
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CHandle::Update()
{
	// �J�����ɑ΂��ă��T�C�Y
	ReSizeToCamera();

	// �͂ݒ�����
	Grab();

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// �n���h���Ƃ̔���
//==========================================================================
void CHandle::CollisionHandle()
{
	// �}�E�X���擾
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();

	m_bHoverHandle = false;
	bool bChange = false;
	for (int i = 0; i < HandleAngle::ANGLE_MAX; i++)
	{
		m_pArrow[i]->SetColor(mylib_const::DEFAULT_COLOR);

		MyLib::AABB aabb = m_pArrow[i]->GetAABB();
		MyLib::Matrix mtx = m_pArrow[i]->GetWorldMtx();
		float time = 0.0f;

		if (!m_bHoverHandle) {
			m_bHoverHandle = UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mtx, time);
		}

		if (m_bHoverHandle &&
			!bChange)
		{
			m_pArrow[i]->SetColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
			bChange = true;
			m_moveAngle = static_cast<HandleAngle>(i);
		}
	}


	// ���S�Ɣ���
	if (!m_bHoverHandle) {

		m_pCenterSphere->SetColor(mylib_const::DEFAULT_COLOR);

		MyLib::AABB aabb = m_pCenterSphere->GetAABB();
		MyLib::Matrix mtx = m_pCenterSphere->GetWorldMtx();
		float time = 0.0f;

		if (!m_bHoverHandle) {
			m_bHoverHandle = UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mtx, time);
		}

		if (m_bHoverHandle){
			m_pCenterSphere->SetColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
			m_moveAngle = static_cast<HandleAngle>(HandleAngle::ANGLE_MAX);
		}
	}
}

//==========================================================================
// �J�����ɑ΂��ă��T�C�Y
//==========================================================================
void CHandle::ReSizeToCamera()
{
	// �ʒu�擾
	MyLib::Vector3 pos = GetPosition();

	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 cameraPosition = pCamera->GetPositionV();
	D3DXMATRIX projection = pCamera->GetMtxProjection();
	D3DXMATRIX view = pCamera->GetMtxView();
	D3DVIEWPORT9 viewport = pCamera->GetViewPort();

	// �J����������܂ł̋������v�Z
	MyLib::Vector3 t = (pos - cameraPosition);
	float distanceToArrow = t.Length();

	CInputMouse::GetInstance()->SetFarDistance(distanceToArrow);

	// ���̃��f���̊g�嗦��ݒ�
	m_scale = 1.0f * (distanceToArrow / 2000.0f);

	for (int i = 0; i < HandleAngle::ANGLE_MAX; i++)
	{
		if (m_pArrow[i] == nullptr) {
			continue;
		}

		// �J�����ɑ΂��ď�ɓ����������ɂ���
		{
			MyLib::Vector3 position = m_pArrow[i]->GetPosition();
			MyLib::Vector3 originpos = m_pArrow[i]->GetOriginPosition();

			m_pArrow[i]->SetPosition(pos + originpos * m_scale);
			m_pArrow[i]->SetScale(m_scale);
			m_pLine[i]->SetLeftPosition(pos);
			m_pLine[i]->SetRightPosition(m_pArrow[i]->GetPosition());
		}

		// �F�ݒ�
		if (m_pArrow[i]->GetColor() != mylib_const::DEFAULT_COLOR) {
			m_pLine[i]->SetColor({ 0.0f, 1.0f, 1.0f, 1.0f });
		}
		else {
			m_pLine[i]->SetColor(COLOR[i]);
		}
	}

	// ���S�X�V
	m_pCenterSphere->SetPosition(pos);
	m_pCenterSphere->SetScale(m_scale);
}

//==========================================================================
// �Ȃɂ��Ȃ�
//==========================================================================
void CHandle::StateNone()
{
	// �n���h���Ƃ̔���
	CollisionHandle();
}

//==========================================================================
// �͂�
//==========================================================================
void CHandle::StateGrab()
{
	if (m_moveAngle < HandleAngle::ANGLE_MAX &&
		m_pArrow[m_moveAngle] != nullptr) {
		m_pArrow[m_moveAngle]->SetColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
	}
	else if (m_moveAngle == HandleAngle::ANGLE_MAX &&
		m_pCenterSphere != nullptr) {
		m_pCenterSphere->SetColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CHandle::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	// ��ɕ`�悷��


	// ���S
	if (m_pCenterSphere != nullptr) {

		if (m_pCenterSphere->GetColor() != mylib_const::DEFAULT_COLOR) {
			m_pCenterSphere->Draw(m_pCenterSphere->GetColor());
		}
		else {
			m_pCenterSphere->Draw();
		}
	}

	for (int i = 0; i < HandleAngle::ANGLE_MAX; i++)
	{
		if (m_pArrow[i]->GetColor() != mylib_const::DEFAULT_COLOR){
			m_pArrow[i]->Draw(m_pArrow[i]->GetColor());
		}
		else {
			m_pArrow[i]->Draw();
		}
	}

	// Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CHandle::SetPosition(const MyLib::Vector3& pos)
{
	// �ʒu�ݒ�
	CObject::SetPosition(pos);

	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 cameraPosition = pCamera->GetPositionV();

	for (int i = 0; i < HandleAngle::ANGLE_MAX; i++)
	{
		if (m_pArrow[i] == nullptr) {
			continue;
		}

		// �J�����ɑ΂��ď�ɓ����������ɂ���
		{
			MyLib::Vector3 position = m_pArrow[i]->GetPosition();
			MyLib::Vector3 originpos = m_pArrow[i]->GetOriginPosition();

			// �J����������܂ł̋������v�Z
			MyLib::Vector3 t = (pos - cameraPosition);
			float distanceToArrow = t.Length();

			// ���̃��f���̊g�嗦��ݒ�
			float scaleFactor = 1.0f * (distanceToArrow / 2000.0f);

			m_pArrow[i]->SetPosition(pos + originpos * scaleFactor);
			m_pLine[i]->SetLeftPosition(pos);
			m_pLine[i]->SetRightPosition(m_pArrow[i]->GetPosition());
		}
	}
}