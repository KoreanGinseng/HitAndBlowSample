#include "TitleScene.h"
#include <Mof.h>

// MANAGER
#include    "../Manager/ResourceManager.h"
#include    "../Manager/TextureManager.h"
#include    "../SceneManager/ISceneChanger.h"

#include    "../SceneChangeEffect/FadeShader.h"
#include    "../SceneChangeEffect/SceneChangeEffectFade.h"
#include    "SceneDefine.h"

// _DEBUG用
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
	constexpr float play_button_size_w = 200.0f;
	constexpr float play_button_size_h =  50.0f;
	constexpr float exit_button_size_w = 200.0f;
	constexpr float exit_button_size_h =  50.0f;
	constexpr float exit_button_offset = 150.0f;
	const CRectangle play_button_rect{ 
		(screen_w - play_button_size_w) * 0.5f,
		(screen_h - play_button_size_h) * 0.5f,
		(screen_w + play_button_size_w) * 0.5f,
		(screen_h + play_button_size_h) * 0.5f
	};
	const CRectangle exit_button_rect{
		(screen_w - play_button_size_w) * 0.5f,
		(screen_h - play_button_size_h) * 0.5f + exit_button_offset,
		(screen_w + play_button_size_w) * 0.5f,
		(screen_h + play_button_size_h) * 0.5f + exit_button_offset
	};
}

TitleScene::TitleScene() 
	: m_BG_View()
{
}

TitleScene::~TitleScene() 
{
}

void TitleScene::init() 
{
	m_BG_View.setBGTexture("BG_Title");
}

void TitleScene::update() 
{
	m_BG_View.update();
	if (Scene::g_pSceneChanger->isChange())
	{
		return;
	}
	Vector2 mp;
	g_pInput->GetMousePos(mp);
	if (g_pInput->IsMouseKeyPull(MOFMOUSE_LBUTTON) && play_button_rect.CollisionPoint(mp))
	{
		Scene::g_pSceneChanger->change(
			SceneName::Game,
			std::make_unique<sip::SceneChangeEffectFade>(&Scene::g_FadeShader, 1.0f)
		);
	}
	if (g_pInput->IsMouseKeyPull(MOFMOUSE_LBUTTON) && exit_button_rect.CollisionPoint(mp))
	{
		PostQuitMessage(0);
	}
}

void TitleScene::draw() 
{
	m_BG_View.draw();
	auto pLogo = CResourceManager::GetTextureManager()->Get("Title_Logo").get();
	if (pLogo)
	{
		pLogo->Render(screen_w * 0.5f, pLogo->GetHeight() * 1.5f, TEXALIGN_CENTERCENTER);
	}
	Vector2 mp;
	g_pInput->GetMousePos(mp);
	MofU32 play_top_color    = MOF_COLOR_CWHITE;
	MofU32 play_bottom_color = MOF_COLOR_CBLACK;
	Mof::Rectangle calc_rect;
	bool is_play_mouse_over = play_button_rect.CollisionPoint(mp);
	if (is_play_mouse_over && g_pInput->IsMouseKeyHold(MOFMOUSE_LBUTTON))
	{
		std::swap(play_top_color, play_bottom_color);
	}
	CGraphicsUtilities::CalculateStringRect(0, 0, "プレイ", calc_rect);
	CGraphicsUtilities::RenderFillRect(play_button_rect, play_top_color, play_bottom_color, play_top_color, play_bottom_color);
	CGraphicsUtilities::RenderRect(play_button_rect, (is_play_mouse_over ? MOF_COLOR_HBLUE : MOF_COLOR_HBLACK));
	CGraphicsUtilities::RenderString(
		(screen_w - calc_rect.Right ) * 0.5f,
		(screen_h - calc_rect.Bottom) * 0.5f,
		MOF_COLOR_BLACK,
		"プレイ"
	);
	MofU32 exit_top_color    = MOF_COLOR_CWHITE;
	MofU32 exit_bottom_color = MOF_COLOR_CBLACK;
	bool is_exit_mouse_over = exit_button_rect.CollisionPoint(mp);
	if (is_exit_mouse_over && g_pInput->IsMouseKeyHold(MOFMOUSE_LBUTTON))
	{
		std::swap(exit_top_color, exit_bottom_color);
	}
	CGraphicsUtilities::CalculateStringRect(0, 0, "終了", calc_rect);
	CGraphicsUtilities::RenderFillRect(exit_button_rect, exit_top_color, exit_bottom_color, exit_top_color, exit_bottom_color);
	CGraphicsUtilities::RenderRect(exit_button_rect, (is_exit_mouse_over ? MOF_COLOR_HBLUE : MOF_COLOR_HBLACK));
	CGraphicsUtilities::RenderString(
		(screen_w - calc_rect.Right ) * 0.5f,
		(screen_h - calc_rect.Bottom) * 0.5f + exit_button_offset,
		MOF_COLOR_BLACK,
		"終了"
	);
}

void TitleScene::release() 
{
}