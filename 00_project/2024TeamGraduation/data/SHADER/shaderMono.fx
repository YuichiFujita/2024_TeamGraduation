//============================================================
//
//	単色描画シェーダーのエフェクトファイル [shaderMono.fx]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	グローバル変数宣言
//************************************************************
float4x4 g_mtxWorld	: WORLD;		// ワールドマトリックス
float4x4 g_mtxView	: VIEW;			// ビューマトリックス
float4x4 g_mtxProj	: PROJECTION;	// プロジェクションマトリックス

float4 g_colDraw;	// ピクセル描画色

//************************************************************
//	サンプラー宣言
//************************************************************
sampler texObject : register(s0);	// オブジェクトテクスチャ

//************************************************************
//	構造体定義
//************************************************************
// 頂点シェーダー出力情報
struct VS_OUTPUT
{
	float4 pos : POSITION;	// 頂点座標
	float4 col : COLOR0;	// 頂点色
	float2 tex : TEXCOORD0;	// テクスチャ色
};

//************************************************************
//	グローバル関数
//************************************************************
//============================================================
//	頂点座標の射影変換
//============================================================
float4 TransVertex
(
	in float4	inPos		: POSITION,		// 頂点座標
	in float4x4	inMtxWorld	: WORLD,		// ワールドマトリックス
	in float4x4	inMtxView	: VIEW,			// ビューマトリックス
	in float4x4	inMtxProj	: PROJECTION	// プロジェクションマトリックス
)
{
	// 頂点座標を射影変換
	inPos = mul(inPos, inMtxWorld);
	inPos = mul(inPos, inMtxView);
	inPos = mul(inPos, inMtxProj);

	// 変換後の頂点座標を返す
	return inPos;
}

//============================================================
//	頂点シェーダー
//============================================================
void VS
(
	in	float4		inPos : POSITION,	// 頂点座標
	in	float4		inCol : COLOR0,		// 頂点色
	in	float2		inTex : TEXCOORD0,	// テクスチャ色
	out	VS_OUTPUT	outVertex			// 頂点情報
)
{
	// ローカル頂点座標を射影変換し設定
	outVertex.pos = TransVertex(inPos, g_mtxWorld, g_mtxView, g_mtxProj);

	// 
	outVertex.col = inCol;

	// 
	outVertex.tex = inTex;
}

//============================================================
//	ピクセルシェーダー
//============================================================
void PS
(
	in	VS_OUTPUT	inVertex,		// 頂点情報
	out	float4		outCol : COLOR0	// ピクセル色
)
{
#if 0
	// 
	inVertex.col *= tex2D(texObject, inVertex.tex);

	// 輝度計算 (一般的な重み付け)
	float fLuminance = dot(inVertex.col.rgb, float3(0.299f, 0.587f, 0.114f));

	// コントラストを強化 (閾値処理)
	float threshold = 0.35f; // しきい値 (0.0f - 1.0f)
	float strongContrast = fLuminance > threshold ? 1.0f : 0.0f;

	// グレースケール値をRGB成分に反映
	outCol = float4(strongContrast, strongContrast, strongContrast, 1.0f);
#else
	// テクスチャ色の取得
	float4 color = inVertex.col * tex2D(texObject, inVertex.tex);

	// グレースケールに変換
	float fLuminance = dot(color.rgb, float3(0.299f, 0.587f, 0.114f));

	// しきい値
	float thresholdBlack = 0.16f;	// 黒の範囲 (暗い部分)
	float thresholdWhite = 0.32f;	// 白の範囲 (明るい部分)
	if (fLuminance < thresholdBlack)
	{
		// ピクセルの色情報を黒に設定
		outCol = float4(0.0f, 0.0f, 0.0f, color.a);
	}
	else if (fLuminance < thresholdWhite)
	{
		// ピクセルの色情報をグレーに設定
		outCol = float4(fLuminance, fLuminance, fLuminance, color.a);
	}
	else
	{
		// ピクセルの色情報を白に設定
		outCol = float4(1.0, 1.0, 1.0, color.a);
	}
#endif
}

//============================================================
//	テクニック関数
//============================================================
technique TShader
{
	pass P0
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader  = compile ps_2_0 PS();
	}
}
