#include "TailTipUI.h"

namespace TailTipUI {

	GeneralElement::GeneralElement()
	{
		pos = glm::vec4(0);
		hidden = false;
		parent = nullptr;
		font = nullptr;
		draggable = false;
		blockParentdragging = true;
		oldHoverstate = false;
		isDragged = false;
		renderRadius = glm::vec4(0);
		radiusSmoothing = 0.05f;
		radiusParameter = 0.1f;
	}

	GeneralElement::~GeneralElement()
	{
		if (parent != nullptr) {
			parent->DeattatchChild(this);
		}
		for (auto c : children) {
			c->SetParent(nullptr);
			delete c;
		}
	}

	void GeneralElement::SetParent(GeneralElement* newparent)
	{
		parent = newparent;
	}

	GeneralElement* GeneralElement::GetParent()
	{
		return parent;
	}

	void GeneralElement::AttatchChild(GeneralElement* child)
	{
		children.push_back(child);
		child->SetParent(this);
	}

	void GeneralElement::DeattatchChild(GeneralElement* child)
	{
		for (auto i = children.begin(); i != children.end(); i++) {
			if (*i == child) {
				children.erase(i);
				break;
			}
		}
	}

	void GeneralElement::SetHidden(bool isHidden)
	{
		hidden = isHidden;
	}

	bool GeneralElement::GetHidden() {
		return hidden;
	}

	void GeneralElement::SetPos(glm::vec4 position)
	{
		pos = position;
	}

	glm::vec4 GeneralElement::GetPos() {
		return pos;
	}

	void GeneralElement::SetName(std::string newname) {
		name = newname;
	}

	std::string GeneralElement::GetName() {
		return name;
	}

	//Render also updates the callbaks, ...
	void GeneralElement::Render()
	{
		if (hidden) return;
		glm::vec4 mouse = GetMouseInfo();
		SDL_Keycode key = GetCurrentButton();
		bool hoverstate = GetHover();
		if (hoverstate && !oldHoverstate) {
			if (HoverCallback)
				HoverCallback(name);
		}
		oldHoverstate = hoverstate;

		if (isDragged) {
			if (draggmouse[2] == 1) {
				glm::vec4 absolutePosition = RelativePositionToParent();
				glm::vec2 absoluteMovement((mouse.x - draggmouse.x), (mouse.y - draggmouse.y));
				glm::vec2 relativeMovement(absoluteMovement.x*(pos[2] / absolutePosition[2]), absoluteMovement.y*(pos[3] / absolutePosition[3]));
				pos = pos + glm::vec4(relativeMovement, 0.0f, 0.0f);
				if (pos.x < 0.0f) pos.x = 0.0f;
				if (pos.y < 0.0f) pos.y = 0.0f;
				if (pos.x + pos[2] > 1.0) pos.x = 1 - pos[2];
				if (pos.y + pos[3] > 1.0) pos.y = 1 - pos[3];
			}
		}
		if (GetLeftclick()) {
			if (draggable) {
				//Only dragg if no child is hoverd that is dragable and does not block parentdragging
				bool childBlock = false;
				for (auto c : children) {
					childBlock |= c->GetHover() && (c->GetDraggable() || c->GetBlockParentdragging()) || c->IsCurrentlyDragged();
				}
				isDragged = !childBlock;
			}
			if (draggmouse[2] == 0) {
				if (LeftCallback)
					LeftCallback(name);
			}
		}
		else {
			isDragged = false;
		}
		if (GetRightclick() && draggmouse[3] == 0) {
			if (RightCallback)
				RightCallback(name);
		}
		draggmouse = mouse;
		draggkey = key;
		_Render();
		for (auto c : children) {
			c->Render();
		}
	}

	void GeneralElement::_Render()
	{
	}

	glm::vec4 GeneralElement::RelativePositionToParent() {
		if (parent == nullptr) {
			return pos;
		}
		glm::vec4 parentPos = parent->RelativePositionToParent();
		return glm::vec4(parentPos[0] + pos[0] * parentPos[2], parentPos[1] + pos[1] * parentPos[3], pos[2] * parentPos[2], pos[3] * parentPos[3]);
	}

