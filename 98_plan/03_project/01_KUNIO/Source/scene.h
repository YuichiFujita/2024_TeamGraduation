//=============================================================================
// 
//  �V�[���w�b�_�[ [scene.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SCENE_H_
#define _SCENE_H_	// ��d�C���N���[�h�h�~

#include "elevation.h"

//==========================================================================
// �O���錾
//==========================================================================
class CCamera;
class CEdit_Map;

//==========================================================================
// �N���X��`
//==========================================================================
// �V�[���N���X��`
class CScene
{
public:
	// ���[�h�̗񋓌^��`
	enum MODE
	{
		MODE_NONE = 0,		// �Ȃ�
		MODE_TITLE,			// �^�C�g��
		MODE_TUTORIAL,		// �`���[�g���A��
		MODE_GAME,			// �Q�[��
		MODE_GAMETUTORIAL,	// �Q�[��
		MODE_RESULT,		// ���U���g
		MODE_RANKING,		// �����L���O
		MODE_MAX
	};

	CScene();
	~CScene();

	// �I�[�o�[���C�h����֐�
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	virtual void Kill();

	void ResetScene();	// �V�[���̃��Z�b�g
	static CScene *Create(MODE mode);
	static CElevation *GetElevation();	// �I�u�W�F�N�g3D�̎擾
	MODE GetMode();						// ���݂̃��[�h�擾

protected:

private:

	MODE m_mode;				// ���݂̃��[�h
	static CElevation *m_pObject3DMesh;				// �I�u�W�F�N�g3D���b�V���̃I�u�W�F�N�g
	CEdit_Map* m_pEditMap;
};



#endif