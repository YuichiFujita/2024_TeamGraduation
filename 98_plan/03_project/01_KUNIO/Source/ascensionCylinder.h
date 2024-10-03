//=============================================================================
// 
//  �ڕW�n�_�w�b�_�[ [ascensionCylinder.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SSCENSIONCYLINDER_H_
#define _SSCENSIONCYLINDER_H_	// ��d�C���N���[�h�h�~

#include "meshcylinder.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �ڕW�n�_�N���X��`
class CAscensionCylinder : public CMeshCylinder
{
public:
	CAscensionCylinder(int nPriority = 6);
	~CAscensionCylinder();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	static CAscensionCylinder *Create(MyLib::Vector3 pos, float fWidthLen, float fHeightLen);

private:

};



#endif