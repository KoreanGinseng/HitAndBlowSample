#ifndef ISCENECHANGER_H_
#define ISCENECHANGER_H_

// INCLUDE
#include "IScene.h"
#include "ISceneChangeEffect.h"

namespace sip
{
    /**
    * @brief シーンチェンジインターフェース
    */
    class ISceneChanger
    {
    public:

        /**
        * @brief デストラクタ
        */
        virtual ~ISceneChanger() = default;
        
        /**
        * @brief 指定したシーンへ変更する
        * @param[in] key 変更するシーンのキー
        * @return true : 変更完了, false : 変更失敗
        */
        virtual bool change(int key) = 0;
        
        /**
        * @brief 指定したシーンへ変更する
        * @param[in] key    変更するシーンのキー
        * @param[in] effect 変更するシーンの画面効果
        * @return true : 変更完了, false : 変更失敗
        */
        virtual bool change(int key, SceneChangeEffectPtr effect) = 0;
        
        /**
        * @brief シーン変更中のフラグを取得する
        * @return true : 変更中, false : 変更完了
        */
        virtual bool isChange() = 0;
    };

    // 名前の置き換え
    using SceneChangerPtr = std::shared_ptr<ISceneChanger>;
}

#endif // !ISCENECHANGER_H_
