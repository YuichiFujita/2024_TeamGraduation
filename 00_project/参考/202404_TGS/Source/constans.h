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
	const int MAX_OBJ = 1600;			// �I�u�W�F�N�g�̍ő吔
	const int PRIORITY_NUM = 12;			// �D�揇�ʂ̍ő吔
	const int PRIORITY_DEFAULT = 3;		// �D�揇�ʂ̃f�t�H���g
	const int PRIORITY_DEF2D = 4;		// �D�揇�ʂ�2D�f�t�H���g
	const int PRIORITY_ZSORT = 10;		// Z�\�[�g��p�̗D�揇��
	const int NUM_KEY_MAX = 256;		// �L�[�̍ő吔
	const int MAX_STRING = 512;			// �ő啶����
	const int MAX_PARTS = 32;			// �p�[�c�ő吔
	const int MAX_PATTEN_ENEMY = 64;	// �G�̍ő�p�^�[��
	const int MAX_WAVE_ENEMY = 32;		// �G�̍ő�p�^�[��
	const int MAX_BASE_ENEMY = 32;		// �G�̋��_�ő吔
	const int MAX_ENEMY = 256;			// �G�̍ő吔
	const int ENEMY_PRIORITY = 2;		// �G�̗D�揇��
	const int MAX_FIXEDMOVE = 32;		// ���̓����ő吔
	const int MAX_FIXEDMOVE_INFO = 16;	// ���̓������ő吔
	const int MAX_CAMERAAXIS = 62;		// �J�������̍ő吔
	const int MAX_BULLET = 256;			// �e�̍ő吔
	const int MAX_LIMITEREA = 16;		// �G���A�����̍ő吔
	const int MAX_STAGE = 64;			// �X�e�[�W�̍ő吔
	const float GRAVITY = 0.7f;			// �d��
	const int MAX_VTX = 51200;			// ���_�̍ő吔
	const float KILL_Y = -600.0f;		// ���ł�����W
	const int DMG_BOUNCE = 15;			// ���˕Ԃ��̈З�
	const int DMG_SLASH = 2;			// �a���̈З�
	const int BULLETAPPEARANCE_NUM = 2;	// �����ڂ̒e�̐�
	const int MAX_PLAYER = 4;			// �v���C���[�̐�
	const int SHAPE_LIMITEREA = 4;		// �G���A�����̑��p�`
	const float MAX_ROCKONDISTANCE_GAME = 1600.0f;		// (�Q�[����)�ő働�b�N�I������
	const float MAX_ROCKONDISTANCE_BOSS = 4000.0f;		// (�{�X�펞)�ő働�b�N�I������
	const float RADIUS_STAGE = 2300.0f;					// �X�e�[�W�̔��a
	const D3DXCOLOR DEFAULT_COLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// �f�t�H���g�̐F
	const D3DXCOLOR FIELDCOLOR_01 = D3DXCOLOR(0.6f, 0.4f, 0.1f, 1.0f);	// �t�B�[���h�̐F
	const D3DXCOLOR PLAYERBEAM_COLOR = D3DXCOLOR(0.5f, 0.5f, 0.9f, 1.0f);	// �v���C���[�̃r�[���F
	const D3DXCOLOR UNIONBEAM_COLOR = D3DXCOLOR(0.2f, 0.2f, 0.9f, 1.0f);	// �v���C���[�̃r�[���F
	const D3DXCOLOR ENEMYBEAM_COLOR = D3DXCOLOR(0.8f, 0.1f, 0.8f, 1.0f);	// �G�̃r�[���F
	const MyLib::Vector3 DEFAULT_SCALE = MyLib::Vector3(1.0f, 1.0f, 1.0f);	// �X�P�[���̃f�t�H���g�l
	const MyLib::Vector3 DEFAULT_VECTOR3 = MyLib::Vector3(0.0f, 0.0f, 0.0f);	// Vector3�̃f�t�H���g�l
	const D3DXVECTOR2 DEFAULT_VECTOR2 = D3DXVECTOR2(0.0f, 0.0f);		// Vector2�̃f�t�H���g�l
}

#endif