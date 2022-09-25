#ifndef ISCENECREATOR_H_
#define ISCENECREATOR_H_

// INCLUDE
#include "IScene.h"

namespace sip
{
    /**
    * @brief シーンクリエイターインターフェース
    */
    class ISceneCreator
    {
    public:
    
        /**
        * @brief デストラクタ
        */
        virtual ~ISceneCreator() = default;

        /**
        * @brief シーンを生成する
        * @return 生成されたシーンポインタ
        */
        virtual ScenePtr create() = 0;
    };

    // 名前の置き換え
    using SceneCreatorPtr = std::shared_ptr<ISceneCreator>;
}

#endif // !ISCENECREATOR_H_
