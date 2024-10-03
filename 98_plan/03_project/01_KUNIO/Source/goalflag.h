//=============================================================================
// 
//  �T���v��_�I�u�W�F�N�gX�w�b�_�[ [sample_objX.h]
//  Author : �����V����̃v���O����(���؂肵�Ă܂���)
// 
//=============================================================================

#ifndef _GOALFLAG_H_
#define _GOALFLAG_H_		// ��d�C���N���[�h�h�~

#include "objectX.h"
#include "listmanager.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �T���v��_�I�u�W�F�N�gX�N���X
class CGoalflagX : public CObjectX
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum Sample
	{
		SAMPLE_WAO = 0,
		SAMPLE_WHO,
		SAMPLE_MAX
	};

	CGoalflagX(int nPriority = 6);
	~CGoalflagX();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	// �����o�֐�
	void Kill();		// �폜

	//=============================
	// �ÓI�֐�
	//=============================
	static CGoalflagX* Create(float fLength);
	static CListManager<CGoalflagX> GetListObj() { return m_List; }	// ���X�g�擾

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CGoalflagX::* GOALFLAG_FUNC)();
	static GOALFLAG_FUNC m_GoalflagFuncList[];	// �֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	void SampleWho();	// �t�[
	void SampleWao();	// ���I

	// ���̑�
	void Swing();	// �h��

	//=============================
	// �����o�ϐ�
	//=============================
	bool m_bClear;					// �N���A
	float m_fStateTime;				// ��ԃJ�E���^�[
	Sample m_state;					// ���
	MyLib::Vector3 m_DestRot_Old;	// �ڕW�̌���
	MyLib::Vector3 m_DestRot;		// �ڕW�̌���
	static CListManager<CGoalflagX> m_List;	// ���X�g

};


#endif