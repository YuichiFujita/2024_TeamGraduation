//=============================================================================
// 
// エフェクシアオブジェクト処理 [EffekseerObj.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _EFFEKSEEROBJ_H_
#define _EFFEKSEEROBJ_H_		// 二重インクルード防止のマクロを定義する

#include "MyEffekseer.h"

//==========================================================================
// クラス定義
//==========================================================================
// エフェクシアオブジェクト
class CEffekseerObj
{
public:

	CEffekseerObj();
	~CEffekseerObj();

	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	void Stop();	// 停止
	void DeleteLater(float time);	// 後で消す
	Effekseer::Handle GetHandle() { return m_EffekseerInfo.handle; }	// ハンドル取得

	void SetTrigger(int trigger);					// トリガー送信 
	void SetPosition(const MyLib::Vector3& pos);	// 位置設定
	void SetRotation(const MyLib::Vector3& rot);	// 向き設定
	void SetMatrix(const MyLib::Matrix& mtx);		// マトリックス設定
	Effekseer::Matrix43 GetMatrix();				// マトリックス取得
	void SetScale(const float scale);				// スケール設定

	/**
	@brief	エフェクト生成
	@details パスの拡張子は[.efkefc]です
	@param	label		[in]	エフェクトラベル
	@param	pos			[in]	位置
	@param	rot			[in]	向き
	@param	move		[in]	移動量
	@param	scale		[in]	拡大率
	@param	bAutoDeath	[in]	自動削除
	@return	CEffekseerObj ポインタ
	*/
	static CEffekseerObj* Create(const CMyEffekseer::EFKLABEL& label, const MyLib::Vector3& pos, const MyLib::Vector3& rot, const MyLib::Vector3 move, float scale, bool bAutoDeath = true);
	static CListManager<CEffekseerObj> GetListObj() { return m_List; }	// リスト取得

private:

	CMyEffekseer::EFKLABEL m_Label;	// エフェクトのラベル
	CMyEffekseer::MyEffekseerInfo m_EffekseerInfo;	// エフェクト情報
	bool m_bDeath;			// 死亡フラグ
	bool m_bDeleteLater;	// 後で死亡フラグ
	float m_fDeleteTimer;	// 死亡タイマー
	static CListManager<CEffekseerObj> m_List;	// リスト
};

#endif