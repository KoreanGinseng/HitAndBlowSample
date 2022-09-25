#ifndef ISCENE_H_
#define ISCENE_H_

// INCLUDE
#include <memory>

namespace sip
{
    /**
    * @brief シーンインターフェース
    */
    class IScene
    {
    public:
        /**
        * @brief シーンを初期化する
        */
        virtual void init() = 0;

        /**
        * @brief シーンを更新する
        */
        virtual void update() = 0;
        
        /**
        * @brief シーンを描画する
        */
        virtual void draw() = 0;
        
        /**
        * @brief シーンを解放する
        */
        virtual void release() = 0;
    };

    // 名前の置き換え
    using ScenePtr = std::shared_ptr<IScene>;
}

#endif // !ISCENE_H_
