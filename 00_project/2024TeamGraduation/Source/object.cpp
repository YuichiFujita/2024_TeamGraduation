//=============================================================================
// 
//  �I�u�W�F�N�g���� [object.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "object.h"
#include "manager.h"
#include "camera.h"
#include "MyEffekseer.h"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CObject::m_nNumAll = 0;	// ����
std::map<CObject::LAYER, std::map<int, std::vector<CObject*>>> CObject::m_pObj = {};	// �I�u�W�F�N�g�i�[�p

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObject::CObject(int nPriority, const LAYER layer)
{
	if (nPriority < 0 || nPriority >= mylib_const::PRIORITY_NUM) {
		return;
	}

	// �l�̃N���A
	m_pos = 0.0f;		// �ʒu
	m_posOld = 0.0f;	// �O��̈ʒu
	m_rot = 0.0f;		// ����
	m_move = 0.0f;		// �ړ���
	m_rotOrigin = 0.0f;	// ���̌���

	m_nPriority = nPriority;	// �D�揇��
	m_Layer = layer;			// ���C���[��
	m_type = TYPE_NONE;			// ���
	m_bDeath = false;			// ���S�t���O
	m_bDisp = true;				// �`��t���O
	m_bPosibleMove_WorldPause = false;		// ���E��~���ɓ�����t���O
	m_nNumAll++;				// �������Z

	// �I�u�W�F�N�g�i�[
	m_pObj[layer][nPriority].emplace_back();
	m_pObj[layer][nPriority].back() = this;

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObject::~CObject()
{

}

//==========================================================================
// �S�ẴI�u�W�F�N�g�̔j��
//==========================================================================
void CObject::ReleaseAll()
{
	// �폜����v�f
	std::map<LAYER, std::map<int, std::vector<CObject*>>> objectsToRemove;
	for (auto& layer : m_pObj)
	{
		for (auto& priority : layer.second)
		{
			for (auto itr = priority.second.begin(); itr != priority.second.end();)
			{
				CObject* pObj = (*itr);

				if (pObj->m_type != TYPE_NONE)
				{// NONE����Ȃ����

					// �I������
					pObj->Uninit();

					// ��ŏ���vector�Ɋi�[
					objectsToRemove[layer.first][priority.first].push_back(*itr);
					itr = priority.second.erase(itr);
				}
				else
				{
					if (pObj->IsDeath())
					{
						// �I������
						pObj->Uninit();

						// ��ŏ���vector�Ɋi�[
						objectsToRemove[layer.first][priority.first].push_back(*itr);
						itr = priority.second.erase(itr);
					}
					else
					{
						itr++;
					}

				}
			}
		}
	}

	//*******************************
	// ���S����
	//*******************************
	for (auto& layer : objectsToRemove)
	{
		for (auto& priority : layer.second)
		{
			for (const auto& obj : priority.second)
			{
				// �������Z
				m_nNumAll--;
				delete obj;
			}
		}
	}
	objectsToRemove.clear();
}

//==========================================================================
// �S�ẴI�u�W�F�N�g�̍X�V����
//==========================================================================
void CObject::UpdateAll(const float fDeltaTime, const float fDeltaRate, const float fSlowRate)
{
	// �}�l�[�W���[�擾
	CManager* pMgr = CManager::GetInstance();
	if (pMgr == nullptr) return;

	// ���E�̒�~�󋵎擾
	bool bWorldPause = pMgr->IsWorldPaused();

	// �X�V�t���O
	bool bUpdate = false;

	// �폜����v�f
	std::map<LAYER, std::map<int, std::vector<CObject*>>> objectsToRemove;
	for (auto& layer : m_pObj)
	{
		for (auto& priority : layer.second)
		{
			auto& vec = priority.second; // �C�e���[�^������Ȃ����߂ɎQ�Ƃ��擾

			for (int i = 0; i < (int)vec.size(); i++)
			{
				CObject* pObj = priority.second[i];

				// �X�V�t���O�X�V
				bUpdate = !bWorldPause || (bWorldPause && pObj->IsPosibleMove_WorldPause());

				if (bUpdate &&
					!pObj->m_bDeath &&
					pObj->m_type != TYPE_NONE)
				{// �^�C�v��NONE�ȊO

					// �X�V����
					pObj->Update(fDeltaTime, fDeltaRate, fSlowRate);
				}

#if 1
				if (pObj->m_bDeath)
				{
					// ��ŏ���vector�Ɋi�[
					objectsToRemove[layer.first][priority.first].push_back(pObj);
					priority.second.erase(std::find(priority.second.begin(), priority.second.end(), pObj)); // erase�v�f��itr���C���N�������g
				}
#endif
			}
		}
	}

#if 1
	//*******************************
	// ���S����
	//*******************************
	for (auto& layer : objectsToRemove)
	{
		for (auto& priority : layer.second)
		{
			for (const auto& obj : priority.second)
			{
				// �������Z
				m_nNumAll--;
				obj->Uninit();
				delete obj;
			}
		}
	}
	objectsToRemove.clear();
#endif
}

//==========================================================================
// �S�ẴI�u�W�F�N�g�̕`�揈��
//==========================================================================
void CObject::DrawAll()
{
	bool bEfkDraw = false;	// �G�t�F�N�V�A�̕`��
	for (auto& layer : m_pObj)
	{
		for (auto& priority : layer.second)
		{
			if (priority.first == mylib_const::PRIORITY_ZSORT)
			{// Z�\�[�g�`��
				DrawZSort(layer.first, priority.first);
			}
			else
			{// �ʏ�`��
				DrawNone(layer.first, priority.first);
			}

			if (layer.first == LAYER::LAYER_DEFAULT &&
				(priority.first == 3 || (priority.first > 3 && !bEfkDraw)))
			{
				// �G�t�F�N�V�A�̍X�V���`��
				CMyEffekseer* pEffekseer = CMyEffekseer::GetInstance();
				if (pEffekseer != nullptr)
				{
					CManager* pManager = GET_MANAGER;
					pEffekseer->Update(pManager->GetDeltaTime(), pManager->GetDeltaRate(), pManager->GetSlowRate());
					bEfkDraw = true;
				}
			}
		}
	}

	if (!bEfkDraw)
	{
		// �G�t�F�N�V�A�̍X�V���`��
		CMyEffekseer* pEffekseer = CMyEffekseer::GetInstance();
		if (pEffekseer != nullptr)
		{
			CManager* pManager = GET_MANAGER;
			pEffekseer->Update(pManager->GetDeltaTime(), pManager->GetDeltaRate(), pManager->GetSlowRate());
		}
	}
}

//==========================================================================
// �ʏ�`��
//==========================================================================
void CObject::DrawNone(const LAYER layer, int nPriority)
{
	for (const auto& obj : m_pObj[layer][nPriority])
	{
		if (obj->m_bDisp &&
			obj->m_bDeath == false &&
			obj->m_type != TYPE_NONE)
		{// NONE����Ȃ����
			obj->Draw();
		}
	}
}

//==========================================================================
// Z�\�[�g�`��
//==========================================================================
void CObject::DrawZSort(const LAYER layer, int nPriority)
{
	// �v�f�R�s�[
	std::vector<CObject*> tempVector;

	// �T�C�Y���m��
	tempVector.reserve(m_pObj[layer][nPriority].size());
	std::move(m_pObj[layer][nPriority].begin(), m_pObj[layer][nPriority].end(), std::back_inserter(tempVector));

	// Z�\�[�g
	std::sort(tempVector.begin(), tempVector.end(), ZSort);

	for (const auto& obj : tempVector)
	{
		if (obj->m_bDisp &&
			obj->m_bDeath == false &&
			obj->m_type != TYPE_NONE)
		{// NONE����Ȃ����
			obj->Draw();
		}
	}
}

//==========================================================================
// �X�N���[�����W�擾(Z���W)
//==========================================================================
float CObject::ScreenZ() const 
{
	// �J�����̍��W�ϊ��s����擾
	D3DXMATRIX mtxCamera = CManager::GetInstance()->GetCamera()->GetMtxView();

	// �I�u�W�F�N�g���W���J��������̍��W�ɕϊ�
	MyLib::Vector3 screenPos;
	D3DXVec3TransformCoord(&screenPos, &m_pos, &mtxCamera);

	// �J�������_���猩��Z���W
	float screenZ = -screenPos.z;

	return screenZ;
}

//==========================================================================
// Z�\�[�g�̔�r�֐�
//==========================================================================
bool CObject::ZSort(const CObject *obj1, const CObject *obj2)
{
	return obj1->ScreenZ() < obj2->ScreenZ();
}

//==========================================================================
// �tZ�\�[�g�̔�r�֐�
//==========================================================================
bool CObject::ZSortInverse(const CObject* obj1, const CObject* obj2)
{
	return obj1->ScreenZ() > obj2->ScreenZ();
}

//==========================================================================
// �I�u�W�F�N�g�̎��S����
//==========================================================================
void CObject::Release()
{
	// ���S�t���O�𗧂Ă�
	m_bDeath = true;
}
