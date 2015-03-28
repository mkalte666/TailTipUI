#include "TailTipUI_image.h"

namespace TailTipUI
{

	Image::Image()
		: freeOnDelete(false), texId(0)
	{
	}

	Image::Image(GLuint id, bool freeOnDelete)
		: freeOnDelete(freeOnDelete), texId(id)
	{

	}

	Image::~Image()
	{
		if (freeOnDelete && texId != 0) {
			glDeleteTextures(1, &texId);
		}
	}

	void Image::SetImage(GLuint id, bool free)
	{
		texId = id;
		freeOnDelete = free;
	}

	void Image::_Render()
	{
		RenderElementByTexture(texId, RelativePositionToParent(), renderRadius, radiusParameter, smoothing);
	}

}; //namespace TailTipUI
