//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _PARTICLE_H_		//このマクロ定義がされていなかったら
#define _PARTICLE_H_		//二重インクルード防止のマクロを定義する

//==========================================================================
// 名前空間
//==========================================================================
// 定数定義
namespace my_particle
{
	// 列挙型定義
	enum TYPE
	{
		TYPE_NONE = 0,		// 煙のパーティクル
		TYPE_SMOKE,			// 煙のパーティクル
		TYPE_SMOKE_RED,		// 煙のパーティクル赤
		TYPE_SMOKE_YEL,		// 煙のパーティクル黄
		TYPE_EXPLOSION,		// 爆発のパーティクル
		TYPE_OFFSETTING,	// 弾打ち消し
		TYPE_OFFSETTING_2D,	// 弾打ち消し(2D)
		TYPE_ENEMY_FADE,	// 敵のフェード
		TYPE_FIRE_WALLTORCH,	// 壁松明の火
		TYPE_FIRE_STANDTORCH,	// 壁松明の火
		TYPE_BATTLESTART,		// 戦闘開始
		TYPE_FLOWERINGCHARGE,		// 開花チャージ
		TYPE_POLLENDROP,	//花粉落とす
		TYPE_POLLENLOST,	//ぶつかって花粉落とす
		TYPE_MAX
	};

	void Create(const MyLib::Vector3& pos, my_particle::TYPE nType);
}

#endif