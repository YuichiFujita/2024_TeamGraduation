//=============================================================================
// 
//  �^�C�g���w�b�_�[ [title.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TITLE_H_
#define _TITLE_H_	// ��d�C���N���[�h�h�~

#include "manager.h"

class CTitleLogo;
class CTitle_PressEnter;
class CKeyConfigSetting;
class CPeopleManager;

//==========================================================================
// �O���錾
//==========================================================================
class CSpawnEnvironment;

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
	enum SCENETYPE
	{
		SCENETYPE_NONE = 0,			// �Ȃɂ��Ȃ�
		SCENETYPE_FADEIN,			// �t�F�[�h�C��
		SCENETYPE_FADEOUT_LOGO,		// ���S�t�F�[�h�A�E�g
		SCENETYPE_SOUNDSETTING,		// �T�E���h�ݒ�
		SCENETYPE_OTHERSETTING,		// ���̑��ݒ�
		SCENETYPE_MAX
	};

	CTitle();
	~CTitle();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// �V�[���̎��
	void SetSceneType(SCENETYPE type) { m_SceneType = type; }
	SCENETYPE GetSceneType() { return m_SceneType; }

	// ���̑�
	CTitle_PressEnter* GetTitlePressEnter() { return m_pPressEnter; }
	CKeyConfigSetting* GetSetting() { return m_pConfigSetting; }
	void SetSetting(CKeyConfigSetting* pSetting) { m_pConfigSetting = pSetting; }

	// �ÓI�֐�
	static CTitle* GetInstance();	// �C���X�^���X�擾
	static CTitle* Create();		// ��������

private:

	//=============================
	// �֐��|�C���^
	//=============================
	typedef void(CTitle::* SCENE_FUNC)();
	static SCENE_FUNC m_SceneFunc[];

	//=============================
	// �����o�֐�
	//=============================
	void SceneNone();			// �Ȃɂ��Ȃ�
	void SceneFadeInLogo();		// ���S�t�F�[�h�C��
	void SceneFadeOutLoGo();		// ���S�t�F�[�h�A�E�g

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fSceneTime;			// �V�[���J�E���^�[
	SCENETYPE m_SceneType;		// �V�[���̎��
	CTitleLogo* m_pLogo;		// ���S�̃|�C���^
	CKeyConfigSetting* m_pConfigSetting;
	CTitle_PressEnter* m_pPressEnter;	// �v���X�G���^�[
	CSpawnEnvironment* m_pSpawn_Leaf;	// �~��t����
	CPeopleManager* m_pPeopleManager;	// �l�}�l�[�W��

	static CTitle* m_pThisPtr;	// ���g�̃|�C���^
};



#endif