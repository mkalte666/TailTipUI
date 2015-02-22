#include "TailTipUI_area.h"

namespace TailTipUI {
	Area::Area() {
		
	}

	Area::Area(glm::vec4 background) {
		bgcolor = background;
	}

	void Area::Render() {
		if (hidden) return;

		RenderSingleColor(bgcolor, RelativePositionToParent());

		GeneralElement::Render();
	}
};