#include "GameScene.h"
#include <array>
#include <bitset>
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
#define ENUM_TO_INT8(x) (static_cast<std::int8_t>(x))
#define ENUM_TO_INT32(x) (static_cast<std::int32_t>(x))
#define ENUM_TO_UINT8(x) (static_cast<std::uint8_t>(x))
#define ENUM_TO_UINT32(x) (static_cast<std::uint32_t>(x))

	enum class InGameStep {
		Select,
		Post,
		Check,
		Result,
		Win,

		MAX
	};
	constexpr std::int32_t ingame_step_max = ENUM_TO_INT32(InGameStep::MAX);

	enum class InputNum {
		Zero,
		One,
		Two,
		Three,
		Four,
		Five,
		Six,
		Seven,
		Eight,
		Nine,

		MAX
	};
	constexpr std::int32_t input_num_max = ENUM_TO_INT32(InputNum::MAX);

	enum class PostIndex {
		Zero,
		One,
		Two,

		MAX
	};
	constexpr std::int32_t post_index_max = ENUM_TO_INT32(PostIndex::MAX);

	enum class EditButton {
		Clear,
		Auto,
		Post,

		MAX
	};
	constexpr std::int32_t edit_button_max = ENUM_TO_INT32(EditButton::MAX);

	constexpr const char* edit_button_strs[edit_button_max] = {
		"クリア",
		"自動",
		"決定",
	};

	constexpr auto screen_margin = screen_h * 0.05f;

	constexpr auto top_margin_h_ratio = 0.0500f;
	constexpr auto score_board_h_ratio = 0.6000f;
	constexpr auto object_margin_h_ratio = 0.0125f;
	constexpr auto button_h_ratio = 0.0750f;

	constexpr auto ingame_w = screen_w - screen_margin * 2.0f;
	constexpr auto ingame_h = screen_h - screen_margin * 2.0f;

	constexpr auto draw_line_margin = 30.0f;

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

	const CRectangle num_input_button_rects[input_num_max] = {
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
	const CRectangle post_num_rects[post_index_max] = {
		{ post_num_offsetx + post_num_area.Left + (post_num_area_margin + post_num_size) * 0, post_num_area.Top, post_num_offsetx + post_num_area.Left + (post_num_area_margin + post_num_size) * 0 + post_num_size, post_num_area.Bottom },
		{ post_num_offsetx + post_num_area.Left + (post_num_area_margin + post_num_size) * 1, post_num_area.Top, post_num_offsetx + post_num_area.Left + (post_num_area_margin + post_num_size) * 1 + post_num_size, post_num_area.Bottom },
		{ post_num_offsetx + post_num_area.Left + (post_num_area_margin + post_num_size) * 2, post_num_area.Top, post_num_offsetx + post_num_area.Left + (post_num_area_margin + post_num_size) * 2 + post_num_size, post_num_area.Bottom },
	};

	const float edit_num_button_w = edit_num_button_area.GetWidth() / 3.0f;
	const CRectangle edit_num_button_rects[edit_button_max] = {
		{ edit_num_button_area.Left + edit_num_button_w * 0, edit_num_button_area.Top, edit_num_button_area.Left + edit_num_button_w * 1, edit_num_button_area.Bottom },
		{ edit_num_button_area.Left + edit_num_button_w * 1, edit_num_button_area.Top, edit_num_button_area.Left + edit_num_button_w * 2, edit_num_button_area.Bottom },
		{ edit_num_button_area.Left + edit_num_button_w * 2, edit_num_button_area.Top, edit_num_button_area.Left + edit_num_button_w * 3, edit_num_button_area.Bottom }
	};
	const float return_button_w = 200.0f;
	const float return_button_h =  50.0f;
	const CRectangle return_button_rect{
		(screen_w - return_button_w) * 0.5f,
		(screen_h - return_button_h) * 0.5f,
		(screen_w + return_button_w) * 0.5f,
		(screen_h + return_button_h) * 0.5f
	};

	struct ResultData {
		int post_nums[post_index_max]{ -1, -1, -1 };
		int hit{ 0 };
		int blow{ 0 };
	};
}

