//=============================================================================
//
// ���e�}�l�[�W������ [charmManager.h]
// Author : Kai Takada
//
//=============================================================================
#ifndef _CHARMMANAGER_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _CHARMMANAGER_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "charmValueManager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPlayer;
class CCollisionLine_Box;

//==========================================================================
// �N���X��`
//==========================================================================
// ���e�}�l�[�W���N���X
class CCharmManager
{
public:

	//=============================
	// �萔
	//=============================
	static constexpr float TIME_CHAIN = 3.0f;	// �`�F�C���\�Ȏ���

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CCharmManager();
	~CCharmManager();

	//=============================
	// �����o�֐�
	//=============================
	virtual HRESULT Init();		// ������
	virtual void Uninit();		// �I��
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �X�V

	//-----------------------------
	// ���e����グ�֐�
	//-----------------------------
	float GetPrisetHypeTime(const CCharmValueManager::ETypeAdd preset);				// �v���Z�b�g����オ�莞�Ԏ擾
	void SetHypeTime(const CCharmValueManager::ETypeAdd preset);					// ����オ�莞�Ԑݒ� (�v���Z�b�g)
	inline void SetHypeTime(const float fTime)	{ m_fHypeTime = fTime; }			// ����オ�莞�Ԑݒ� (���Ԏw��)
	inline float GetHypeTime() const			{ return m_fHypeTime; }				// ����オ�莞�Ԏ擾
	inline bool IsHype() const					{ return (m_fHypeTime > 0.0f); }	// ����オ��t���O�擾

	//-----------------------------
	// �񃂃e�֐�
	//-----------------------------
	void UnCharm(CPlayer* pPlayer, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �񃂃e�܂Ƃ�

	//--------------------------
	// ���̑�
	//--------------------------

	//=============================
	// �ÓI�֐�
	//=============================
	static CCharmManager* Create();		// ��������
	static CCharmManager* GetInstance() { return m_pThisPtr; }	// �C���X�^���X�擾

private:

	//=============================
	// �����o�֐�
	//=============================
	void LongHold(CPlayer* pPlayer, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���������Ă�
	void EdgeEscape(CPlayer* pPlayer, const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �[�ɓ���������
	bool CheckEdgeEscape(CPlayer* pPlayer);	// �[�ɓ���������

	//=============================
	// �����o�ϐ�
	//=============================
#if _DEBUG
	CCollisionLine_Box* m_pCourtSizeBox[4];	// �R�[�g�T�C�Y�̃{�b�N�X
#endif
	static CCharmManager* m_pThisPtr;	// ���g�̃|�C���^
	float m_fHypeTime;	// ����オ�莞��
};

#endif