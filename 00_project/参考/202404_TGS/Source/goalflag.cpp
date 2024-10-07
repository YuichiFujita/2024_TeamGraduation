//=============================================================================
// 
//  �S�[���t���O���� [goalflag.cpp]
//  Author : �����V����̃v���O����(���؂肵�Ă܂���)
// 
//=============================================================================
#include "goalflag.h"
#include "manager.h"
#include "calculation.h"
#include "player.h"
#include "game.h"
#include "stagecleartext.h"
#include "spline.h"
#include "course.h"
#include "input.h"
#include "camera.h"
#include "camera_motion.h"
#include "sound.h"
#include "blackframe.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\checkpoint\\goal_flag.x";
	const float TIME_SWING = 2.0f;
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CGoalflagX::GOALFLAG_FUNC CGoalflagX::m_GoalflagFuncList[] =
{
	& CGoalflagX::SampleWho,	// �t�[
	& CGoalflagX::SampleWao,	// ���I
};

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CListManager<CGoalflagX> CGoalflagX::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CGoalflagX::CGoalflagX(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	m_fStateTime = 0.0f;	// ��ԃJ�E���^�[
	m_state = SAMPLE_WAO;	// ���
	m_DestRot = MyLib::Vector3();		// �ڕW�̌���
	m_bClear = false;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CGoalflagX::~CGoalflagX()
{

}

//==========================================================================
// ��������
//==========================================================================
CGoalflagX* CGoalflagX::Create(float fLength)
{
	// �������̊m��
	CGoalflagX* pObj = DEBUG_NEW CGoalflagX;

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();

		// �ʒu��񂹂��Ă���
		pObj->SetPosition(MySpline::GetSplinePosition_NonLoop(CGame::GetInstance()->GetCourse()->GetVecPosition(), fLength));
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CGoalflagX::Init()
{
	// ���X�g�ɒǉ�
	m_List.Regist(this);

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// ����������
	HRESULT hr = CObjectX::Init(MODEL);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// �����̌����ݒ�
	m_DestRot.x = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
	m_DestRot.y = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
	m_DestRot.z = UtilFunc::Transformation::Random(40, 80) * 0.001f;

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CGoalflagX::Uninit()
{

	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CGoalflagX::Kill()
{

	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CGoalflagX::Update()
{
	// ��ԃJ�E���^�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// ��ԕʏ���
	(this->*(m_GoalflagFuncList[m_state]))();

	// �h��
	Swing();

	// �ʒu���擾
	MyLib::Vector3 pos = CObjectX::GetPosition();

	// ���X�g���[�v
	CListManager<CPlayer> PlayerList = CPlayer::GetListObj();
	CPlayer* pPlayer = PlayerList.GetData(0);
	
	// �v���C���[�̈ʒu���擾
	MyLib::Vector3 Playerpos = pPlayer->GetPosition();

	if (m_bClear) { return; }

	CInputKeyboard* pKey = CInputKeyboard::GetInstance();

	if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_GOAL &&
		Playerpos.x >= pos.x || pKey->GetTrigger(DIK_3))
	{// �S�[����������

		// �T�E���h�Đ�
		CSound::GetInstance()->StopSound(CSound::LABEL_BGM_GAME);
		CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_GOAL);

		// �K�v�ȃQ�[���̏�Ԑݒ肵�Ă�
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_GOAL);

		// �J�����ݒ�
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		pCamera->SetStateCameraV(DEBUG_NEW CStateCameraV_Goal);
		pCamera->GetMotion()->SetPosition(Playerpos);
		pCamera->GetMotion()->SetMotion(CCameraMotion::MOTION::MOTION_GOAL, CCameraMotion::EASING::Linear);

		// �N���A�e�L�X�g����
		CStageClearText::Create(MyLib::Vector3(640.0f, 400.0f, 0.0f));

		// �S�[�����̐ݒ�
		pPlayer->GoalSetting();

		// ���t���[���폜
		CBlackFrame::GetInstance()->SetState(CBlackFrame::STATE::STATE_IN);

		// �N���A����n�m
		m_bClear = true;
	}
}

//==========================================================================
// �h��
//==========================================================================
void CGoalflagX::Swing()
{
	MyLib::Vector3 rot = GetRotation();

	// �T�C���J�[�u���
	float ratio = UtilFunc::Correction::EasingEaseInOutSine(0.0f, 1.0f, 0.0f, TIME_SWING, m_fStateTime);
	rot = m_DestRot_Old + (m_DestRot - m_DestRot_Old) * ratio;
	SetRotation(rot);

	if (m_fStateTime >= TIME_SWING)
	{
		m_fStateTime = 0.0f;
		
		m_DestRot_Old = m_DestRot;
		
		m_DestRot.x = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
		m_DestRot.y = UtilFunc::Transformation::Random(-110, 110) * 0.001f;
		m_DestRot.z *= -1;
	}
}

//==========================================================================
// �t�[
//==========================================================================
void CGoalflagX::SampleWho()
{

}

//==========================================================================
// ���I
//==========================================================================
void CGoalflagX::SampleWao()
{

}

//==========================================================================
// �`�揈��
//==========================================================================
void CGoalflagX::Draw()
{
	// �`��
	CObjectX::Draw();
}

