#include "TailTipUI_button.h"

namespace TailTipUI {

	Button::Button()
	{
		widthlockText = true;
		buttonText = new Text();
		buttonText->SetWidthLock(widthlockText);
		AttatchChild(buttonText);
	}

	Button::Button(std::string text)
	{
		widthlockText = true;
		buttonText = new Text(text);
		AttatchChild(buttonText);
		glm::vec4 textpos = buttonText->GetPos();
	}

	Button::~Button()
	{
		//we MUST NOT nelete buttonText since thats handled by the child-element system
	}
	void Button::SetFont(TTF_Font* newfont) 
	{
		Area::SetFont(newfont);
		if (buttonText != nullptr) {
			buttonText->SetFont(newfont);
			glm::vec4 textpos = buttonText->GetPos();
			buttonText->SetPos(glm::vec4(0.5 - textpos[2] / 2.0f, 0.5 - textpos[3] / 2.0f, textpos[2], textpos[3]));
		}
		
	}

	void Button::SetName(std::string newname)
	{
		Area::SetName(newname);
		if (buttonText != nullptr) {
			buttonText->SetName(newname);
			glm::vec4 textpos = buttonText->GetPos();
			buttonText->SetPos(glm::vec4(0.5 - textpos[2] / 2.0f, 0.5 - textpos[3] / 2.0f, textpos[2], textpos[3]));
		}
	}

	void Button::SetId(std::string id)
	{
		Area::SetId(id);

		if (buttonText != nullptr) {
			buttonText->SetId(id + "_text");
		}
	}

	void Button::SetPos(glm::vec4 newpos)
	{
		Area::SetPos(newpos);

		if (buttonText != nullptr) {
			buttonText->SetPos(glm::vec4(0,0,.9f, .9f));
			buttonText->ForceUpdate();
			glm::vec4 tPos = buttonText->GetPos();
			buttonText->SetPos(glm::vec4(.5f - tPos[2] / 2.f, .5f - tPos[3] / 2.f, tPos[2], tPos[3]));
		}
	}

	void Button::SetForgroundColor(glm::vec4 color)
	{
		Area::SetForgroundColor(color);
		if (buttonText != nullptr) {
			buttonText->SetForgroundColor(color);
		}
	}

	void Button::SetTextWidthlock(bool b)
	{
		widthlockText = b;
	}

	bool Button::GetTextWidthlock()
	{
		return widthlockText;
	}

	void Button::_InternalHoverEvent()
	{
		if (usesEventColor) {
			glm::vec4 tmpColor = bgcolor;
			bgcolor = eventColor;
			eventColor = bgcolor;
		}
	}

	void Button::_InternalStopHoverEvent()
	{
		if (usesEventColor) {
			glm::vec4 tmpColor = bgcolor;
			bgcolor = eventColor;
			eventColor = bgcolor;
		}

	}
}; //namespace TailTipUI