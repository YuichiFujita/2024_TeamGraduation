//=============================================================================
// 
// ���g�̃��C�u���� [mylib.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MYLIB_H_
#define _MYLIB_H_		// ��d�C���N���[�h�h�~

#include <json.hpp>

/**
	@brief	�}�C���C�u����
*/
namespace MyLib
{

	/**
	@brief	Vector3 �y3�������z
	*/
	struct Vector3 : public D3DXVECTOR3
	{
		using D3DXVECTOR3::D3DXVECTOR3;

		// �f�t�H���g�R���X�g���N�^
		Vector3() : D3DXVECTOR3(0.0f, 0.0f, 0.0f) {}

		// �����t���R���X�g���N�^
		Vector3(const float xyz) : D3DXVECTOR3(xyz, xyz, xyz) {}
		Vector3(float x, float y, float z) : D3DXVECTOR3(x, y, z) {}

		// JSON����̓ǂݍ���
		void from_json(const nlohmann::json& j)
		{
			j.at("x").get_to(x);
			j.at("y").get_to(y);
			j.at("z").get_to(z);
		}

		// JSON�ւ̏�������
		void to_json(nlohmann::json& j) const
		{
			j["x"] = x;
			j["y"] = y;
			j["z"] = z;
		}

		//--------------------------
		// json�̃t�����h��
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
		// ���Z
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
		// ���Z
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
		// ���Z���
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
		// ���Z���
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

		/**
		@brief	�x�N�g���̒���
		@return	���߂�ꂽ����
		*/
		inline float Length()
		{
			return sqrtf(x * x + y * y + z * z);
		}

		/**
		@brief	XZ�x�N�g���̒���
		@return	���߂�ꂽ����
		*/
		inline float LengthXZ()
		{
			return sqrtf(x * x + z * z);
		}

		/**
		@brief	XY�x�N�g���̒���
		@return	���߂�ꂽ����
		*/
		inline float LengthXY()
		{
			return sqrtf(x * x + y * y);
		}

		/**
		@brief	�x�N�g���̒�����2��
		@return	���߂�ꂽ����
		*/
		inline float LengthSquared() const
		{
			return x * x + y * y + z * z;
		}

		/**
		@brief	XZ�̌���
		@return	����
		*/
		inline float AngleXZ(const Vector3& v)
		{
			return atan2f(x - v.x, z - v.z);
		}

		/**
		@brief	XY�̌���
		@return	����
		*/
		inline float AngleXY(const Vector3& v)
		{
			return atan2f(x - v.x, y - v.y);
		}

		/**
		@brief	�x�N�g���̋���
		@return	2�̃x�N�g���̋���
		*/
		float Distance(const Vector3& v) const 
		{
			return sqrtf((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y) + (v.z - z) * (v.z - z));
		}

		/**
		@brief	XZ���ʂ̃x�N�g���̋���
		@return	2�̃x�N�g���̋���(XZ����)
		*/
		float DistanceXZ(const Vector3& v) const
		{
			return sqrtf((v.x - x) * (v.x - x) + (v.z - z) * (v.z - z));
		}

		/**
		@brief	�x�N�g���̋�����2��
		@return	2�̃x�N�g���̋�����2��
		*/
		float DistSquared(const Vector3& v) const 
		{
			return (v.x - x) * (v.x - x) + (v.y - y) * (v.y - y) + (v.z - z) * (v.z - z);
		}

		/**
		@brief	�P�ʃx�N�g��
		@return	���K�����ꂽ�x�N�g��
		*/
		inline Vector3 Normal()
		{
			Vector3 result;
			D3DXVec3Normalize(&result, this);
			return result;
		}

		/**
		@brief	�O��
		@return	2�̃x�N�g���̊O��
		*/
		Vector3 Cross(const Vector3& v) const 
		{
			return Vector3(
				y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x);
		}

		/**
		@brief	����
		@return	2�̃x�N�g���̓���
		*/
		float Dot(const Vector3& v) const
		{
			return x * v.x + y * v.y + z * v.z;
		}

		/**
		@brief	���������ϊ�
		@return	2�_�̐��������p�x
		*/
		float GetHorizontalAngle(const Vector3& v) const 
		{
			return atan2f((x - v.x), (z - v.z));
		}

		/**
		@brief	�x�N�g���𔽓]
		@return	���]���ꂽ�x�N�g��
		*/
		inline Vector3 Invert()
		{
			return Vector3(-x, -y, -z);
		}