namespace {
	std::bitset<ENUM_TO_UINT32(InputNum::MAX)> memo_num{ "1111111111" };
	int enter_nums[post_index_max] = { -1, -1, -1 };
	int post_numbers[post_index_max] = { -1, -1, -1 };
	int answer_numbers[post_index_max] = { -1, -1, -1 };
	int post_select_cursor = 0;
	float draw_score_board_offset = 0.0f;

	InGameStep ingame_step = InGameStep::Select;
	std::array<std::function<void(void)>, ingame_step_max> ingame_step_functions;
	std::array<std::function<void(void)>, edit_button_max> edit_button_functions;
	std::vector<ResultData> result_datas;
}

namespace {
	void ClearButtonExec() {
		for (auto& num : post_numbers) {
			num = -1;
		}
		post_select_cursor = 0;
	}

	void AutoButtonExec() {
		std::vector<int> empty_index = { 0, 1, 2 };
		for (int i = 0; i < post_index_max; i++) {
			if (enter_nums[i] < 0) {
				continue;
			}
			post_numbers[i] = enter_nums[i];
			std::remove_if(empty_index.begin(), empty_index.end(), [&](int n) { return i == n; });
		}
		if (empty_index.size() == 0) {
			return;
		}
		std::vector<int> nums;
		nums.reserve(input_num_max);
		auto uinput_num_max = memo_num.size();
		for (std::size_t i = 0; i < uinput_num_max; i++) {
			if (!memo_num.test(i)) {
				continue;
			}
			nums.emplace_back(i);
		}
		while (empty_index.size()) {
			auto post_index = CUtilities::Random(empty_index.size());
			auto num_index = CUtilities::Random(nums.size());
			post_numbers[empty_index[post_index]] = nums[num_index];
			empty_index.erase(empty_index.begin() + post_index);
			nums.erase(nums.begin() + num_index);
		}
	}

	void PostButtonExec() {
		for (int i = 0; i < post_index_max; i++) {
			if (post_numbers[i] < 0) {
				return;
			}
		}
		post_select_cursor = -1;
		ingame_step = InGameStep::Post;
	}

	void InGameInit() {
		memo_num.set();
		enter_nums[0] = enter_nums[1] = enter_nums[2] = -1;
		post_numbers[0] = post_numbers[1] = post_numbers[2] = -1;
		answer_numbers[0] = CUtilities::Random(0, 3);
		answer_numbers[1] = CUtilities::Random(3, 6);
		answer_numbers[2] = CUtilities::Random(6, 10);
		post_select_cursor = 0;
		ingame_step = InGameStep::Select;
		result_datas.clear();
	}
}

