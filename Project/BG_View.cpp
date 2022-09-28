#include "BG_View.h"
#include "Manager/ResourceManager.h"
#include "Manager/TextureManager.h"

namespace {
	constexpr float bg_scroll_speed = 0.5f;
}

BG_View::BG_View()
	: m_pBGTexture{ nullptr }
	, m_Scroll{ 0.0f }
{
}

BG_View::~BG_View()
{
	m_pBGTexture = nullptr;
}

void BG_View::update()
{
	m_Scroll += bg_scroll_speed;
}

void BG_View::draw()
{
	if (!m_pBGTexture) return;
	const int h = m_pBGTexture->GetHeight();
	const int w = m_pBGTexture->GetWidth();
	const int sch = g_pGraphics->GetTargetHeight();
	const int scw = g_pGraphics->GetTargetWidth();
	const int scroll = static_cast<int>(m_Scroll) % h;
	const float offset_y = static_cast<float>(scroll - h);
	for (float x = 0; x < scw; x += w)
	{
		for (float y = offset_y; y < sch; y += h)
		{
			m_pBGTexture->Render(x, y);
		}
	}
}

void BG_View::setBGTexture(std::string_view key)
{
	m_pBGTexture = CResourceManager::GetTextureManager()->Get(key).get();
}
