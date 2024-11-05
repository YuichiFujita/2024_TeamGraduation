//=============================================================================
// 
//  �W���E�H�[���w�b�_�[ [sample_objX.h]
//  Author : Takeru
// 
//=============================================================================

#ifndef _GYM_WALL_H_
#define _GYM_WALL_H_		// ��d�C���N���[�h�h�~

#include "objectX.h"
#include "listmanager.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �T���v��_�I�u�W�F�N�gX�N���X
class CGymWall : public CObjectX
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

	CGymWall(int nPriority = 6);
	~CGymWall();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;

	//=============================
	// �����o�֐�
	//=============================
	void Kill();		// �폜

	//=============================
	// �ÓI�֐�
	//=============================
	/**
	@brief		��������
	@details	�K�v������Έ����ǉ�
	*/
	static CGymWall *Create();
	static CListManager<CGymWall> GetListObj() { return m_List; }	// ���X�g�擾

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CGymWall::*SAMPLE_FUNC)();
	static SAMPLE_FUNC m_SampleFuncList[];	// �֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	void SampleWho();	// �t�[
	void SampleWao();	// ���I

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fStateTime;				// ��ԃJ�E���^�[
	Sample m_state;					// ���
	static CListManager<CGymWall> m_List;	// ���X�g

};


#endif