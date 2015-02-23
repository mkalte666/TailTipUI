#include "TailTipUI_text.h"

namespace TailTipUI {

	Text::Text() 
	{
		tex = 0;
	}

	Text::Text(std::string name)
	{
		tex = 0;
		this->name = name;
	}

	Text::Text(std::string name, TTF_Font* font)
	{
		this->font = font;
		this->name = name;
		tex = 0;
		_UpdateText();
	}

	void Text::SetFont(TTF_Font* newfont) 
	{
		if (newfont == nullptr) return;
		font = newfont;
		_UpdateText();
	}

	void Text::SetName(std::string newname)
	{
		GeneralElement::SetName(newname);
		_UpdateText();;
	}

	void Text::SetForgroundColor(glm::vec4 color)
	{
		GeneralElement::SetForgroundColor(color);
		_UpdateText();;
	}

	void Text::_Render() {
		if (font != nullptr && name != "" && tex != 0) {
			glm::vec4 renderpos = RelativePositionToParent();
			//renderpos[2] = renderpos[3] * textsizeratio;
			RenderElementByTexture(tex, renderpos);
		}
	}

	void Text::_UpdateText() {
		if (font == nullptr || name == "") {
			return;
		}
		SDL_Color color = { 255 * (int)fgcolor.r, 255 * (int)fgcolor.g, 255 * (int)fgcolor.b };
		SDL_Surface* s;
		s = TTF_RenderText_Blended(font, name.c_str(), color);
		textsizeratio = (float)s->w / (float)s->h;
		pos[2] = pos[3] * textsizeratio;
		if (s != nullptr) {
			if (tex != NULL) {
				glDeleteTextures(1, &tex);
			}
			tex = SurfaceToTexture(s);
			SDL_FreeSurface(s);
		}
	}

};