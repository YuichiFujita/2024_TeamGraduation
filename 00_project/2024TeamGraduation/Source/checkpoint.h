//=============================================================================
// 
//  �`�F�b�N�|�C���g[checkpoint.h]
//  Author : �����V����̃v���O����(���؂肵�Ă܂���)
// 
//=============================================================================

#ifndef _CHECKPOINT_H_
#define _CHECKPOINT_H_		// ��d�C���N���[�h�h�~

#include "objectX.h"
#include "listmanager.h"

class CObjectBillboard;
class CEffekseerObj;

//==========================================================================
// �N���X��`
//==========================================================================
//�`�F�b�N�|�C���g�N���X
class CCheckpoint : public CObjectX
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum State
	{
		SWING = 0,
		ROTATE,
		MAX
	};

	CCheckpoint(int nPriority = 6);
	~CCheckpoint();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	// �����o�֐�
	void Kill();		// �폜
	float GetLength() { return m_fLength; }	// �����擾
	void SetLength(const float length);	// �����ݒ�
	float GetPassedTime() { return m_fPassedTime; }

	//=============================
	// �ÓI�֐�
	//=============================
	static CCheckpoint* Create(const float length);
	static void Load(const std::string filename);
	static int GetSaveID() { return m_nSaveID; }	// �Z�[�uID�擾
	static void ResetSaveID();	// �Z�[�uID���Z�b�g
	static CListManager<CCheckpoint> GetListObj() { return m_List; }	// ���X�g�擾

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CCheckpoint::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	// ���
	void StateSwing();	// �����
	void StateRotate();	// ��]

	// ���̑�
	void CreateEffect();	// �G�t�F�N�g����
	void CreateWaterEffect(int max);	// ���G�t�F�N�g����

	//=============================
	// �����o�ϐ�
	//=============================
	State m_state;					// ���
	float m_fStateTime;				// ��ԃJ�E���^�[
	float m_fLength;				// ����
	float m_fPassedTime;			// �ʉ߂����Ƃ��̎���
	bool m_bIsPassed;				// �ʉ߂������ǂ���
	MyLib::Vector3 m_DestRot_Old;	// �ڕW�̌���
	MyLib::Vector3 m_DestRot;		// �ڕW�̌���
	CObjectBillboard* m_pEffect;	// �G�t�F�N�g�̃|�C���^
	CEffekseerObj* m_pEffekseerObj;	// �G�t�F�N�V�A�̃I�u�W�F�N�g
	static CListManager<CCheckpoint> m_List;	// ���X�g

	static int m_nAll;		// ����
	static int m_nSaveID;	// save id
	int m_MyIndex;			// ������ID
};


#endif