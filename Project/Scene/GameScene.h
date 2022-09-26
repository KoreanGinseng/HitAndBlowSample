#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include "../SceneManager/IScene.h"

class GameScene : public sip::IScene {
public:
    explicit GameScene();
    virtual ~GameScene();

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;
    virtual void release() override;
};

#endif // !GAME_SCENE_H_
