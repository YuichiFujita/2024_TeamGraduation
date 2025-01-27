//=============================================================================
// 
//  �`���[�g���A���w�b�_�[ [tutorial.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "manager.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �`���[�g���A���N���X
class CTutorial : public CScene
{
public:

	//=============================
	// �R���X�g���N�^,�f�X�g���N�^
	//=============================
	CTutorial();
	~CTutorial();

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
	

	//=============================
	// �ÓI�֐�
	//=============================
	static CTutorial* GetInstance() { return m_pThisPtr; }	// �C���X�^���X�擾
	static CTutorial* Create();							// ��������

private:

	//=============================
	// �֐��|�C���^
	//=============================
	typedef void(CTutorial::* SCENE_FUNC)(const float, const float, const float);
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
	static CTutorial* m_pThisPtr;		// ���g�̃|�C���^
};



#endif