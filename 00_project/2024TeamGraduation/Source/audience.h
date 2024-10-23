//==========================================================================
// 
//  �ϋq�w�b�_�[ [audience.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _AUDIENCE_H_
#define _AUDIENCE_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "object.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �ϋq�N���X
class CAudience : public CObject
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum EObjType
	{
		OBJTYPE_ANIM = 0,	// 3D�|���S��
		OBJTYPE_LOWPOLY,	// ���[�|���L�����N�^�[
		OBJTYPE_HIGHPOLY,	// �n�C�|���L�����N�^�[
		OBJTYPE_MAX			// ���̗񋓌^�̑���
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CAudience(int nPriority = mylib_const::PRIORITY_DEFAULT, const LAYER layer = LAYER::LAYER_DEFAULT);
	~CAudience();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;
	virtual void Kill();	// �폜

	//=============================
	// �������z�֐�
	//=============================
	virtual void SetPosition(const MyLib::Vector3& pos) override = 0;	// �ʒu�ݒ�
	virtual MyLib::Vector3 GetPosition() const override = 0;			// �ʒu�擾
	virtual void SetMove(const MyLib::Vector3& move) override = 0;		// �ړ��ʐݒ�
	virtual MyLib::Vector3 GetMove() const override = 0;				// �ړ��ʎ擾
	virtual void SetRotation(const MyLib::Vector3& rot) override = 0;	// �����ݒ�
	virtual MyLib::Vector3 GetRotation() const override = 0;			// �����擾

	//=============================
	// ���z�֐�
	//=============================


	//=============================
	// �����o�֐�
	//=============================


	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CAudience* Create(EObjType type);

private:

	//=============================
	// �����o�ϐ�
	//=============================
	EObjType m_type;	// �I�u�W�F�N�g���
};

#endif
