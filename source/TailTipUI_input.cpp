#include "TailTipUI_input.h"

namespace TailTipUI
{
	Input::Input()
		: bgpos(0.0f),maxChars(0), wasDeleting(false)
	{
	
	}

	Input::~Input()
	{

	}

	void Input::SetPos(glm::vec4 p) {
		Text::SetPos(p);
		bgpos = p;
	}

	glm::vec4 Input::GetPos() {
		return bgpos;
	}
	
	bool Input::GetHover()
	{
		bool ret = false;
		glm::vec4 tmp = pos;
		pos = bgpos;
		ret = GeneralElement::GetHover();
		pos = tmp;
		return ret;
	}
	void Input::_Render()
	{
		glm::vec4 tmppos = pos;
		pos = bgpos;
		RenderSingleColor(bgcolor, RelativePositionToParent(), renderRadius, radiusParameter, smoothing);	
		pos = tmppos;
			
		if (inFocus) {
			std::string b = Info::GetTextBuffer();
			std::string oldName = intext;
			intext+= b;
			name+=b;
			Info::ResetTextBuffer();
			if (name.size() > 0 && specialCallback && Info::GetCurrentButton()[SDL_SCANCODE_RETURN]) {
				specialCallback(this);
				name.clear();
				intext.clear();
				Info::ResetTextBuffer();
			}
			if (name.size() > 0 && Info::GetCurrentButton()[SDL_SCANCODE_BACKSPACE] && !wasDeleting) {
				intext.pop_back();
				//oh thats was not an ascii character 
				if(intext.size()>0 && name[name.size()-1] > 127) {
					intext.pop_back();
				}
				wasDeleting = true;
			}
			else {
				wasDeleting = false;
			}

			std::string textName = intext;
			if (oldName != intext) {
				Text::SetName(textName);
			}

			glm::vec4 textPos = Text::GetPos();
			if (textName.size() > maxChars && textPos.x + textPos[2] > 1.0f){
				if (maxChars != 0 ) {
					//we check agains maxChars+1: if it DOSNT collide, we decrese maxChars by one, otherwise nawt.
					textName = name.substr(name.size() - maxChars-1, maxChars+1);
					Text::SetName(textName);
					textPos = RelativePositionToParent();
					if (!(textPos.x + textPos[2] > 1.0f)) {
						maxChars++;
					}
					else {
						textName = name.substr(name.size() - maxChars, maxChars);
						SetName(textName);
						textPos = RelativePositionToParent();
					}
				}
				if (maxChars == 0  || (textPos.x + textPos[2] > 1.0f)) {
					maxChars = textName.size();
					while (textName.size() > 1 && textPos.x + textPos[2] > 1.0f) {
						textName.erase(textName.begin());
						maxChars--;
						SetName(textName);
						textPos = RelativePositionToParent();
					}
				}
			}
			Info::ResetTextBuffer();
		}
		else {
			wasDeleting = false;
		}
		Text::_Render();	
	}

	void Input::_Focus()
	{
		Info::ResetTextBuffer();
	}

	void Input::_LostFocus()
	{
		name += Info::GetTextBuffer();
		Info::ResetTextBuffer();
	}
};
