//=============================================================================
// 
//  �}�l�[�W���w�b�_�[ [manager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MANAGER_H_
#define _MANAGER_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"
#include "scene.h"
#include "sound.h"
#include "MyEffekseer.h"
#include "EffekseerObj.h"

#include "input_mouse.h"
#include "input_keyboard.h"
#include "input_gamepad.h"

// �O���錾
class CRenderer;
class CLightManager;
class CCamera;
class CInput;
class CSound;
class CTexture;
class CDebugProc;
class CPlayer;
class CMap;
class CXLoad;
class CFont;
class CCharacterAnim;
class CEdit;
class CElevation;
class CEnemyManager;
class CFade;
class CInstantFade;
class CPause;
class CPowerGauge;
class CResultManager;
class CRankingManager;

//==========================================================================
// �N���X��`
//==========================================================================
// �}�l�[�W���N���X��`
class CManager
{
public:

	enum BuildMode
	{
		MODE_DEBUG = 0,
		MODE_RELEASE,
		MODE_MAX
	};

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();
	void Load();// �ǂݍ���

	static CManager *Create();		// ��������
	static CManager *GetInstance();	// �C���X�^���X�擾

	CRenderer *GetRenderer();
	CDebugProc *GetDebugProc();
	CLightManager *GetLight();
	CCamera *GetCamera();
	CEdit *GetEdit();
	CFont *GetFont();	// �t�H���g�擾
	CCharacterAnim* GetCharacterAnim();	// �L�����N�^�[�A�j���[�V�����擾
	CFade *GetFade();
	CInstantFade *GetInstantFade();	// �J�ڂȂ��t�F�[�h�擾
	CPause *GetPause();
	CResultManager *GetResultManager();		// ���U���g�}�l�[�W���擾
	CRankingManager *GetRankingManager();	// �����L���O�}�l�[�W���擾
	CScene *GetScene();	// �V�[���擾

	int GetNumPlayer();				// �v���C���[�̐��擾
	void SetNumPlayer(int nNum);	// �v���C���[�̐��ݒ�

	void UpdateDeltaTime();								// �o�ߎ��ԍX�V
	float GetDeltaTime() const { return m_fDeltaTime; }	// �o�ߎ��Ԏ擾
	void UpdateDeltaRate();								// �o�ߎ��Ԃ̊����X�V
	float GetDeltaRate() const { return m_fDeltaRate; }	// �o�ߎ��Ԃ̊����擾
	void SetSlowRate(const float fRate) { m_fSlowRate = fRate; }	// ���x�ቺ�̊����X�V
	float GetSlowRate() const { return m_fSlowRate; }				// ���x�ቺ�̊����擾

	void SetMode(CScene::MODE mode);				// ���̃��[�h�ݒ�
	CScene::MODE GetMode();							// ���݂̃��[�h�擾
	void ChangePauseMode(CScene::MODE mode);		// �|�[�Y�̎�ސ؂�ւ�
	bool IsWireframe() { return m_bWireframe; }		// ���C���[�t���[��
	CScene::MODE GetOldMode() { return m_OldMode; }	// �O��̃��[�h�擾
	bool IsLoadComplete() { return m_bLoadComplete; }
	bool Is2DDisp() { return m_bDisp_2D; }			// 2D�\���̔���
	void SerEnableDisp_UI(bool bDisp) { m_bDisp_UI = bDisp; }			// UI�\���̔���
	bool IsDisp_UI() { return m_bDisp_UI; }			// UI�\���̔���

	bool IsWindowed() { return m_bWindowed; }	// �E�B���h�E���[�h��
	void ChangeDisplayMode(bool bWindow);		// �f�B�X�v���C���[�h�ύX

	static bool IsDisp_ImGui()			{ return m_bDisp_ImGui; }			// ImGui�̕`�攻��
	static bool IsDisp_BoxColliders()	{ return m_bDisp_BoxColliders; }	// �����蔻��{�b�N�X�̕`�攻��
	static bool IsDisp_CheckPoint()		{ return m_bDisp_CheckPoint; }		// �`�F�b�N�|�C���g�̕`�攻��

private:

	void Reset(CScene::MODE mode);
	void NoLoadSetMode(CScene::MODE mode);		// ���̃��[�h�ݒ�

	CRenderer *m_pRenderer;				// �����_���[
	CInput*m_pInput;					// ���͋@��
	CSound* m_pSound;					// �T�E���h
	CDebugProc *m_pDebugProc;			// �f�o�b�O�\��
	CLightManager *m_pLight;			// ���C�g
	CCamera *m_pCamera;					// �J����
	CTexture *m_pTexture;				// �e�N�X�`��
	CXLoad* m_pXLoad;					// X�t�@�C��
	CFont* m_pFont;						// �t�H���g
	CCharacterAnim* m_pCharacterAnim;	// �L�����N�^�[�A�j���[�V����
	CEdit *m_pEdit;						// �G�f�B�b�g
	CScene *m_pScene;					// �V�[��
	CFade *m_pFade;						// �t�F�[�h
	CInstantFade *m_pInstantFade;		// �J�ڂȂ��t�F�[�h
	CPause *m_pPause;					// �|�[�Y
	CResultManager *m_pResultManager;	// ���U���g�}�l�[�W��
	CRankingManager *m_pRankingManager;	// �����L���O�}�l�[�W��
	CMyEffekseer* m_pMyEffekseer;		// �G�t�F�N�V�A

	bool m_bWireframe;		// ���C���[�t���[��
	CScene::MODE m_OldMode;	// �O��̃��[�h
	DWORD m_dwOldTime;		// �O��̏����J�n����
	DWORD m_dwCurTime;		// ����̏����J�n����
	float m_fDeltaTime;		// �o�ߎ���
	float m_fDeltaRate;		// �o�ߎ��Ԃ̊���
	float m_fSlowRate;		// ���x�ቺ�̊���
	int m_nNumPlayer;		// �v���C���[�̐�
	float m_fLoadTimer;		// ���[�h�̃^�C�}�[
	bool m_bLoadComplete;	// ���[�h�����̃t���O
	bool m_bLoadFadeSet;	// ���[�h�̃t�F�[�h�ݒ�t���O
	bool m_bNowLoading;		// ���[�h�����̃t���O
	bool m_bFirstLoad;		// ���񃍁[�h
	bool m_bDisp_2D;		// 2D�̕\��
	bool m_bDisp_UI;		// UI�̕\��
	bool m_bWindowed;		// �E�B���h�E���[�h��

	static CManager *m_pManager;	// �}�l�[�W��

	static bool m_bDisp_ImGui;			// ImGui�̕`�攻��
	static bool m_bDisp_BoxColliders;	// �����蔻��{�b�N�X�̕`�攻��
	static bool m_bDisp_CheckPoint;		// �`�F�b�N�|�C���g�̕`�攻��
};


#endif