//#include "UIAlert.h"
//#include "..\Utils\GlobalVars.h"
//#include "..\Utils\Interfaces.h"
//#include "..\Hooks.h"
//#include "..\Utils\Utils.h"
//
//namespace alert
//{
//	CCSGO_HudUniqueAlerts* m_element = nullptr;
//
//	std::string_view m_last_text = "";
//
//	float m_last_push = std::numeric_limits<float>::max();
//	float m_timeout = 5.f;
//
//	void setup_element()
//	{
//		if (!m_element)
//			m_element = csgo::hud()->FindElement<CCSGO_HudUniqueAlerts>("CCSGO_HudUniqueAlerts");
//	}
//
//	void check()
//	{
//		if (g_pGlobalVars->curtime > m_last_push + m_timeout)
//		{
//			m_element->GetPanel2D()->HidePanel();
//
//			m_last_push = std::numeric_limits<float>::max();
//		}
//	}
//
//	void push(const std::string_view text)
//	{
//		setup_element();
//
//		m_last_text = text;
//
//		m_element->GetPanel2D()->ShowAlert(m_last_text.data(), PANEL_SHOW);
//
//		m_last_push = g_pGlobalVars->curtime;
//	}
//}