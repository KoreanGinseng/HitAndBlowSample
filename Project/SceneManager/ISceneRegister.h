#ifndef ISCENEREGISTER_H_
#define ISCENEREGISTER_H_

// INCLUDE
#include "ISceneCreator.h"

namespace sip
{
    /**
    * @brief シーンレジスタインターフェース
    */
    class ISceneRegister
    {
    public:
        /**
        * @brief デストラクタ
        */
        virtual ~ISceneRegister() = default;

        /**
        * @brief シーンを登録する
        * @param[in] key     登録するキー
        * @param[in] creator 登録するシーン生成クラス
        */
        virtual void regist(int key, SceneCreatorPtr creator) = 0;
    };

    // 名前の置き換え
    using SceneRegisterPtr = std::shared_ptr<ISceneRegister>;
}

#endif // !ISCENEREGISTER_H_
