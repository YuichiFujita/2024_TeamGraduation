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

	CTitle();
	~CTitle();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

	// �V�[���̎��
	ESceneType GetSceneType() { return m_SceneType; }
	void ChangeScene(ESceneType type);	// �V�[���؂�ւ�


	// �ÓI�֐�
	static CTitle* GetInstance();	// �C���X�^���X�擾
	static CTitle* Create();		// ��������

private:

	//=============================
	// �֐��|�C���^
	//=============================
	typedef void(CTitle::* SCENE_FUNC)(const float, const float, const float);
	static SCENE_FUNC m_SceneFunc[];

	//=============================
	// �����o�֐�
	//=============================
	void UpdateScene(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);		// �V�[���̍X�V����
	void SceneNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �Ȃɂ��Ȃ�
	void SceneFadeInLogo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���S�t�F�[�h�C��
	void SceneFadeOutLoGo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���S�t�F�[�h�A�E�g

	//=============================
	// �����o�ϐ�
	//=============================
	ESceneType m_SceneType;		// �V�[���̎��
	float m_fSceneTime;			// �V�[���J�E���^�[
	CTitleLogo* m_pLogo;		// ���S�̃|�C���^
	CTitleScene* m_pTitleScene;		// �^�C�g���V�[��
	static CTitle* m_pThisPtr;	// ���g�̃|�C���^
};



#endif