#include "ResourceManager.h"
#include "TextureManager.h"

std::unique_ptr<CResourceManager> CResourceManager::m_pInstance = nullptr;

CResourceManager::CResourceManager()
    : m_pTextureManager{ std::make_unique<CTextureManager>() }
{
}

CResourceManager::~CResourceManager()
{
}

/*
* @brief 画像管理クラスの取得
* @return 画像管理クラスポインタ
*/
std::unique_ptr<CTextureManager>& CResourceManager::GetTextureManager()
{
    return GetInstance()->m_pTextureManager;
}

/*
* @brief 全解放
*/
void CResourceManager::Release()
{
    if (m_pInstance == nullptr) {
        return;
    }
    GetTextureManager()->Release();
    m_pInstance.reset(nullptr);
}

/*
* @brief インスタンスの生成
*/
void CResourceManager::Create()
{
    m_pInstance.reset(NEW CResourceManager());
}
/*
* @brief 素材管理クラスの取得
* @return 素材管理クラスポインタ
*/
std::unique_ptr<CResourceManager>& CResourceManager::GetInstance()
{
    if (m_pInstance == nullptr) {
        Create();
    }
    return m_pInstance;
}
