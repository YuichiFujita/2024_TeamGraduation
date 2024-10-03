//=============================================================================
// 
//  �K���X�����w�b�_�[ [glassclush.h]
//  Author : �����V����l
// 
//=============================================================================

#ifndef _GLASSCLUSH_H_
#define _GLASSCLUSH_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �T���v��_�I�u�W�F�N�g2D�N���X
class CGlassclush : public CObject2D
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_MAX
	};

	CGlassclush(int nPriority = 3);
	~CGlassclush();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//=============================
	// �ÓI�֐�
	//=============================
	static CGlassclush* Create();
	static CGlassclush* GetInstance() { return m_ThisPtr; }	// �C���X�^���X�擾
	static void Kill();

private:

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn

	// ���̑��֐�

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	float m_fStateTime;		// ��ԃJ�E���^�[
	State m_state;			// ���

	// ���̑��ϐ�
	static CGlassclush* m_ThisPtr;		// ���g�̃|�C���^
};


#endif