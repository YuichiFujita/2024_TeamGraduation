//=============================================================================
// 
//  サンプル_オブジェクト2Dヘッダー [movie.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MOVIE_H_
#define _MOVIE_H_	// 二重インクルード防止

#include "object2d.h"

struct IBasicVideo;

//==========================================================================
// クラス定義
//==========================================================================
// サンプル_オブジェクト2Dクラス
class CMovie : public CObject2D
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum State
	{
		STATE_NONE = 0,		// なにもない
		STATE_MAX
	};

	CMovie(int nPriority = 7);
	~CMovie();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void UpdateTextureFromVideoFrame(IBasicVideo* pBasicVideo);

	//=============================
	// 静的関数
	//=============================
	static CMovie* Create();

private:

	//=============================
	// メンバ関数
	//=============================
	// 状態系

	// その他関数
	void DecodeVideoFrame(const std::string& filename);
	HRESULT RemoveVideoRenderer();

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	float m_fStateTime;		// 状態カウンター
	State m_state;			// 状態

	
};


#endif