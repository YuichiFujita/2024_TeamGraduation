//=============================================================================
// 
//  �X�R�A�w�b�_�[ [score.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SCORE_H_
#define _SCORE_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "resultmanager.h"

//==========================================================================
// �O���錾
//==========================================================================
class CMultiNumber;

//==========================================================================
// �N���X��`
//==========================================================================
// �w�i�N���X��`
class CScore
{
public:

	CScore(int nPriority = 6);
	~CScore();

	static CScore *Create();
	static CScore *Create(MyLib::Vector3 pos);
	void SetPosition(const MyLib::Vector3 pos);
	MyLib::Vector3 GetPosition() const;

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	HRESULT Init(MyLib::Vector3 pos);
	void Uninit();
	void Update();
	void Draw();

	void Add(int nValue);

private:
	
	void SetValue();

	int m_nNum;								// �l
	CMultiNumber *m_apNumber;			// �����̃I�u�W�F�N�g
	int m_nTexIdx;					// �e�N�X�`���̃C���f�b�N�X�ԍ�
};



#endif