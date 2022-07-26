#include "TextureManager.h"

namespace {
    static TexturePtr Invalid = nullptr; //!< 無効値
}

CTextureManager::CTextureManager()
    : m_Map{}
{
}

CTextureManager::~CTextureManager()
{
}

/*
* @brief 読み込み
* @param[in] pFile ファイルパス
* @param[in] name  登録名
* @param return true : 成功, false : 失敗
*/
bool CTextureManager::Load(LPCMofChar pFile, std::string_view name)
{
    // 要素を作って読み込む
    TexturePtr pTexture = std::make_unique<CTexture>();
    if (pTexture && !pTexture->Load(pFile)) {
        pTexture->Release();
        assert(false);
        return false;
    }
    // 成功すればマップに登録する
    m_Map.insert(std::make_pair(name, std::move(pTexture)));
    return true;
}

/*
* @brief 削除
* @param[in] 登録名
*/
void CTextureManager::Erase(std::string_view name)
{
    // 検索して見つからなければなにもしない
    auto& it = m_Map.find(std::string(name));
    if (it == m_Map.end()) {
        assert(false);
        return;
    }
    // 解放してマップから削除
    it->second->Release();
    m_Map.erase(it);
}

/*
* @brief 要素の全解放
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
* @brief 取得
* @param[in] 登録名
* @param return テクスチャポインタ, nullptr : 失敗
*/
RefTexturePtr CTextureManager::Get(std::string_view name)
{
    // 検索して見つからなければ nullptr を返す
    auto& texture = m_Map.find(std::string(name));
    if (texture == m_Map.end()) {
        assert(false);
        return Invalid;
    }
    return texture->second;
}
