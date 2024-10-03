//=============================================================================
// 
//  ����\���w�b�_�[ [judgeobj.h]
//  Author : �Ό��D�n
// 
//=============================================================================

#ifndef _JUDGE_OBJ_H_
#define _JUDGE_OBJ_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"
#include "judge.h"

//�O���錾
class CJudgeStateFunc;

//==========================================================================
// �N���X��`
//==========================================================================
// �퓬�J�n�N���X
class CJudgeObj : public CObject2D
{
public:

	CJudgeObj(int nPriority = 7);
	~CJudgeObj();


	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();

	static CJudgeObj* Create(const MyLib::Vector3& pos,const CJudge::JUDGE judge);
	void CreateState();

private:

	//=============================
	// �񋓌^��`
	//=============================
	enum VTXTYPE
	{
		VTXTYPE_RIGHTUP = 0,	// �E��
		VTXTYPE_LEFTDW,		// ����
		VTXTYPE_MAX
	};

	// �\���̒�`
	struct SRockOnMarker
	{
		CObject2D* p2D;	// 2D�I�u�W�F�̃|�C���^
		float distance;		// �Ԋu
		int Angle;			// ����
		float distancetimer;	// ��������

		SRockOnMarker() : p2D(nullptr), distance(0.0f), Angle(0), distancetimer(0.0f) {}
	};

	//=============================
	// �����o�ϐ�
	//=============================
	SRockOnMarker m_RockOnInfo[VTXTYPE_MAX];	// �}�[�J�[���
	CJudgeStateFunc* m_pStateFunc;	// ��Ԋ֐��i����ɂ���ĕς��j
	int m_Judge;
};

#endif