//=============================================================================
// 
//  �I�u�W�F�N�g�w�b�_�[ [object.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _OBJECT_H_
#define _OBJECT_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "constans.h"

class CObject2D;
class CObject3D;
class CObject3DMesh;
class CObjectX;
class CObjectChara;

//==========================================================================
// �N���X��`
//==========================================================================
// �I�u�W�F�N�g�N���X��`
class CObject
{
public:

	// �񋓌^��`
	typedef enum
	{
		TYPE_NONE = 0,		// ����
		TYPE_PLAYER,		// �v���C���[
		TYPE_ENEMY,			// �G
		TYPE_BULLET,		// �e
		TYPE_EXPLOSION,		// ����
		TYPE_BG,			// �w�i
		TYPE_EFFECT2D,		// 2D�G�t�F�N�g
		TYPE_EFFECT3D,		// 3D�G�t�F�N�g
		TYPE_SCORE,			// �X�R�A
		TYPE_TIMER,			// �^�C�}�[
		TYPE_BLOCK,			// �u���b�N
		TYPE_ITEM,			// �A�C�e��
		TYPE_FIELD,			// �t�B�[���h
		TYPE_HPGAUGE,		// HP�Q�[�W
		TYPE_MAGIC_CIRCLE,	// ���@�w
		TYPE_CAR,			// ��
		TYPE_SHADOW,		// �e
		TYPE_MAP,			// �}�b�v
		TYPE_XFILE,			// X�t�@�C��
		TYPE_MODEL,			// ���f��
		TYPE_EDIT,			// �G�f�B�b�g
		TYPE_MESHFIELD,		// ���b�V���t�B�[���h
		TYPE_ELEVATION,		// �N���t�B�[���h
		TYPE_MESHWALL,		// ���b�V���E�H�[��
		TYPE_MESHCYLINDER,	// ���b�V���V�����_�[
		TYPE_MESHDOME,		// ���b�V���h�[��
		TYPE_MESHDONUTS,	// ���b�V���h�[�i�c
		TYPE_BALLAST,		// ���I
		TYPE_MAGIC_ICE,		// �X���@
		TYPE_TARGETPOINT,	// �ڕW�̒n�_
		TYPE_SPEED_RUSH,	// �������b�V��
	}TYPE;

	CObject(int nPriority = mylib_const::DEFAULT_PRIORITY);
	virtual ~CObject();

	// �h���N���X�ŃI�[�o�[���C�h���ꂽ�֐�����ΕK�v(�������z�֐�)
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void SetVtx(void);
	virtual void SetPosition(const D3DXVECTOR3 pos);	// �ʒu�ݒ�
	virtual D3DXVECTOR3 GetPosition(void) const;		// �ʒu�擾
	virtual void SetMove(const D3DXVECTOR3 rot);	// �ړ��ʐݒ�
	virtual D3DXVECTOR3 GetMove(void) const;		// �ړ��ʎ擾
	virtual void SetRotation(const D3DXVECTOR3 rot);	// �����ݒ�
	virtual D3DXVECTOR3 GetRotation(void) const;		// �����擾
	virtual bool Hit(const int nValue);					// �Q���̃o�[�`�����q�b�g����

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static const int GetNumAll(void) { return m_nNumAll; }
	static const int GetNumPriorityAll(int nPriority) { return m_nNumPriorityAll[nPriority]; }

	void SetType(const TYPE type);
	TYPE GetType(void) const;
	int GetID(void) { return m_nID; }
	static CObject *GetObject(int nPriority, int nIdx);
	CObject *GetObject(void);
	virtual CObject2D *GetObject2D(void);
	virtual CObject3D *GetObject3D(void);
	virtual CObject3DMesh *GetObject3DMesh(void);
	virtual CObjectX *GetObjectX(void);
	virtual CObjectChara *GetObjectChara(void);

protected:

	void Release(void);

private:
	int m_nID;				// �������g��ID
	int m_nPriority;		// �D�揇��
	static int m_nNumAll;	// �I�u�W�F�N�g�̑���
	static int m_nNumPriorityAll[mylib_const::PRIORITY_NUM];
	TYPE m_type;			// ���
	static CObject *m_apObject[mylib_const::PRIORITY_NUM][mylib_const::MAX_OBJ];	// �I�u�W�F�N�g�̃I�u�W�F�N�g
};

#endif