	void GeneralElement::SetForgroundColor(glm::vec4 color)
	{
		fgcolor = color;
	}

	void GeneralElement::SetBackgroundColor(glm::vec4 color)
	{
		bgcolor = color;
	}

	glm::vec4 GeneralElement::GetForgroundColor()
	{
		return fgcolor;
	}

	glm::vec4 GeneralElement::GetBackgroundColor()
	{
		return bgcolor;
	}

	void GeneralElement::SetRadius(glm::vec4 r)
	{
		renderRadius = r;
	}

	glm::vec4 GeneralElement::GetRadius()
	{
		return renderRadius;
	}

	void GeneralElement::SetFont(TTF_Font* newfont)
	{
		newfont = font;
	}

	TTF_Font* GeneralElement::GetFont()
	{
		return font;
	}

	void GeneralElement::SetDraggable(bool isdraggable)
	{
		draggable = isdraggable;
	}

	bool GeneralElement::GetDraggable()
	{
		return draggable;
	}

	bool GeneralElement::IsCurrentlyDragged()
	{
		return isDragged;
	}

	void GeneralElement::SetBlockParentdragging(bool isdraggable)
	{
		blockParentdragging = isdraggable;
	}

	bool GeneralElement::GetBlockParentdragging()
	{
		return blockParentdragging;
	}


	bool GeneralElement::GetHover()
	{
		glm::vec4 relativePos = RelativePositionToParent();
		glm::vec4 mpos = GetMouseInfo();
		return (mpos.x >= relativePos.x && mpos.y >= relativePos.y && mpos.x <= (relativePos.x + relativePos[2]) && mpos.y <= (relativePos.y + relativePos[3]));
	}

	bool GeneralElement::GetLeftclick()
	{
		glm::vec4 mpos = GetMouseInfo();
		return (GetHover() && mpos[2] != 0.0);
	}

	bool GeneralElement::GetRightclick()
	{
		glm::vec4 mpos = GetMouseInfo();
		return (GetHover() && mpos[3] != 0.0);
	}

	void GeneralElement::SetHoverCallback(ElementCallbackType c)
	{
		HoverCallback = c;
	}

	void GeneralElement::GetLeftclickCallback(ElementCallbackType c)
	{
		LeftCallback = c;
	}
	void GeneralElement::GetRightclickCallback(ElementCallbackType c)
	{
		RightCallback = c;
	}

	glm::vec4 GeneralElement::GetMouseInfo()
	{
		if (parent == nullptr) {
			return glm::vec4(0);
		}
		return parent->GetMouseInfo();
	}

	SDL_Keycode GeneralElement::GetCurrentButton()
	{
		if (parent == nullptr) {
			return NULL;
		}
		return parent->GetCurrentButton();
	}

	void GeneralElement::SetRadiusSmoothing(float s) 
	{
		radiusSmoothing = s;
	}

	float GeneralElement::GetRadiusSmoothing()
	{
		return radiusSmoothing;
	}

	void GeneralElement::SetRadiusParameter(float b) 
	{
		radiusParameter = b;
	}

	float GeneralElement::GetRadiusParameter()
	{
		return radiusParameter;
	}

	ChildElement::ChildElement()
		: GeneralElement()
	{

	}



	ChildElement::~ChildElement()
	{

	}

	Root::Root(GLuint destinationFramebuffer)
	{
		framebuffer = destinationFramebuffer;
		pos = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	}

	Root::~Root()
	{

	}

	void Root::AttatchChild(ChildElement* child)
	{
		GeneralElement::AttatchChild(child);
	}

	void Root::DeattatchChild(ChildElement* child)
	{
		GeneralElement::DeattatchChild(child);
	}

	void Root::Render()
	{
		GLint oldFBO;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		GeneralElement::Render();
		glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
	}

	glm::vec4 Root::GetMouseInfo()
	{
		if (mousecallback)
			return mousecallback();
		return glm::vec4(0);
	}

	SDL_Keycode Root::GetCurrentButton()
	{
		if (buttoncallback)
			return buttoncallback();
		return NULL;
	}

