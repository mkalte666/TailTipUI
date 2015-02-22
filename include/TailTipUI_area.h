#pragma once

#include "TailTipUI.h"

namespace TailTipUI {

	class Area : public ChildElement {
	public:
		Area();
		Area(glm::vec4 background);

		virtual void Render() override;
	};

};