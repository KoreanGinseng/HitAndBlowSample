#pragma once
#include <Mof.h>
#include <memory>
#include <string>

// 数字描画クラス
class CDrawNumbers
{
public:
    explicit CDrawNumbers();
    virtual ~CDrawNumbers();

    void Render(MofFloat x, MofFloat y, MofFloat sx = 1.0f, MofFloat sy = 1.0f) const;

    void SetNum(MofU32 n) {
        m_NumString = std::to_string(n);
    }

private:
    
    std::shared_ptr<CTexture>   m_pTexture;                //!< 画像ポインタ
    std::string                 m_NumString;               //!< 数字文字列
};

