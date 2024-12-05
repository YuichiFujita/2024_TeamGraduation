//============================================================
//
//	�P�F�`��V�F�[�_�[�̃G�t�F�N�g�t�@�C�� [shaderMono.fx]
//	Author�F���c�E��
//
//============================================================
//************************************************************
//	�O���[�o���ϐ��錾
//************************************************************
float4x4 g_mtxWorld	: WORLD;		// ���[���h�}�g���b�N�X
float4x4 g_mtxView	: VIEW;			// �r���[�}�g���b�N�X
float4x4 g_mtxProj	: PROJECTION;	// �v���W�F�N�V�����}�g���b�N�X

float4 g_colDraw;	// �s�N�Z���`��F

//************************************************************
//	�T���v���[�錾
//************************************************************
sampler texObject : register(s0);	// �I�u�W�F�N�g�e�N�X�`��

//************************************************************
//	�\���̒�`
//************************************************************
// ���_�V�F�[�_�[�o�͏��
struct VS_OUTPUT
{
	float4 pos : POSITION;	// ���_���W
	float4 col : COLOR0;	// ���_�F
	float2 tex : TEXCOORD0;	// �e�N�X�`���F
};

//************************************************************
//	�O���[�o���֐�
//************************************************************
//============================================================
//	���_���W�̎ˉe�ϊ�
//============================================================
float4 TransVertex
(
	in float4	inPos		: POSITION,		// ���_���W
	in float4x4	inMtxWorld	: WORLD,		// ���[���h�}�g���b�N�X
	in float4x4	inMtxView	: VIEW,			// �r���[�}�g���b�N�X
	in float4x4	inMtxProj	: PROJECTION	// �v���W�F�N�V�����}�g���b�N�X
)
{
	// ���_���W���ˉe�ϊ�
	inPos = mul(inPos, inMtxWorld);
	inPos = mul(inPos, inMtxView);
	inPos = mul(inPos, inMtxProj);

	// �ϊ���̒��_���W��Ԃ�
	return inPos;
}

//============================================================
//	���_�V�F�[�_�[
//============================================================
void VS
(
	in	float4		inPos : POSITION,	// ���_���W
	in	float4		inCol : COLOR0,		// ���_�F
	in	float2		inTex : TEXCOORD0,	// �e�N�X�`���F
	out	VS_OUTPUT	outVertex			// ���_���
)
{
	// ���[�J�����_���W���ˉe�ϊ����ݒ�
	outVertex.pos = TransVertex(inPos, g_mtxWorld, g_mtxView, g_mtxProj);

	// 
	outVertex.col = inCol;

	// 
	outVertex.tex = inTex;
}

//============================================================
//	�s�N�Z���V�F�[�_�[
//============================================================
void PS
(
	in	VS_OUTPUT	inVertex,		// ���_���
	out	float4		outCol : COLOR0	// �s�N�Z���F
)
{
#if 0
	// 
	inVertex.col *= tex2D(texObject, inVertex.tex);

	// �P�x�v�Z (��ʓI�ȏd�ݕt��)
	float fLuminance = dot(inVertex.col.rgb, float3(0.299f, 0.587f, 0.114f));

	// �R���g���X�g������ (臒l����)
	float threshold = 0.35f; // �������l (0.0f - 1.0f)
	float strongContrast = fLuminance > threshold ? 1.0f : 0.0f;

	// �O���[�X�P�[���l��RGB�����ɔ��f
	outCol = float4(strongContrast, strongContrast, strongContrast, 1.0f);
#else
	// �e�N�X�`���F�̎擾
	float4 color = inVertex.col * tex2D(texObject, inVertex.tex);

	// �O���[�X�P�[���ɕϊ�
	float fLuminance = dot(color.rgb, float3(0.299f, 0.587f, 0.114f));

	// �������l
	float thresholdBlack = 0.16f;	// ���͈̔� (�Â�����)
	float thresholdWhite = 0.32f;	// ���͈̔� (���邢����)
	if (fLuminance < thresholdBlack)
	{
		// �s�N�Z���̐F�������ɐݒ�
		outCol = float4(0.0f, 0.0f, 0.0f, color.a);
	}
	else if (fLuminance < thresholdWhite)
	{
		// �s�N�Z���̐F�����O���[�ɐݒ�
		outCol = float4(fLuminance, fLuminance, fLuminance, color.a);
	}
	else
	{
		// �s�N�Z���̐F���𔒂ɐݒ�
		outCol = float4(1.0, 1.0, 1.0, color.a);
	}
#endif
}

//============================================================
//	�e�N�j�b�N�֐�
//============================================================
technique TShader
{
	pass P0
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader  = compile ps_2_0 PS();
	}
}
