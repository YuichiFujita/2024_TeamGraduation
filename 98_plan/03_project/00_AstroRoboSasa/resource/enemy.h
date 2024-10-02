//============================================================
//
//	敵ヘッダー [enemy.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//************************************************************
//	インクルードファイル
//************************************************************
#include "object3D.h"

//************************************************************
//	クラス定義
//************************************************************
// 敵クラス
class CEnemy : public CObject3D
{
public:
	// コンストラクタ
	CEnemy();

	// デストラクタ
	~CEnemy();

	// オーバーライド関数
	HRESULT Init(void) override;	// 初期化
	void Uninit(void) override;		// 終了
	void Update(const float fDeltaTime) override;	// 更新
	void Draw(CShader *pShader = nullptr) override;	// 描画

	// 静的メンバ関数
	static CEnemy *Create(const D3DXVECTOR3& rPos);	// 生成

	// メンバ関数
	bool CollisionBullet(const D3DXVECTOR3& rPos);	// 弾との当たり判定
	bool CollisionPlayer(const D3DXVECTOR3& rPos);	// プレイヤーとの当たり判定

private:
	// メンバ変数
	D3DXVECTOR3 m_move;	// 移動量
	float m_fCurTime;	// 経過時間
};

#endif	// _ENEMY_H_
