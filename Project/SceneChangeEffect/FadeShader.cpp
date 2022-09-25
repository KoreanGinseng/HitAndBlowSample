#include "FadeShader.h"

/*
* @biref �R���X�g���N�^
*/
RuleFade::CFadeShader::CFadeShader()
    : m_ConstantBuffer{ { 0.25f, 0.00f, 0.00f, 0.00f } }
    , m_pMaskTexture{ nullptr }
    , m_pRuleShader{ nullptr }
    , m_pRuleShaderBind{ nullptr }
{
}

/*
* @brief �f�X�g���N�^
*/
RuleFade::CFadeShader::~CFadeShader()
{
    Release();
}

/*
* @brief �V�F�[�_�[�̐���
* @return true ����, false ���s
*/
bool RuleFade::CFadeShader::Create()
{
    Release();
    std::string shader = rule_fade_for_mof;
    m_pRuleShader = std::make_unique<CShader>();
    if (!m_pRuleShader->Load((void*)shader.data(), shader.size())) {
        return false;
    }
    m_pRuleShaderBind = std::make_unique<CShaderBind_SpriteBase>();
    if (!m_pRuleShaderBind->Create(m_pRuleShader.get())) {
        return false;
    }
    if (!m_pRuleShaderBind->CreateShaderBuffer("cbRuleFade", sizeof(cbRuleFade))) {
        return false;
    }
    if (!m_pRuleShaderBind->CreateShaderResource("txMask")) {
        return false;
    }

    MofU32 sw = g_pGraphics->GetTargetWidth();
    MofU32 sh = g_pGraphics->GetTargetHeight();
    CGraphicsUtilities::SetScreenSize(sw, sh, m_pRuleShaderBind.get());
    return true;
}

/*
* @brief �V�F�[�_�[�̉��
*/
void RuleFade::CFadeShader::Release()
{
    if (m_pRuleShaderBind.get()) {
        m_pRuleShaderBind->Release();
        m_pRuleShaderBind.reset();
    }
    if (m_pRuleShader.get()) {
        m_pRuleShader->Release();
        m_pRuleShader.reset();
    }
}

/*
* @brief �}�X�N�摜�̐ݒ�
*/
void RuleFade::CFadeShader::SetMaskTexture(CTexture* pMaskTexture)
{
    m_pMaskTexture = pMaskTexture;
}

/*
* @brief �t�F�[�h���Ԃ̐ݒ�(0.0f ~ 1.0f)
* @param[in] �t�F�[�h����
*/
void RuleFade::CFadeShader::SetFadeTime(float t)
{
    m_ConstantBuffer.param[1] = MOF_CLIPING(t, 0.0f, 1.0f);
}

/*
* @brief ���E�̞B����(0.0f ~ 1.0f)
* @param[in] ���E�l
*/
void RuleFade::CFadeShader::SetSoftRange(float v)
{
    m_ConstantBuffer.param[0] = MOF_CLIPING(v, 0.0f, 1.0f);
}

/*
* @brief �V�F�[�_�[�ɒl��ݒ�
*/
void RuleFade::CFadeShader::SetBuffer()
{
    if (m_pRuleShaderBind.get()) {
        m_pRuleShaderBind->GetShaderBuffer(0)->SetBuffer(&m_ConstantBuffer);
        if (m_pMaskTexture) {
            m_pRuleShaderBind->GetShaderResource(0)->SetResource(m_pMaskTexture);
        }
    }
}

/*
* @brief �V�F�[�_�[�̎擾
* @return �V�F�[�_�[
*/
CShader* RuleFade::CFadeShader::GetShader()
{
    return m_pRuleShader.get();
}

/*
* @brief �V�F�[�_�[�̎擾
* @return �V�F�[�_�[
*/
const CShader* RuleFade::CFadeShader::GetShader() const
{
    return m_pRuleShader.get();
}

/*
* @brief �V�F�[�_�[�o�C���h�̎擾
* @return �V�F�[�_�[�o�C���h
*/
CShaderBind_SpriteBase* RuleFade::CFadeShader::GetShaderBind()
{
    return m_pRuleShaderBind.get();
}

/*
* @brief �V�F�[�_�[�o�C���h�̎擾
* @return �V�F�[�_�[�o�C���h
*/
const CShaderBind_SpriteBase* RuleFade::CFadeShader::GetShaderBind() const
{
    return m_pRuleShaderBind.get();
}
