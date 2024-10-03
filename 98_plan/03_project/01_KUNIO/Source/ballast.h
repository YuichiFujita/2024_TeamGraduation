//=============================================================================
// 
//  ���I�w�b�_�[ [ballast.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _BALLAST_H_
#define _BALLAST_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object.h"

//==========================================================================
// �O���錾
//==========================================================================
class CObjectX;

//==========================================================================
// �}�N����`
//==========================================================================
#define MAX_BALLAST	(64)	// ���I�̐�
#define DEFAULTMOVE_BALLAST	(MyLib::Vector3(2.0f, 12.0f, 2.0f))

//==========================================================================
// �N���X��`
//==========================================================================
// ���I�N���X��`
class CBallast : public CObject
{
public:

	// �񋓌^��`
	typedef enum
	{
		TYPE_STONE = 0,	// ��
		TYPE_ICE,		// �X
		TYPE_MAX
	}TYPE;

	CBallast(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CBallast();

	static CBallast *Create(MyLib::Vector3 pos, MyLib::Vector3 move, int nNum = 10, float scale = 1.0f, float fAlpha = 1.0f, TYPE type = TYPE_STONE);

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:

	MyLib::Vector3 m_posOrigin;		// ���_
	MyLib::Vector3 m_moveOrigin;		// ���̈ړ���
	D3DXCOLOR m_col;				// �F
	D3DXCOLOR m_colOrigin;			// ���F
	TYPE m_type;					// ���
	int m_nNumAll;					// ����
	int m_nCntParabola;				// ���I�̈ړ��J�E���g
	int m_nLife;					// ����
	int m_nMaxLife;					// �ő����
	float m_fScale;
	static const char *m_pFileName[TYPE_MAX];	// �t�@�C����
	CObjectX *m_pObjX[MAX_BALLAST];	// �I�u�W�F�N�gX�̃|�C���^
	static int m_nIdxXFile[TYPE_MAX];			// X�t�@�C���̃C���f�b�N�X�ԍ�
};


#endif