//=============================================================================
// 
// Imgui�̃}�l�[�W�� [Imguimanager.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _IMGUIMANAGER_H_
#define _IMGUIMANAGER_H_		// ��d�C���N���[�h�h�~

/**
	@brief	Imgui�̃}�l�[�W��
*/
namespace ImguiMgr
{
	void Init(HWND hwnd, LPDIRECT3DDEVICE9 pd3dDevice);	// ����������
	void Update();	// �X�V����
	void Draw();	// �`�揈��
	void Uninit();	// �I������

	void Backup();
	void Restore();
}



#endif