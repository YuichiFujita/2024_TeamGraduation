//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _PARTICLE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _PARTICLE_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//==========================================================================
// �N���X��`
//==========================================================================
// �p�[�e�B�N���N���X��`
class CParticle
{
public:

	// �񋓌^��`
	typedef enum
	{
		TYPE_NONE = 0,		// ���̃p�[�e�B�N��
		TYPE_SMOKE,			// ���̃p�[�e�B�N��
		TYPE_SMOKE_RED,		// ���̃p�[�e�B�N����
		TYPE_SMOKE_YEL,		// ���̃p�[�e�B�N����
		TYPE_EXPLOSION,		// �����̃p�[�e�B�N��
		TYPE_MANYTXT_MINI,	// ��ʔ�����UI(�~�j)
		TYPE_RESULTMESSAGE,	// ���U���g���b�Z�[�W
		TYPE_OFFSETTING,	// �e�ł�����
		TYPE_SUPERATTACK,	// ���U��
		TYPE_ICE_SPAWN,		// �X�o��
		TYPE_CHECK,			// �m�F�p
		TYPE_CANCELABLE,	// �L�����Z���\
		TYPE_MAX
	}TYPE;

	CParticle(int nPriority = 1);
	~CParticle();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Create(D3DXVECTOR3 pos, TYPE nType);

	static int GetNumAll(void);

private:
	void Smoke(void);
	void SmokeRed(void);
	void SmokeYel(void);
	void ManyUI_Mini(void);
	void ResultMessage(void);
	void OffSetting(void);
	void SuperAttack(void);
	void IceSpawn(void);
	void Check(void);
	void Cancelable(void);

	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_move;		// �ړ���
	D3DXCOLOR m_col;		// �F
	float m_fRadius;		// ���a
	float m_fAngle;			// ����
	float m_fMove;			// �ړ���
	int m_nLife;			// ����
	TYPE m_nType;			// ���
	static int m_nNumAll;	// ����
};

#endif