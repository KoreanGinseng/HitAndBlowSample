#ifndef BG_VIEW_H_
#define BG_VIEW_H_

#include <Mof.h>

class BG_View {
public:
    explicit BG_View();
    ~BG_View();
    void update();
    void draw();

    void setBGTexture(std::string_view key);
private:
    float m_Scroll;
    CTexture* m_pBGTexture;
};

#endif // !BG_VIEW_H_