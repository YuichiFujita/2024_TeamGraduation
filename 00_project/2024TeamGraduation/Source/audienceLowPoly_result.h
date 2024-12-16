//==========================================================================
// 
//  �ϋq_���[�|���L�������U���g�w�b�_�[ [audienceLowPoly_result.h]
//  Author : Kai Takada
// 
//==========================================================================

#ifndef _AUDIENCE_LOWPOLY_RESULT_H_
#define _AUDIENCE_LOWPOLY_RESULT_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "audienceLowPoly.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �ϋq_���[�|���L�����N���X��`
class CAudienceLowPolyResult : public CAudienceLowPoly
{
public:

	//=============================
	// �񋓌^
	//=============================
	// �ϐ�T�C�h��
	enum EAreaSide
	{
		SIDE_NONE = 0,	// ����
		SIDE_REVERSE,	// �t
		SIDE_MAX		// ���̗񋓌^�̑���
	};

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CAudienceLowPolyResult(CAudienceLowPoly::EObjType type, CGameManager::ETeamSide team);
	~CAudienceLowPolyResult();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

protected:

private:

	//=============================
	// �����o�֐�
	//=============================
	void CalcWatchPositionFar() override;	// �ϐ�ʒu�v�Z (��)
	void CalcWatchPositionSide() override;	// �ϐ�ʒu�v�Z (��)
	void CalcWatchPositionNear() override;	// �ϐ�ʒu�v�Z (��O)
	
	void EndSettingSpawn() override;		// �X�|�[���I�����̐ݒ�

	//=============================
	// �����o�ϐ�
	//=============================
	EAreaSide m_side;		// �ϐ�T�C�h
};

#endif
