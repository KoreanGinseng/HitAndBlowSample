/*************************************************************************//*!

					@file	GameApp.cpp
					@brief	基本ゲームアプリ。

															@author	濱田　享
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

// _DEBUG用
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
		@brief			アプリケーションの初期化
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void) {

	// 素材フォルダに指定
	CUtilities::SetCurrentDirectory("Resource");

	// 画像の読み込み
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
		@brief			アプリケーションの更新
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Update(void) {
	//キーの更新
	g_pInput->RefreshKey();

	#ifdef _DEBUG
	{	// 左CTRLキーでデバッグ描画
		debug::g_IsDebug = g_pInput->IsKeyHold(MOFKEY_LCONTROL);
	}
	#endif // _DEBUG

	Scene::g_pSceneManager->update();

	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの描画
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Render(void) {
	//描画開始
	g_pGraphics->RenderStart();
	//画面のクリア
	g_pGraphics->ClearTarget(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0);

	Scene::g_pSceneManager->draw();

	//描画の終了
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの解放
		@param			None

		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Release(void) {

	Scene::g_pSceneManager->release();
	Scene::g_pSceneManager.reset();

	Scene::g_FadeShader.Release();

	// 素材の解放
	CResourceManager::Release();

	return TRUE;
}