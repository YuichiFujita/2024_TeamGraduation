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
#include "object3D_Anim.h"

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
	static constexpr float START_LINE = 300.0f;
	static constexpr float END_LINE = 600.0f;

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CAudienceAnim();
	~CAudienceAnim();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	void Kill() override;	// �폜����
	inline void SetPosition(const MyLib::Vector3& pos) override	{ m_pAnim3D->SetPosition(pos); }		// �ʒu�ݒ�
	inline MyLib::Vector3 GetPosition() const override			{ return m_pAnim3D->GetPosition(); }	// �ʒu�擾
	inline void SetMove(const MyLib::Vector3& move) override	{ m_pAnim3D->SetMove(move); }			// �ړ��ʐݒ�
	inline MyLib::Vector3 GetMove() const override				{ return m_pAnim3D->GetMove(); }		// �ړ��ʎ擾
	inline void SetRotation(const MyLib::Vector3& rot) override	{ m_pAnim3D->SetRotation(rot); }		// �����ݒ�
	inline MyLib::Vector3 GetRotation() const override			{ return m_pAnim3D->GetRotation(); }	// �����擾

private:

	//=============================
	// �����o�ϐ�
	//=============================
	CObject3DAnim* m_pAnim3D;	// �A�j���[�V����3D���
};

#endif