		/**
		@brief	�w�肳�ꂽ���e�덷�͈͓��ɂ��邩����
		@param	target	[in]	�ڕW�l
		@param	range	[in]	���e�͈�
		@return	���茋��
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

		/**
		@brief	�w�肳�ꂽ���e�덷�͈͓��ɂ��邩����
		@param	target	[in]	�ڕW�l
		@param	range	[in]	���e�͈�
		@return	���茋��
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

		/**
		@brief	�w�肳�ꂽ���e�덷�͈͓��ɂ��邩����
		@param	target	[in]	�ڕW�l
		@param	range	[in]	���e�͈�
		@return	���茋��
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

		/**
		@brief	�w�肳�ꂽ���e�덷�͈͓��ɂ��邩����
		@param	target	[in]	�ڕW�l
		@param	range	[in]	���e�͈�
		@return	���茋��
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

		/**
		@brief	�w�肳�ꂽ���e�덷�͈͓��ɂ��邩����
		@param	range	[in]	���e�͈�
		@return	���茋��
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

		/**
		@brief	�v�f���[����
		@return	���茋��
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

		/**
		@brief	�x�N�g����90�x��](X��)
		*/
		Vector3 rotateAroundX()
		{
			float angle = 90 * D3DX_PI / 180.0f;

			return Vector3(
				x,
				y * cosf(angle) - z * sinf(angle),
				y * sinf(angle) + z * cosf(angle));
		}

		/**
		@brief	�x�N�g����90�x��](Y��)
		*/
		Vector3 rotateAroundY()
		{
			float angle = 90 * D3DX_PI / 180.0f;

			return Vector3(
				x * cosf(angle) + z * sinf(angle),
				y,
				-x * sinf(angle) + z * cosf(angle));
		}

		/**
		@brief	�x�N�g����90�x��](Z��)
		*/
		Vector3 rotateAroundZ()
		{
			float angle = 90 * D3DX_PI / 180.0f;

			return Vector3(
				x * cosf(angle) - y * sinf(angle),
				x * sinf(angle) + y * cosf(angle),
				z);
		}

		/**
		@brief	������ϊ�
		@return	(x, y, z)�̌`���ŕϊ����ꂽ������
		*/
		inline std::string ToString() const
		{
			std::stringstream text;
			text << "(" << x << ", " << y << ", " << z << ")";
			return text.str();
		}
	};

	/**
	@brief	�}�g���b�N�X�y�s����z
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

		// �f�t�H���g�R���X�g���N�^
		Matrix() {}

		// �����t���R���X�g���N�^
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
		// ���Z
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
		// ���Z
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
		// ��Z
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
		// ���Z
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
		// ��r
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
		// �v�Z�֐�
		//--------------------------
		/**
		@brief	�P�ʍs��
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

		/**
		@brief	�}�g���b�N�X���m�̊|���Z
		@param	mtx1	[in]	�s����
		@param	mtx2	[in]	�s����
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

		/**
		@brief	�ړ��s��̍쐬
		@param	pos		[in]	�ʒu���
		@return	void
		*/
		inline void Translation(const Vector3& pos)
		{
			Identity();

			_41 = pos.x;
			_42 = pos.y;
			_43 = pos.z;
		}

