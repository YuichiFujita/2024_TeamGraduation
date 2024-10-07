//=============================================================================
// 
//  手前側配置物ヘッダー [frontobj_manager.h]
//  Author : Ibuki Okusada
// 
//=============================================================================

#ifndef _FRONTOBJ_MANAGER_H_
#define _FRONTOBJ_MANAGER_H_		// 二重インクルード防止

#include "listmanager.h"
#include "baggage.h"

//==========================================================================
// クラス定義
//==========================================================================
// マップの障害物マネージャクラス
class CFrontObjManager
{
public:

	CFrontObjManager();
	~CFrontObjManager();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	//=============================
	// メンバ関数
	//=============================
	void Kill();	// 削除
	std::vector<CObjectX*> GetInfo() { return m_BaggageInfo; }
	void Regist(CObjectX* pObj);
	void Reset() { m_BaggageInfo.clear();}
	void Erase(CObjectX* pObj) 
	{ 
		// 自分自身をリストから探す
		std::vector<CObjectX*>::iterator itr = std::find(m_BaggageInfo.begin(), m_BaggageInfo.end(), pObj);

		// 要素が見つかった場合のみ削除
		if (itr != m_BaggageInfo.end())
		{
			m_BaggageInfo.erase(itr);
		}
	}

	//=============================
	// 静的関数
	//=============================
	static CFrontObjManager* Create();	// 生成
	static CFrontObjManager* GetInstance() { return m_ThisPtr; }	// インスタンス取得

private:

	//=============================
	// メンバ変数
	//=============================
	std::vector<CObjectX*> m_BaggageInfo;	// 障害物情報
	static CFrontObjManager* m_ThisPtr;		// 自身のポインタ
};


#endif