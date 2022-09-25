#include "FadeShader.h"

/*
* @biref コンストラクタ
*/
RuleFade::CFadeShader::CFadeShader()
    : m_ConstantBuffer{ { 0.25f, 0.00f, 0.00f, 0.00f } }
    , m_pMaskTexture{ nullptr }
    , m_pRuleShader{ nullptr }
    , m_pRuleShaderBind{ nullptr }
{
}

/*
* @brief デストラクタ
*/
RuleFade::CFadeShader::~CFadeShader()
{
    Release();
}

/*
* @brief シェーダーの生成
* @return true 成功, false 失敗
*/
bool RuleFade::CFadeShader::Create()
{
    Release();
    std::string shader = rule_fade_for_mof;
    m_pRuleShader = std::make_unique<CShader>();
    if (!m_pRuleShader->Load((void*)shader.data(), shader.size())) {
        return false;
    }
    m_pRuleShaderBind = std::make_unique<CShaderBind_SpriteBase>();
    if (!m_pRuleShaderBind->Create(m_pRuleShader.get())) {
        return false;
    }
    if (!m_pRuleShaderBind->CreateShaderBuffer("cbRuleFade", sizeof(cbRuleFade))) {
        return false;
    }
    if (!m_pRuleShaderBind->CreateShaderResource("txMask")) {
        return false;
    }

    MofU32 sw = g_pGraphics->GetTargetWidth();
    MofU32 sh = g_pGraphics->GetTargetHeight();
    CGraphicsUtilities::SetScreenSize(sw, sh, m_pRuleShaderBind.get());
    return true;
}

/*
* @brief シェーダーの解放
*/
void RuleFade::CFadeShader::Release()
{
    if (m_pRuleShaderBind.get()) {
        m_pRuleShaderBind->Release();
        m_pRuleShaderBind.reset();
    }
    if (m_pRuleShader.get()) {
        m_pRuleShader->Release();
        m_pRuleShader.reset();
    }
}

/*
* @brief マスク画像の設定
*/
void RuleFade::CFadeShader::SetMaskTexture(CTexture* pMaskTexture)
{
    m_pMaskTexture = pMaskTexture;
}

/*
* @brief フェード時間の設定(0.0f ~ 1.0f)
* @param[in] フェード時間
*/
void RuleFade::CFadeShader::SetFadeTime(float t)
{
    m_ConstantBuffer.param[1] = MOF_CLIPING(t, 0.0f, 1.0f);
}

/*
* @brief 境界の曖昧さ(0.0f ~ 1.0f)
* @param[in] 境界値
*/
void RuleFade::CFadeShader::SetSoftRange(float v)
{
    m_ConstantBuffer.param[0] = MOF_CLIPING(v, 0.0f, 1.0f);
}

/*
* @brief シェーダーに値を設定
*/
void RuleFade::CFadeShader::SetBuffer()
{
    if (m_pRuleShaderBind.get()) {
        m_pRuleShaderBind->GetShaderBuffer(0)->SetBuffer(&m_ConstantBuffer);
        if (m_pMaskTexture) {
            m_pRuleShaderBind->GetShaderResource(0)->SetResource(m_pMaskTexture);
        }
    }
}

/*
* @brief シェーダーの取得
* @return シェーダー
*/
CShader* RuleFade::CFadeShader::GetShader()
{
    return m_pRuleShader.get();
}

/*
* @brief シェーダーの取得
* @return シェーダー
*/
const CShader* RuleFade::CFadeShader::GetShader() const
{
    return m_pRuleShader.get();
}

/*
* @brief シェーダーバインドの取得
* @return シェーダーバインド
*/
CShaderBind_SpriteBase* RuleFade::CFadeShader::GetShaderBind()
{
    return m_pRuleShaderBind.get();
}

/*
* @brief シェーダーバインドの取得
* @return シェーダーバインド
*/
const CShaderBind_SpriteBase* RuleFade::CFadeShader::GetShaderBind() const
{
    return m_pRuleShaderBind.get();
}
