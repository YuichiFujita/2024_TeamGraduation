//=============================================================================
// 
//  �L�[�R���t�B�O�ɉ������{�^���\�� [controlkeydisp.h]
//  Author : �Ό��D�n
// 
//=============================================================================

#ifndef _KEYDISP_H_
#define _KEYDISP_H_	// ��d�C���N���[�h�h�~

#include "object2D.h"
#include "input_gamepad.h"
#include <map>

//==========================================================================
// �N���X��`
//==========================================================================
class CControlKeyDisp : public CObject2D
{
public:

	CControlKeyDisp(int nPriority = 11);
	~CControlKeyDisp();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void Kill();	// �폜
	static CControlKeyDisp* Create(MyLib::Vector3 pos, MyLib::Vector3 rot, float width, float height, int type);	// ��������

	void SetType(int type);
	static void Load(CInputGamepad::BUTTON button, int nIdx) { m_buttonTextures[button] = nIdx; }

private:

	//=============================
	// �����o�ϐ�
	//=============================
	static std::map<CInputGamepad::BUTTON, int> m_buttonTextures;
};


#endif