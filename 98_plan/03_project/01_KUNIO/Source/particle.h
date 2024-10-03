//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _PARTICLE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _PARTICLE_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// ���O���
//==========================================================================
// �萔��`
namespace my_particle
{
	// �񋓌^��`
	enum TYPE
	{
		TYPE_NONE = 0,		// ���̃p�[�e�B�N��
		TYPE_SMOKE,			// ���̃p�[�e�B�N��
		TYPE_SMOKE_RED,		// ���̃p�[�e�B�N����
		TYPE_SMOKE_YEL,		// ���̃p�[�e�B�N����
		TYPE_EXPLOSION,		// �����̃p�[�e�B�N��
		TYPE_OFFSETTING,	// �e�ł�����
		TYPE_OFFSETTING_2D,	// �e�ł�����(2D)
		TYPE_ENEMY_FADE,	// �G�̃t�F�[�h
		TYPE_FIRE_WALLTORCH,	// �Ǐ����̉�
		TYPE_FIRE_STANDTORCH,	// �Ǐ����̉�
		TYPE_BATTLESTART,		// �퓬�J�n
		TYPE_FLOWERINGCHARGE,		// �J�ԃ`���[�W
		TYPE_POLLENDROP,	//�ԕ����Ƃ�
		TYPE_POLLENLOST,	//�Ԃ����ĉԕ����Ƃ�
		TYPE_MAX
	};

	void Create(const MyLib::Vector3& pos, my_particle::TYPE nType);
}

#endif