#include "TailTipUI_input.h"

namespace TailTipUI
{
	Input::Input()
		: maxChars(0), wasDeleting(false)
	{
		InputText = new Text();
		InputText->SetPos(glm::vec4(0.05, 0.1, .0f, .8f));
		AttatchChild(InputText);
	}

	Input::~Input()
	{

	}

	void Input::SetFont(TTF_Font* newfont)
	{
		Area::SetFont(newfont);
		if (InputText != nullptr) {
			InputText->SetFont(newfont);
		}

	}

	void Input::SetName(std::string newname)
	{
		Area::SetName(newname);
		if (InputText != nullptr) {
			InputText->SetName(newname);
		}
	}

	void Input::SetId(std::string id)
	{
		Area::SetId(id);

		if (InputText != nullptr) {
			InputText->SetId(id + "_text");
		}
	}


	void Input::SetForgroundColor(glm::vec4 color)
	{
		Area::SetForgroundColor(color);
		if (InputText != nullptr) {
			InputText->SetForgroundColor(color);
		}
	}

	void Input::_Render()
	{
		Area::_Render();
		if (inFocus) {
			std::string b = Info::GetTextBuffer();
			std::string oldName = name;
			name+= b;

			if (name.size() > 0 && specialCallback && Info::GetCurrentButton()[SDL_SCANCODE_RETURN]) {
				specialCallback(this);
				name.clear();
				Info::ResetTextBuffer();
			}
			if (name.size() > 0 && Info::GetCurrentButton()[SDL_SCANCODE_BACKSPACE] && !wasDeleting) {
				name.pop_back();
				//oh thats was not an ascii character 
				if(name.size()>0 && name[name.size()-1] > 127) {
					name.pop_back();
				}
				wasDeleting = true;
			}
			else {
				wasDeleting = false;
			}

			std::string textName = name;
			if (oldName != name) {
				InputText->SetName(textName);
			}

			glm::vec4 textPos = InputText->RelativePositionToParent();
			glm::vec4 ownPos = RelativePositionToParent();
			if (textName.size() > maxChars && textPos.x + textPos[2] > ownPos.x + ownPos[2]){
				if (maxChars != 0 ) {
					//we check agains maxChars+1: if it DOSNT collide, we decrese maxChars by one, otherwise nawt.
					textName = name.substr(name.size() - maxChars-1, maxChars+1);
					InputText->SetName(textName);
					textPos = InputText->RelativePositionToParent();
					if (!(textPos.x + textPos[2] > ownPos.x + ownPos[2])) {
						maxChars++;
					}
					else {
						textName = name.substr(name.size() - maxChars, maxChars);
						InputText->SetName(textName);
						textPos = InputText->RelativePositionToParent();
					}
				}
				if (maxChars == 0  || (textPos.x + textPos[2] > ownPos.x + ownPos[2])) {
					maxChars = textName.size();
					while (textName.size() > 1 && textPos.x + textPos[2] > ownPos.x + ownPos[2]) {
						textName.erase(textName.begin());
						maxChars--;
						InputText->SetName(textName);
						textPos = InputText->RelativePositionToParent();
					}
				}
			}
			Info::ResetTextBuffer();
		}
		else {
			wasDeleting = false;
		}
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
