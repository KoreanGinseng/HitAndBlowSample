#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include "../SceneManager/IScene.h"
#include "../BG_View.h"

class GameScene : public sip::IScene {
public:
    explicit GameScene();
    virtual ~GameScene();

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;
    virtual void release() override;
private:
    BG_View m_BG_View;
};

#endif // !GAME_SCENE_H_
