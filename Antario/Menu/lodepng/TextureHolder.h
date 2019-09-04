//#pragma once
//
//class TextureHolder
//{
//public:
//	typedef std::shared_ptr<TextureHolder> Ptr;
//	TextureHolder()
//		: m_pSurface(INTERFACES.GetSurface()), m_iH(0), m_iW(0), m_bgColor(255, 255, 255, 255), m_bValid(false)
//	{ };
//
//	TextureHolder(const unsigned char* pRawRGBAData, uint32 W, uint32 H)
//		: m_pSurface(), m_iH(H), m_iW(W), m_bgColor(255, 255, 255, 255), m_bValid(false)
//	{
//		m_iTexture = m_pSurface->CreateNewTextureID(true);
//		if (!m_iTexture)
//			return;
//		m_pSurface->DrawSetTextureRGBA(m_iTexture, pRawRGBAData, W, H, false, false);
//		m_bValid = true;
//	};
//
//	bool IsValid() const
//	{
//		return m_bValid;
//	};
//
//	int GetTextureId() const
//	{
//		return m_iTexture;
//	};
//
//	bool Draw(int x, int y, float scale = 1.0)
//	{
//		if (!m_pSurface->IsTextureIDValid(m_iTexture))
//			return false;
//		m_pSurface->DrawSetColor(m_bgColor);
//		m_pSurface->DrawSetTexture(m_iTexture);
//		m_pSurface->DrawTexturedRect(x, y, x + m_iW * scale, y + m_iH * scale);
//		return true;
//	};
//
//protected:
//	uint32 m_iTexture;
//	uint32 m_iW, m_iH;
//	Color  m_bgColor;
//	bool   m_bValid;
//
//	vgui::ISurface* m_pSurface;
//};