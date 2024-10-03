//=============================================================================
// 
//  HP�Q�[�W�̐�[�w�b�_�[ [hp_gauge_tip_billboard.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _HPGAUGE_TIP_BILLBOARD_H_
#define _HPGAUGE_TIP_BILLBOARD_H_	// ��d�C���N���[�h�h�~

#include "hp_gauge_tip.h"
#include "objectBillboard.h"

//==========================================================================
// �N���X��`
//==========================================================================
// HP�Q�[�W�̐�[�N���X
class CHPGaugeTip_Billboard : public CHPGaugeTip
{
public:

	CHPGaugeTip_Billboard(int nPriority = 10);
	~CHPGaugeTip_Billboard();


	// �I�[�o�[���C�h���ꂽ�֐�
	//HRESULT Init(const MyLib::Vector3& leftpos, const MyLib::Vector3& rightpos) override;
	void Uninit() override;

	void Kill();

	void SetLeftPosition(const MyLib::Vector3& pos) override;
	void SetRightPosition(const MyLib::Vector3& pos) override;

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


	void CreateTip(int nTexIdx) override;


	//=============================
	// �����o�ϐ�
	//=============================
	CObjectBillboard* m_pObjBillboard[VTXTYPE_MAX];	// �I�u�W�F�N�g2D�̃|�C���^
};


#endif