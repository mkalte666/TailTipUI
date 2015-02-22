#include "TailTipUI_area.h"

namespace TailTipUI {
	Area::Area() {
		
	}

	Area::Area(glm::vec4 background) {
		bgcolor = background;
	}

	void Area::_Render() {
		RenderSingleColor(bgcolor, RelativePositionToParent());
	}
};