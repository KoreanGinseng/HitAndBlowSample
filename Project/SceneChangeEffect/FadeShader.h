#pragma once
#include <Mof.h>
#include <memory>

namespace RuleFade {
	/*
	* @brief ���[���t�F�[�h�p�R���X�^���g�o�b�t�@
	*/
	struct cbRuleFade
	{
		Vector4 param; //!< x:softRange, y:fadeTime, z:none, w:none
	};

	/*
	* @brief �V�F�[�_�[�t�F�[�h�N���X
	*/
	class CFadeShader
	{
	public:
		/*
		* @biref �R���X�g���N�^
		*/
		explicit CFadeShader();

		/*
		* @brief �f�X�g���N�^
		*/
		virtual ~CFadeShader();

		/*
		* @brief �V�F�[�_�[�̐���
		* @return true ����, false ���s
		*/
		virtual bool Create();

		/*
		* @brief �V�F�[�_�[�̉��
		*/
		virtual void Release();

		/*
		* @brief �}�X�N�摜�̐ݒ�
		*/
		virtual void SetMaskTexture(CTexture* pMaskTexture);

		/*
		* @brief �t�F�[�h���Ԃ̐ݒ�(0.0f ~ 1.0f)
		* @param[in] �t�F�[�h����
		*/
		virtual void SetFadeTime(float t);

		/*
		* @brief ���E�̞B����(0.0f ~ 1.0f)
		* @param[in] ���E�l
		*/
		virtual void SetSoftRange(float v);

		/*
		* @brief �V�F�[�_�[�ɒl��ݒ�
		*/
		virtual void SetBuffer();

		/*
		* @brief �V�F�[�_�[�̎擾
		* @return �V�F�[�_�[
		*/
		virtual CShader* GetShader();
		virtual const CShader* GetShader() const;

		/*
		* @brief �V�F�[�_�[�o�C���h�̎擾
		* @return �V�F�[�_�[�o�C���h
		*/
		virtual CShaderBind_SpriteBase* GetShaderBind();
		virtual const CShaderBind_SpriteBase* GetShaderBind() const;

	protected:
		cbRuleFade                                  m_ConstantBuffer;      //!< �t�F�[�h�p�����[�^
		std::unique_ptr<CShader>                    m_pRuleShader;         //!< �V�F�[�_�[
		std::unique_ptr<CShaderBind_SpriteBase>     m_pRuleShaderBind;     //!< �V�F�[�_�[�o�C���h
		CTexture*                                   m_pMaskTexture;        //!< �}�X�N�摜�|�C���^
	};

#pragma region ���V�F�[�_�[�t�@�C��
	constexpr const char* rule_fade_for_mof = u8R"(
//! �R���X�^���g�o�b�t�@
cbuffer cbSceneParam : register(b0)
{
	matrix mtxView : packoffset(c0);
	matrix mtxProj : packoffset(c4);
};
cbuffer cbMeshParam : register(b1)
{
	float4 cbvOffset: packoffset(c0);
	float4 cbvSize  : packoffset(c1); 
	float4 cbtOffset: packoffset(c2);
	float4 cbtSize  : packoffset(c3);
	float4 cbColor  : packoffset(c4);
	matrix mtxWorld : packoffset(c5);
};

cbuffer cbRuleFade : register(b4)
{
    float4  param : packoffset( c0 );
};

//! ���_����
struct InputVS
{
	float4 pos    : POSITION;
	float2 tex    : TEXCOORD;
};

struct OutputVS
{
	float4 pos    : SV_POSITION;
	float2 tex    : TEXCOORD0;
	float4 color  : COLOR0;
};

Texture2D txDiffuse : register( t0 );
SamplerState samLinear : register( s0 );
Texture2D txMask : register( t1 );

//! ���_�V�F�[�_
OutputVS RenderVS( InputVS inVert )
{
	OutputVS outVert;
	float4 pv     = mul(float4(inVert.pos.xyz * cbvSize.xyz + cbvOffset.xyz, 1), mtxWorld);
	float4 Pos    = mul(float4(pv.xyz, 1), mtxView);
	outVert.pos   = mul(Pos, mtxProj);
	outVert.color = cbColor;
	outVert.tex   = inVert.tex * cbtSize.xy + cbtOffset.xy;
	return outVert;
}

//! �s�N�Z���V�F�[�_
float4 RenderPS( OutputVS inPixel ) : SV_TARGET
{
    // ��l���v�Z
    float rangedValue = param[1] * (param[0] * 2.0f + 1.0f) - param[0];
    // �ŏ��l�ƍő�l�����߂�
    float minValue = rangedValue - param[0];
    float maxValue = rangedValue + param[0];
    // ���[���摜�̔Z�x���擾
    float maskValue = txMask.Sample(samLinear, inPixel.tex).r;
    // smoothstep(�G���~�[�g�⊮)�œ��ߒl������
    float alpha = smoothstep(minValue, maxValue, maskValue);
    
    // ���蓖�Ă��e�N�X�`�����g��
    float4 texCol = txDiffuse.Sample(samLinear, inPixel.tex) * inPixel.color;
    return float4(texCol.rgb, alpha);
}

technique11 TShader
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, RenderVS()));
		SetGeometryShader(NULL);
		SetHullShader(NULL);
		SetDomainShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, RenderPS()));
		SetComputeShader(NULL);
	}
})";
#pragma endregion
}