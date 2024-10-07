//=============================================================================
// 
//  HP�Q�[�W�̐�[�w�b�_�[ [hp_gauge_tip.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _HPGAUGE_TIP_H_
#define _HPGAUGE_TIP_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"

//==========================================================================
// �N���X��`
//==========================================================================
// HP�Q�[�W�̐�[�N���X
class CHPGaugeTip : public CObject
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum TYPE
	{
		TYPE_2D = 0,
		TYPE_BILLBOARD,
		TYPE_MAX
	};

	CHPGaugeTip(int nPriority = 6);
	~CHPGaugeTip();


	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override { return S_OK; }
	virtual HRESULT Init(const MyLib::Vector3& leftpos, const MyLib::Vector3& rightpos);
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override {}

	virtual void Kill();

	virtual void SetLeftPosition(const MyLib::Vector3& pos);
	virtual void SetRightPosition(const MyLib::Vector3& pos);
	static CHPGaugeTip* Create(const MyLib::Vector3& leftpos, const MyLib::Vector3& rightpos, TYPE type = TYPE::TYPE_2D);

protected:

	virtual void CreateTip(int nTexIdx);

private:

	//=============================
	// �񋓌^��`
	//=============================
	enum VTXTYPE
	{
		VTXTYPE_LEFT = 0,	// ��
		VTXTYPE_RIGHT,		// �E
		VTXTYPE_MAX
	};

	//=============================
	// �����o�ϐ�
	//=============================
	CObject2D* m_pObj2D[VTXTYPE_MAX];	// �I�u�W�F�N�g2D�̃|�C���^
};


#endif