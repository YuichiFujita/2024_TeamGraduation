//=============================================================================
// 
//  �퓬�J�n�w�b�_�[ [battlestart.h]
//  Author : ���n�Ή�, �Ό��D�n
// 
//=============================================================================

#ifndef _BATTLESTART_H_
#define _BATTLESTART_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �퓬�J�n�N���X
class CBattleStart : public CObject2D
{
public:

	CBattleStart(int nPriority = 7);
	~CBattleStart();


	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();

	static CBattleStart* Create(const MyLib::Vector3& pos);

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

	enum STATE
	{
		STATE_EXPANTION = 0,
		STATE_WAIT,
		STATE_DROP,
		STATE_FADEOUT,
		STATE_MAX
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
	// �֐����X�g
	//=============================
	typedef void(CBattleStart::*STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// ��Ԋ֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	void StateExpansion();	//�g��
	void StateWait();		//�҂�
	void StateDrop();		//���Ƃ�
	void StateFadeOut();	// �t�F�[�h�A�E�g

	//=============================
	// �����o�ϐ�
	//=============================
	STATE m_state;		// ���
	float m_fStateTime;	// ��ԃ^�C�}�[
	SRockOnMarker m_RockOnInfo[VTXTYPE_MAX];	// �}�[�J�[���
};


#endif