namespace {
	void SelectStep() {
		Vector2 mp;
		g_pInput->GetMousePos(mp);
		if (g_pInput->IsMouseKeyPull(MOFMOUSE_LBUTTON)) {
			for (int i = 0; i < post_index_max; i++) {
				if (post_num_rects[i].CollisionPoint(mp)) {
					post_select_cursor = i;
					break;
				}
			}
			for (int i = 0; i < input_num_max; i++) {
				if (num_input_button_rects[i].CollisionPoint(mp)) {
					for (int j = 0; j < post_index_max; j++) {
						if (post_numbers[j] == i) {
							post_numbers[j] = -1;
							break;
						}
					}
					post_numbers[post_select_cursor] = i;
					post_select_cursor = (post_select_cursor + 1) % post_index_max;
					break;
				}
			}
			for (int i = 0; i < edit_button_max; i++) {
				if (edit_num_button_rects[i].CollisionPoint(mp)) {
					edit_button_functions[i]();
					break;
				}
			}
		}
	}
	void PostStep() {
		ResultData post;
		for (int i = 0; i < post_index_max; i++) {
			post.post_nums[i] = post_numbers[i];
		}
		result_datas.push_back(post);

		const auto& result_num_count = result_datas.size();
		const float draw_result_bottom = score_board_l_area.Top + 30 * result_num_count;
		if (score_board_l_area.Bottom <= draw_result_bottom) {
			draw_score_board_offset = draw_result_bottom - score_board_l_area.Bottom;
		}
		ingame_step = InGameStep::Check;
	}
	void CheckStep() {
		auto result = result_datas.rbegin();
		for (int i = 0; i < post_index_max; i++) {
			for (int j = 0; j < post_index_max; j++) {
				if (i == j) {
					if (result->post_nums[i] == answer_numbers[j]) {
						result->hit++;
					}
				}
				else {
					if (result->post_nums[i] == answer_numbers[j]) {
						result->blow++;
					}
				}
			}
		}
		ingame_step = InGameStep::Result;
	}
	void ResultStep() {
		auto result = result_datas.rbegin();
		if (result->hit == 3) {
			ingame_step = InGameStep::Win;
			return;
		}
		edit_button_functions[ENUM_TO_INT32(EditButton::Clear)]();
		if (result->hit == 0 && result->blow == 0) {
			for (const auto& num : result->post_nums) {
				memo_num.reset(num);
			}
		}
		if ((result->hit + result->blow) == 3) {
			for (int i = 0; i < input_num_max; i++) {
				if (i != result->post_nums[0]
					&& i != result->post_nums[1]
					&& i != result->post_nums[2]) {
					memo_num.reset(i);
				}
			}
		}
		ingame_step = InGameStep::Select;
	}
	void WinStep() {
		Vector2 mp;
		g_pInput->GetMousePos(mp);
		if (return_button_rect.CollisionPoint(mp) && g_pInput->IsMouseKeyPull(MOFMOUSE_LBUTTON)) {
			Scene::g_pSceneChanger->change(
				SceneName::Title,
				std::make_unique<sip::SceneChangeEffectFade>(&Scene::g_FadeShader, 1.0f)
			);
		}
	}
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::init()
{
	// ゲームの初期化
	InGameInit();

	// ステップ関数の登録
	ingame_step_functions[ENUM_TO_INT32(InGameStep::Select)] = SelectStep;
	ingame_step_functions[ENUM_TO_INT32(InGameStep::Post)] = PostStep;
	ingame_step_functions[ENUM_TO_INT32(InGameStep::Check)] = CheckStep;
	ingame_step_functions[ENUM_TO_INT32(InGameStep::Result)] = ResultStep;
	ingame_step_functions[ENUM_TO_INT32(InGameStep::Win)] = WinStep;

	// ボタン関数の登録
	edit_button_functions[ENUM_TO_INT32(EditButton::Clear)] = ClearButtonExec;
	edit_button_functions[ENUM_TO_INT32(EditButton::Auto)] = AutoButtonExec;
	edit_button_functions[ENUM_TO_INT32(EditButton::Post)] = PostButtonExec;
}

void GameScene::update()
{
	if (Scene::g_pSceneChanger->isChange())
	{
		return;
	}
	// ステップに対応した関数を実行
	ingame_step_functions[ENUM_TO_INT32(ingame_step)]();

	// スコアボード範囲内スクロール
	Vector2 mousePos; g_pInput->GetMousePos(mousePos);
	const float wheel = g_pInput->GetMouseWheelMove();
	if (score_board_l_area.CollisionPoint(mousePos) && wheel != 0.0f)
	{
		const auto& result_num_count = result_datas.size();
		const float draw_result_max = draw_line_margin * result_num_count;
		if (score_board_l_area.GetHeight() <= draw_result_max) {
			draw_score_board_offset -= wheel * 0.125f;
			draw_score_board_offset = MOF_CLIPING(draw_score_board_offset, 0.0f, draw_result_max - score_board_l_area.GetHeight());
		}
	}
}

void GameScene::draw()
{
	g_pGraphics->SetStencilEnable(TRUE);
	g_pGraphics->SetStencilValue(1);
	g_pGraphics->SetStencilControl(COMPARISON_ALWAYS, STENCIL_REPLACE, STENCIL_REPLACE, STENCIL_REPLACE);
	auto score_board_rect = score_board_l_area;
	score_board_rect.Expansion(-score_board_l_area.GetWidth() * 0.05f, 0);
	CGraphicsUtilities::RenderFillRect(score_board_rect, MOF_COLOR_HWHITE);
	auto& tex = CResourceManager::GetTextureManager()->Get("GUI_4x.png");
	tex->Render(score_board_rect, board_center_src);

	g_pGraphics->SetStencilControl(COMPARISON_EQUAL, STENCIL_KEEP, STENCIL_KEEP, STENCIL_KEEP);
	const auto& result_num_count = result_datas.size();
	for (int i = 0; i < result_num_count; i++) {
		const auto& result = result_datas[i];
		CGraphicsUtilities::RenderString(
			score_board_rect.Left,
			score_board_rect.Top + draw_line_margin * i - draw_score_board_offset, MOF_COLOR_BLACK,
			"%4d | %d %d %d | %d | %d",
			i + 1, result.post_nums[0], result.post_nums[1], result.post_nums[2],
			result.hit, result.blow
		);
	}
	g_pGraphics->SetStencilEnable(FALSE);

#ifdef _DEBUG
	{
		if (debug::g_IsDebug) {
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

	for (int i = 0; i < input_num_max; i++) {
		const auto& rect = num_input_button_rects[i];
		CGraphicsUtilities::RenderFillRect(rect, MOF_COLOR_HWHITE);
		CGraphicsUtilities::RenderString(rect.Left, rect.Top, "%d", i);
	}
	for (int i = 0; i < post_index_max; i++) {
		const auto& rect = post_num_rects[i];
		CGraphicsUtilities::RenderFillRect(rect, MOF_COLOR_HWHITE);
		if (post_numbers[i] >= 0) {
			CGraphicsUtilities::RenderString(rect.Left, rect.Top, "%d", post_numbers[i]);
		}
		if (i == post_select_cursor) {
			CGraphicsUtilities::RenderRect(rect, MOF_COLOR_RED);
		}
	}
	for (int i = 0; i < edit_button_max; i++) {
		const auto& rect = edit_num_button_rects[i];
		CGraphicsUtilities::RenderFillRect(rect, MOF_COLOR_HWHITE);
		CRectangle str_rect;
		CGraphicsUtilities::CalculateStringRect(0, 0, edit_button_strs[i], str_rect);
		CGraphicsUtilities::RenderString(
			rect.Left + (rect.GetWidth() - str_rect.GetWidth()) * 0.5f,
			rect.Top + (rect.GetHeight() - str_rect.GetHeight()) * 0.5f,
			edit_button_strs[i]
		);
		CGraphicsUtilities::RenderRect(rect, MOF_COLOR_BLACK);
	}

	if (ingame_step == InGameStep::Win) {
		Vector2 mp;
		g_pInput->GetMousePos(mp);
		bool is_mouse_over = return_button_rect.CollisionPoint(mp);
		Mof::Rectangle calc_rect;
		CGraphicsUtilities::CalculateStringRect(0, 0, "タイトルへ", calc_rect);
		CGraphicsUtilities::RenderFillRect(0, 0, screen_w, screen_h, MOF_ALPHA_HWHITE(128));
		CGraphicsUtilities::RenderString(screen_w * 0.45f, screen_h * 0.3f, "%d手で勝利！", result_datas.size());
		MofU32 top_color    = is_mouse_over ? MOF_COLOR_CBLACK : MOF_COLOR_CWHITE;
		MofU32 bottom_color = is_mouse_over ? MOF_COLOR_CWHITE : MOF_COLOR_CBLACK;
		CGraphicsUtilities::RenderFillRect(return_button_rect, top_color, bottom_color, top_color, bottom_color);
		CGraphicsUtilities::RenderRect(return_button_rect, (is_mouse_over ? MOF_COLOR_HBLUE : MOF_COLOR_HWHITE));
		CGraphicsUtilities::RenderString(
			(screen_w - calc_rect.Right ) * 0.5f,
			(screen_h - calc_rect.Bottom) * 0.5f,
			MOF_COLOR_BLACK,
			"タイトルへ"
		);
	}
}

void GameScene::release()
{
}

