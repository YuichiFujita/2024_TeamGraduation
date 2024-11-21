//=============================================================================
// 
//  �G���g���[�w�b�_�[ [entry.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _ENTRY_H_
#define _ENTRY_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "manager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CEntryScene;

//==========================================================================
// �N���X��`
//==========================================================================
// �G���g���[�N���X
class CEntry : public CScene
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum ESceneType
	{
		SCENETYPE_NONE = 0,			// �Ȃɂ��Ȃ�
		SCENETYPE_SETUPTEAM,		// �`�[�����ݒ�
		SCENETYPE_DRESSUP,			// �����ւ��E������ݒ�
		SCENETYPE_GAMESETTING,		// �ڍאݒ�
		SCENETYPE_MAX
	};

	CEntry();
	~CEntry();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

	//=============================
	// �����o�֐�
	//=============================
	//--------------------------
	// �V�[��
	//--------------------------
	void SetSceneType(ESceneType type);					// �V�[���ݒ�
	ESceneType GetSceneType() { return m_SceneType; }	// �V�[���擾

	//=============================
	// �ÓI�֐�
	//=============================
	static CEntry* GetInstance() { return m_pThisPtr; }	// �C���X�^���X�擾
	static CEntry* Create();							// ��������

private:

	//=============================
	// �֐��|�C���^
	//=============================
	typedef void(CEntry::* SCENE_FUNC)(const float, const float, const float);
	static SCENE_FUNC m_SceneFunc[];

	//=============================
	// �����o�֐�
	//=============================
	void SceneNone(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);			// �Ȃɂ��Ȃ�
	void SceneFadeInLogo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���S�t�F�[�h�C��
	void SceneFadeOutLoGo(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// ���S�t�F�[�h�A�E�g

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fSceneTime;			// �V�[���J�E���^�[
	ESceneType m_SceneType;		// �V�[���̎��
	CEntryScene* m_pEntryScene;	// �G���g���[�V�[��
	static CEntry* m_pThisPtr;	// ���g�̃|�C���^
};



#endif