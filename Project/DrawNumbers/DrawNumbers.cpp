#include "DrawNumbers.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/TextureManager.h"

namespace {
    enum Num {
        NUM_1, NUM_2, NUM_3, NUM_4, NUM_5,
        NUM_6, NUM_7, NUM_8, NUM_9, NUM_0,

        NUM_COUNT,
    };
    static const MofS32 width        = 160;
    static const MofS32 height       = 200;
    static const MofS32 width_count  =   5;
    static const MofS32 height_count =   2;
    static const MofS32 width_max    = 800;
    static const MofS32 height_max   = 410;
    static const Mof::Rectangle rects[NUM_COUNT] = {
        { width * NUM_1,                   0, width * NUM_1 + width,     height },
        { width * NUM_2,                   0, width * NUM_2 + width,     height },
        { width * NUM_3,                   0, width * NUM_3 + width,     height },
        { width * NUM_4,                   0, width * NUM_4 + width,     height },
        { width * NUM_5,                   0, width * NUM_5 + width,     height },
        { width * NUM_1, height_max - height, width * NUM_1 + width, height_max },
        { width * NUM_2, height_max - height, width * NUM_2 + width, height_max },
        { width * NUM_3, height_max - height, width * NUM_3 + width, height_max },
        { width * NUM_4, height_max - height, width * NUM_4 + width, height_max },
        { width * NUM_5, height_max - height, width * NUM_5 + width, height_max },
    };
    static const MofS32 draw_offsets_x[NUM_COUNT] = {
        10, 5, 0, -5, -10,
        10, 5, 0, -5, -10,
    };
}

CDrawNumbers::CDrawNumbers()
    : m_pTexture{ nullptr }
{
    // �摜��ݒ�
    m_pTexture = CResourceManager::GetTextureManager()->Get("Nums.png");
}

CDrawNumbers::~CDrawNumbers()
{
}

/*
* @brief �`��
* @param[in] x  �`�悷��ʒuX
* @param[in] y  �`�悷��ʒuY
* @param[in] sx �`�悷��g�嗦X
* @param[in] sy �`�悷��g�嗦Y
*/
void CDrawNumbers::Render(MofFloat x, MofFloat y, MofFloat sx, MofFloat sy) const
{
    // �摜���Ȃ��Ȃ�`�悵�Ȃ�
    if (m_pTexture == nullptr) {
        assert(false);
        return;
    }

    // �����𒲂ׂ�
    const auto length = m_NumString.length();

    // �ꌅ���`�悵�Ă���
    MofFloat offsetX = x;
    for (std::size_t i = 0; i < length; i++) {
        const MofS8 str[2] = { m_NumString[i], '\0' };
        auto num  = std::atoi(str) - 1;
        if (num < 0) {
            num = 9;
        }
        const auto& srec = rects[num];
        const auto& draw_offset_x = draw_offsets_x[num];
        m_pTexture->RenderScale(offsetX + draw_offset_x, y, sx, sy, srec);
        offsetX += (srec.Right - srec.Left) * sx;
    }
}
