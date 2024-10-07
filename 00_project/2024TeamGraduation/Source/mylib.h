//=============================================================================
// 
// 自身のライブラリ [mylib.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MYLIB_H_
#define _MYLIB_H_		// 二重インクルード防止

#include <json.hpp>

/*
	@brief	マイライブラリ
*/
namespace MyLib
{

	/*
		@brief	Vector3 【3次元情報】
	*/
	struct Vector3 : public D3DXVECTOR3
	{
		using D3DXVECTOR3::D3DXVECTOR3;

		// デフォルトコンストラクタ
		Vector3() : D3DXVECTOR3(0.0f, 0.0f, 0.0f) {}

		// 引数付きコンストラクタ
		Vector3(const float xyz) : D3DXVECTOR3(xyz, xyz, xyz) {}
		Vector3(float x, float y, float z) : D3DXVECTOR3(x, y, z) {}

		// JSONからの読み込み
		void from_json(const nlohmann::json& j)
		{
			j.at("x").get_to(x);
			j.at("y").get_to(y);
			j.at("z").get_to(z);
		}

		// JSONへの書き込み
		void to_json(nlohmann::json& j) const
		{
			j["x"] = x;
			j["y"] = y;
			j["z"] = z;
		}

		//--------------------------
		// jsonのフレンド化
		//--------------------------
		friend void from_json(const nlohmann::json& j, MyLib::Vector3& v)
		{
			v.from_json(j);
		}

		friend void to_json(nlohmann::json& j, const MyLib::Vector3& v)
		{
			v.to_json(j);
		}

		//--------------------------
		// 加算
		//--------------------------
		inline Vector3 operator + (const float& o) const
		{
			return Vector3(x + o, y + o, z + o);
		}

		inline Vector3 operator + (const Vector3& o) const
		{
			return Vector3(x + o.x, y + o.y, z + o.z);
		}

		//--------------------------
		// 減算
		//--------------------------
		inline Vector3 operator - (const float& o) const
		{
			return Vector3(x - o, y - o, z - o);
		}

		inline Vector3 operator - () const
		{
			return Vector3(-x, -y, -z);
		}

		inline Vector3 operator - (const Vector3& o) const
		{
			return Vector3(x - o.x, y - o.y, z - o.z);
		}

		//--------------------------
		// 加算代入
		//--------------------------
		inline Vector3& operator += (const float& o)
		{
			x += o;
			y += o;
			z += o;
			return *this;
		}

		inline Vector3& operator += (const Vector3& o)
		{
			x += o.x;
			y += o.y;
			z += o.z;
			return *this;
		}

		//--------------------------
		// 減算代入
		//--------------------------
		inline Vector3& operator -= (const float& o)
		{
			x -= o;
			y -= o;
			z -= o;
			return *this;
		}

		inline Vector3& operator -= (const Vector3& o)
		{
			x -= o.x;
			y -= o.y;
			z -= o.z;
			return *this;
		}

		/*
			@brief	ベクトルの長さ
			@return	求められた長さ
		*/
		inline float Length()
		{
			return sqrtf(x * x + y * y + z * z);
		}

		/*
			@brief	XZベクトルの長さ
			@return	求められた長さ
		*/
		inline float LengthXZ()
		{
			return sqrtf(x * x + z * z);
		}

		/*
			@brief	XYベクトルの長さ
			@return	求められた長さ
		*/
		inline float LengthXY()
		{
			return sqrtf(x * x + y * y);
		}

		/*
			@brief	ベクトルの長さの2乗
			@return	求められた長さ
		*/
		inline float LengthSquared() const
		{
			return x * x + y * y + z * z;
		}

		/*
			@brief	XZの向き
			@return	向き
		*/
		inline float AngleXZ(const Vector3& v)
		{
			return atan2f(x - v.x, z - v.z);
		}

		/*
			@brief	XYの向き
			@return	向き
		*/
		inline float AngleXY(const Vector3& v)
		{
			return atan2f(x - v.x, y - v.y);
		}

		/*
			@brief	ベクトルの距離
			@return	2つのベクトルの距離
		*/
		float Distance(const Vector3& v) const 
		{
			return sqrtf((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y) + (v.z - z) * (v.z - z));
		}

		/*
			@brief	XZ平面のベクトルの距離
			@return	2つのベクトルの距離(XZ平面)
		*/
		float DistanceXZ(const Vector3& v) const
		{
			return sqrtf((v.x - x) * (v.x - x) + (v.z - z) * (v.z - z));
		}

		/*
			@brief	ベクトルの距離の2乗
			@return	2つのベクトルの距離の2乗
		*/
		float DistSquared(const Vector3& v) const 
		{
			return (v.x - x) * (v.x - x) + (v.y - y) * (v.y - y) + (v.z - z) * (v.z - z);
		}

		/*
			@brief	単位ベクトル
			@return	正規化されたベクトル
		*/
		inline Vector3 Normal()
		{
			Vector3 result;
			D3DXVec3Normalize(&result, this);
			return result;
		}

