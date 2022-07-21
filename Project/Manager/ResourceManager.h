#pragma once
#include <Mof.h>
#include <memory>

// 前方宣言
class CTextureManager;

// 素材管理クラス
class CResourceManager
{
public:

    ~CResourceManager();

    static std::unique_ptr<CTextureManager>& GetTextureManager();

    static void Release();

private:

    CResourceManager();

    static void Create();

    static std::unique_ptr<CResourceManager>& GetInstance();

    static std::unique_ptr<CResourceManager> m_pInstance;       //!< インスタンス

    std::unique_ptr<CTextureManager>         m_pTextureManager; //!< 画像管理クラス
};

