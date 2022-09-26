#ifndef TITLE_SCENE_H_
#define TITLE_SCENE_H_

#include "../SceneManager/IScene.h"

class TitleScene : public sip::IScene {
public:

    explicit TitleScene();
    virtual ~TitleScene();

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;
    virtual void release() override;

};

#endif // !TITLE_SCENE_H_
