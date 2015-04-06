#include "TailTipUI_text.h"

namespace TailTipUI {

	Text::Text() 
	{
		tex = 0; 
		wlock = false;
	}

	Text::Text(std::string name)
	{
		tex = 0;
		this->name = name;
		wlock = false;
	}

	Text::Text(std::string name, TTF_Font* font)
	{
		this->font = font;
		this->name = name;
		tex = 0;
		wlock = false;
		_UpdateText();
	}
	
	void Text::SetPos(glm::vec4 p)
	{
		bool reset =false;
		if(p[3]!=pos[3]||p[2]!=pos[2]) {
			reset = true;
		}
		GeneralElement::SetPos(p);
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
		_UpdateText();
	}

	void Text::SetForgroundColor(glm::vec4 color)
	{
		GeneralElement::SetForgroundColor(color);
		_UpdateText();;
	}

	void Text::SetWidthLock(bool b) {
		wlock = b;
	}

	bool Text::GetWidthLock()
	{
		return wlock;
	}

	

	glm::vec4 Text::RelativePositionToParent()
	{
		glm::vec4 p = ChildElement::RelativePositionToParent();
		return p;
	}

	void Text::_Render() {
		if (font != nullptr && name != "" && tex != 0) {
			glm::vec4 renderpos = RelativePositionToParent();
			RenderElementByTexture(tex, renderpos, renderRadius, radiusParameter, smoothing);
		}
	}

	void Text::_UpdateText() {
		if (font == nullptr || name == "") {
			return;
		}
		SDL_Color color = { static_cast<Uint8>(255*fgcolor.r), static_cast<Uint8>(255*fgcolor.g), static_cast<Uint8>(255*fgcolor.b) };
		SDL_Surface* s;
		s = TTF_RenderUTF8_Blended(font, name.c_str(), color);
		
			
		textsizeratio = (float)s->w / (float)s->h;
		pos[2] = pos[3] * textsizeratio;	
		if (s != nullptr) {
			if (tex != 0) {
				glDeleteTextures(1, &tex);
			}
			tex = SurfaceToTexture(s);
			SDL_FreeSurface(s);
		}
	}

	void Text::ForceUpdate()
	{
		_UpdateText();
	}

};
