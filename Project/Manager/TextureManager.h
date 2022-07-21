#pragma once
#include <Mof.h>
#include <unordered_map>
#include <string_view>
#include <memory>

// 名前の置き換え
using TexturePtr = std::shared_ptr<CTexture>;

// 画像管理クラス
class CTextureManager
{
public:

    using Map = std::unordered_map<std::string, TexturePtr>;

    explicit CTextureManager();
    virtual ~CTextureManager();

    bool Load(LPCMofChar pFile, std::string_view name);

    void Erase(std::string_view name);

    void Release();

    const TexturePtr Get(std::string_view name) const;

    const TexturePtr operator[](std::string name) const {
        return Get(name);
    }

private:

    Map m_Map; //!< マップ
};

