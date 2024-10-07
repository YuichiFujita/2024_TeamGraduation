//=============================================================================
// 
//  �ו��w�b�_�[ [baggage.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _BAGGAGE_H_
#define _BAGGAGE_H_		// ��d�C���N���[�h�h�~

#include "objectQuaternion.h"
#include "listmanager.h"

class CObject3D;
class CEffekseerObj;

//==========================================================================
// �N���X��`
//==========================================================================
// �ו��N���X
class CBaggage : public CObjectQuaternion
{
public:
	
	//=============================
	// �񋓌^��`
	//=============================
	enum TYPE
	{
		TYPE_CLOTH = 0,	// �z
		TYPE_TMP_YMDHOUSE,
		TYPE_TMP_FLOWER,
		TYPE_MAX
	};

	// ���
	enum STATE
	{
		STATE_NONE = 0,		// �ʏ�
		STATE_DAMAGE,		// �_���[�W
		STATE_DEAD,			// ���S
		STATE_APPEARANCE_WAIT,	// �o��
		STATE_APPEARANCE,	// �o��
		STATE_PASS,			// �p�X
		STATE_GOAL,			// �S�[��
		STATE_SEND,			// �͂���
		STATE_RETURN,		// ����x
		STATE_RECEIVE,		// receive
		STATE_FALL,			// ��������
		STATE_MAX
	};

	//=============================
	// �ו����
	//=============================
	struct SBaggageInfo
	{
		std::string path;	// ���f���t�@�C���p�X
		float weight;		// �d��
		float width;		// �Ԃꕝ
		float cycle;		// ����
		int life;			// �̗�

		SBaggageInfo() : path(""), weight(0.0f), width(0.0f), cycle(0.0f), life(0) {}
	};

	CBaggage(int nPriority = 4);
	~CBaggage();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	//=============================
	// �����o�֐�
	//=============================
	void Kill();		// �폜
	void UIDraw();
	void SetForce(const MyLib::Vector3& power) { m_force = power; }				// �͐ݒ�
	void AddForce(const MyLib::Vector3& power, const MyLib::Vector3& ActPos);	// �͒ǉ�
	void SetModelPass(const std::string path) { m_baggageInfo.path = path; }
	void SetCycle(const float& deviation) { m_baggageInfo.cycle = deviation; }
	void SetWidth(const float& deviWidth) { m_baggageInfo.width = deviWidth; }
	void SetWeight(const float& weight) { m_baggageInfo.weight = weight; }
	bool IsLand() { return m_bLand; }	// ���n����
	bool IsHit() { return m_bHit; }		// Hit()�ł̔����Ԃ�
	bool IsEnd() { return m_bEnd; }		// �I�������Ԃ�
	bool IsFall() { return m_bfall; }	// ��������
	void SetIsFall(bool fall) { m_bfall = fall; }	// ��������

	MyLib::Vector3 GetVeloRot() { return m_velorot; }	// ��]���x
	void SetVeloRot(const MyLib::Vector3& velorot) { m_velorot = velorot; }
	void Reset();						// ���胊�Z�b�g

	void SetAwayStartPosition(const MyLib::Vector3& pos) { m_posAwayStart = pos; }	// ������уX�^�[�g�n�_
	MyLib::Vector3 GetAwayStartPosition() { return m_posAwayStart; }				// ������уX�^�[�g�n�_


	int GetMapBlock() { return m_nMapBlock; }		// �}�b�v�̃u���b�N�擾

	// ��Ԍn
	void SetState(STATE state);				// ��Ԑݒ�
	STATE GetState() { return m_state; }	// ��Ԏ擾

	//=============================
	// �ÓI�֐�
	//=============================
	static CBaggage* Create(TYPE type, SBaggageInfo info);
	static CListManager<CBaggage> GetListObj() { return m_List; }	// ���X�g�擾

private:

	//=============================
	// �֐����X�g
	//=============================
	// ��ԃ��X�g
	typedef void(CBaggage::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];
	
	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	void UpdateState();		// ��ԍX�V
	void StateNone();		// �Ȃ�
	void StateDamage();		// �_���[�W
	void StateDead();		// ���S
	void StateAppearance_Wait();	// �o���҂�
	void StateAppearance();	// �o��
	void StatePass();		// �p�X
	void StateGoal();		// �S�[��
	void StateSend();		// �͂���
	void StateReturn();		// ����
	void StateReceive();	// ����
	void StateFall();

	// ���̑�
	void Controll();	// ����
	bool Hit();			// ��Q������
	void HitItem();		// �A�C�e���Ƃ̔���
	void DebugTransform();	// �f�o�b�O���ό`

	//=============================
	// �����o�ϐ�
	//=============================
	TYPE m_type;				// ���
	STATE m_state;				// ���
	float m_fStateTimer;		// ��ԃ^�C�}�[
	float m_fWeight;			// �d��
	MyLib::Vector3 m_force;		// ��
	MyLib::Vector3 m_velorot;	// ��]���x
	MyLib::Vector3 m_posAwayStart;		// ������уX�^�[�g�n�_
	bool m_bLand;				// ���n����
	bool m_bHit;				// ��Q���Փ˔���
	bool m_bEnd;				// �I������
	SBaggageInfo m_baggageInfo;
	int m_nMapBlock;			// �}�b�v�̃u���b�N
	int m_nLife;				// �̗�
	bool m_bfall;				// ��������
	float m_fDeviation;
	static CListManager<CBaggage> m_List;	// ���X�g
	CObject3D* m_p3D;
	CEffekseerObj* m_pEffekseerObj;		// �G�t�F�N�g�V�A�I�u�W�F�N�g
};


#endif