#pragma once
#include <Mof.h>
#include <memory>

namespace RuleFade {
	/*
	* @brief ルールフェード用コンスタントバッファ
	*/
	struct cbRuleFade
	{
		Vector4 param; //!< x:softRange, y:fadeTime, z:none, w:none
	};

	/*
	* @brief シェーダーフェードクラス
	*/
	class CFadeShader
	{
	public:
		/*
		* @biref コンストラクタ
		*/
		explicit CFadeShader();

		/*
		* @brief デストラクタ
		*/
		virtual ~CFadeShader();

		/*
		* @brief シェーダーの生成
		* @return true 成功, false 失敗
		*/
		virtual bool Create();

		/*
		* @brief シェーダーの解放
		*/
		virtual void Release();

		/*
		* @brief マスク画像の設定
		*/
		virtual void SetMaskTexture(CTexture* pMaskTexture);

		/*
		* @brief フェード時間の設定(0.0f ~ 1.0f)
		* @param[in] フェード時間
		*/
		virtual void SetFadeTime(float t);

		/*
		* @brief 境界の曖昧さ(0.0f ~ 1.0f)
		* @param[in] 境界値
		*/
		virtual void SetSoftRange(float v);

		/*
		* @brief シェーダーに値を設定
		*/
		virtual void SetBuffer();

		/*
		* @brief シェーダーの取得
		* @return シェーダー
		*/
		virtual CShader* GetShader();
		virtual const CShader* GetShader() const;

		/*
		* @brief シェーダーバインドの取得
		* @return シェーダーバインド
		*/
		virtual CShaderBind_SpriteBase* GetShaderBind();
		virtual const CShaderBind_SpriteBase* GetShaderBind() const;

	protected:
		cbRuleFade                                  m_ConstantBuffer;      //!< フェードパラメータ
		std::unique_ptr<CShader>                    m_pRuleShader;         //!< シェーダー
		std::unique_ptr<CShaderBind_SpriteBase>     m_pRuleShaderBind;     //!< シェーダーバインド
		CTexture*                                   m_pMaskTexture;        //!< マスク画像ポインタ
	};

#pragma region 生シェーダーファイル
	constexpr const char* rule_fade_for_mof = u8R"(
//! コンスタントバッファ
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

//! 頂点属性
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

//! 頂点シェーダ
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

//! ピクセルシェーダ
float4 RenderPS( OutputVS inPixel ) : SV_TARGET
{
    // 基準値を計算
    float rangedValue = param[1] * (param[0] * 2.0f + 1.0f) - param[0];
    // 最小値と最大値を決める
    float minValue = rangedValue - param[0];
    float maxValue = rangedValue + param[0];
    // ルール画像の濃度を取得
    float maskValue = txMask.Sample(samLinear, inPixel.tex).r;
    // smoothstep(エルミート補完)で透過値を決定
    float alpha = smoothstep(minValue, maxValue, maskValue);
    
    // 割り当てたテクスチャを使う
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