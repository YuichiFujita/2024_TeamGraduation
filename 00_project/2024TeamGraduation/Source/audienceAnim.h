//==========================================================================
// 
//  �ϋq_�A�j���[�V����3D�w�b�_�[ [audienceAnim.h]
//  Author : ���c�E��
// 
//==========================================================================

#ifndef _AUDIENCE_ANIM_H_
#define _AUDIENCE_ANIM_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "audience.h"
#include "gamemanager.h"
#include "objectCharaAnim.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �ϋq_�A�j���[�V����3D�N���X��`
class CAudienceAnim : public CAudience
{
public:

	//=============================
	// �萔
	//=============================
	static constexpr float NEAR_LINE = 1350.0f;	// ��O�̐����ʒu���
	static constexpr float FAR_LINE = 1750.0f; 	// ���̐����ʒu���

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CAudienceAnim(EObjType type, CGameManager::TeamSide team);
	~CAudienceAnim();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	void Kill() override;	// �폜����
	inline void SetPosition(const MyLib::Vector3& pos) override	{ m_pAnimChara->SetPosition(pos); }		// �ʒu�ݒ�
	inline MyLib::Vector3 GetPosition() const override			{ return m_pAnimChara->GetPosition(); }	// �ʒu�擾
	inline void SetMove(const MyLib::Vector3& move) override	{ m_pAnimChara->SetMove(move); }		// �ړ��ʐݒ�
	inline MyLib::Vector3 GetMove() const override				{ return m_pAnimChara->GetMove(); }		// �ړ��ʎ擾
	inline void SetRotation(const MyLib::Vector3& rot) override	{ m_pAnimChara->SetRotation(rot); }		// �����ݒ�
	inline MyLib::Vector3 GetRotation() const override			{ return m_pAnimChara->GetRotation(); }	// �����擾

private:

	//=============================
	// �����o�ϐ�
	//=============================
	CObjectCharaAnim* m_pAnimChara;	// �L�����N�^�[�A�j���[�V�������
};

#endif