		/*
			@brief	外積
			@return	2つのベクトルの外積
		*/
		Vector3 Cross(const Vector3& v) const 
		{
			return Vector3(
				y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x);
		}

		/*
			@brief	内積
			@return	2つのベクトルの内積
		*/
		float Dot(const Vector3& v) const
		{
			return x * v.x + y * v.y + z * v.z;
		}

		/*
			@brief	水平方向変換
			@return	2点の水平方向角度
		*/
		float GetHorizontalAngle(const Vector3& v) const 
		{
			return atan2f((x - v.x), (z - v.z));
		}

		/*
			@brief	ベクトルを反転
			@return	反転されたベクトル
		*/
		inline Vector3 Invert()
		{
			return Vector3(-x, -y, -z);
		}

		/*
			@brief	指定された許容誤差範囲内にあるか判定
			@param	target	[in]	目標値
			@param	range	[in]	許容範囲
			@return	判定結果
		*/
		inline bool IsNearlyTargetX(float target, float range) const
		{
			if (abs(x) <= target + range &&
				abs(x) >= target - range)
			{
				return true;
			}
			return false;
		}

		/*
			@brief	指定された許容誤差範囲内にあるか判定
			@param	target	[in]	目標値
			@param	range	[in]	許容範囲
			@return	判定結果
		*/
		inline bool IsNearlyTargetY(float target, float range) const
		{
			if (abs(y) <= target + range &&
				abs(y) >= target - range)
			{
				return true;
			}
			return false;
		}

		/*
			@brief	指定された許容誤差範囲内にあるか判定
			@param	target	[in]	目標値
			@param	range	[in]	許容範囲
			@return	判定結果
		*/
		inline bool IsNearlyTargetZ(float target, float range) const
		{
			if (abs(z) <= target + range &&
				abs(z) >= target - range)
			{
				return true;
			}
			return false;
		}

		/*
			@brief	指定された許容誤差範囲内にあるか判定
			@param	target	[in]	目標値
			@param	range	[in]	許容範囲
			@return	判定結果
		*/
		inline bool IsNearlyTarget(float target, float range) const
		{
			if (IsNearlyTargetX(target, range) &&
				IsNearlyTargetY(target, range) &&
				IsNearlyTargetZ(target, range))
			{
				return true;
			}
			return false;
		}

		/*
			@brief	指定された許容誤差範囲内にあるか判定
			@param	range	[in]	許容範囲
			@return	判定結果
		*/
		inline bool IsNearlyZero(float range) const
		{
			if (abs(x) <= range &&
				abs(y) <= range &&
				abs(z) <= range)
			{
				return true;
			}
			return false;
		}

		/*
			@brief	要素がゼロか
			@return	判定結果
		*/
		inline bool IsZero() const
		{
			if (x == 0.0f &&
				y == 0.0f &&
				z == 0.0f)
			{
				return true;
			}
			return false;
		}

		/*
			@brief	ベクトルを90度回転(X軸)
		*/
		Vector3 rotateAroundX()
		{
			float angle = 90 * D3DX_PI / 180.0f;

			return Vector3(
				x,
				y * cosf(angle) - z * sinf(angle),
				y * sinf(angle) + z * cosf(angle));
		}

		/*
			@brief	ベクトルを90度回転(Y軸)
		*/
		Vector3 rotateAroundY()
		{
			float angle = 90 * D3DX_PI / 180.0f;

			return Vector3(
				x * cosf(angle) + z * sinf(angle),
				y,
				-x * sinf(angle) + z * cosf(angle));
		}

		/*
			@brief	ベクトルを90度回転(Z軸)
		*/
		Vector3 rotateAroundZ()
		{
			float angle = 90 * D3DX_PI / 180.0f;

			return Vector3(
				x * cosf(angle) - y * sinf(angle),
				x * sinf(angle) + y * cosf(angle),
				z);
		}

		/*
			@brief	文字列変換
			@return	(x, y, z)の形式で変換された文字列
		*/
		inline std::string ToString() const
		{
			std::stringstream text;
			text << "(" << x << ", " << y << ", " << z << ")";
			return text.str();
		}
	};

	/*
		@brief	Color 【色情報】
	*/
	struct Color : public D3DXCOLOR
	{
		// デフォルトコンストラクタ
		Color() : D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f) {}
	
		// 引数付きコンストラクタ
		Color(const float all) : D3DXCOLOR(all, all, all, all) {}
		Color(const float col, const float alpha) : D3DXCOLOR(col, col, col, alpha) {}
		Color(const float r, const float g, const float b, const float a) : D3DXCOLOR(r, g, b, a) {}
	
		/*
			@brief	D3DCOLOR型(DWORD)からの初期化コンストラクタ
		*/
		Color(const D3DCOLOR argb)
		{
			const float fRate = 1.0f / 255.0f;
			a = fRate * (float)(unsigned char)(argb >> 24);
			r = fRate * (float)(unsigned char)(argb >> 16);
			g = fRate * (float)(unsigned char)(argb >> 8);
			b = fRate * (float)(unsigned char)(argb >> 0);
		}
	
