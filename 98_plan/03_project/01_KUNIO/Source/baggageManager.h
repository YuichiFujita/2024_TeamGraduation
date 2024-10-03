//=============================================================================
// 
//  �ו��}�l�[�W���w�b�_�[ [baggageManager.h]
//  Author : �Ό��D�n
// 
//=============================================================================

#ifndef _BAGGAGE_MANAGER_H_
#define _BAGGAGE_MANAGER_H_		// ��d�C���N���[�h�h�~

#include "listmanager.h"
#include "baggage.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �}�b�v�̏�Q���}�l�[�W���N���X
class CBaggageManager
{
public:

	CBaggageManager();
	~CBaggageManager();

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

	// ���n
	void SetBaggageInfo(const std::vector<CBaggage::SBaggageInfo>& info) { m_BaggageInfo = info; }	// �ו����ݒ�
	void SetBaggageInfo(const CBaggage::SBaggageInfo& info, int idx) { m_BaggageInfo[idx] = info; }	// �ו����ݒ�
	std::vector<CBaggage::SBaggageInfo> GetBaggageInfo() { return m_BaggageInfo; }					// �ו����擾
	CBaggage::SBaggageInfo GetBaggageInfo(int idx) { return m_BaggageInfo[idx]; }						// �ו����擾

	// ����
	CBaggage* CreateBaggage(CBaggage::TYPE idx);

	// ���o��
	void Save();	// �Z�[�u
	void Load();	// ���[�h

	//=============================
	// �ÓI�֐�
	//=============================
	static CBaggageManager* Create();	// ����
	static CBaggageManager* GetInstance() { return m_ThisPtr; }	// �C���X�^���X�擾
private:

	//=============================
	// �����o�ϐ�
	//=============================
	std::vector<CBaggage::SBaggageInfo> m_BaggageInfo;	// ��Q�����
	static CBaggageManager* m_ThisPtr;		// ���g�̃|�C���^
};


#endif