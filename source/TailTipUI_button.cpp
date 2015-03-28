#include "TailTipUI_button.h"

namespace TailTipUI {

	Button::Button()
	{
		widthlockText = false;
		buttonText = new Text();
		buttonText->SetWidthLock(widthlockText);
		buttonText->SetPos(glm::vec4(.0f, .0f, 1.f, 1.f));
		AttatchChild(buttonText);
	}

	Button::Button(std::string text)
	{
		widthlockText = true;
		buttonText = new Text(text);
		AttatchChild(buttonText);
	}

	Button::~Button()
	{
		//we MUST NOT nelete buttonText since thats handled by the child-element system
	}

	void Button::_UpdateText()
	{
		buttonText->ForceUpdate();
		glm::vec4 textpos = buttonText->GetPos();
		buttonText->SetPos(glm::vec4(0.f, 0.f, textpos[2], textpos[3]));
	}

	void Button::SetFont(TTF_Font* newfont) 
	{
		Area::SetFont(newfont);
		if (buttonText != nullptr) {
			buttonText->SetFont(newfont);
			glm::vec4 textpos = buttonText->GetPos();
			buttonText->SetPos(glm::vec4(0.5 - textpos[2] / 2.0f, 0.1, textpos[2], textpos[3]));
		}
		
	}

	void Button::SetName(std::string newname)
	{
		Area::SetName(newname);
		if (buttonText != nullptr) {
			buttonText->SetName(newname);
			_UpdateText();
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
			_UpdateText();
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
