#pragma once
#include "TailTipUI.h"

namespace TailTipUI {
	
	class Text : public ChildElement {
	public:
		Text();
		Text(std::string name);
		Text(std::string name, TTF_Font* font);

		virtual void SetFont(TTF_Font* newfont) override;
		virtual void SetName(std::string newname) override;
		virtual void SetForgroundColor(glm::vec4 color) override;
		
		
	private:
		
		float textsizeratio;
		GLuint tex;

	protected:
		virtual void _Render() override;
		void _UpdateText();
	};
};