		/*
			@brief	D3DXCOLOR型(float4)からの初期化コンストラクタ
		*/
		Color(const D3DXCOLOR col) : D3DXCOLOR(col) {}
	
		// デストラクタ
		~Color() {}
	
		//********************************************************
		//	メンバ関数
		//********************************************************
		inline void Black(void)					{ *this = Color(0.0f, 0.0f, 0.0f, 1.0f); }		// 黒色/不透明
		inline void Black(const float fAlpha)	{ *this = Color(0.0f, 0.0f, 0.0f, fAlpha); }	// 黒色/透明度
		inline void White(void)					{ *this = Color(1.0f, 1.0f, 1.0f, 1.0f); }		// 白色/不透明
		inline void White(const float fAlpha)	{ *this = Color(1.0f, 1.0f, 1.0f, fAlpha); }	// 白色/透明度
		inline void Red(void)					{ *this = Color(1.0f, 0.0f, 0.0f, 1.0f); }		// 赤色/不透明
		inline void Red(const float fAlpha)		{ *this = Color(1.0f, 0.0f, 0.0f, fAlpha); }	// 赤色/透明度
		inline void Green(void)					{ *this = Color(0.0f, 1.0f, 0.0f, 1.0f); }		// 緑色/不透明
		inline void Green(const float fAlpha)	{ *this = Color(0.0f, 1.0f, 0.0f, fAlpha); }	// 緑色/透明度
		inline void Blue(void)					{ *this = Color(0.0f, 0.0f, 1.0f, 1.0f); }		// 青色/不透明
		inline void Blue(const float fAlpha)	{ *this = Color(0.0f, 0.0f, 1.0f, fAlpha); }	// 青色/透明度
		inline void Yellow(void)				{ *this = Color(1.0f, 1.0f, 0.0f, 1.0f); }		// 黄色/不透明
		inline void Yellow(const float fAlpha)	{ *this = Color(1.0f, 1.0f, 0.0f, fAlpha); }	// 黄色/透明度
		inline void Cyan(void)					{ *this = Color(0.0f, 1.0f, 1.0f, 1.0f); }		// 水色/不透明
		inline void Cyan(const float fAlpha)	{ *this = Color(0.0f, 1.0f, 1.0f, fAlpha); }	// 水色/透明度
		inline void Purple(void)				{ *this = Color(1.0f, 0.0f, 1.0f, 1.0f); }		// 紫色/不透明
		inline void Purple(const float fAlpha)	{ *this = Color(1.0f, 0.0f, 1.0f, fAlpha); }	// 紫色/透明度
	
		/*
			@brief	カラーコードから色/透明度の設定
		*/
		inline void Code(const std::string& rCode, const float fAlpha)
		{
			assert((int)rCode.size() == 6);
			int R = std::stoi(rCode.substr(0, 2), nullptr, 16);
			int G = std::stoi(rCode.substr(2, 2), nullptr, 16);
			int B = std::stoi(rCode.substr(4, 2), nullptr, 16);
			*this = D3DCOLOR_RGBA(R, G, B, (int)(fAlpha * 255.0f));
		}
	
		/*
			@brief	カラーコードから色の設定
		*/
		inline void Code(const std::string& rCode)
		{
			this->Code(rCode, 1.0f);
		}
	