		/**
		@brief	��]�s��̍쐬(Yaw�APitch�ARoll�i���[�A�s�b�`�A���[���j��)
		@param	yaw		[in]	���[
		@param	pitch	[in]	�s�b�`
		@param	roll	[in]	���[��
		@return	void
		*/
		inline void RotationYawPitchRoll(float yaw, float pitch, float roll)
		{
			Identity();

			// ���W�A���ϊ�
			float cosYaw = cosf(yaw);
			float sinYaw = sinf(yaw);
			float cosPitch = cosf(pitch);
			float sinPitch = sinf(pitch);
			float cosRoll = cosf(roll);
			float sinRoll = sinf(roll);

			// �s��v�Z
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

		/**
		@brief	�g������|�����킹��
		@param	scale	[in]	�g����
		@return	void
		*/
		inline void Scaling(const Vector3& scale)
		{
			Identity();

			_11 = scale.x;
			_22 = scale.y;
			_33 = scale.z;
		}

		/**
		@brief	�ϊ��s��ɉ����Ē��_�ϊ�
		@param	pos		[in]	���ɂ��钸�_
		@param	mtx		[in]	�ϊ��s��
		@return	�ϊ���̒��_
		*/
		inline Vector3 Coord(const Vector3& pos)
		{
			Vector3 result;

			// �x�N�g���Ɗ|�����킹��
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

		///**
		//@brief	�s�񎮂̌v�Z
		//@return	�s��
		//*/
		//float Determinant(const D3DXMATRIX& mat) 
		//{
		//	float determinant =
		//		mat._11 * (mat._22 * mat._33 - mat._23 * mat._32) -
		//		mat._12 * (mat._21 * mat._33 - mat._23 * mat._31) +
		//		mat._13 * (mat._21 * mat._32 - mat._22 * mat._31);
		//	return determinant;
		//}


		//--------------------------
		// �ϊ��֐�
		//--------------------------
		/**
		@brief	���[���h���W�擾
		@return	���[���h���W
		*/
		inline Vector3 GetWorldPosition()
		{
			return Vector3(_41, _42, _43);
		}

		/**
		@brief	�g����擾
		@return	�g����
		*/
		inline Vector3 GetScale()
		{
			Vector3 vec1 = MyLib::Vector3(_11, _12, _13);
			Vector3 vec2 = MyLib::Vector3(_21, _22, _23);
			Vector3 vec3 = MyLib::Vector3(_31, _32, _33);

			// �}�g���b�N�X����X�P�[�����𒊏o
			Vector3 scale;
			scale.x = vec1.Length();
			scale.y = vec2.Length();
			scale.z = vec3.Length();
			return scale;
		}

		/**
		@brief	�O���x�N�g���擾
		@return	�O���x�N�g��
		*/
		inline Vector3 GetForwardVector()
		{
			return Vector3(_13, _23, _33);
		}

		/**
		@brief	D3DXMATRIX�^�ɕϊ�����
		@return	D3DXMATRIX�^�̗v�f
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

	/**
	@brief	HitResult�y�Փˏ��z
	*/
	struct HitResult
	{
		bool ishit;				// �Փˌ���
		MyLib::Vector3 hitpos;	// �Փ˒n�_

		// �f�t�H���g�R���X�g���N�^
		HitResult() : ishit(false), hitpos() {}

		// �p�����[�^�t���R���X�g���N�^
		HitResult(bool bHit, MyLib::Vector3 pos) : ishit(bHit), hitpos(pos) {}
	};

	/**
	@brief	HitResult_Character�y�L�����N�^�[�̏Փˏ��z
	*/
	struct HitResult_Character
	{
		bool ishit;				// �Փˌ���
		bool isdamage;			// �_���[�W����������
		bool isdeath;			// ���S������
		MyLib::Vector3 hitpos;	// �Փ˒n�_

		// �f�t�H���g�R���X�g���N�^
		HitResult_Character() : ishit(false), isdamage(false), isdeath(false), hitpos() {}

		// �p�����[�^�t���R���X�g���N�^
		HitResult_Character(bool bHit, bool bDamage, bool bDeath, MyLib::Vector3 pos) : 
			ishit(bHit), isdamage(bDamage), isdeath(bDeath), hitpos(pos) {}
	};

	/**
	@brief	AABB�y�{�b�N�X���z
	*/
	struct AABB 
	{
		MyLib::Vector3 vtxMin;	// �ŏ��l
		MyLib::Vector3 vtxMax;	// �ő�l

		// �f�t�H���g�R���X�g���N�^
		AABB() : vtxMin(), vtxMax() {}

		// �p�����[�^�t���R���X�g���N�^
		AABB(const MyLib::Vector3& min, const MyLib::Vector3& max) : vtxMin(min), vtxMax(max) {}
	};


	/**
	@brief	BOX�R���C�_�[
	*/
	struct Collider_BOX
	{
		MyLib::Vector3 vtxMin;	// �ŏ��l
		MyLib::Vector3 vtxMax;	// �ő�l
		MyLib::Vector3 offset;	// �I�t�Z�b�g�ʒu
		MyLib::Matrix worldmtx;	// ���[���h�}�g���b�N�X

		// �f�t�H���g�R���X�g���N�^
		Collider_BOX() : vtxMin(), vtxMax(), offset(), worldmtx() {}

		// �p�����[�^�t���R���X�g���N�^
		Collider_BOX(const MyLib::Vector3& min, const MyLib::Vector3& max, const MyLib::Vector3& offset, const MyLib::Matrix mtx) :
			vtxMin(min), vtxMax(max), offset(offset), worldmtx(mtx) {}


		// JSON����̓ǂݍ���
		void from_json(const nlohmann::json& j)
		{
			j.at("vtxMin").get_to(vtxMin);
			j.at("vtxMax").get_to(vtxMax);
			j.at("offset").get_to(offset);
		}

		// JSON�ւ̏�������
		void to_json(nlohmann::json& j) const
		{
			j = nlohmann::json
			{
				{"vtxMin", vtxMin},
				{"vtxMax", vtxMax},
				{"offset", offset},
			};
		}

		// �}�g���b�N�X�擾
		MyLib::Matrix GetMtx() { return worldmtx; }

		// �I�t�Z�b�g�v�Z
		inline void TransformOffset(const MyLib::Matrix& mtx)
		{
			MyLib::Matrix transMtx;
			transMtx.Translation(offset);
			worldmtx.Multiply(transMtx, mtx);
		}
	};
	
}

#if 0
// nlohmann::json�̓��ꉻ
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