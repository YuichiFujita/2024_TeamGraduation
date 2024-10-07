//=============================================================================
// 
//  �T���v��_�I�u�W�F�N�g2D�w�b�_�[ [movie.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MOVIE_H_
#define _MOVIE_H_	// ��d�C���N���[�h�h�~

#include "object2d.h"

struct IBasicVideo;

//==========================================================================
// �N���X��`
//==========================================================================
// �T���v��_�I�u�W�F�N�g2D�N���X
class CMovie : public CObject2D
{
public:

	//=============================
	// ��ԗ�
	//=============================
	enum State
	{
		STATE_NONE = 0,		// �Ȃɂ��Ȃ�
		STATE_MAX
	};

	CMovie(int nPriority = 7);
	~CMovie();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void UpdateTextureFromVideoFrame(IBasicVideo* pBasicVideo);

	//=============================
	// �ÓI�֐�
	//=============================
	static CMovie* Create();

private:

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԍn

	// ���̑��֐�
	void DecodeVideoFrame(const std::string& filename);
	HRESULT RemoveVideoRenderer();

	//=============================
	// �����o�ϐ�
	//=============================
	// ��Ԍn
	float m_fStateTime;		// ��ԃJ�E���^�[
	State m_state;			// ���

	
};


#endif