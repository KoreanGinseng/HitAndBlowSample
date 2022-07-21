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
* @brief �摜�Ǘ��N���X�̎擾
* @return �摜�Ǘ��N���X�|�C���^
*/
std::unique_ptr<CTextureManager>& CResourceManager::GetTextureManager()
{
    return GetInstance()->m_pTextureManager;
}

/*
* @brief �S���
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
* @brief �C���X�^���X�̐���
*/
void CResourceManager::Create()
{
    m_pInstance.reset(NEW CResourceManager());
}
/*
* @brief �f�ފǗ��N���X�̎擾
* @return �f�ފǗ��N���X�|�C���^
*/
std::unique_ptr<CResourceManager>& CResourceManager::GetInstance()
{
    if (m_pInstance == nullptr) {
        Create();
    }
    return m_pInstance;
}
