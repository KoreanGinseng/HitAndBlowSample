#include "TextureManager.h"

CTextureManager::CTextureManager()
    : m_Map{}
{
}

CTextureManager::~CTextureManager()
{
}

/*
* @brief “Ç‚İ‚İ
* @param[in] pFile ƒtƒ@ƒCƒ‹ƒpƒX
* @param[in] name  “o˜^–¼
* @param return true : ¬Œ÷, false : ¸”s
*/
bool CTextureManager::Load(LPCMofChar pFile, std::string_view name)
{
    // —v‘f‚ğì‚Á‚Ä“Ç‚İ‚Ş
    TexturePtr pTexture = std::make_shared<CTexture>();
    if (!pTexture->Load(pFile)) {
        pTexture->Release();
        return false;
    }
    // ¬Œ÷‚·‚ê‚Îƒ}ƒbƒv‚É“o˜^‚·‚é
    m_Map.insert(std::make_pair(name, pTexture));
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
        return;
    }
    // ‰ğ•ú‚µ‚Äƒ}ƒbƒv‚©‚çíœ
    it->second->Release();
    m_Map.erase(it);
}

/*
* @brief —v‘f‚Ì‘S‰ğ•ú
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
* @brief æ“¾
* @param[in] “o˜^–¼
* @param return ƒeƒNƒXƒ`ƒƒƒ|ƒCƒ“ƒ^, nullptr : ¸”s
*/
const TexturePtr CTextureManager::Get(std::string_view name) const
{
    // ŒŸõ‚µ‚ÄŒ©‚Â‚©‚ç‚È‚¯‚ê‚Î nullptr ‚ğ•Ô‚·
    const auto& texture = m_Map.find(std::string(name));
    if (texture == m_Map.end()) {
        return nullptr;
    }
    return texture->second;
}
