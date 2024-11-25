//=============================================================================
//
// ���e�}�l�[�W������ [charmManager.h]
// Author : Kai Takada
//
//=============================================================================
#ifndef _CHARMMANAGER_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _CHARMMANAGER_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// �O���錾
//==========================================================================
class CPlayer;

//==========================================================================
// �N���X��`
//==========================================================================
// ���e�}�l�[�W���N���X
class CCharmManager
{
public:

	CCharmManager();
	~CCharmManager();

	//=============================
	// �����o�֐�
	//=============================
	virtual HRESULT Init();		// ������
	virtual void Uninit();		// �I��

	//--------------------------
	// ���̑�
	//--------------------------

	// �ÓI�֐�
	static CCharmManager* Create();	// ��������
	static CCharmManager* GetInstance() { return m_pThisPtr; }	// �C���X�^���X�擾

	//-----------------------------
	// �񃂃e�֐�
	//-----------------------------
	void UnCharm(CPlayer* pPlayer, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �񃂃e�܂Ƃ�

private:

	//-----------------------------
	// �����o�֐�
	//-----------------------------
	void LongHold(CPlayer* pPlayer, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���������Ă�
	void EdgeEscape(CPlayer* pPlayer, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �[�ɓ���������
	
	bool CheckEdgeEscape(CPlayer* pPlayer);		// �[�ɓ���������

	//=============================
	// �����o�ϐ�
	//=============================

	static CCharmManager* m_pThisPtr;	// ���g�̃|�C���^
};

#endif