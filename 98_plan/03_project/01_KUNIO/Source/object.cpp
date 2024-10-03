//=============================================================================
// 
//  �I�u�W�F�N�g���� [object.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "object.h"
#include "manager.h"
#include "3D_effect.h"
#include "camera.h"
#include "MyEffekseer.h"
#include "fog.h"
#include "baggage.h"
#include "game.h"

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
int CObject::m_nNumAll = 0;	// ����
int CObject::m_nNumPriorityAll[mylib_const::PRIORITY_NUM] = {};
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
	memset(&m_pEffect[0], 0, sizeof(m_pEffect));	// �G�t�F�N�g�̃|�C���^
	m_nNumEffectParent = 0;		// �G�t�F�N�g�̐e�ݒ肵����
	m_bHitstopMove = false;		// �q�b�g�X�g�b�v���ɓ������̃t���O
	m_nNumAll++;				// �������Z
	m_nNumPriorityAll[nPriority]++;

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
}

//==========================================================================
// �S�ẴI�u�W�F�N�g�̍X�V����
//==========================================================================
void CObject::UpdateAll()
{

#if _DEBUG
	// �G�f�B�b�g�̏��擾
	CEdit* pEdit = CManager::GetInstance()->GetEdit();
#endif

	// �q�b�g�X�g�b�v����
	bool bHitstop = CManager::GetInstance()->IsHitStop();

	// �폜����v�f
	std::map<LAYER, std::map<int, std::vector<CObject*>>> objectsToRemove;
	for (auto& layer : m_pObj)
	{
		for (auto& priority : layer.second)
		{
			auto& vec = priority.second; // �C�e���[�^������Ȃ����߂ɎQ�Ƃ��擾

			for (int i = 0; i < vec.size(); i++)
			{
				CObject* pObj = priority.second[i];

#if _DEBUG
				if (!pObj->m_bDeath &&
					pEdit != nullptr &&
					(pObj->m_type == TYPE_EDIT || pObj->m_type == TYPE_XFILE) &&
					(pObj->m_bHitstopMove || (!pObj->m_bHitstopMove && !bHitstop)))
				{// �G�f�B�b�g��Ԃ�������G�f�B�b�g�̂ݍX�V

					// �X�V����
					pObj->Update();
				}
				else if (
					!pObj->m_bDeath &&
					pEdit == nullptr &&
					pObj->m_type != TYPE_NONE &&
					(pObj->m_bHitstopMove || (!pObj->m_bHitstopMove && !bHitstop)))
				{// �G�f�B�b�g��Ԃ���Ȃ� && �^�C�v��NONE�ȊO

					// �X�V����
					pObj->Update();
				}
#else
				if (!pObj->m_bDeath &&
					pObj->m_type != TYPE_NONE &&
					(pObj->m_bHitstopMove || (!pObj->m_bHitstopMove && !bHitstop)))
				{// �^�C�v��NONE�ȊO

					// �X�V����
					pObj->Update();
				}
#endif

				if (pObj->m_bDeath)
				{
					// ��ŏ���vector�Ɋi�[
					objectsToRemove[layer.first][priority.first].push_back(pObj);
					priority.second.erase(std::find(priority.second.begin(), priority.second.end(), pObj)); // erase�v�f��itr���C���N�������g
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
				obj->Uninit();
				delete obj;
			}
		}
	}
	objectsToRemove.clear();
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
					pEffekseer->Update();
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
			pEffekseer->Update();
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
// �`��ݒ�
//==========================================================================
void CObject::SetEnableDisp(bool bDisp)
{
	m_bDisp = bDisp;
}

//==========================================================================
// �G�t�F�N�g�ݒ�
//==========================================================================
int CObject::SetEffectParent(CEffect3D *pEffect3D)
{
	int nIdx = 0;
	for (int nCntEffect = 0; nCntEffect < mylib_const::MAX_OBJ; nCntEffect++)
	{
		if (m_pEffect[nCntEffect] != nullptr)
		{// nullptr����Ȃ�������
			continue;
		}

		// �G�t�F�N�g�̃|�C���^��n��
		m_pEffect[nCntEffect] = pEffect3D;
		nIdx = nCntEffect;

		// �G�t�F�N�g�̐e�ݒ肵�������Z
		m_nNumEffectParent++;
		break;
	}

	return nIdx;
}

//==========================================================================
// �e�ݒ肵���G�t�F�N�g�̐��擾
//==========================================================================
int CObject::GetEffectParentNum()
{
	return m_nNumEffectParent;
}

//==========================================================================
// �e�ݒ肵���G�t�F�N�g�擾
//==========================================================================
CEffect3D *CObject::GetEffectParent(int nIdx)
{
	return m_pEffect[nIdx];
}

//==========================================================================
// �G�t�F�N�g�̉��
//==========================================================================
void CObject::ReleaseEffect(int nIdx)
{
	// �G�t�F�N�gnullptr�ɂ���
	m_pEffect[nIdx] = nullptr;

	// �G�t�F�N�g�̐e�ݒ肵�������Z
	m_nNumEffectParent--;
}

//==========================================================================
// �G�t�F�N�g�̉��
//==========================================================================
void CObject::UninitEffect()
{
	for (int nCntEffect = 0; nCntEffect < mylib_const::MAX_OBJ; nCntEffect++)
	{
		if (m_pEffect[nCntEffect] == nullptr)
		{// nullptr��������
			continue;
		}
		m_pEffect[nCntEffect]->UninitParent();
		m_pEffect[nCntEffect] = nullptr;
	}
}

//==========================================================================
// �I�u�W�F�N�g�̎��S����
//==========================================================================
void CObject::Release()
{
	// �G�t�F�N�g�̉��
	UninitEffect();

	// ���S�t���O�𗧂Ă�
	m_bDeath = true;
}
//
////==========================================================================
//// �I�u�W�F�N�g�̔j���E���S����
////==========================================================================
//void CObject::Death()
//{
//	
//}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CObject::SetVtx()
{

}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CObject::SetPosition(const MyLib::Vector3& pos)
{
	m_pos = pos;
}

//==========================================================================
// �ʒu���Z
//==========================================================================
void CObject::AddPosition(const MyLib::Vector3& pos)
{
	m_pos += pos;
}

//==========================================================================
// �ʒu�擾
//==========================================================================
MyLib::Vector3 CObject::GetPosition() const
{
	return m_pos;
}

//==========================================================================
// �ʒu�ݒ�
//==========================================================================
void CObject::SetOldPosition(const MyLib::Vector3& posOld)
{
	m_posOld = posOld;
}

//==========================================================================
// �ʒu�擾
//==========================================================================
MyLib::Vector3 CObject::GetOldPosition() const
{
	return m_posOld;
}

//==========================================================================
// ���̈ʒu�ݒ�
//==========================================================================
void CObject::SetOriginPosition(const MyLib::Vector3& pos)
{
	m_posOrigin = pos;
}

//==========================================================================
// ���̈ʒu�擾
//==========================================================================
MyLib::Vector3 CObject::GetOriginPosition() const
{
	return m_posOrigin;
}

//==========================================================================
// �ړ��ʐݒ�
//==========================================================================
void CObject::SetMove(const MyLib::Vector3& move)
{
	m_move = move;
}

//==========================================================================
// �ړ��ʉ��Z
//==========================================================================
void CObject::AddMove(const MyLib::Vector3& move)
{
	m_move += move;
}

//==========================================================================
// �ړ��ʎ擾
//==========================================================================
MyLib::Vector3 CObject::GetMove() const
{
	return m_move;
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CObject::SetRotation(const MyLib::Vector3& rot)
{
	m_rot = rot;
}

//==========================================================================
// �������Z
//==========================================================================
void CObject::AddRotation(const MyLib::Vector3& rot)
{
	m_rot += rot;
}

//==========================================================================
// �����擾
//==========================================================================
MyLib::Vector3 CObject::GetRotation() const
{
	return m_rot;
}

//==========================================================================
// ���̌����ݒ�
//==========================================================================
void CObject::SetOriginRotation(const MyLib::Vector3& rot)
{
	m_rotOrigin = rot;
}

//==========================================================================
// ���̌����擾
//==========================================================================
MyLib::Vector3 CObject::GetOriginRotation() const
{
	return m_rotOrigin;
}

//==========================================================================
// ��ނ̐ݒ�
//==========================================================================
void CObject::SetType(const TYPE type)
{
	m_type = type;
}

//==========================================================================
// ��ނ̎擾
//==========================================================================
CObject::TYPE CObject::GetType() const
{
	return m_type;
}

//==========================================================================
// �I�u�W�F�N�g�̎擾
//==========================================================================
CObject *CObject::GetObject()
{
	return this;
}

//==========================================================================
// ���S�̔���
//==========================================================================
bool CObject::IsDeath()
{
	return m_bDeath;
}

//==========================================================================
// �I�u�W�F�N�g2D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject2D *CObject::GetObject2D()
{
	return nullptr;
}

//==========================================================================
// �I�u�W�F�N�g3D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject3D *CObject::GetObject3D()
{
	return nullptr;
}

//==========================================================================
// �I�u�W�F�N�g3D���b�V���I�u�W�F�N�g�̎擾
//==========================================================================
CObject3DMesh *CObject::GetObject3DMesh()
{
	return nullptr;
}

//==========================================================================
// �I�u�W�F�N�gX�I�u�W�F�N�g�̎擾
//==========================================================================
CObjectX *CObject::GetObjectX()
{
	return nullptr;
}

//==========================================================================
// �I�u�W�F�N�g�L�����N�^�[�I�u�W�F�N�g�̎擾
//==========================================================================
CObjectChara *CObject::GetObjectChara()
{
	return nullptr;
}