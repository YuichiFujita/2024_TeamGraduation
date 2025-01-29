//==========================================================================
// 
//  �ϋq_�n�C�|���L�������U���g�w�b�_�[ [audienceHighPoly_result.h]
//  Author : Kai Takada
// 
//==========================================================================

#ifndef _AUDIENCE_HIGHPOLY_RESULT_H_
#define _AUDIENCE_HIGHPOLY_RESULT_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �C���N���[�h�t�@�C��
//==========================================================================
#include "audienceHighPoly.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObjectX;

//==========================================================================
// �N���X��`
//==========================================================================
// �ϋq_�n�C�|���L�����N���X��`
class CAudienceHighPolyResult : public CAudienceHighPoly
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
	CAudienceHighPolyResult(CAudienceHighPoly::EObjType type, CGameManager::ETeamSide team);
	~CAudienceHighPolyResult();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;

	bool SetNTR(CGameManager::ETeamSide team) override;		// NTR�ݒ�

protected:

private:
	//=============================
	// �����o�֐�
	//=============================
	void CalcWatchPositionFar() override;	// �ϐ�ʒu�v�Z (��)
	void CalcWatchPositionSide() override;	// �ϐ�ʒu�v�Z (��)
	void CalcWatchPositionNear() override;	// �ϐ�ʒu�v�Z (��O)
	
	void EndSettingSpawn() override;			// �X�|�[���I�����̐ݒ�

	//=============================
	// �����o�ϐ�
	//=============================
	EAreaSide m_side;		// �ϐ�T�C�h
};

#endif
