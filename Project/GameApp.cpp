/*************************************************************************//*!

					@file	GameApp.cpp
					@brief	��{�Q�[���A�v���B

															@author	�_�c�@��
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"

// MANAGER
#include    "Manager/ResourceManager.h"
#include    "Manager/TextureManager.h"

// TEST
#include    "DrawNumbers/DrawNumbers.h"
std::unique_ptr<CDrawNumbers> g_pDrawNums = nullptr;
	
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̏�����
		@param			None

		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Initialize(void) {

	// ��ʃT�C�Y�̕ύX
	g_pGraphics->SetScreenSize(1280, 720);

	// �f�ރt�H���_�Ɏw��
	CUtilities::SetCurrentDirectory("Resource");

	// �摜�̓ǂݍ���
	if (!CResourceManager::GetTextureManager()->Load("Nums.png", "Nums.png")) {
		return FALSE;
	}

	g_pDrawNums.reset(NEW CDrawNumbers());
	g_pDrawNums->SetNum(1234567890);

	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̍X�V
		@param			None

		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Update(void) {
	//�L�[�̍X�V
	g_pInput->RefreshKey();

	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̕`��
		@param			None

		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Render(void) {
	//�`��J�n
	g_pGraphics->RenderStart();
	//��ʂ̃N���A
	g_pGraphics->ClearTarget(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0);

	// �e�X�g�`��
	g_pDrawNums->Render(0, 0, 0.5f, 0.5f);

	//�`��̏I��
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̉��
		@param			None

		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Release(void) {

	// �f�ނ̉��
	CResourceManager::Release();

	return TRUE;
}