#ifndef SCENECHANGEEFFECTFADE_H_
#define SCENECHANGEEFFECTFADE_H_

// INCLUDE
#include "../SceneManager/ISceneChangeEffect.h"
#include "FadeShader.h"

namespace sip 
{
    /**
    * @brief シーンチェンジフェード効果クラス
    */
    class SceneChangeEffectFade : public ISceneChangeEffect
    {
    public:
        /**
        * @brief コンストラクタ
        * @param[in] softRange 境界値
        */
        explicit SceneChangeEffectFade(RuleFade::CFadeShader* pShader, float fadeTime = 0.5f, float softRange = 0.25f);

        /**
        * @brief デストラクタ
        */
        virtual ~SceneChangeEffectFade();

        /**
        * @brief フェード効果を更新する
        */
        virtual void update() override;

        /**
        * @brief フェード効果を描画する
        * @param[in] prev    前シーンポインタ
        * @param[in] current 今シーンポインタ
        */
        virtual void draw(ScenePtr prev, ScenePtr current) override;

        /**
        * @brief 画面効果の終了フラグを取得する
        * @return true : 終了, false : 画面効果中
        */
        virtual bool hasEnded() const override;

    private:
        float                     m_FadeTime{ 0.5f };   //!< フェードにかける時間
        bool                      m_isFadeIn{ false };  //!< フェードINフラグ
        RuleFade::CFadeShader*    m_pRuleFade{};        //!< ルールフェード
        float                     m_Timer{ 0.0f };
        std::unique_ptr<CTexture> m_pTarget{ nullptr };
    };
}

#endif // !SCENECHANGEEFFECTFADE_H_
