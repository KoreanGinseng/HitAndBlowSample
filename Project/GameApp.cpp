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

namespace {

	// ��ʃT�C�Y�A�N�Z�T
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

// _DEBUG�p
#ifdef _DEBUG
namespace {
	bool g_IsDebug = false;
}
#endif//_DEBUG
	
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

	#ifdef _DEBUG
	{	// ��CTRL�L�[�Ńf�o�b�O�`��
		g_IsDebug = g_pInput->IsKeyHold(MOFKEY_LCONTROL);
	}
	#endif // _DEBUG


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