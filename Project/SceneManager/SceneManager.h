#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

// INCLUDE
#include "ISceneRegister.h"
#include "ISceneChanger.h"
#include <unordered_map>

namespace sip
{
    /**
    * @brief シーン管理インターフェース
    */
    class ISceneManager : public ISceneChanger, public ISceneRegister
    {
    public:
        /**
        * @brief デストラクタ
        */
        virtual ~ISceneManager() = default;

        /**
        * @brief シーン管理を更新する
        */
        virtual void update() = 0;

        /**
        * @brief シーン管理を描画する
        */
        virtual void draw() = 0;

        /**
        * @brief シーン管理を開放する
        */
        virtual void release() = 0;
    };
    //---------------------------------------------------------------------

    /**
    * @brief シーン管理クラス
    */
    class SceneManager : public ISceneManager
    {
    public:
        /**
        * @brief シーン生成クラス
        * @tparam T シーン継承クラス
        */
        template < class T >
        class SceneCreator : public ISceneCreator
        {
        public:
            /**
            * @brief シーンを生成する
            * @return 生成されたシーンポインタ
            */
            virtual ScenePtr create() override
            {
                return std::make_shared<T>();
            }
        };

    public:
        /**
        * @brief コンストラクタ
        */
        SceneManager()
            : m_pCurrentScene(nullptr)
            , m_pPrevScene(nullptr)
            , m_SceneMap()
            , m_pChangeEffect(nullptr)
        {

        }

        /**
        * @brief デストラクタ
        */
        virtual ~SceneManager() 
        {
        }

        /**
        * @brief シーンを登録する
        * @param[in] key     登録するキー
        * @param[in] creator 登録するシーン生成クラス
        */
        virtual void regist(int key, SceneCreatorPtr creator) override
        {
            m_SceneMap[key] = std::move(creator);
        }

        /**
        * @brief シーンを登録する
        * @tparam T シーン継承クラス
        * @param[in] key 登録するキー
        */
        template < class T >
        void regist(int key)
        {
            regist(key, std::make_shared<SceneCreator<T>>());
        }

        /**
        * @brief 指定したシーンへ変更する
        * @param[in] key 変更するシーンのキー
        * @return true : 変更完了, false : 変更失敗
        */
        virtual bool change(int key) override
        {
            // 新しいシーンを作成
            ScenePtr scene_tmp = m_SceneMap[key]->create();
            // 新しいシーンの初期化
            scene_tmp->init();
            // 古いシーンの破棄
            if (m_pCurrentScene)
            {
                m_pCurrentScene->release();
                m_pCurrentScene.reset();
            }
            // 新しいシーンを代入
            m_pCurrentScene = scene_tmp;
            return true;
        }

        /**
        * @brief 指定したシーンへ変更する
        * @param[in] key    変更するシーンのキー
        * @param[in] effect 変更するシーンの画面効果
        * @return true : 変更完了, false : 変更失敗
        */
        virtual bool change(int key, SceneChangeEffectPtr effect) override
        {
            if (m_pChangeEffect)
            {
                return false;
            }
            // 古いシーンとエフェクトを保存
            if (m_pCurrentScene)
            {
                m_pChangeEffect = effect;
                m_pPrevScene = m_pCurrentScene;
                m_pCurrentScene = nullptr;
            }
            change(key);
            return true;
        }

        /**
        * @brief シーン変更中のフラグを取得する
        * @return true : 変更中, false : 変更完了
        */
        virtual bool isChange() override
        {
            return (m_pChangeEffect != nullptr);
        }

        /**
        * @brief シーン管理を更新する
        */
        virtual void update() override
        {

            if (m_pCurrentScene && m_pPrevScene && m_pChangeEffect)
            {
                m_pCurrentScene->update();
                m_pChangeEffect->update();
                if (m_pChangeEffect->hasEnded())
                {
                    m_pPrevScene->release();
                    m_pPrevScene.reset();
                    m_pChangeEffect.reset();
                }
            }
            else if (m_pCurrentScene) { m_pCurrentScene->update(); }
        }

        /**
        * @brief シーン管理を描画する
        */
        virtual void draw() override
        {

            if (m_pCurrentScene && m_pPrevScene && m_pChangeEffect)
            {
                m_pChangeEffect->draw(m_pPrevScene, m_pCurrentScene);
            }
            else if (m_pCurrentScene) { m_pCurrentScene->draw(); }
        }

        /**
        * @brief シーン管理を開放する
        */
        virtual void release() override
        {
            if (m_pPrevScene)
            {
                m_pPrevScene->release();
                m_pPrevScene.reset();
            }
            if (m_pCurrentScene)
            {
                m_pCurrentScene->release();
                m_pCurrentScene.reset();
            }
            m_SceneMap.clear();
        }

    private:

        ScenePtr                                 m_pCurrentScene; //!< 今のシーン
        ScenePtr                                 m_pPrevScene;    //!< 前のシーン
        SceneChangeEffectPtr                     m_pChangeEffect; //!< 画面変更効果
        std::unordered_map<int, SceneCreatorPtr> m_SceneMap;      //!< シーン生成マップ
    };

}


#endif // !SCENEMANAGER_H_
