//=============================================================================
// 
//  �ڂ݃w�b�_�[ [flower_bud.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _FLOWER_BUD_H_
#define _FLOWER_BUD_H_	// ��d�C���N���[�h�h�~

#include "objectX.h"
#include "multinumber.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �ڂ݃N���X��`
class CFlowerBud : public CObjectX
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum STATE
	{
		STATE_WAIT = 0,		// �ҋ@
		STATE_CHARGE,		// �`���[�W
		STATE_FLOWERING,	// �J��
		STATE_MAX
	};

	CFlowerBud(int nPriority = 6);
	~CFlowerBud();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;

	void SetSatate(STATE state) { m_state = state; }
	void SetCurrentPollen(int maxPollen, int pollen);
	int GetSpawnNum() { return m_nSpawnNum; }

	static CFlowerBud* Create(const MyLib::Vector3& pos, int maxPollen, int currentPollen);	// ����
	static CFlowerBud* GetInstance() { return m_ThisPtr; }

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CFlowerBud::* STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// ��Ԋ֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	void StateWait() {}			// �ҋ@
	void StateCharge();			// ����
	void StateFlowering();		// �J��

	//=============================
	// �����o�ϐ�
	//=============================
	STATE m_state;		// ���
	float m_fStateTime;	// ��ԃ^�C�}�[
	float m_fRatio;	// ����
	int m_nSpawnNum;	// ������

	static CFlowerBud* m_ThisPtr;
};


#endif