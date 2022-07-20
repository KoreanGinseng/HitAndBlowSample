#include "TextureManager.h"

CTextureManager::CTextureManager()
    : m_Map{}
{
}

CTextureManager::~CTextureManager()
{
}

bool CTextureManager::Load(LPCMofChar pFile, std::string_view name)
{
    TexturePtr pTexture = std::make_shared<CTexture>();
    if (!pTexture->Load(pFile)) {
        pTexture->Release();
        return false;
    }
    m_Map.insert(std::make_pair(name, pTexture));
    return true;
}

const TexturePtr& const CTextureManager::Get(std::string_view name) const
{
    const auto& texture = m_Map.find(std::string(name));
    if (texture == m_Map.end()) {
        return nullptr;
    }
    return texture->second;
}
