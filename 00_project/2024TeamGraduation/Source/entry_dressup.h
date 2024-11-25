//=============================================================================
//
// �h���X�A�b�v�ݒ菈�� [entry_dressup.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _ENTRY_DRESSUP_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _ENTRY_DRESSUP_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "entryscene.h"
#include "player.h"

//==========================================================================
// �O���錾
//==========================================================================
class CDressup;

//==========================================================================
// �N���X��`
//==========================================================================
// �h���X�A�b�v�ݒ�N���X
class CEntry_Dressup : public CEntryScene
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	// �G�f�B�b�g������
	enum EEditType
	{
		EDIT_PROCESS = 0,	// ���ۂ̕ύX
		EDIT_CHANGETYPE,	// �ύX������
		EDIT_MAX
	};

	// �ύX������
	enum EChangeType
	{
		TYPE_HAIR = 0,		// ��
		TYPE_ACCESSORY,		// �A�N�Z�T���[
		TYPE_FACE,			// ��
		TYPE_BODY,			// �̌^
		TYPE_HANDEDNESS,	// ������
		TYPE_MAX
	};

	//=============================
	// �\���̒�`
	//=============================
	struct SEntryDressup	// �����ւ����
	{
		CPlayer* pPlayer;		// �v���C���[
		CDressup* pHair;		// �������ւ�
		CDressup* pAccessory;	// �A�N�Z�����ւ�
		CDressup* pFace;		// �璅���ւ�
		EEditType editType;		// �G�f�B�b�g������
		EChangeType changeType;	// �ύX������

		// �R���X�g���N�^
		SEntryDressup() : pPlayer(nullptr), pHair(nullptr), pAccessory(nullptr), pFace(nullptr), editType(EEditType::EDIT_PROCESS), changeType(EChangeType::TYPE_HAIR) {}
	};

	CEntry_Dressup();
	~CEntry_Dressup();

	//=============================
	// �����o�֐�
	//=============================
	virtual HRESULT Init() override;		// ������
	virtual void Uninit() override;		// �I��
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;	// �X�V

	//--------------------------
	// ���̑�
	//--------------------------
	virtual void Debug() override;

private:

	//=============================
	// �����o�֐�
	//=============================
	void Save();					// �Z�[�u
	void Load();					// ���[�h
	void ChangeEditType(int nLoop, int nControllIdx);		// �G�f�B�b�g�����ޕύX
	void ChangeChangeType(int nLoop, int nControllIdx);	// �ύX����ӏ��̎�ޕύX

	void ChangeBodyType(int nLoop, int nControllIdx);		// �̌^�ύX
	void ChangeHandedness(int nLoop, int nControllIdx);	// ������ύX
	void ReCreatePlayer(int i, CPlayer::EHandedness handedness, CPlayer::EBody body);		// �v���C���[�Đ���

	//=============================
	// �����o�ϐ�
	//=============================
	int m_nPlayerNum[CGameManager::ETeamSide::SIDE_MAX];	// �v���C���[�̐�
	std::vector<SEntryDressup> m_vecDressupInfo;			// �����ւ����
};

#endif