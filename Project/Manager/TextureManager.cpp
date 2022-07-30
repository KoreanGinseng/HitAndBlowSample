#include "TextureManager.h"

namespace {
    static TexturePtr Invalid = nullptr; //!< �����l
}

CTextureManager::CTextureManager()
    : m_Map{}
{
}

CTextureManager::~CTextureManager()
{
}

/*
* @brief �ǂݍ���
* @param[in] pFile �t�@�C���p�X
* @param[in] name  �o�^��
* @param return true : ����, false : ���s
*/
bool CTextureManager::Load(LPCMofChar pFile, std::string_view name)
{
    // �v�f������ēǂݍ���
    TexturePtr pTexture = std::make_unique<CTexture>();
    if (pTexture && !pTexture->Load(pFile)) {
        pTexture->Release();
        assert(false);
        return false;
    }
    // ��������΃}�b�v�ɓo�^����
    m_Map.insert(std::make_pair(name, std::move(pTexture)));
    return true;
}

/*
* @brief �폜
* @param[in] �o�^��
*/
void CTextureManager::Erase(std::string_view name)
{
    // �������Č�����Ȃ���΂Ȃɂ����Ȃ�
    auto& it = m_Map.find(std::string(name));
    if (it == m_Map.end()) {
        assert(false);
        return;
    }
    // ������ă}�b�v����폜
    it->second->Release();
    m_Map.erase(it);
}

/*
* @brief �v�f�̑S���
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
* @brief �擾
* @param[in] �o�^��
* @param return �e�N�X�`���|�C���^, nullptr : ���s
*/
RefTexturePtr CTextureManager::Get(std::string_view name)
{
    // �������Č�����Ȃ���� nullptr ��Ԃ�
    auto& texture = m_Map.find(std::string(name));
    if (texture == m_Map.end()) {
        assert(false);
        return Invalid;
    }
    return texture->second;
}
