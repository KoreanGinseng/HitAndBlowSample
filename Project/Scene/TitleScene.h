#ifndef TITLE_SCENE_H_
#define TITLE_SCENE_H_

#include "../SceneManager/IScene.h"
#include "../BG_View.h"

class TitleScene : public sip::IScene {
public:

    explicit TitleScene();
    virtual ~TitleScene();

    virtual void init() override;
    virtual void update() override;
    virtual void draw() override;
    virtual void release() override;
private:

    BG_View m_BG_View;

};

#endif // !TITLE_SCENE_H_
