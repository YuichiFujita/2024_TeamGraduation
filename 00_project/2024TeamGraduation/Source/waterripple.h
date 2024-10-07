//=============================================================================
// 
//  ����w�b�_�[ [waterripple.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _WATERRIPPLE_H_
#define _WATERRIPPLE_H_	// ��d�C���N���[�h�h�~

#include "object3DMesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ����N���X
class CWaterRipple : public CObject3DMesh
{

public:

	struct Info
	{
		float height;		// ����
		float velocity;		// ���x
		float length;		// ����
		float thickness;	// �g�̑���
		int life;			// ����
		int maxLife;		// �ő����

		Info() : height(0.0f), velocity(0.0f), length(0.0f), thickness(0.0f), life(0), maxLife(0) {}
		Info(float _height, float _velocity, float _length, float _thickness, int _life, int _maxLife) :
			height(_height), velocity(_velocity), length(_length), thickness(_thickness), life(_life), maxLife(_maxLife) {}
	};

	CWaterRipple(const int block, const float blockSize, int nPriority = 3, const LAYER layer = LAYER::LAYER_MAP);
	~CWaterRipple();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void SetVtx();

	static CWaterRipple *Create(
		const int block, const float blockSize, 
		const MyLib::Vector3& pos, float height, float velocity, float thickness, int life);	// ����

private:

	// �����o�֐�
	void SetVtxPosition();	// ���_���W

	Info m_Info;
	const int m_Block;	// ����
	const float m_BlockSize;	// �u���b�N�T�C�Y
};



#endif