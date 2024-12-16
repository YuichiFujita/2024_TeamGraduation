//==========================================================================
// 
//  �ϋq_�A�j���[�V����3D���U���g�w�b�_�[ [audienceAnim_result.h]
//  Author : Kai Takada
// 
//==========================================================================

#ifndef _AUDIENCE_ANIM_RESULT_H_
#define _AUDIENCE_ANIM_RESULT_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "audienceAnim.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �ϋq_�A�j���[�V����3D�N���X��`
class CAudienceAnimResult : public CAudienceAnim
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CAudienceAnimResult(CAudienceAnim::EObjType type, CGameManager::ETeamSide team);
	~CAudienceAnimResult();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

protected:

private:

	void CalcWatchPositionFar() override;	// �ϐ�ʒu�v�Z (��)
	void CalcWatchPositionUp() override;		// �ϐ�ʒu�v�Z (��)
	void CalcWatchPositionNear() override;	// �ϐ�ʒu�v�Z (��O)
};

#endif
