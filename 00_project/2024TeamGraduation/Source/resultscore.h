//=============================================================================
// 
//  �X�R�A�w�b�_�[ [resultscore.h]
//  Author : ���c����
// 
//=============================================================================

#ifndef _BATTLERESULT_H_
#define _BATTLERESULT_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"
#include "multinumber.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ��ʃN���X
class CResultScore : public CObject
{
public:

	CResultScore(int nPriority = 11);
	~CResultScore();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();	// �폜
	static CResultScore* Create(int score);	// ��������

private:

	//=============================
	// �����o�֐�
	//=============================
	void CreateScore();

	//=============================
	// �����o�ϐ�
	//=============================
	int m_nScore;
	float m_fStateTime;
	CObject2D* m_pObj2D;
	CMultiNumber* m_pScore;			// �������X�R�A

};


#endif