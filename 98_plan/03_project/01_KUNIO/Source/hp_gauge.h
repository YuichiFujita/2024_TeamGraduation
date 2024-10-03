//=============================================================================
// 
//  �̗̓Q�[�W�w�b�_�[ [hp_gauge.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _HP_GAUGE_H_
#define _HP_GAUGE_H_	// ��d�C���N���[�h�h�~

#include "objectBillboard.h"

class CHPGaugeTip;

//==========================================================================
// �N���X��`
//==========================================================================
// �̗̓Q�[�W�N���X��`
class CHP_Gauge : public CObject
{
public:

	CHP_Gauge(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CHP_Gauge();

	static CHP_Gauge *Create(float fPosLen, int nMaxLife, float fSizeBuff = 1.0f);

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetVtx();
	void SetVtx(int nCntGauge);

	void Kill();
	void UpdatePosition(MyLib::Vector3 pos, int nLife);

private:

	// �񋓌^��`
	typedef enum
	{
		VTXTYPE_BLACK = 0,	// ���Q�[�W
		VTXTYPE_PINK,		// �s���N�Q�[�W
		VTXTYPE_MAX
	}VTXTYPE;

	// �\���̒�`
	struct SHP_Gauge
	{
		CObjectBillboard *pObjBillboard;	// �r���{�[�h�̃I�u�W�F�N�g
		float fMaxWidth;					// ���̍ő�l
		float fWidthDest;					// ���̍���
		float fMaxHeight;					// �����̍ő�l
	};

	void ChangeColor(int nCntGauge);		// �F�ύX
	void GaugeDecrement(int nCntGauge);		// ��������

	static int m_nNumAll;					// ����
	static const char *m_apTextureFile[];	// �e�N�X�`���̃t�@�C��
	SHP_Gauge m_HPGauge[VTXTYPE_MAX];		// HP�Q�[�W�̏��
	float m_fPosLength;						// ���_����̒���
	int m_nLife;							// �̗�
	int m_nMaxLife;							// �ő�̗�
	int m_nTexIdx[VTXTYPE_MAX];				// �e�N�X�`���̃C���f�b�N�X�ԍ�
	CHPGaugeTip* m_pTip;	// �Q�[�W�̐�[
};


#endif