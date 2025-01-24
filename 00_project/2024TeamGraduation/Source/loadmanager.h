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
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);
	void Draw();

	void LoadScene(CScene::MODE mode);
	void UnLoad();	// ���[�h���
	void ResetLoad();	// ���[�h���Z�b�g
	bool IsLoadComplete();	// ���[�h�������t���O�擾
	bool IsLoopMoment() const;	// ���[�h��ʌJ��Ԃ��u�Ԏ擾
	DWORD GetNumLoop() const;	// ���[�h��ʃ��[�v���擾
	void ResetNumLoop();		// ���[�h��ʃ��[�v�����Z�b�g
	void SetEnableAutoPlay(const bool bPlay);	// ���[�h��ʎ����Đ��ݒ�

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