//=============================================================================
// 
//  �Q�[���w�b�_�[ [game.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _GAME_H_
#define _GAME_H_	// ��d�C���N���[�h�h�~

#include "scene.h"
#include "gamemanager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CEdit;

//==========================================================================
// �N���X��`
//==========================================================================
// �Q�[���N���X��`
class CGame : public CScene
{
public:

	// �G�f�B�b�g�^�C�v
	enum EditType
	{
		EDITTYPE_OFF = 0,		// �I�t
		EDITTYPE_MAP,			// �}�b�v
		EDITTYPE_OBSTACLE,		// ��Q��
		EDITTYPE_COURSE,		// �R�[�X
		EDITTYPE_WATERSTONE,	// ������
		EDITTYPE_JUDGEZONE,		// ����]�[��
		EDITTYPE_MAX
	};

	CGame();
	~CGame();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	EditType GetEditType() { return m_EditType; }
	CGameManager *GetGameManager();
	void SetEnableClear(bool bClear) { m_clear = bClear; }		// �N���A�̃t���O
	bool IsClear() { return m_clear; }						// �N���A�t���O�̎擾
	bool IsEdit() { return m_bEdit; }							// �G�f�B�b�g�̔���

	// �ÓI�֐�
	static CGame* GetInstance() { return m_pThisPtr; }	// �C���X�^���X�擾
	static CGame* Create(CScene::MODE mode);			// ��������

protected:

	virtual void InitByMode();	// ���[�h�ʏ�����

	//=============================
	// �����o�֐�
	//=============================
	void EditReset();
	void ChangeEdit();	// �G�f�B�b�g�؂�ւ�����

	//=============================
	// �����o�ϐ�
	//=============================
	CGameManager *m_pGameManager;			// �Q�[���}�l�[�W���̃I�u�W�F�N�g
	EditType m_EditType;					// �G�f�B�b�g�̎��
	CEdit* m_pEdit;							// �G�f�B�^�[
	bool m_bEdit;				// �G�f�B�b�g�̔���
	bool m_clear; // �N���A����

	static CGame* m_pThisPtr;	// ���g�̃|�C���^
};



#endif