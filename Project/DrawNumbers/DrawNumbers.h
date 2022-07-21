#pragma once
#include <Mof.h>
#include <memory>
#include <string>

// �����`��N���X
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
    
    std::shared_ptr<CTexture>   m_pTexture;                //!< �摜�|�C���^
    std::string                 m_NumString;               //!< ����������
};
