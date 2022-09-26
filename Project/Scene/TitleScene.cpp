#include "TitleScene.h"
#include <Mof.h>

// MANAGER
#include    "../Manager/ResourceManager.h"
#include    "../Manager/TextureManager.h"
#include    "../SceneManager/ISceneChanger.h"

#include    "../SceneChangeEffect/FadeShader.h"
#include    "../SceneChangeEffect/SceneChangeEffectFade.h"
#include    "SceneDefine.h"

// _DEBUG—p
#ifdef _DEBUG
namespace debug {
	extern bool g_IsDebug;
}
#endif//_DEBUG

namespace Scene {
	extern sip::SceneChangerPtr g_pSceneChanger;
	extern RuleFade::CFadeShader g_FadeShader;
}

namespace {

}

TitleScene::TitleScene() 
{
}

TitleScene::~TitleScene() 
{
}

void TitleScene::init() 
{
}

void TitleScene::update() 
{
	if (Scene::g_pSceneChanger->isChange())
	{
		return;
	}
	if (g_pInput->IsKeyPull(MOFKEY_RETURN))
	{
		//InGameInit();
		Scene::g_pSceneChanger->change(
			SceneName::Game,
			std::make_unique<sip::SceneChangeEffectFade>(&Scene::g_FadeShader, 2.0f)
		);
	}
}

void TitleScene::draw() 
{
	CGraphicsUtilities::RenderString(0, 0, "title");
}

void TitleScene::release() 
{
}