		/*
			@brief	透明度がゼロか
			@return	判定結果
		*/
		inline bool IsAlpha(void) const
		{
			if (a <= 0.0f) { return true; }
			return false;
		}
	};

	/*
		@brief	マトリックス【行列情報】
	*/
	struct Matrix
	{
	public:

		union 
		{
			struct 
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;

			};
			float m[4][4];
		};

		// デフォルトコンストラクタ
		Matrix() {}

		// 引数付きコンストラクタ
		Matrix(
			float f11, float f12, float f13, float f14,
			float f21, float f22, float f23, float f24,
			float f31, float f32, float f33, float f34,
			float f41, float f42, float f43, float f44)
		{
			_11 = f11;
			_12 = f12;
			_13 = f13;
			_14 = f14;

			_21 = f21;
			_22 = f22;
			_23 = f23;
			_24 = f24;

			_31 = f31;
			_32 = f32;
			_33 = f33;
			_34 = f34;

			_41 = f41;
			_42 = f42;
			_43 = f43;
			_44 = f44;
		}

		Matrix(const D3DXMATRIX& mtx)
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					m[i][j] = mtx.m[i][j];
				}
			}
		}

		//--------------------------
		// 加算
		//--------------------------
		inline Matrix operator + (const Matrix& mtx) const
		{
			return Matrix(
				_11 + mtx._11, _12 + mtx._12, _13 + mtx._13, _14 + mtx._14,
				_21 + mtx._21, _22 + mtx._22, _23 + mtx._23, _24 + mtx._24,
				_31 + mtx._31, _32 + mtx._32, _33 + mtx._33, _34 + mtx._34,
				_41 + mtx._41, _42 + mtx._42, _43 + mtx._43, _44 + mtx._44);
		}

		inline Matrix operator + () const
		{
			return *this;
		}
		
		inline Matrix& operator += (const Matrix& mtx)
		{
			for (int i = 0; i < 4; ++i) 
			{
				for (int j = 0; j < 4; ++j) 
				{
					m[i][j] += mtx.m[i][j];
				}
			}
			return *this;
		}

		//--------------------------
		// 減算
		//--------------------------
		inline Matrix operator - (const Matrix& mtx) const
		{
			return Matrix(
				_11 - mtx._11, _12 - mtx._12, _13 - mtx._13, _14 - mtx._14,
				_21 - mtx._21, _22 - mtx._22, _23 - mtx._23, _24 - mtx._24,
				_31 - mtx._31, _32 - mtx._32, _33 - mtx._33, _34 - mtx._34,
				_41 - mtx._41, _42 - mtx._42, _43 - mtx._43, _44 - mtx._44);
		}

		inline Matrix operator - () const
		{
			return Matrix(
				-_11, -_12, -_13, -_14,
				-_21, -_22, -_23, -_24,
				-_31, -_32, -_33, -_34,
				-_41, -_42, -_43, -_44);
		}
		
		inline Matrix& operator -= (const Matrix& mtx)
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					m[i][j] -= mtx.m[i][j];
				}
			}
			return *this;
		}

		//--------------------------
		// 乗算
		//--------------------------
		inline Matrix operator * (const Matrix& mtx) const
		{
			Matrix result;
			result.Multiply(*this, mtx);
			return result;
		}
		
		inline Matrix operator * (float f) const
		{
			return Matrix(
				_11 * f, _12 * f, _13 * f, _14 * f,
				_21 * f, _22 * f, _23 * f, _24 * f,
				_31 * f, _32 * f, _33 * f, _34 * f,
				_41 * f, _42 * f, _43 * f, _44 * f);
		}

		inline Matrix& operator *= (const Matrix& mtx)
		{
			Multiply(*this, mtx);
		}

		inline Matrix& operator *= (float f)
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					m[i][j] *= f;
				}
			}
			return *this;
		}

		//--------------------------
		// 除算
		//--------------------------
		inline Matrix operator / (float f) const
		{
			float invert = 1.0f / f;
			return Matrix(
				_11 * invert, _12 * invert, _13 * invert, _14 * invert,
				_21 * invert, _22 * invert, _23 * invert, _24 * invert,
				_31 * invert, _32 * invert, _33 * invert, _34 * invert,
				_41 * invert, _42 * invert, _43 * invert, _44 * invert);
		}

		inline Matrix& operator /= (float f)
		{
			float invert = 1.0f / f;
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					m[i][j] *= invert;
				}
			}
			return *this;
		}

		//--------------------------
		// 比較
		//--------------------------
		inline bool operator == (const Matrix& mtx) const
		{
			return 0 == memcmp(this, &mtx, sizeof(Matrix));
		}

		inline bool operator != (const Matrix& mtx) const
		{
			return 0 != memcmp(this, &mtx, sizeof(Matrix));
		}


		//--------------------------
		// 計算関数
		//--------------------------
		/*
			@brief	単位行列化
			@return	void
		*/
		inline void Identity()
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					m[i][j] = (i == j) ? 1.0f : 0.0f;
				}
			}
		}

		/*
			@brief	マトリックス同士の掛け算
			@param	mtx1	[in]	行列情報
			@param	mtx2	[in]	行列情報
			@return	void
		*/
		inline void Multiply(Matrix mtx1, Matrix mtx2)
		{
			for (int i = 0; i < 4; ++i) 
			{
				for (int j = 0; j < 4; ++j) 
				{
					m[i][j] = 0.0f;
					for (int k = 0; k < 4; ++k) 
					{
						m[i][j] += mtx1.m[i][k] * mtx2.m[k][j];
					}
				}
			}
		}

		/*
			@brief	移動行列の作成
			@param	pos		[in]	位置情報
			@return	void
		*/
		inline void Translation(const Vector3& pos)
		{
			Identity();

			_41 = pos.x;
			_42 = pos.y;
			_43 = pos.z;
		}

		/*
			@brief	回転行列の作成(Yaw、Pitch、Roll（ヨー、ピッチ、ロール）順)
			@param	yaw		[in]	ヨー
			@param	pitch	[in]	ピッチ
			@param	roll	[in]	ロール
			@return	void
		*/
		inline void RotationYawPitchRoll(float yaw, float pitch, float roll)
		{
			Identity();

			// ラジアン変換
			float cosYaw = cosf(yaw);
			float sinYaw = sinf(yaw);
			float cosPitch = cosf(pitch);
			float sinPitch = sinf(pitch);
			float cosRoll = cosf(roll);
			float sinRoll = sinf(roll);

			// 行列計算
			_11 = cosYaw * cosRoll + sinYaw * sinPitch * sinRoll;
			_12 = sinRoll * cosPitch;
			_13 = -sinYaw * cosRoll + cosYaw * sinPitch * sinRoll;
			_14 = 0.0f;

			_21 = -cosYaw * sinRoll + sinYaw * sinPitch * cosRoll;
			_22 = cosRoll * cosPitch;
			_23 = sinRoll * sinYaw + cosYaw * sinPitch * cosRoll;
			_24 = 0.0f;

			_31 = sinYaw * cosPitch;
			_32 = -sinPitch;
			_33 = cosYaw * cosPitch;
			_34 = 0.0f;

			_41 = 0.0f;
			_42 = 0.0f;
			_43 = 0.0f;
			_44 = 1.0f;
		}

		/*
			@brief	拡大情報を掛け合わせる
			@param	scale	[in]	拡大情報
			@return	void
		*/
		inline void Scaling(const Vector3& scale)
		{
			Identity();

			_11 = scale.x;
			_22 = scale.y;
			_33 = scale.z;
		}

		/*
			@brief	変換行列に応じて頂点変換
			@param	pos		[in]	元にする頂点
			@param	mtx		[in]	変換行列
			@return	変換後の頂点
		*/
		inline Vector3 Coord(const Vector3& pos)
		{
			Vector3 result;

			// ベクトルと掛け合わせる
			result.x = pos.x * m[0][0] + pos.y * m[1][0] + pos.z * m[2][0] + m[3][0];
			result.y = pos.x * m[0][1] + pos.y * m[1][1] + pos.z * m[2][1] + m[3][1];
			result.z = pos.x * m[0][2] + pos.y * m[1][2] + pos.z * m[2][2] + m[3][2];
			float w =  pos.x * m[0][3] + pos.y * m[1][3] + pos.z * m[2][3] + m[3][3];

			if (w != 0.0f) {
				result.x /= w;
				result.y /= w;
				result.z /= w;
			}

			return result;
		}

		//--------------------------
		// 変換関数
		//--------------------------
		/*
			@brief	ワールド座標取得
			@return	ワールド座標
		*/
		inline Vector3 GetWorldPosition()
		{
			return Vector3(_41, _42, _43);
		}

		/*
			@brief	拡大情報取得
			@return	拡大情報
		*/
		inline Vector3 GetScale()
		{
			Vector3 vec1 = MyLib::Vector3(_11, _12, _13);
			Vector3 vec2 = MyLib::Vector3(_21, _22, _23);
			Vector3 vec3 = MyLib::Vector3(_31, _32, _33);

			// マトリックスからスケール情報を抽出
			Vector3 scale;
			scale.x = vec1.Length();
			scale.y = vec2.Length();
			scale.z = vec3.Length();
			return scale;
		}

		/*
			@brief	前方ベクトル取得
			@return	前方ベクトル
		*/
		inline Vector3 GetForwardVector()
		{
			return Vector3(_13, _23, _33);
		}

		/*
			@brief	D3DXMATRIX型に変換する
			@return	D3DXMATRIX型の要素
		*/
		inline D3DXMATRIX ConvertD3DXMATRIX()
		{
			D3DXMATRIX mtx;
			for (int i = 0; i < 4; ++i) 
			{
				for (int j = 0; j < 4; ++j) 
				{
					mtx.m[i][j] = m[i][j];
				}
			}
			return mtx;
		}
	};

	/*
		@brief	PosGrid2【2次元の整数情報】
	*/
	struct PosGrid2
	{
		// デフォルトコンストラクタ
		PosGrid2() : x(0), y(0) {}
	
		// 引数付きコンストラクタ
		PosGrid2(const int XY) : x(XY), y(XY) {}
		PosGrid2(const int X, const int Y) : x(X), y(Y) {}
	
		// デストラクタ
		~PosGrid2() {}
	
		// メンバ変数
		int x;	// 方眼座標 (x)
		int y;	// 方眼座標 (y)
	
		//********************************************************
		//	演算子オーバーロード
		//********************************************************
		// 加算
		inline PosGrid2 operator+(const PosGrid2& grid) const	{ return PosGrid2(x + grid.x, y + grid.y); }
		inline PosGrid2 operator+(const int nVal) const			{ return PosGrid2(x + nVal, y + nVal); }
	
		// 減算
		inline PosGrid2 operator-(const PosGrid2& grid) const	{ return PosGrid2(x - grid.x, y - grid.y); }
		inline PosGrid2 operator-(const int nVal) const			{ return PosGrid2(x - nVal, y - nVal); }
		inline PosGrid2 operator-(void) const					{ return PosGrid2(-x, -y); }
	
		// 乗算
		inline PosGrid2 operator*(const int nVal) const	{ return PosGrid2(x * nVal, y * nVal); }
	
		// 除算
		inline PosGrid2 operator/(const int nVal) const	{ return PosGrid2(x / nVal, y / nVal); }
	
		// 加算代入
		inline PosGrid2& operator+=(const int nVal)
		{
			x += nVal;
			y += nVal;
			return *this;
		}
		inline PosGrid2& operator+=(const PosGrid2& grid)
		{
			x += grid.x;
			y += grid.y;
			return *this;
		}
	
		// 減算代入
		inline PosGrid2& operator-=(const int nVal)
		{
			x -= nVal;
			y -= nVal;
			return *this;
		}
		inline PosGrid2& operator-=(const PosGrid2& grid)
		{
			x -= grid.x;
			y -= grid.y;
			return *this;
		}
	
		// 乗算代入
		inline PosGrid2& operator*=(const int nVal)
		{
			x *= nVal;
			y *= nVal;
			return *this;
		}
		inline PosGrid2& operator*=(const PosGrid2& grid)
		{
			x *= grid.x;
			y *= grid.y;
			return *this;
		}
	
		// 除算代入
		inline PosGrid2& operator/=(const int nVal)
		{
			x /= nVal;
			y /= nVal;
			return *this;
		}
		inline PosGrid2& operator/=(const PosGrid2& grid)
		{
			x /= grid.x;
			y /= grid.y;
			return *this;
		}
	
		//********************************************************
		//	メンバ関数
		//********************************************************
		/*
			@brief	座標が一致するかの検証
			@return	判定結果
		*/
		inline bool operator==(const PosGrid2& grid) const
		{
			if (grid.x == x && grid.y == y) { return true; }
			return false;
		}
	
		/*
			@brief	座標が一致しないかの検証
			@return	判定結果
		*/
		inline bool operator!=(const PosGrid2& grid) const
		{
			if (grid.x != x || grid.y != y) { return true; }
			return false;
		}
	
		/*
			@brief	方眼座標の反転
			@return	反転された方眼座標
		*/
		inline PosGrid2 Invert() { return PosGrid2(-x, -y); }
	
		/*
			@brief	要素がすべてゼロか
			@return	判定結果
		*/
		inline bool IsZero() const
		{
			if (x == 0 && y == 0) { return true; }
			return false;
		}
	};
	
	/*
		@brief	PosGrid3【3次元の整数情報】
	*/
	struct PosGrid3
	{
		// デフォルトコンストラクタ
		PosGrid3() : x(0), y(0), z(0) {}
	
		// 引数付きコンストラクタ
		PosGrid3(const int XYZ) : x(XYZ), y(XYZ), z(XYZ) {}
		PosGrid3(const int X, const int Y, const int Z) : x(X), y(Y), z(Z) {}
	
		// デストラクタ
		~PosGrid3() {}
	
		// メンバ変数
		int x;	// 方眼座標 (x)
		int y;	// 方眼座標 (y)
		int z;	// 方眼座標 (z)
	
		//********************************************************
		//	演算子オーバーロード
		//********************************************************
		// 加算
		inline PosGrid3 operator+(const PosGrid3& grid) const	{ return PosGrid3(x + grid.x, y + grid.y, z + grid.z); }
		inline PosGrid3 operator+(const int nVal) const			{ return PosGrid3(x + nVal, y + nVal, z + nVal); }
	
		// 減算
		inline PosGrid3 operator-(const PosGrid3& grid) const	{ return PosGrid3(x - grid.x, y - grid.y, z - grid.z); }
		inline PosGrid3 operator-(const int nVal) const			{ return PosGrid3(x - nVal, y - nVal, z - nVal); }
		inline PosGrid3 operator-(void) const					{ return PosGrid3(-x, -y, -z); }
	
		// 乗算
		inline PosGrid3 operator*(const int nVal) const	{ return PosGrid3(x * nVal, y * nVal, z * nVal); }
	
		// 除算
		inline PosGrid3 operator/(const int nVal) const	{ return PosGrid3(x / nVal, y / nVal, z / nVal); }
	
		// 加算代入
		inline PosGrid3& operator+=(const int nVal)
		{
			x += nVal;
			y += nVal;
			z += nVal;
			return *this;
		}
		inline PosGrid3& operator+=(const PosGrid3& grid)
		{
			x += grid.x;
			y += grid.y;
			z += grid.z;
			return *this;
		}
	
		// 減算代入
		inline PosGrid3& operator-=(const int nVal)
		{
			x -= nVal;
			y -= nVal;
			z -= nVal;
			return *this;
		}
		inline PosGrid3& operator-=(const PosGrid3& grid)
		{
			x -= grid.x;
			y -= grid.y;
			z -= grid.z;
			return *this;
		}
	
		// 乗算代入
		inline PosGrid3& operator*=(const int nVal)
		{
			x *= nVal;
			y *= nVal;
			z *= nVal;
			return *this;
		}
		inline PosGrid3& operator*=(const PosGrid3& grid)
		{
			x *= grid.x;
			y *= grid.y;
			z *= grid.z;
			return *this;
		}
	
		// 除算代入
		inline PosGrid3& operator/=(const int nVal)
		{
			x /= nVal;
			y /= nVal;
			z /= nVal;
			return *this;
		}
		inline PosGrid3& operator/=(const PosGrid3& grid)
		{
			x /= grid.x;
			y /= grid.y;
			z /= grid.z;
			return *this;
		}
	
		//********************************************************
		//	メンバ関数
		//********************************************************
		/*
			@brief	座標が一致するかの検証
			@return	判定結果
		*/
		inline bool operator==(const PosGrid3& grid) const
		{
			if (grid.x == x && grid.y == y && grid.z == z) { return true; }
			return false;
		}
	
		/*
			@brief	座標が一致しないかの検証
			@return	判定結果
		*/
		inline bool operator!=(const PosGrid3& grid) const
		{
			if (grid.x != x || grid.y != y || grid.z != z) { return true; }
			return false;
		}
	
		/*
			@brief	方眼座標の反転
			@return	反転された方眼座標
		*/
		inline PosGrid3 Invert() { return PosGrid3(-x, -y, -z); }
	
		/*
			@brief	要素がすべてゼロか
			@return	判定結果
		*/
		inline bool IsZero() const
		{
			if (x == 0 && y == 0 && z == 0) { return true; }
			return false;
		}
	};

	/*
		@brief	HitResult【衝突情報】
	*/
	struct HitResult
	{
		bool ishit;				// 衝突結果
		MyLib::Vector3 hitpos;	// 衝突地点

		// デフォルトコンストラクタ
		HitResult() : ishit(false), hitpos() {}

		// パラメータ付きコンストラクタ
		HitResult(bool bHit, MyLib::Vector3 pos) : ishit(bHit), hitpos(pos) {}
	};

	/*
		@brief	HitResult_Character【キャラクターの衝突情報】
	*/
	struct HitResult_Character
	{
		bool ishit;				// 衝突結果
		bool isdamage;			// ダメージを喰らったか
		bool isdeath;			// 死亡したか
		MyLib::Vector3 hitpos;	// 衝突地点

		// デフォルトコンストラクタ
		HitResult_Character() : ishit(false), isdamage(false), isdeath(false), hitpos() {}

		// パラメータ付きコンストラクタ
		HitResult_Character(bool bHit, bool bDamage, bool bDeath, MyLib::Vector3 pos) : 
			ishit(bHit), isdamage(bDamage), isdeath(bDeath), hitpos(pos) {}
	};

	/*
		@brief	AABB【ボックス情報】
	*/
	struct AABB 
	{
		MyLib::Vector3 vtxMin;	// 最小値
		MyLib::Vector3 vtxMax;	// 最大値

		// デフォルトコンストラクタ
		AABB() : vtxMin(), vtxMax() {}

		// パラメータ付きコンストラクタ
		AABB(const MyLib::Vector3& min, const MyLib::Vector3& max) : vtxMin(min), vtxMax(max) {}
	};


	/*
		@brief	BOXコライダー
	*/
	struct Collider_BOX
	{
		MyLib::Vector3 vtxMin;	// 最小値
		MyLib::Vector3 vtxMax;	// 最大値
		MyLib::Vector3 offset;	// オフセット位置
		MyLib::Matrix worldmtx;	// ワールドマトリックス

		// デフォルトコンストラクタ
		Collider_BOX() : vtxMin(), vtxMax(), offset(), worldmtx() {}

		// パラメータ付きコンストラクタ
		Collider_BOX(const MyLib::Vector3& min, const MyLib::Vector3& max, const MyLib::Vector3& offset, const MyLib::Matrix mtx) :
			vtxMin(min), vtxMax(max), offset(offset), worldmtx(mtx) {}


		// JSONからの読み込み
		void from_json(const nlohmann::json& j)
		{
			j.at("vtxMin").get_to(vtxMin);
			j.at("vtxMax").get_to(vtxMax);
			j.at("offset").get_to(offset);
		}

		// JSONへの書き込み
		void to_json(nlohmann::json& j) const
		{
			j = nlohmann::json
			{
				{"vtxMin", vtxMin},
				{"vtxMax", vtxMax},
				{"offset", offset},
			};
		}

		// マトリックス取得
		MyLib::Matrix GetMtx() { return worldmtx; }

		// オフセット計算
		inline void TransformOffset(const MyLib::Matrix& mtx)
		{
			MyLib::Matrix transMtx;
			transMtx.Translation(offset);
			worldmtx.Multiply(transMtx, mtx);
		}
	};

	// カラー関数空間
	namespace color
	{
		inline Color Black(void)				{ return Color(0.0f, 0.0f, 0.0f, 1.0f); }	// 黒色/不透明
		inline Color Black(const float fAlpha)	{ return Color(0.0f, 0.0f, 0.0f, fAlpha); }	// 黒色/透明度
		inline Color White(void)				{ return Color(1.0f, 1.0f, 1.0f, 1.0f); }	// 白色/不透明
		inline Color White(const float fAlpha)	{ return Color(1.0f, 1.0f, 1.0f, fAlpha); }	// 白色/透明度
		inline Color Red(void)					{ return Color(1.0f, 0.0f, 0.0f, 1.0f); }	// 赤色/不透明
		inline Color Red(const float fAlpha)	{ return Color(1.0f, 0.0f, 0.0f, fAlpha); }	// 赤色/透明度
		inline Color Green(void)				{ return Color(0.0f, 1.0f, 0.0f, 1.0f); }	// 緑色/不透明
		inline Color Green(const float fAlpha)	{ return Color(0.0f, 1.0f, 0.0f, fAlpha); }	// 緑色/透明度
		inline Color Blue(void)					{ return Color(0.0f, 0.0f, 1.0f, 1.0f); }	// 青色/不透明
		inline Color Blue(const float fAlpha)	{ return Color(0.0f, 0.0f, 1.0f, fAlpha); }	// 青色/透明度
		inline Color Yellow(void)				{ return Color(1.0f, 1.0f, 0.0f, 1.0f); }	// 黄色/不透明
		inline Color Yellow(const float fAlpha)	{ return Color(1.0f, 1.0f, 0.0f, fAlpha); }	// 黄色/透明度
		inline Color Cyan(void)					{ return Color(0.0f, 1.0f, 1.0f, 1.0f); }	// 水色/不透明
		inline Color Cyan(const float fAlpha)	{ return Color(0.0f, 1.0f, 1.0f, fAlpha); }	// 水色/透明度
		inline Color Purple(void)				{ return Color(1.0f, 0.0f, 1.0f, 1.0f); }	// 紫色/不透明
		inline Color Purple(const float fAlpha)	{ return Color(1.0f, 0.0f, 1.0f, fAlpha); }	// 紫色/透明度
	
		// カラーコード/不透明
		inline Color Code(const std::string& rCode, const float fAlpha)
		{
			assert((int)rCode.size() == 6);
			int R = std::stoi(rCode.substr(0, 2), nullptr, 16);
			int G = std::stoi(rCode.substr(2, 2), nullptr, 16);
			int B = std::stoi(rCode.substr(4, 2), nullptr, 16);
			Color col = D3DCOLOR_RGBA(R, G, B, (int)(fAlpha * 255.0f));
			return col;
		}
	
		// カラーコード/透明度
		inline Color Code(const std::string& rCode)
		{
			return Code(rCode, 1.0f);
		}
	}
	
	// マテリアル関数空間
	namespace material
	{
		// 黒色マテリアル取得
		inline D3DXMATERIAL Black(void)
		{
			D3DXMATERIAL mat;
			ZeroMemory(&mat, sizeof(mat));
			mat.MatD3D.Diffuse = color::Black();
			return mat;
		}

		// 白色マテリアル取得
		inline D3DXMATERIAL White(void)
		{
			D3DXMATERIAL mat;
			ZeroMemory(&mat, sizeof(mat));
			mat.MatD3D.Diffuse = color::White();
			return mat;
		}

		// 赤色マテリアル取得
		inline D3DXMATERIAL Red(void)
		{
			D3DXMATERIAL mat;
			ZeroMemory(&mat, sizeof(mat));
			mat.MatD3D.Diffuse = color::Red();
			return mat;
		}

		// 緑色マテリアル取得
		inline D3DXMATERIAL Green(void)
		{
			D3DXMATERIAL mat;
			ZeroMemory(&mat, sizeof(mat));
			mat.MatD3D.Diffuse = color::Green();
			return mat;
		}

		// 青色マテリアル取得
		inline D3DXMATERIAL Blue(void)
		{
			D3DXMATERIAL mat;
			ZeroMemory(&mat, sizeof(mat));
			mat.MatD3D.Diffuse = color::Blue();
			return mat;
		}

		// 黄色マテリアル取得
		inline D3DXMATERIAL Yellow(void)
		{
			D3DXMATERIAL mat;
			ZeroMemory(&mat, sizeof(mat));
			mat.MatD3D.Diffuse = color::Yellow();
			return mat;
		}

		// 水色マテリアル取得
		inline D3DXMATERIAL Cyan(void)
		{
			D3DXMATERIAL mat;
			ZeroMemory(&mat, sizeof(mat));
			mat.MatD3D.Diffuse = color::Cyan();
			return mat;
		}

		// 紫色マテリアル取得
		inline D3DXMATERIAL Purple(void)
		{
			D3DXMATERIAL mat;
			ZeroMemory(&mat, sizeof(mat));
			mat.MatD3D.Diffuse = color::Purple();
			return mat;
		}
	}
}

#if 0
// nlohmann::jsonの特殊化
namespace nlohmann 
{
	template <> 
	struct adl_serializer<MyLib::Vector3>
	{
		static void from_json(const json& j, MyLib::Vector3& v)
		{
			v.from_json(j);
		}

		static void to_json(json& j, const MyLib::Vector3& v)
		{
			v.to_json(j);
		}
	};
}
#endif

#endif