//=============================================================================
// 
//  �A�w�b�_�[ [meshbubble.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MESHBUBBLE_H_
#define _MESHBUBBLE_H_	// ��d�C���N���[�h�h�~

#include "meshsphere.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �A�N���X
class CMeshBubble : public CMeshSphere
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum STATE
	{
		STATE_MOVE = 0,
		STATE_BURST,
		STATE_FADEOUT,
		STATE_MAX
	};

	CMeshBubble(int nPriority = 6);
	~CMeshBubble();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	virtual void SetVtx(void) override;

	static CMeshBubble *Create(const MyLib::Vector3& pos, const MyLib::Vector3& move, const float radius, const float destRadius, float repeatCycle);	// ��������

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CMeshBubble::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn
	void StateMove();		// �ړ�����
	void StateBurst();		// �e������
	void StateFadeOut();	// �t�F�[�h�A�E�g����

	float m_fOriginSize;	// ���̃T�C�Y
	float m_fDestSize;		// �ڕW�̃T�C�Y
	float m_fRepeatCycle;	// ��������
	float m_fStateTime;		// ��ԃ^�C�}�[
	STATE m_state;			// ���
};


#endif