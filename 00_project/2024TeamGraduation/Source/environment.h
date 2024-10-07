//=============================================================================
// 
//  ���R���w�b�_�[ [environment.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �N���X��`
//==========================================================================
// ���R���N���X
class CSpawnEnvironment
{
public:

	CSpawnEnvironment() : m_fSpawnTimer(0.0f), m_fSpawnInterval(0.0f), m_fDefaultInterval(0.0f) {}
	CSpawnEnvironment(float timer, float interval) : m_fSpawnTimer(timer), m_fSpawnInterval(interval), m_fDefaultInterval(interval) {}
	virtual ~CSpawnEnvironment() {}

	// �X�V����
	virtual void Update(float deltaTime);

protected:

	//=============================
	// �����o�֐�
	//=============================
	virtual void TriggerSpawn() = 0;	// �������̃g���K�[

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fSpawnTimer;	// �����^�C�}�[
	float m_fSpawnInterval;	// �����Ԋu
	const float m_fDefaultInterval;	// ��_�̐����Ԋu


};

//=============================
// ��C�̐����N���X
//=============================
class CSpawn_Air : public CSpawnEnvironment
{
public:

	CSpawn_Air() : CSpawnEnvironment() {}
	CSpawn_Air(float timer, float interval) : CSpawnEnvironment(timer, interval) {}
	~CSpawn_Air() {}

protected:

	//=============================
	// �����o�֐�
	//=============================
	virtual void TriggerSpawn() override;	// �������̃g���K�[

};

// �^�C�g���p
class CSpawn_Air_Title : public CSpawn_Air
{
public:

	CSpawn_Air_Title() : CSpawn_Air() {}
	CSpawn_Air_Title(float timer, float interval) : CSpawn_Air(timer, interval) {}
	~CSpawn_Air_Title() {}

protected:

	//=============================
	// �����o�֐�
	//=============================
	void TriggerSpawn() override;	// �������̃g���K�[

};

//=============================
// �����t�̐����N���X
//=============================
class CSpawn_FlowLeaf : public CSpawnEnvironment
{
public:

	CSpawn_FlowLeaf() : CSpawnEnvironment() {}
	CSpawn_FlowLeaf(float timer, float interval) : CSpawnEnvironment(timer, interval) {}
	~CSpawn_FlowLeaf() {}

protected:

	//=============================
	// �����o�֐�
	//=============================
	void TriggerSpawn() override;	// �������̃g���K�[

};



//=============================
// �~��t�̐����N���X
//=============================
class CSpawn_Leaf : public CSpawnEnvironment
{
public:

	CSpawn_Leaf() : CSpawnEnvironment() {}
	CSpawn_Leaf(float timer, float interval) : CSpawnEnvironment(timer, interval) {}
	virtual ~CSpawn_Leaf() {}

	// �X�V����
	virtual void Update(float deltaTime) override;

protected:

	//=============================
	// �����o�֐�
	//=============================
	virtual void TriggerSpawn() override;	// �������̃g���K�[

};

class CSpawn_Leaf_Title : public CSpawn_Leaf
{
public:

	CSpawn_Leaf_Title() : CSpawn_Leaf() {}
	CSpawn_Leaf_Title(float timer, float interval) : CSpawn_Leaf(timer, interval) {}
	~CSpawn_Leaf_Title() {}

	// �X�V����
	virtual void Update(float deltaTime) override;

protected:

	//=============================
	// �����o�֐�
	//=============================
	void TriggerSpawn() override;	// �������̃g���K�[

};

class CSpawn_Leaf_Ranking : public CSpawn_Leaf
{
public:

	CSpawn_Leaf_Ranking() : CSpawn_Leaf() {}
	CSpawn_Leaf_Ranking(float timer, float interval) : CSpawn_Leaf(timer, interval) {}
	~CSpawn_Leaf_Ranking() {}

	// �X�V����
	virtual void Update(float deltaTime) override;

protected:

	//=============================
	// �����o�֐�
	//=============================
	void TriggerSpawn() override;	// �������̃g���K�[

};



#endif