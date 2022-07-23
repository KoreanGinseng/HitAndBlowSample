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

// TEST
#include    "DrawNumbers/DrawNumbers.h"
std::unique_ptr<CDrawNumbers> g_pDrawNums = nullptr;

namespace {

	// 画面サイズアクセサ
	constexpr auto screen_w              = 1280.0f;
	constexpr auto screen_h              =  720.0f;
	constexpr auto screen_margin         = screen_h * 0.05f;

	constexpr auto top_margin_h_ratio    = 0.0500f;
	constexpr auto score_board_h_ratio   = 0.6000f;
	constexpr auto object_margin_h_ratio = 0.0125f;
	constexpr auto button_h_ratio        = 0.0750f;

	constexpr auto ingame_w = screen_w - screen_margin * 2.0f;
	constexpr auto ingame_h = screen_h - screen_margin * 2.0f;
	
	const CRectangle ingame_area{ 
		screen_margin, screen_margin,
		screen_w - screen_margin, screen_h - screen_margin
	};
	const CRectangle ingame_half_l_area{
		screen_margin, screen_margin,
		screen_w * 0.5f, screen_h - screen_margin
	};
	const CRectangle ingame_half_r_area{
		screen_w * 0.5f, screen_margin,
		screen_w - screen_margin, screen_h - screen_margin
	};
	const CRectangle score_board_l_area{
		ingame_half_l_area.Left, ingame_half_l_area.Top + ingame_h * top_margin_h_ratio,
		ingame_half_l_area.Right, ingame_half_l_area.Top + ingame_h * top_margin_h_ratio + ingame_h * score_board_h_ratio
	};
	const CRectangle score_board_r_area{
		ingame_half_r_area.Left, ingame_half_r_area.Top + ingame_h * top_margin_h_ratio,
		ingame_half_r_area.Right, ingame_half_r_area.Top + ingame_h * top_margin_h_ratio + ingame_h * score_board_h_ratio
	};
	const CRectangle post_num_area{
		ingame_half_l_area.Left, score_board_l_area.Bottom + ingame_h * object_margin_h_ratio,
		ingame_half_l_area.Right, score_board_l_area.Bottom + ingame_h * (object_margin_h_ratio + button_h_ratio)
	};
	const CRectangle push_num_button_area1{
		post_num_area.Left, post_num_area.Bottom + ingame_h * object_margin_h_ratio,
		post_num_area.Right, post_num_area.Bottom + ingame_h * (object_margin_h_ratio + button_h_ratio)
	};
	const CRectangle push_num_button_area2{
		post_num_area.Left, push_num_button_area1.Bottom + ingame_h * object_margin_h_ratio,
		post_num_area.Right, push_num_button_area1.Bottom + ingame_h * (object_margin_h_ratio + button_h_ratio)
	};
	const CRectangle edit_num_button_area{
		post_num_area.Left, ingame_half_l_area.Bottom - ingame_h * button_h_ratio,
		post_num_area.Right, ingame_half_l_area.Bottom
	};

	//TextureRect
	const CRectangle board_center_src{ 396, 12, 468, 84 };
	const CRectangle board_top_src{ 384, 0, 480, 12 };
	const CRectangle board_bottom_src{ 384, 84, 480, 96 };
	const CRectangle board_left_src{ 384, 0, 396, 96 };
	const CRectangle board_right_src{ 468, 0, 480, 96 };
}

// _DEBUG用
#ifdef _DEBUG
namespace {
	bool g_IsDebug = false;
}
#endif//_DEBUG
	
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
	if (!CResourceManager::GetTextureManager()->Load("Nums.png", "Nums.png")) {
		return FALSE;
	}

	g_pDrawNums.reset(NEW CDrawNumbers());
	g_pDrawNums->SetNum(1234567890);

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
		g_IsDebug = g_pInput->IsKeyHold(MOFKEY_LCONTROL);
	}
	#endif // _DEBUG


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

	// テスト描画
	//g_pDrawNums->Render(0, 0, 0.5f, 0.5f);

	#ifdef _DEBUG
	if (g_IsDebug) {
		// GRID
		{
			CGraphicsUtilities::RenderLine(0, screen_h * 0.5f, screen_w, screen_h * 0.5f, MOF_COLOR_YELLOW);
			CGraphicsUtilities::RenderLine(screen_w * 0.5f, 0, screen_w * 0.5f, screen_h, MOF_COLOR_YELLOW);
		}
		// INGAME
		{
			CGraphicsUtilities::RenderFillRect(ingame_area, MOF_ARGB(128, 128, 0, 128));
			CGraphicsUtilities::RenderFillRect(ingame_half_l_area, MOF_ALPHA_GREEN(128));
			CGraphicsUtilities::RenderFillRect(score_board_l_area, MOF_ALPHA_GREEN(128));
			CGraphicsUtilities::RenderFillRect(post_num_area, MOF_ALPHA_GREEN(128));
			CGraphicsUtilities::RenderFillRect(push_num_button_area1, MOF_ALPHA_GREEN(128));
			CGraphicsUtilities::RenderFillRect(push_num_button_area2, MOF_ALPHA_GREEN(128));
			CGraphicsUtilities::RenderFillRect(edit_num_button_area, MOF_ALPHA_GREEN(128));

			CGraphicsUtilities::RenderFillRect(ingame_half_r_area, MOF_ALPHA_RED(128));
			CGraphicsUtilities::RenderFillRect(score_board_r_area, MOF_ALPHA_RED(128));
		}
	}
	#endif // _DEBUG

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

	// 素材の解放
	CResourceManager::Release();

	return TRUE;
}