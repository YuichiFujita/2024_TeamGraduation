//=============================================================================
// 
//  ���R������ [environment.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "environment.h"
#include "manager.h"
#include "calculation.h"

#include "player.h"
#include "leaf_flow.h"
#include "camera.h"

//==========================================================================
// �X�V����
//==========================================================================
void CSpawnEnvironment::Update(float deltaTime)
{
	// �^�C�}�[���Z
	m_fSpawnTimer += deltaTime;

	if (m_fSpawnTimer >= m_fSpawnInterval)
	{
		// �������̃g���K�[
		TriggerSpawn();
	}
}

//==========================================================================
// ��C�̐������̃g���K�[
//==========================================================================
void CSpawn_Air::TriggerSpawn()
{
	// �^�C�}�[�֘A���Z�b�g
	m_fSpawnTimer = 0.0f;
	m_fSpawnInterval = m_fDefaultInterval + UtilFunc::Transformation::Random(-20, 20) * 0.01f;

	// �v���C���[�擾
	CPlayer* pPlayer = CPlayer::GetListObj().GetData(0);
	MyLib::Vector3 playerPos = pPlayer->GetPosition();

	// ��_�̈ʒu
	MyLib::Vector3 basepos = MyLib::Vector3(playerPos.x, 0.0f, playerPos.z) + MyLib::Vector3(-500.0f, 500.0f, 1000.0f);

	int rand = UtilFunc::Transformation::Random(0, 2);
	switch (rand)
	{
	case 0:
		// ������
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_AIR,
			basepos + MyLib::Vector3(0.0f, 0.0f, UtilFunc::Transformation::Random(-20, 1) * 100.0f),
			MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f, true);
		break;

	case 1:
		// �E�̕�
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_AIR,
			basepos + MyLib::Vector3(0.0f,
				UtilFunc::Transformation::Random(40, 60) * 10.0f,
				UtilFunc::Transformation::Random(-5, 15) * 100.0f),
			MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f, true);
		break;

	case 2:
		// �E��̕�
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_AIR,
			basepos + MyLib::Vector3(0.0f,
				UtilFunc::Transformation::Random(80, 120) * 10.0f,
				UtilFunc::Transformation::Random(-20, 1) * 100.0f),
			MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f, true);
		break;

	default:
		break;
	}

}


//==========================================================================
// ��C�̐������̃g���K�[
//==========================================================================
void CSpawn_Air_Title::TriggerSpawn()
{
	// �^�C�}�[�֘A���Z�b�g
	m_fSpawnTimer = 0.0f;
	m_fSpawnInterval = m_fDefaultInterval + UtilFunc::Transformation::Random(-20, 20) * 0.01f;

	// �J�����擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 posR = pCamera->GetPositionR();

	// ��_�̈ʒu
	MyLib::Vector3 basepos = MyLib::Vector3(posR.x, 0.0f, posR.z) + MyLib::Vector3(0.0f, 500.0f, 500.0f);

	int rand = UtilFunc::Transformation::Random(0, 2);
	switch (rand)
	{
	case 0:
		// ������
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_AIR,
			basepos + MyLib::Vector3(0.0f, 0.0f, UtilFunc::Transformation::Random(-20, 1) * 100.0f),
			MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f, true);
		break;

	case 1:
		// �E�̕�
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_AIR,
			basepos + MyLib::Vector3(0.0f,
				UtilFunc::Transformation::Random(40, 60) * 10.0f,
				UtilFunc::Transformation::Random(-5, 15) * 100.0f),
			MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f, true);
		break;

	case 2:
		// �E��̕�
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_AIR,
			basepos + MyLib::Vector3(0.0f,
				UtilFunc::Transformation::Random(80, 120) * 10.0f,
				UtilFunc::Transformation::Random(-20, 1) * 100.0f),
			MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 20.0f, true);
		break;

	default:
		break;
	}

}

//==========================================================================
// �����t�̐������̃g���K�[
//==========================================================================
void CSpawn_FlowLeaf::TriggerSpawn()
{
	// �^�C�}�[�֘A���Z�b�g
	m_fSpawnTimer = 0.0f;
	m_fSpawnInterval = m_fDefaultInterval + UtilFunc::Transformation::Random(-40, 20) * 0.01f;

	// �v���C���[�擾
	CPlayer* pPlayer = CPlayer::GetListObj().GetData(0);
	float playerMoveLen = pPlayer->GetMoveLength();

	// �����t����
	CLeafFlow* pLeaf = static_cast<CLeafFlow*>(CLeaf::Create(
		MyLib::Vector3(0.0f, 0.0f, UtilFunc::Transformation::Random(-6, 6) * 100.0f + UtilFunc::Transformation::Random(-90, 90)),
		CLeaf::Type::TYPE_FLOW));

	pLeaf->SetMoveLen(playerMoveLen - 1000.0f);
	pLeaf->SetOriginPosition(pLeaf->GetPosition());
}