	void Root::SetMouseCallback(MouseinfoCallbackType c)
	{
		mousecallback = c;
	}

	void Root::SetButtonCallback(ButtoninfoCallbackType c)
	{
		buttoncallback = c;
	}

	//below the blody opengl foo and texture management and stuff
	const char* vertexShader =
		"#version 330 core\n"
		"layout(location = 0) in vec3 vertexPosition_screenspace;"

		"uniform vec4 position;"
		"out vec2 UV;"
		"void main(){"
		"	float xscale = (vertexPosition_screenspace.x + 1) / 2;"
		"	float yscale = (vertexPosition_screenspace.y + 1) / 2;"
		"	vec2 inpos = vec2(position.x * 2 - 1 + xscale * 2 * position[2], (1-position.y-position[3]) * 2 - 1 + yscale * 2 * position[3]);"
		"	gl_Position = vec4(inpos, 0, 1.0);"
		"	UV = vec2(xscale, 1-yscale);"
		"}"
		;

	const char* fragmentShaderTexture =
		"#version 330 core\n"
		"in vec2 UV;"
		"out vec4 color;"
		"uniform sampler2D TextureSampler;"
		"uniform vec4 r;"
		"uniform float b;"
		"uniform float s;"
		"void main(){"
		"   vec2 P;"
		"   float colorFactor=1.0f;"
		"	if (UV.x<=b) {"
		"		P.x=b;"
		"		if(UV.y<=b) {"
		"			P.y=b;"
		"			colorFactor =  1.0f-smoothstep(r[0],r[0]*s,length(UV-P));"
		"		} else if(UV.y>=(1-b)) {"
		"			P.y=1-b;"
		"			colorFactor = 1.0f-smoothstep(r[3],r[3]*s,length(UV-P));"
		"		}"
		"	}"
		"	else if (UV.x>=(1-b)) {"
		"		P.x=1-b;"
		"		if(UV.y<=b) {"
		"			P.y=b;"
		"			colorFactor =  1.0f-smoothstep(r[1],r[1]*s,length(UV-P));"
		"		} else if(UV.y>=(1-b)) {"
		"			P.y=1-b;"
		"			colorFactor =  1.0f-smoothstep(r[2],r[2]*s,length(UV-P));"
		"		}"
		"	}"
		"	color = texture2D(TextureSampler, UV).rgba*colorFactor;"
		"}"
		;

	const char* fragmentShaderSingleColor =
		"#version 330 core\n"
		"in vec2 UV;"
		"out vec4 color;"
		"uniform vec4 incolor;"
		"uniform vec4 r;"
		"uniform float b;"
		"uniform float s;"
		"void main(){"
		"   vec2 P;"
		"   float colorFactor=1.0f;"
		"	if (UV.x<=b) {"
		"		P.x=b;"
		"		if(UV.y<=b) {"
		"			P.y=b;"
		"			colorFactor =  1.0f-smoothstep(r[0],r[0]*s,length(UV-P));"
		"		} else if(UV.y>=(1-b)) {"
		"			P.y=1-b;"
		"			colorFactor = 1.0f-smoothstep(r[3],r[3]*s,length(UV-P));"
		"		}"
		"	}"
		"	else if (UV.x>=(1-b)) {"
		"		P.x=1-b;"
		"		if(UV.y<=b) {"
		"			P.y=b;"
		"			colorFactor =  1.0f-smoothstep(r[1],r[1]*s,length(UV-P));"
		"		} else if(UV.y>=(1-b)) {"
		"			P.y=1-b;"
		"			colorFactor =  1.0f-smoothstep(r[2],r[2]*s,length(UV-P));"
		"		}"
		"	}"
		"	color = incolor*colorFactor;"
		"}"
		;

	static const GLfloat g_quad[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};

