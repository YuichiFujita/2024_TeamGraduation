//=============================================================================
// 
//  �I�u�W�F�N�g�~�Q�[�W(2D)�w�b�_�[ [object_circlegauge2D.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECT_CIRCLEGAUGE2D_H_
#define _OBJECT_CIRCLEGAUGE2D_H_	// ��d�C���N���[�h�h�~

#include "object.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g�~�Q�[�W(2D)�N���X��`
class CObjectCircleGauge2D : public CObject
{
public:

	//=============================
	// �R���X�g���N�^/�f�X�g���N�^
	//=============================
	CObjectCircleGauge2D(int nPriority = 7, const LAYER layer = LAYER::LAYER_2D);
	~CObjectCircleGauge2D();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	void Draw() override;
	virtual void Kill() override;	// ���I�폜����
	virtual void SetVtx() override;

	//=============================
	// �����o�֐�
	//=============================
	void BindTexture(int nIdx) { m_nTexIdx = nIdx; }				// �e�N�X�`���̃C���f�b�N�X�ԍ����蓖��
	void SetColor(const D3DXCOLOR& col) { m_col = col; }			// �F�ݒ�
	D3DXCOLOR GetColor() const { return m_col; }					// �F�擾
	void SetRate(const float fRate) { m_fRate = fRate; }			// �����ݒ�
	float GetRate() const { return m_fRate; }						// �����擾
	void SetRateDest(const float fRate) { m_fRateDest = fRate; }	// �ڕW�̊����ݒ�
	float GetRateDest() const { return m_fRateDest; }				// �ڕW�̊����擾
	void SetSize(float fSize) { m_fSize = fSize; }					// �T�C�Y�ݒ�
	float GetSize() { return m_fSize; }								// �T�C�Y�擾

	//=============================
	// �ÓI�����o�֐�
	//=============================
	static CObjectCircleGauge2D *Create(int nPolygon, float fSize);

private:

	//=============================
	// �����o�ϐ�
	//=============================
	D3DXCOLOR m_col;	// �F
	float m_fSize;		// �T�C�Y
	float m_fRate;		// ����
	float m_fRateDest;	// �ڕW�̊���
	int m_nNumVertex;	// ���_��
	int m_nTexIdx;		// �e�N�X�`���̃C���f�b�N�X�ԍ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@
};

#endif