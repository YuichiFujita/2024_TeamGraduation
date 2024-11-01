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
	const int MAX_OBJ = 1600;			// �I�u�W�F�N�g�̍ő吔
	const int PRIORITY_NUM = 8;			// �D�揇�ʂ̍ő吔
	const int DEFAULT_PRIORITY = 3;		// �D�揇�ʂ̃f�t�H���g
	const int DEF2D_PRIORITY = 4;		// �D�揇�ʂ�2D�f�t�H���g
	const int MAX_STRING = 512;			// �ő啶����
	const int MAX_MODEL = 64;			// ���f���ő吔
	const int MAX_PARTS = 32;			// �p�[�c�ő吔
	const int MAX_PATTEN_ENEMY = 32;	// �G�̍ő�p�^�[��
	const int MAX_ENEMY = 256;			// �G�̍ő吔
	const float GRAVITY = 1.95f;		// �d��
	const D3DXCOLOR DEFAULT_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �f�t�H���g�̐F
	const D3DXVECTOR3 DEFAULT_SCALE = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// �X�P�[���̃f�t�H���g�l
	const D3DXVECTOR3 DEFAULT_VECTOR3 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// Vector3�̃f�t�H���g�l
	const D3DXVECTOR2 DEFAULT_VECTOR2 = D3DXVECTOR2(0.0f, 0.0f);		// Vector2�̃f�t�H���g�l
}

#endif