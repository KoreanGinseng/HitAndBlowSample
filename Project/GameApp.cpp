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
	const CRectangle board_src{ 384, 0, 480, 96 };
	const CRectangle board_center_src{ 396, 12, 468, 84 };
	const CRectangle board_top_src{ 384, 0, 480, 12 };
	const CRectangle board_bottom_src{ 384, 84, 480, 96 };
	const CRectangle board_left_src{ 384, 0, 396, 96 };
	const CRectangle board_right_src{ 468, 0, 480, 96 };

	const float input_button_w = push_num_button_area1.GetHeight() * (4.0f / 3.0f);
	const float input_button_area_margin = push_num_button_area2.Top - push_num_button_area1.Bottom;
	const float input_button_offsetx = (ingame_half_l_area.GetWidth() - input_button_area_margin * 4 - input_button_w * 5) * 0.5f;

	const CRectangle num_input_button_rects[10] = {
		{ push_num_button_area1.Left + (input_button_area_margin + input_button_w) * 0 + input_button_offsetx, push_num_button_area1.Top, push_num_button_area1.Left + input_button_w * 1 + input_button_area_margin * 0 + input_button_offsetx, push_num_button_area1.Bottom },
		{ push_num_button_area1.Left + (input_button_area_margin + input_button_w) * 1 + input_button_offsetx, push_num_button_area1.Top, push_num_button_area1.Left + input_button_w * 2 + input_button_area_margin * 1 + input_button_offsetx, push_num_button_area1.Bottom },
		{ push_num_button_area1.Left + (input_button_area_margin + input_button_w) * 2 + input_button_offsetx, push_num_button_area1.Top, push_num_button_area1.Left + input_button_w * 3 + input_button_area_margin * 2 + input_button_offsetx, push_num_button_area1.Bottom },
		{ push_num_button_area1.Left + (input_button_area_margin + input_button_w) * 3 + input_button_offsetx, push_num_button_area1.Top, push_num_button_area1.Left + input_button_w * 4 + input_button_area_margin * 3 + input_button_offsetx, push_num_button_area1.Bottom },
		{ push_num_button_area1.Left + (input_button_area_margin + input_button_w) * 4 + input_button_offsetx, push_num_button_area1.Top, push_num_button_area1.Left + input_button_w * 5 + input_button_area_margin * 4 + input_button_offsetx, push_num_button_area1.Bottom },
		{ push_num_button_area2.Left + (input_button_area_margin + input_button_w) * 0 + input_button_offsetx, push_num_button_area2.Top, push_num_button_area2.Left + input_button_w * 1 + input_button_area_margin * 0 + input_button_offsetx, push_num_button_area2.Bottom },
		{ push_num_button_area2.Left + (input_button_area_margin + input_button_w) * 1 + input_button_offsetx, push_num_button_area2.Top, push_num_button_area2.Left + input_button_w * 2 + input_button_area_margin * 1 + input_button_offsetx, push_num_button_area2.Bottom },
		{ push_num_button_area2.Left + (input_button_area_margin + input_button_w) * 2 + input_button_offsetx, push_num_button_area2.Top, push_num_button_area2.Left + input_button_w * 3 + input_button_area_margin * 2 + input_button_offsetx, push_num_button_area2.Bottom },
		{ push_num_button_area2.Left + (input_button_area_margin + input_button_w) * 3 + input_button_offsetx, push_num_button_area2.Top, push_num_button_area2.Left + input_button_w * 4 + input_button_area_margin * 3 + input_button_offsetx, push_num_button_area2.Bottom },
		{ push_num_button_area2.Left + (input_button_area_margin + input_button_w) * 4 + input_button_offsetx, push_num_button_area2.Top, push_num_button_area2.Left + input_button_w * 5 + input_button_area_margin * 4 + input_button_offsetx, push_num_button_area2.Bottom }
	};

	const float post_num_size = post_num_area.GetHeight() * (16.0f / 9.0f);
	const float post_num_area_margin = input_button_area_margin;
	const float post_num_offsetx = (ingame_half_l_area.GetWidth() - post_num_area_margin * 2 - post_num_size * 3) * 0.5f;
	const CRectangle post_num_rects[3] = {
		{ post_num_offsetx + post_num_area.Left + (post_num_area_margin + post_num_size) * 0, post_num_area.Top, post_num_offsetx + post_num_area.Left +(post_num_area_margin + post_num_size) * 0 + post_num_size, post_num_area.Bottom },
		{ post_num_offsetx + post_num_area.Left + (post_num_area_margin + post_num_size) * 1, post_num_area.Top, post_num_offsetx + post_num_area.Left +(post_num_area_margin + post_num_size) * 1 + post_num_size, post_num_area.Bottom },
		{ post_num_offsetx + post_num_area.Left + (post_num_area_margin + post_num_size) * 2, post_num_area.Top, post_num_offsetx + post_num_area.Left +(post_num_area_margin + post_num_size) * 2 + post_num_size, post_num_area.Bottom },
	};
}

namespace {
	int post_numbers[3] = { -1, -1, -1 };
	int post_select_cursor = 0;
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
	{
		if (!CResourceManager::GetTextureManager()->Load("Nums.png", "Nums.png")) {
			return FALSE;
		}
		if (!CResourceManager::GetTextureManager()->Load("Cryos_Mini_GUI/GUI/GUI_4x.png", "GUI_4x.png")) {
			return FALSE;
		}
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

	Vector2 mp;
	g_pInput->GetMousePos(mp);
	for (int i = 0; i < 3; i++) {
		if (post_num_rects[i].CollisionPoint(mp) && g_pInput->IsMouseKeyPull(MOFMOUSE_LBUTTON)) {
			post_select_cursor = i;
			break;
		}
	}
	for (int i = 0; i < 10; i++) {
		if (::num_input_button_rects[i].CollisionPoint(mp) && g_pInput->IsMouseKeyPull(MOFMOUSE_LBUTTON)) {
			for (int j = 0; j < 3; j++) {
				if (post_numbers[j] == i) {
					post_numbers[j] = -1;
					break;
				}
			}
			post_numbers[post_select_cursor] = i;
			post_select_cursor = (post_select_cursor + 1) % 3;
			break;
		}
	}


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
	{
		auto score_board_rect = score_board_l_area;
		score_board_rect.Expansion(-score_board_l_area.GetWidth() * 0.05f, 0);
		CGraphicsUtilities::RenderFillRect(score_board_rect, MOF_COLOR_HWHITE);
		auto& tex = CResourceManager::GetTextureManager()->Get("GUI_4x.png");
		tex->Render(score_board_rect, board_center_src);
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
	}
	#endif // _DEBUG
	
	for (int i = 0; i < 10; i++) {
		const auto& rect = num_input_button_rects[i];
		CGraphicsUtilities::RenderFillRect(rect, MOF_COLOR_HWHITE);
		CGraphicsUtilities::RenderString(rect.Left, rect.Top, "%d", i);
	}
	for (int i = 0; i < 3; i++) {
		const auto& rect = post_num_rects[i];
		CGraphicsUtilities::RenderFillRect(rect, MOF_COLOR_HWHITE);
		if (post_numbers[i] >= 0) {
			CGraphicsUtilities::RenderString(rect.Left, rect.Top, "%d", post_numbers[i]);
		}
		if (i == post_select_cursor) {
			CGraphicsUtilities::RenderRect(rect, MOF_COLOR_RED);
		}
	}

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