//=============================================================================
// 
//  �����΃}�l�[�W���w�b�_�[ [waterstoneManager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _WATERSTONE_MANAGER_H_
#define _WATERSTONE_MANAGER_H_		// ��d�C���N���[�h�h�~

#include "listmanager.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �����΃}�l�[�W���N���X
class CWaterStone_Manager
{
public:

	//=============================
	// �\���̒�`
	//=============================
	// �Ώ��
	struct SStoneInfo
	{
		MyLib::Vector3 pos;	// �ʒu
		MyLib::Vector3 rot;	// ����
		int type;			// ���

		SStoneInfo() : pos(), rot(), type(0) {}
		SStoneInfo(const MyLib::Vector3& _pos, const MyLib::Vector3& _rot, int _type) : pos(_pos), rot(_rot), type(_type) {}
	};


	CWaterStone_Manager();
	~CWaterStone_Manager();

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

	// ���o��
	void Save();	// �Z�[�u
	void Load();	// ���[�h

	//=============================
	// �ÓI�֐�
	//=============================
	static CWaterStone_Manager* Create();	// ����
	static CWaterStone_Manager* GetInstance() { return m_ThisPtr; }	// �C���X�^���X�擾
private:

	//=============================
	// �����o�֐�
	//=============================
	void CreateStone(const std::vector<SStoneInfo>& vecinfo);	// �ΐ���

	//=============================
	// �����o�ϐ�
	//=============================
	static CWaterStone_Manager* m_ThisPtr;		// ���g�̃|�C���^
};


#endif