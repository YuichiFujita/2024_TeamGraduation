//=============================================================================
// 
//  �T���v��_�I�u�W�F�N�gX�w�b�_�[ [sample_objX.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SAMPLE_OBJX_H_
#define _SAMPLE_OBJX_H_		// ��d�C���N���[�h�h�~

#include "object.h"

// �O���錾
class CGymWall;

//==========================================================================
// �N���X��`
//==========================================================================
// �T���v��_�I�u�W�F�N�gX�N���X
class CGymWallManager : public CObject
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum Wall
	{
		WALL_OFF = 0,
		WALL_ON,
		WALL_MAX
	};

	CGymWallManager(int nPriority = 6);
	~CGymWallManager();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update(const float fDeltaTime, const float fDeltaRate, const float fSlowRate) override;
	virtual void Draw() override;

	//=============================
	// �����o�֐�
	//=============================
	void Kill();		// �폜
	void SetIsWall(bool set) { m_bWall = set; }

	//=============================
	// �ÓI�֐�
	//=============================
	/**
	@brief		��������
	@details	�K�v������Έ����ǉ�
	*/
	static CGymWallManager *Create();

private:

	//=============================
	// �����o�֐�
	//=============================
	void State();

	//=============================
	// �����o�ϐ�
	//=============================
	CGymWall* m_pGymWall;			// �ǃ|�C���^
	bool m_bWall;
};


#endif