	GLuint SurfaceToTexture(SDL_Surface* s)
	{

		GLenum textureFormat = GL_BGR;
		if (s->format->BytesPerPixel == 4) {
			if (s->format->Rmask == 0x000000ff) {
				textureFormat = GL_RGBA;
			}
			else {
				textureFormat = GL_BGRA;
			}
		}
		else {
			if (s->format->Rmask == 0x0000000ff) {
				textureFormat = GL_RGB;
			}
			else {
				textureFormat = GL_BGR;
			}
		}

		GLuint TextureId = 0;
		glGenTextures(1, &TextureId);
		glBindTexture(GL_TEXTURE_2D, TextureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, textureFormat, GL_UNSIGNED_BYTE, s->pixels);		
		return TextureId;
	}

	GLuint _LoadProgram(std::string VertexShaderCode, std::string FragmentShaderCode){
		// Create the shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		GLint Result = GL_FALSE;
		int InfoLogLength;
		// Compile Vertex Shader
		char const * VertexSourcePointer = VertexShaderCode.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> VertexShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

		// Compile Fragment Shader
		char const * FragmentSourcePointer = FragmentShaderCode.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

		// Link the program
		//fprintf(stdout, "Linking program\n");
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> ProgramErrorMessage(std::max(InfoLogLength, int(1)));
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		return ProgramID;
	}

	void RenderElementByTexture(GLuint tex, glm::vec4 pos, glm::vec4 radiusKomponent, float b, float s) {
		static GLuint programId = 0;
		static GLuint posPos = 0;
		static GLuint texPos = 0;
		static GLuint rPos = 0;
		static GLuint bPos = 0;
		static GLuint sPos = 0;
		static GLuint quad = 0;
		if (programId == 0) {
			programId = _LoadProgram(vertexShader, fragmentShaderTexture);
			posPos = glGetUniformLocation(programId, "position");
			texPos = glGetUniformLocation(programId, "TextureSampler");
			rPos = glGetUniformLocation(programId, "r");
			bPos = glGetUniformLocation(programId, "b");
			sPos = glGetUniformLocation(programId, "s");
			glGenBuffers(1, &quad);
			glBindBuffer(GL_ARRAY_BUFFER, quad);	
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad), g_quad, GL_STATIC_DRAW);
		}

		glUseProgram(programId);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(texPos, 0);
		
		glUniform4f(posPos, pos[0], pos[1], pos[2], pos[3]);
		float rFactor = sqrt(2 * b*b);
		glUniform4f(rPos, sqrt(2 * b*b) - radiusKomponent[0], sqrt(2 * b*b) - radiusKomponent[1], sqrt(2 * b*b) - radiusKomponent[2], sqrt(2 * b*b) - radiusKomponent[3]);
		glUniform1f(bPos, b);
		glUniform1f(sPos, 1.0f + s);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
	}

	void RenderSingleColor(glm::vec4 color, glm::vec4 pos, glm::vec4 radiusKomponent, float b, float s)
	{
		static GLuint programId = 0;
		static GLuint posPos = 0;
		static GLuint colorPos = 0;
		static GLuint rPos = 0;
		static GLuint bPos = 0;
		static GLuint sPos = 0;
		static GLuint quad = 0;
		if (programId == 0) {
			programId = _LoadProgram(vertexShader, fragmentShaderSingleColor);
			posPos = glGetUniformLocation(programId, "position");
			colorPos = glGetUniformLocation(programId, "incolor");
			rPos = glGetUniformLocation(programId, "r");
			bPos = glGetUniformLocation(programId, "b");
			sPos = glGetUniformLocation(programId, "s");
			glGenBuffers(1, &quad);
			glBindBuffer(GL_ARRAY_BUFFER, quad);
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad), g_quad, GL_STATIC_DRAW);
		}
		glUseProgram(programId);
		glUniform4f(posPos, pos[0], pos[1], pos[2], pos[3]);
		glUniform4f(colorPos, color[0], color[1], color[2], color[3]);
		float rFactor = sqrt(2 * b*b);
		glUniform4f(rPos, sqrt(2 * b*b) - radiusKomponent[0], sqrt(2 * b*b) - radiusKomponent[1], sqrt(2 * b*b) - radiusKomponent[2], sqrt(2 * b*b) - radiusKomponent[3]);
		glUniform1f(bPos, b);
		glUniform1f(sPos, 1.0f+s);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
	}
}; 