//==========================================================================
// �~��t�̍X�V����
//==========================================================================
void CSpawn_Leaf::Update(float deltaTime)
{
	// �v���C���[�擾
	CPlayer* pPlayer = CPlayer::GetListObj().GetData(0);
	if (pPlayer == nullptr) return;

	MyLib::Vector3 playerMove = pPlayer->GetMove();	// �ړ��ʎ擾
	float fVelocity = pPlayer->GetVelocity();		// �ړ����x�擾

	// �ړ����x����
	float ratio = playerMove.x / (fVelocity);

	// �ړ����x�ɉ����Đ����Ԋu�Z������
	m_fSpawnTimer += deltaTime * ratio;

	// �e�̍X�V����
	CSpawnEnvironment::Update(deltaTime);
}

//==========================================================================
// �~��t�̐������̃g���K�[
//==========================================================================
void CSpawn_Leaf::TriggerSpawn()
{
	// �^�C�}�[�֘A���Z�b�g
	m_fSpawnTimer = 0.0f;
	m_fSpawnInterval = m_fDefaultInterval + UtilFunc::Transformation::Random(-40, 40) * 0.01f;

	// �v���C���[�擾
	CPlayer* pPlayer = CPlayer::GetListObj().GetData(0);
	if (pPlayer == nullptr) return;
	MyLib::Vector3 playerPos = pPlayer->GetPosition();

	// �J�����擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// ��_�̈ʒu
	MyLib::Vector3 basepos = MyLib::Vector3(playerPos.x, 0.0f, playerPos.z) + MyLib::Vector3(0.0f, 700.0f + pCamera->GetPositionR().y, 0.0f);

	// �~��t����
	CLeaf* pLeaf = CLeaf::Create(
		basepos + MyLib::Vector3(
			UtilFunc::Transformation::Random(-5, 20) * 100.0f + UtilFunc::Transformation::Random(-90, 90),
			UtilFunc::Transformation::Random(0, 3) * 100.0f,
			UtilFunc::Transformation::Random(-5, 5) * 100.0f + UtilFunc::Transformation::Random(-90, 90)),
		CLeaf::Type::TYPE_FALL);
}


//==========================================================================
// �~��t�̍X�V����
//==========================================================================
void CSpawn_Leaf_Title::Update(float deltaTime)
{
	// �e�̍X�V����
	CSpawnEnvironment::Update(deltaTime);
}

//==========================================================================
// �~��t�̐������̃g���K�[
//==========================================================================
void CSpawn_Leaf_Title::TriggerSpawn()
{
	// �^�C�}�[�֘A���Z�b�g
	m_fSpawnTimer = 0.0f;
	m_fSpawnInterval = m_fDefaultInterval + UtilFunc::Transformation::Random(-40, 40) * 0.01f;

	// �J�����擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// ��_�̈ʒu
	MyLib::Vector3 basepos = pCamera->GetPositionR();
	basepos.z = 0.0f;

	basepos += MyLib::Vector3(-800.0f, 400.0f, 100.0f);

	// �~��t����
	CLeaf* pLeaf = CLeaf::Create(
		basepos + MyLib::Vector3(
			UtilFunc::Transformation::Random(-5, 20) * 100.0f + UtilFunc::Transformation::Random(-90, 90),
			UtilFunc::Transformation::Random(0, 3) * 100.0f,
			UtilFunc::Transformation::Random(-5, 5) * 100.0f + UtilFunc::Transformation::Random(-90, 90)),
		CLeaf::Type::TYPE_FALL);
}


//==========================================================================
// �~��t(�����L���O)�̍X�V����
//==========================================================================
void CSpawn_Leaf_Ranking::Update(float deltaTime)
{
	// �e�̍X�V����
	CSpawnEnvironment::Update(deltaTime);
}

//==========================================================================
// �~��t(�����L���O)�̐������̃g���K�[
//==========================================================================
void CSpawn_Leaf_Ranking::TriggerSpawn()
{
	// �^�C�}�[�֘A���Z�b�g
	m_fSpawnTimer = 0.0f;
	m_fSpawnInterval = m_fDefaultInterval + UtilFunc::Transformation::Random(-40, 40) * 0.01f;

	// �J�����擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// ��_�̈ʒu
	MyLib::Vector3 basepos = pCamera->GetPositionR();
	basepos += MyLib::Vector3(0.0f, 700.0f, 500.0f);

	// �~��t����
	CLeaf* pLeaf = CLeaf::Create(
		basepos + MyLib::Vector3(
			UtilFunc::Transformation::Random(-20, 20) * 100.0f + UtilFunc::Transformation::Random(-90, 90),
			UtilFunc::Transformation::Random(0, 3) * 100.0f,
			UtilFunc::Transformation::Random(-5, 5) * 100.0f + UtilFunc::Transformation::Random(-90, 90)),
		CLeaf::Type::TYPE_FALL);
}
