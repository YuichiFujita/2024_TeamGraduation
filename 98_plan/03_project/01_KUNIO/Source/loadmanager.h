//=============================================================================
// 
// ���[�h�}�l�[�W�� [loadmanager.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _LOADMANAGER_H_
#define _LOADMANAGER_H_		// ��d�C���N���[�h�h�~

#include "scene.h"

class CLoadScreen;

//==========================================================================
// �N���X��`
//==========================================================================
// ���[�h�}�l�[�W���N���X
class CLoadManager
{
public:

	CLoadManager();
	~CLoadManager();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	void LoadScene(CScene::MODE mode);
	void UnLoad();	// ���[�h���
	void ResetLoad();	// ���[�h���Z�b�g
	bool IsLoadComplete();	// ���[�h�������t���O�擾

	// �ÓI�֐�
	static CLoadManager* Create();

private:

	//=============================
	// �����o�֐�
	//=============================
	void LoadInBackground();
	void Load();
	void ResetInternalLoad();	// �V�����V�[�����Z�b�g����O�̃��Z�b�g

	//=============================
	// �����o�ϐ�
	//=============================
	std::thread m_LoadingThread;
	bool m_bLoadComplete;		// ���[�h�����t���O
	CScene::MODE m_ModeNext;	// ���̃��[�h
	CLoadScreen* m_pLoadScreen;	// ���[�h���
};


#endif