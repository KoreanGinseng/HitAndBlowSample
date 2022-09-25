#ifndef ISCENECHANGEEFFECT_H_
#define ISCENECHANGEEFFECT_H_

// INCLUDE
#include "IScene.h"

namespace sip
{
    /**
    * @brief シーンチェンジ効果インターフェース
    */
    class ISceneChangeEffect
    {
    public:

        /**
        * @brief デストラクタ
        */
        virtual ~ISceneChangeEffect() = default;
        
        /**
        * @brief 画面効果を更新する
        */
        virtual void update() = 0;
        
        /**
        * @brief 画面効果を描画する
        * @param[in] prev    前シーンポインタ
        * @param[in] current 今シーンポインタ
        */
        virtual void draw(ScenePtr prev, ScenePtr current) = 0;
        
        /**
        * @brief 画面効果の終了フラグを取得する
        * @return true : 終了, false : 画面効果中
        */
        virtual bool hasEnded() const = 0;
    };

    // 名前の置き換え
    using SceneChangeEffectPtr = std::shared_ptr<ISceneChangeEffect>;
}

#endif // !ISCENECHANGEEFFECT_H_
