//=============================================================================
//
// �����ւ����� [dressup.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _DRESSUP_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _DRESSUP_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// �O���錾
//==========================================================================
class CObjectChara;

//==========================================================================
// �N���X��`
//==========================================================================
// �����ւ��N���X
class CDressup
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum EType
	{
		TYPE_HAIR = 0,	// ��
		TYPE_ACCESSORY,	// �A�N�Z�T���[
		TYPE_MAX
	};

	CDressup();
	~CDressup();

	//=============================
	// �����o�֐�
	//=============================
	virtual HRESULT Init();		// ������
	virtual void Uninit();		// �I��
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate);	// �X�V

	//--------------------------
	// ���̑�
	//--------------------------
	virtual void Debug();

	// �ÓI�֐�
	static CDressup* Create(EType type, CObjectChara* pObjChara, int nSwitchType);	// ��������

protected:

	void LoadAll(const std::wstring& folder);	// �S�ǂݍ���

	//=============================
	// �����o�ϐ�
	//=============================
	int m_nSwitchIdx;			// �؂�ւ���C���f�b�N�X
	int m_nNowIdx;				// ���݂̃C���f�b�N�X
	CObjectChara* m_pObjChara;	// �L�����N�^�[�̃|�C���^
	std::vector<std::string> m_vecModelName;	// ���f����

};

#endif