#pragma once
#include <Mof.h>
#include <memory>

// �O���錾
class CTextureManager;

// �f�ފǗ��N���X
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

    static std::unique_ptr<CResourceManager> m_pInstance;       //!< �C���X�^���X

    std::unique_ptr<CTextureManager>         m_pTextureManager; //!< �摜�Ǘ��N���X
};

