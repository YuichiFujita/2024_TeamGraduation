//=============================================================================
// 
//  ����]�[���w�b�_�[ [judgezone.h]
//  Author : �Ό��D�n
// 
//=============================================================================
#ifndef _JUDGE_ZONE_H_
#define _JUDGE_ZONE_H_		// ��d�C���N���[�h�h�~
#include "listmanager.h"
#include "judge.h"
#include <map>

//==========================================================================
// �N���X��`
//==========================================================================
//**************************************************************************
// ����]�[���S��
//**************************************************************************
class CJudgeZone
{
public:
	// ����]�[���\����
	struct SJudgeZone
	{
		float start, end;	// �n�_,�I�_
		float borderHeight;

		SJudgeZone() : start(0.0f), end(0.0f), borderHeight(0.0f) {}
		SJudgeZone(float _start, float _end, float _border) : start(_start), end(_end), borderHeight(_border) {}
	};

	CJudgeZone();
	virtual ~CJudgeZone();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();

	//=============================
	// �����o�֐�
	//=============================
	virtual void Check();
	virtual CJudge::JUDGE Judge();

	// �擾�E�ݒ�
	SJudgeZone GetZone() { return m_zone; }
	CJudge::SJudgeCondition GetInfo(CJudge::BORDER border) { return m_aJudgeInfo[border]; }
	void SetZone(const SJudgeZone zone) { m_zone = zone; }
	void SetInfo(CJudge::BORDER border, CJudge::SJudgeCondition info) { m_aJudgeInfo[border] = info; }
	bool IsEnable() { return m_isEnable; }
	std::string GetPath() { return m_path; }
	void SetPath(std::string path) { m_path = path; }

	//=============================
	// �ÓI�֐�
	//=============================
	static CJudgeZone* Create(const float start, const float end, const float borderHeight);	// ����
	static CListManager<CJudgeZone> GetListObj() { return m_List; }								// ���X�g�擾

private:
	SJudgeZone m_zone;
	CJudge* m_pJudge;
	bool m_isEnable;
	CJudge::SJudgeCondition m_aJudgeInfo[2];
	std::string m_path;
	static CListManager<CJudgeZone> m_List;
};

#endif