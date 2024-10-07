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

	CObjectCircleGauge2D(int nPriority = 7, const LAYER layer = LAYER::LAYER_2D);
	~CObjectCircleGauge2D();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	virtual void SetVtx();

	void SetColor(const D3DXCOLOR col);		// �F�ݒ�
	D3DXCOLOR GetColor() const;			// �F�擾
	void SetRate(const float fRate);		// �����ݒ�
	float GetRate() const;				// �����擾
	void SetRateDest(const float fRate);	// �ڕW�̊����ݒ�
	float GetRateDest() const;			// �ڕW�̊����擾
	void SetSize(float fSize) { m_fSize = fSize; }	// �T�C�Y�ݒ�
	float GetSize() { return m_fSize; }

	void BindTexture(int nIdx);	// �e�N�X�`���̃C���f�b�N�X�ԍ����蓖��
	static CObjectCircleGauge2D *Create(int nPolygon, float fSize);
protected:

private:

	D3DXCOLOR m_col;	// �F
	float m_fSize;		// �T�C�Y
	float m_fRate;		// ����
	float m_fRateDest;	// �ڕW�̊���
	int m_nNumVertex;	// ���_��
	int m_nTexIdx;		// �e�N�X�`���̃C���f�b�N�X�ԍ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@
};

#endif