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
#include    "SceneManager/SceneManager.h"
// SCENE
#include    "Scene/SceneDefine.h"
#include    "Scene/GameScene.h"
#include    "Scene/TitleScene.h"
#include    "SceneChangeEffect/FadeShader.h"
#include    "SceneChangeEffect/SceneChangeEffectFade.h"

// _DEBUG�p
#ifdef _DEBUG
namespace debug {
	bool g_IsDebug = false;
}
#endif//_DEBUG

namespace Scene {
	sip::SceneManagerPtr g_pSceneManager;
	sip::SceneChangerPtr g_pSceneChanger;
	RuleFade::CFadeShader g_FadeShader;
}

	
/*************************************************************************//*!
		@brief			�A�v���P�[�V�����̏�����
		@param			None

		@return			TRUE		����<br>
						����ȊO	���s�A�G���[�R�[�h���߂�l�ƂȂ�
*//**************************************************************************/
MofBool CGameApp::Initialize(void) {

	// �f�ރt�H���_�Ɏw��
	CUtilities::SetCurrentDirectory("Resource");

	// �摜�̓ǂݍ���
	{
		if (!CResourceManager::GetTextureManager()->Load("Nums.png", "Nums.png")) {
			return FALSE;
		}
		if (!CResourceManager::GetTextureManager()->Load("Cryos_Mini_GUI/GUI/GUI_4x.png", "GUI_4x.png")) {
			return FALSE;
		}
		if (!CResourceManager::GetTextureManager()->Load("Rule1080p/150.png", "FadeOut")) {
			return FALSE;
		}
		if (!CResourceManager::GetTextureManager()->Load("Rule1080p/171.png", "FadeIn")) {
			return FALSE;
		}
		if (!CResourceManager::GetTextureManager()->Load("Yellow.png", "BG_Title")) {
			return FALSE;
		}
		if (!CResourceManager::GetTextureManager()->Load("Gray.png", "BG_Game")) {
			return FALSE;
		}
		if (!CResourceManager::GetTextureManager()->Load("Title.png", "Title_Logo")) {
			return FALSE;
		}
	}
	if (!Scene::g_FadeShader.Create())
	{
		return FALSE;
	}
	
	Scene::g_pSceneManager = std::make_shared<sip::SceneManager>();
	Scene::g_pSceneManager->regist<TitleScene>(SceneName::Title);
	Scene::g_pSceneManager->regist<GameScene>(SceneName::Game);

	Scene::g_pSceneChanger = Scene::g_pSceneManager;
	Scene::g_pSceneChanger->change(SceneName::Title);
	
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

	#ifdef _DEBUG
	{	// ��CTRL�L�[�Ńf�o�b�O�`��
		debug::g_IsDebug = g_pInput->IsKeyHold(MOFKEY_LCONTROL);
	}
	#endif // _DEBUG

	Scene::g_pSceneManager->update();

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

	Scene::g_pSceneManager->draw();

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

	Scene::g_pSceneManager->release();
	Scene::g_pSceneManager.reset();

	Scene::g_FadeShader.Release();

	// �f�ނ̉��
	CResourceManager::Release();

	return TRUE;
}