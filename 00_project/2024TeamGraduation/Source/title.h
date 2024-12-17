//=============================================================================
// 
//  �^�C�g���w�b�_�[ [title.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TITLE_H_
#define _TITLE_H_	// ��d�C���N���[�h�h�~

#include "manager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CTitleLogo;
class CTitleScene;

//==========================================================================
// �N���X��`
//==========================================================================
// �^�C�g���N���X
class CTitle : public CScene
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum ESceneType
	{
		SCENETYPE_NONE = 0,			// �Ȃɂ��Ȃ�
		SCENETYPE_CONTROLLWAIT,		// ����҂�
		SCENETYPE_SUSURU,			// SUSURU
		SCENETYPE_MAX
	};

	enum EState
	{
		STATE_NONE = 0,		// NONE
		STATE_CHANGESCENE,	// �V�[���؂�ւ���
		STATE_MAX
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CTitle();
	~CTitle();

	//=============================
	// �I�[�o�[���C�h���ꂽ�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

	// �V�[���̎��
	ESceneType GetSceneType() { return m_SceneType; }
	void ChangeScene(ESceneType type);	// �V�[���؂�ւ�


	//=============================
	// �ÓI�֐�
	//=============================
	static CTitle* GetInstance();	// �C���X�^���X�擾
	static CTitle* Create();		// ��������

private:

	//=============================
	// �֐��|�C���^
	//=============================
	typedef void(CTitle::* STATE_FUNC)(const float, const float, const float);
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	void UpdateState(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// ��ԍX�V����
	void StateNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �Ȃɂ��Ȃ�
	void StateChangeScene(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �V�[���؂�ւ�

	//=============================
	// �����o�ϐ�
	//=============================
	EState m_state;				// ���
	ESceneType m_SceneType;		// �V�[���̎��
	float m_fSceneTime;			// �V�[���J�E���^�[
	CTitleLogo* m_pLogo;		// ���S�̃|�C���^
	CTitleScene* m_pTitleScene;		// �^�C�g���V�[��
	static CTitle* m_pThisPtr;	// ���g�̃|�C���^
};



#endif