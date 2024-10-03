//=============================================================================
// 
// �L���b�`���ʏ��� [catchresult.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _CATCHRESULT_H_
#define _CATCHRESULT_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "object2D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �L���b�`����
class CCatchResult : public CObject2D
{
public:

	enum TYPE
	{
		TYPE_OK = 0,
		TYPE_FAIL,
		TYPE_MAX
	};

	CCatchResult(int nPriority = 8);
	~CCatchResult();

	//  �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	static CCatchResult* Create(const MyLib::Vector3& pos, TYPE resultType);

protected:

	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_EXPANSION = 0,
		STATE_EXPNONE,
		STATE_FADEOUT,
		STATE_MAX
	};

	//=============================
	// �����o�֐�
	//=============================
	virtual void StateExpansion() = 0;	// �g��
	virtual void StateExpNone() = 0;	// �g��㉽�����Ȃ�
	virtual void StateFadeOut() = 0;	// �t�F�[�h�A�E�g���

	//=============================
	// �����o�ϐ�
	//=============================
	State m_state;			// ���
	float m_fStateTimer;	// ��ԃ^�C�}�[

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CCatchResult::* STATE_FUNC)();	// ��ԏ����̃��X�g
	static STATE_FUNC m_StateFuncList[];
};

#endif