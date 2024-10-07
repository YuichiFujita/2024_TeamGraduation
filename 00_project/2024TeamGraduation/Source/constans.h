//=============================================================================
// 
//  �萔�w�b�_�[ [constans.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _CONSTANS_H_
#define _CONSTANS_H_	// ��d�C���N���[�h�h�~

#include "main.h"

//==========================================================================
// ���O���
//==========================================================================
// �萔��`
namespace mylib_const
{
	const int DEFAULT_FPS = 60;			// �ʏ��FPS
	const int PRIORITY_NUM = 12;		// �D�揇�ʂ̍ő吔
	const int PRIORITY_DEFAULT = 3;		// �D�揇�ʂ̃f�t�H���g
	const int PRIORITY_DEF2D = 4;		// �D�揇�ʂ�2D�f�t�H���g
	const int PRIORITY_ZSORT = 10;		// Z�\�[�g��p�̗D�揇��
	const int NUM_KEY_MAX = 256;		// �L�[�̍ő吔
	const int MAX_STRING = 512;			// �ő啶����
	const int MAX_PARTS = 32;			// �p�[�c�ő吔
	const int MAX_OBJ = 128;			// �p�[�c�ő吔
	const float GRAVITY = 0.7f;			// �d��
	const int MAX_VTX = 51200;			// ���_�̍ő吔
	const float KILL_Y = -600.0f;		// ���ł�����W
	const int MAX_PLAYER = 4;			// �v���C���[�̐�
	const D3DXCOLOR DEFAULT_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �f�t�H���g�̐F
	const MyLib::Vector3 DEFAULT_SCALE = MyLib::Vector3(1.0f, 1.0f, 1.0f);	// �X�P�[���̃f�t�H���g�l
	const D3DXVECTOR2 DEFAULT_VECTOR2 = D3DXVECTOR2(0.0f, 0.0f);		// Vector2�̃f�t�H���g�l
}

#endif