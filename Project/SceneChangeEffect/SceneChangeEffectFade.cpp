// INCLUDE
#include "SceneChangeEffectFade.h"
#include <assert.h>
#include <algorithm>
#include "../Manager/ResourceManager.h"
#include "../Manager/TextureManager.h"

/**
* @brief コンストラクタ
* @param[in] softRange 境界値
*/
sip::SceneChangeEffectFade::SceneChangeEffectFade(RuleFade::CFadeShader* pShader, float fadeTime, float softRange)
    : m_pRuleFade{ pShader }
    , m_isFadeIn{ false }
    , m_FadeTime{ fadeTime }
    , m_pTarget{ nullptr }
{
    // 0以下にならないようにする
    if (m_FadeTime <= 0.0f)
    {
        m_FadeTime = 0.01f;
    }
    m_pTarget = std::make_unique<CTexture>();
    m_pTarget->Create(g_pGraphics->GetTargetWidth(), g_pGraphics->GetTargetHeight(), PIXELFORMAT_R8G8B8A8_UNORM, BUFFERACCESS_GPUREADWRITE);
    m_pRuleFade->SetMaskTexture(CResourceManager::GetTextureManager()->Get("FadeOut").get());
}

/**
* @brief デストラクタ
*/
sip::SceneChangeEffectFade::~SceneChangeEffectFade()
{
    m_pRuleFade->SetMaskTexture(nullptr);
    m_pTarget->Release();
    m_pTarget.reset();
}

/**
* @brief フェード効果を更新する
*/
void sip::SceneChangeEffectFade::update()
{
    if (!m_isFadeIn)
    {
        // フェード値更新
        m_Timer += CUtilities::GetFrameSecond() / m_FadeTime;
        if (m_Timer >= 1.0f)
        {
            m_isFadeIn = true;
            m_pRuleFade->SetMaskTexture(CResourceManager::GetTextureManager()->Get("FadeIn").get());
        }
    }
    else
    {
        // フェード値更新
        m_Timer -= CUtilities::GetFrameSecond() / m_FadeTime;
    }
    // フェード値を(0.0 ~ 1.0)でクリッピング
    m_Timer = std::clamp(m_Timer, 0.0f, 1.0f);
    m_pRuleFade->SetFadeTime(m_Timer);
}

/**
* @brief フェード効果を描画する
* @param[in] prev    前シーンポインタ
* @param[in] current 今シーンポインタ
*/
void sip::SceneChangeEffectFade::draw(ScenePtr prev, ScenePtr current)
{
    auto target_tmp = g_pGraphics->GetRenderTarget();
    auto depth_target = g_pGraphics->GetDepthTarget();
    g_pGraphics->SetRenderTarget(m_pTarget->GetRenderTarget(), depth_target);
    g_pGraphics->ClearTarget(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    // 過去シーンをターゲットに描画する
    if(!m_isFadeIn)
    {
        prev->draw();
    }
    // 現在のシーンをターゲットに描画する
    else 
    {
        current->draw();
    }
    // シェーダーの適応
    {
        g_pGraphics->SetRenderTarget(target_tmp, depth_target);
        g_pGraphics->ClearTarget(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        // コンスタントバッファの設定
        m_pRuleFade->SetBuffer();
        CGraphicsUtilities::RenderTexture(0, 0, m_pTarget.get(), m_pRuleFade->GetShader(), m_pRuleFade->GetShaderBind());
    }
}

/**
* @brief 画面効果の終了フラグを取得する
* @return true : 終了, false : 画面効果中
*/
bool sip::SceneChangeEffectFade::hasEnded() const
{
    return m_isFadeIn && (m_Timer == 0.0f);
}
