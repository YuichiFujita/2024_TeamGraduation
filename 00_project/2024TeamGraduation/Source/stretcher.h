//=============================================================================
// 
//  �S�˃w�b�_�[ [stretcher.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _STRETCHER_H_
#define _STRETCHER_H_		// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "gamemanager.h"
#include "objectChara.h"
#include "listmanager.h"
#include "objectX.h"

//==========================================================================
// �O���錾
//==========================================================================
class CSchoolNurse;	// �ی��̐搶
class CPlayer;		// �v���C���[

//==========================================================================
// �N���X��`
//==========================================================================
// �S�˃N���X
class CStretcher : public CObjectX
{
public:
	
	//=============================
	// �񋓌^��`
	//=============================
	// ��Ԓ�`
	enum EState
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_GO,			// ������
		STATE_COLLECT,		// ���
		STATE_BACK,			// �߂�
		STATE_MAX
	};

	CStretcher(int nPriority = 6);
	~CStretcher();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	void Kill() override;		// �폜

	//=============================
	// �����o�֐�
	//=============================
	void SetState(EState state);							// ��Ԑݒ�
	void SetEnableEnd(bool bEnd) { m_bEndSetting = bEnd; }	// �I���̐ݒ�
	CPlayer* GetCollectPlayer();							// �������v���C���[�擾

	//=============================
	// �ÓI�֐�
	//=============================
	// ��������
	static CStretcher* Create(CPlayer* pPlayer);

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CStretcher::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];							// ��Ԋ֐�

	//=============================
	// �����o�֐�
	//=============================
	//-----------------------------
	// ��Ԋ֐�
	//-----------------------------
	virtual void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// ��ԍX�V
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �Ȃ�
	void StateGo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ������
	void StateCollect(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���
	void StateBack(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �߂�

	//-----------------------------
	// ���̑�
	//-----------------------------
	void PosAdjNurse();	// �搶�̈ʒu�␳
	void AddCollectPlayer(CPlayer* pPlayer) { m_pCollectPlayer.push_back(pPlayer); }	// �������v���C���[�ǉ�
	void CollectEndSetting();				// ����I���̐ݒ�

	//=============================
	// �����o�ϐ�
	//=============================
	//-----------------------------
	// ���
	//-----------------------------
	EState m_Oldstate;							// �O��̏��
	EState m_state;								// ���
	float m_fStateTime;							// ��Ԏ���

	//-----------------------------
	// ���̑��ϐ�
	//-----------------------------
	std::vector<CPlayer*> m_pCollectPlayer;		// �������v���C���[
	std::vector<CPlayer*> m_pCarryPlayer;		// �^�ԃv���C���[
	CSchoolNurse* m_pLeftNurse;					// ���̐搶
	CSchoolNurse* m_pRightNurse;				// �E�̐搶
	CShadow* m_pShadow;							// �e
	float m_fWidth;								// ��
	MyLib::Vector3 m_startPos;					// �J�n�ʒu
	bool m_bEndSetting;							// �I���̃Z�b�e�B���O
	static CStretcher* m_pThisPtr;				// ���g�̃|�C���^
};


#endif