//=============================================================================
// 
//  ��O���z�u���w�b�_�[ [frontobj_manager.h]
//  Author : Ibuki Okusada
// 
//=============================================================================

#ifndef _FRONTOBJ_MANAGER_H_
#define _FRONTOBJ_MANAGER_H_		// ��d�C���N���[�h�h�~

#include "listmanager.h"
#include "baggage.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �}�b�v�̏�Q���}�l�[�W���N���X
class CFrontObjManager
{
public:

	CFrontObjManager();
	~CFrontObjManager();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	//=============================
	// �����o�֐�
	//=============================
	void Kill();	// �폜
	std::vector<CObjectX*> GetInfo() { return m_BaggageInfo; }
	void Regist(CObjectX* pObj);
	void Reset() { m_BaggageInfo.clear();}
	void Erase(CObjectX* pObj) 
	{ 
		// �������g�����X�g����T��
		std::vector<CObjectX*>::iterator itr = std::find(m_BaggageInfo.begin(), m_BaggageInfo.end(), pObj);

		// �v�f�����������ꍇ�̂ݍ폜
		if (itr != m_BaggageInfo.end())
		{
			m_BaggageInfo.erase(itr);
		}
	}

	//=============================
	// �ÓI�֐�
	//=============================
	static CFrontObjManager* Create();	// ����
	static CFrontObjManager* GetInstance() { return m_ThisPtr; }	// �C���X�^���X�擾

private:

	//=============================
	// �����o�ϐ�
	//=============================
	std::vector<CObjectX*> m_BaggageInfo;	// ��Q�����
	static CFrontObjManager* m_ThisPtr;		// ���g�̃|�C���^
};


#endif