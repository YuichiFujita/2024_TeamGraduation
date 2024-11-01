//=============================================================================
// 
//  �}�l�[�W���w�b�_�[ [manager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MANAGER_H_
#define _MANAGER_H_	// ��d�C���N���[�h�h�~

#include "main.h"

// �}�N����`
#define MOVE_LRUP			(0.75f)	// �΂ߏ�ړ�
#define MOVE_LR				(0.5f)	// �����ړ�
#define MOVE_LRDW			(0.25f)	// �΂ߏ�ړ�

// �O���錾
class CRenderer;
class CLight;
class CCamera;
class CInputKeyboard;
class CInputGamepad;
class CInputMouse;
class CSound;
class CDebugProc;
class CPlayer;
class CParticle;
class CScore;
class CTimer;
class CBG;
class CMap;
class CTexture;
class CXLoad;
class CEdit;
class CElevation;
class CEnemyManager;

//==========================================================================
// �N���X��`
//==========================================================================
// �}�l�[�W���N���X��`
class CManager
{
public:

	// �G�f�B�b�g�̗񋓌^��`
	enum EDITTYPE
	{
		EDITTYPE_EDIT = 0,	// �G�f�B�b�g
		EDITTYPE_PLAY,		// ���s
		EDITTYPE_SETUP,		// �Z�b�g�A�b�v
		EDITTYPE_MAX
	};

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Reset(void);
	static CRenderer *GetRenderer(void);
	static CInputKeyboard *GetInputKeyboard(void);
	static CInputGamepad *GetInputGamepad(void);
	static CInputMouse *GetInputMouse(void);
	static CSound *GetSound(void);
	static CDebugProc *GetDebugProc(void);
	static CPlayer *GetPlayer(void);
	static CPlayer **GetPlayer(int n);
	static CParticle *GetParticle(void);
	static CScore *GetScore(void);
	static CTimer *GetTimer(void);
	static CBG *GetBg(void);
	static CLight *GetLight(void);
	static CCamera *GetCamera(void);
	static CMap *GetMap(void);
	static CTexture *GetTexture(void);
	static CXLoad *GetXLoad(void);
	static CEdit *GetEdit(void);
	static CElevation *GetObject3DMesh(void);
	static CEnemyManager *GetEnemyManager(void);
	static EDITTYPE GetEditType(void) { return m_editType; }
	static void SetEditType(EDITTYPE type) { m_editType = type; }
private:

	static CRenderer *m_pRenderer;				// �����_���[�̃I�u�W�F�N�g
	static CInputKeyboard *m_pInputKeyboard;	// �L�[�{�[�h�̃I�u�W�F�N�g
	static CInputGamepad *m_pInputGamepad;		// �Q�[���p�b�h�̃I�u�W�F�N�g
	static CInputMouse *m_pInputMouse;			// �}�E�X�̃I�u�W�F�N�g
	static CSound *m_pSound;					// �T�E���h�̃I�u�W�F�N�g
	static CDebugProc *m_pDebugProc;			// �f�o�b�O�\���̃I�u�W�F�N�g
	static CPlayer *m_pPlayer;					// �v���C���[�̃I�u�W�F�N�g
	static CParticle *m_pParticle;				// �p�[�e�B�N���̃I�u�W�F�N�g
	static CScore *m_pScore;					// �X�R�A�̃I�u�W�F�N�g
	static CTimer *m_pTimer;					// �^�C�}�[�̃I�u�W�F�N�g
	static CBG *m_pBg;							// �w�i�̃I�u�W�F�N�g
	static CLight *m_pLight;					// ���C�g�̃I�u�W�F�N�g
	static CCamera *m_pCamera;					// �J�����̃I�u�W�F�N�g
	static CMap *m_pMap;						// �}�b�v�̃I�u�W�F�N�g
	static CTexture *m_pTexture;				// �e�N�X�`���̃I�u�W�F�N�g
	static CXLoad *m_pXLoad;					// X�t�@�C���̃I�u�W�F�N�g
	static CEdit *m_pEdit;						// �G�f�B�b�g�̃I�u�W�F�N�g
	static CElevation *m_pObject3DMesh;			// �I�u�W�F�N�g3D���b�V���̃I�u�W�F�N�g
	static CEnemyManager *m_pEnemyManager;		// �G�}�l�[�W���̃I�u�W�F�N�g
	static bool m_bWireframe;					// ���C���[�t���[��
	static EDITTYPE m_editType;					// �G�f�B�b�g�̎��
};



#endif