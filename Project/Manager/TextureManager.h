#pragma once
#include "../Common.h"
#include <unordered_map>
#include <string_view>
#include <optional>

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

    RefTexturePtr Get(std::string_view name);

    RefTexturePtr operator[](std::string name) {
        return Get(name);
    }

private:

    Map m_Map; //!< マップ
};

