//=============================================================================
//
// ���e�}�l�[�W������ [charmManager.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _CHARMMANAGER_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _CHARMMANAGER_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// ���e�}�l�[�W���N���X
class CCharmManager
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum ETypeAdd
	{
		ADD_HIT = 0,		// �q�b�g
		ADD_JUSTCATCH,		// �W���X�g�L���b�`
		ADD_COVERCATCH,		// �J�o�[�L���b�`
		ADD_DODGE,			// ���
		ADD_SPECIAL,		// �X�y�V����
		ADD_MAX
	};

	//=============================
	// �񋓌^��`
	//=============================
	enum ETypeSub
	{
		SUB_INVADE_RETURN_HIT = 0,		// ���C���z���Ė߂��Ă�Œ��ɂ�����
		SUB_EDGE_ESCAPE,				// �[�ɓ����܂���
		SUB_INVADE_RUN,					// �����Ă����ă��C���z��(�{�[������)
		SUB_LONG_HOLD,					// �����ƃ{�[�������ē����Ȃ�
		SUB_MAX
	};

	CCharmManager();
	~CCharmManager();

	//=============================
	// �����o�֐�
	//=============================
	virtual HRESULT Init();		// ������
	virtual void Uninit();		// �I��

	//--------------------------
	// ���̑�
	//--------------------------
	virtual float GetAddValue(ETypeAdd type);	// ���Z�ʎ擾
	virtual float GetSubValue(ETypeSub type);	// ���Z�ʎ擾

	// �ÓI�֐�
	static CCharmManager* Create();	// ��������
	static CCharmManager* GetInstance() { return m_pThisPtr; }	// �C���X�^���X�擾

private:


	//=============================
	// �����o�ϐ�
	//=============================
	float m_fAddValue[ETypeAdd::ADD_MAX];	// ���Z��
	float m_fSubValue[ETypeSub::SUB_MAX];	// ���Z��
	static CCharmManager* m_pThisPtr;	// ���g�̃|�C���^
};

#endif