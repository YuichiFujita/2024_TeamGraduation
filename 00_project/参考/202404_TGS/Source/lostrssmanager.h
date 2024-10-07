//======================================================
//
//デバイスロストなどで破棄が必要なものマネージャーのヘッダ[lostrssmanager.h]
//Author:石原颯馬
//
//======================================================
#ifndef _LOSTRSS_MANAGER_H_	//多重インクルード対策
#define _LOSTRSS_MANAGER_H_

#include <list>

//ロストするものに継承させるインターフェース
__interface ILostResource
{
	void Backup() = 0;
	void Restore() = 0;
};

//オブジェクトクラス
class CLostResourceManager
{
public:
	//コンストラクタ・デストラクタ
	CLostResourceManager();
	~CLostResourceManager();

	//マネージャー管理
	static CLostResourceManager* GetInstance();
	static void Release();

	//ロストするもの登録・削除
	void Regist(ILostResource* lostrss) { m_lostResources.emplace_back(lostrss); }
	void Remove(ILostResource* lostrss);

	//ロストするものバックアップ・再生成
	void Backup();
	void Restore();
private:
	static CLostResourceManager* m_pInstance;	//インスタンス（シングルトンにするため）
	std::list<ILostResource*> m_lostResources;
};

#endif	//多重