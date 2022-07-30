#include "TextureManager.h"

namespace {
    static TexturePtr Invalid = nullptr; //!< –³Œø’l
}

CTextureManager::CTextureManager()
    : m_Map{}
{
}

CTextureManager::~CTextureManager()
{
}

/*
* @brief “Ç‚Ýž‚Ý
* @param[in] pFile ƒtƒ@ƒCƒ‹ƒpƒX
* @param[in] name  “o˜^–¼
* @param return true : ¬Œ÷, false : Ž¸”s
*/
bool CTextureManager::Load(LPCMofChar pFile, std::string_view name)
{
    // —v‘f‚ðì‚Á‚Ä“Ç‚Ýž‚Þ
    TexturePtr pTexture = std::make_unique<CTexture>();
    if (pTexture && !pTexture->Load(pFile)) {
        pTexture->Release();
        assert(false);
        return false;
    }
    // ¬Œ÷‚·‚ê‚Îƒ}ƒbƒv‚É“o˜^‚·‚é
    m_Map.insert(std::make_pair(name, std::move(pTexture)));
    return true;
}

/*
* @brief íœ
* @param[in] “o˜^–¼
*/
void CTextureManager::Erase(std::string_view name)
{
    // ŒŸõ‚µ‚ÄŒ©‚Â‚©‚ç‚È‚¯‚ê‚Î‚È‚É‚à‚µ‚È‚¢
    auto& it = m_Map.find(std::string(name));
    if (it == m_Map.end()) {
        assert(false);
        return;
    }
    // ‰ð•ú‚µ‚Äƒ}ƒbƒv‚©‚çíœ
    it->second->Release();
    m_Map.erase(it);
}

/*
* @brief —v‘f‚Ì‘S‰ð•ú
*/
void CTextureManager::Release()
{
    for (auto& it : m_Map) {
        if (it.second) {
            it.second->Release();
        }
    }
    m_Map.clear();
}

/*
* @brief Žæ“¾
* @param[in] “o˜^–¼
* @param return ƒeƒNƒXƒ`ƒƒƒ|ƒCƒ“ƒ^, nullptr : Ž¸”s
*/
RefTexturePtr CTextureManager::Get(std::string_view name)
{
    // ŒŸõ‚µ‚ÄŒ©‚Â‚©‚ç‚È‚¯‚ê‚Î nullptr ‚ð•Ô‚·
    auto& texture = m_Map.find(std::string(name));
    if (texture == m_Map.end()) {
        assert(false);
        return Invalid;
    }
    return texture->second;
}
