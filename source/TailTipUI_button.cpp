#include "TailTipUI_button.h"

namespace TailTipUI {

	Button::Button()
		: Text(), textScale(1.0f)
	{

	}

	Button::Button(std::string text)
		: Text(text), textScale(1.0f)
	{
	}

	Button::~Button()
	{
		//we MUST NOT nelete buttonText since thats handled by the child-element system
	}

	void Button::_Render()
	{
		RenderSingleColor(bgcolor, RelativePositionToParent(), renderRadius, radiusParameter, smoothing);
		glm::vec4 tmpPos = pos;
		if(!centered) {
			pos.x =pos.x+(1.0f-textScale)*0.5f*pos[2];
		}
		pos.y =pos.y+(1.0f-textScale)*0.5f*pos[3];
		pos[2] = pos[2]*textScale;
		pos[3] = pos[3]*textScale;
		Text::_Render();
		pos = tmpPos;
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

	void Button::SetTextScale(float s) {
		textScale = s;
	}

	float Button::GetTextScale() {
		return textScale;
	}
}; //namespace TailTipUI
