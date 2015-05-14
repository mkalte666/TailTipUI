#include "TailTipUI.h"

namespace TailTipUI {

	int Info::width = 0;
	int Info::height = 0;
	std::string Info::name = std::string("none");
	MouseinfoCallbackType Info::mousecallback = MouseinfoCallbackType();
	ButtoninfoCallbackType Info::buttoncallback = ButtoninfoCallbackType();
	FontLoaderFunctionType Info::fontCallback = defaultFontLoader;
	ImageLoaderFunctionType Info::imageCallback = ImageLoaderFunctionType();
	TextBufferResetFunctionType Info::textBufferResetCallback = TextBufferResetFunctionType();
	GetTextBufferFunctionType Info::getTextBufferCallback = GetTextBufferFunctionType();
	SetTextBufferFunctionType Info::setTextBufferCallback = SetTextBufferFunctionType();

	Info::Info(std::string windowname, int w, int h)
	{
		name = windowname;
		width = w;
		height = h;
	}
	Info::~Info()
	{
	}

	glm::vec4 Info::GetMouseInfo()
	{
		if (mousecallback)
			return mousecallback();
		return glm::vec4(0);
	}

	const Uint8* Info::GetCurrentButton()
	{
		if (buttoncallback)
			return buttoncallback();
		return NULL;
	}

	TTF_Font* Info::GetFont(std::string name, int size)
	{
		if (fontCallback) {
			return fontCallback(name, size);
		}
		return nullptr;
	}

	GLuint Info::GetImage(std::string name)
	{
		if (imageCallback) {
			return imageCallback(name);
		}
		return 0;
	}

	std::string Info::GetTextBuffer()
	{
		if (getTextBufferCallback) {
			return getTextBufferCallback();
		}
		return "";
	}

	void Info::ResetTextBuffer()
	{
		if (textBufferResetCallback) {
			textBufferResetCallback();
		}
	}

	void Info::SetTextBuffer(std::string t)
	{
		if (setTextBufferCallback) {
			setTextBufferCallback(t);
		}
	}

	void Info::SetMouseCallback(MouseinfoCallbackType c)
	{
		mousecallback = c;
	}

	void Info::SetButtonCallback(ButtoninfoCallbackType c)
	{
		buttoncallback = c;
	}

	void Info::SetFontCallback(FontLoaderFunctionType c)
	{
		fontCallback = c;
	}

	void Info::SetImageCallback(ImageLoaderFunctionType c)
	{
		imageCallback = c;
	}

	void Info::SetTextBufferResetCallback(TextBufferResetFunctionType c)
	{
		textBufferResetCallback = c;
	}

	void Info::SetGetTextBufferCallback(GetTextBufferFunctionType c)
	{
		getTextBufferCallback = c;
	}

	void Info::SetTextBufferSetCallback(SetTextBufferFunctionType c)
	{
		setTextBufferCallback = c;
	}

	TTF_Font* defaultFontLoader(std::string name, int size)
	{
		TTF_Font *newfont = TTF_OpenFont(name.c_str(), size);
		if (newfont == NULL) {
			return nullptr;
		}
		return newfont;
	}


	StandaloneSetup::StandaloneSetup(std::string name, int width, int height, int x, int y, Uint32 flags, int mayor, int minor, int depth)
		: name(name), window(nullptr), valid(false)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			return;
		}

		if (mayor < 3 || (mayor == 3 && minor < 3) || (depth != 16 && depth != 24 && depth != 32) || width == 0 || height == 0) {
			return;
		}
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, mayor);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depth);

		window = SDL_CreateWindow(name.c_str(), x, y, width, height, flags);
		if (!window) {
			return;
		}
		valid = true;
		Info(name, width, height);
	}

	StandaloneSetup::~StandaloneSetup()
	{
		if (window) {
			SDL_DestroyWindow(window);
		}
		SDL_Quit();
	}

	bool& StandaloneSetup::IsValid() 
	{
		return valid;
	}

	void StandaloneSetup::Update()
	{
		if (!valid) return;

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				valid = false;
			}
		}

		SDL_GL_SwapWindow(window);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	GeneralElement::GeneralElement()
		: parent(nullptr), pos(0), centered(false), name(""),id(""),
		fgcolor(0),bgcolor(0),eventColor(0),usesEventColor(false),
		font(nullptr), hidden(false), draggable(nullptr), isDragged(false),
		blockParentdragging(nullptr), draggmouse(0),draggkey(nullptr),
		oldHoverstate(false), inFocus(false), smoothing(0.0f),renderRadius(0), 
		radiusParameter(0.1f)
	{

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

	void GeneralElement::SetCentered(bool b)
	{
		centered = b;
	}

	bool GeneralElement::GetCentered()
	{
		return centered;
	}	

	void GeneralElement::SetName(std::string newname) {
		name = newname;
	}

	std::string GeneralElement::GetName() {
		return name;
	}

	void GeneralElement::SetId(std::string newid) {
		id = newid;
	}

	std::string GeneralElement::GetId() {
		return id;
	}

	GeneralElement* GeneralElement::GetElementById(std::string searchId)
	{
		if (id == searchId) {
			return this;
		}

		for (auto c : children) {
			GeneralElement* e = c->GetElementById(searchId);
			if (e != nullptr) {
				return e;
			}
		}
		return nullptr;

	}

	//Render also updates the callbaks, ...
	void GeneralElement::Render()
	{
		if (hidden) {
			inFocus = false;
			return;
		}
		glm::vec4 mouse = Info::GetMouseInfo();
		const Uint8* key = Info::GetCurrentButton();
		bool hoverstate = GetHover();
		if (hoverstate && !oldHoverstate) {
			if (HoverCallback) {
				HoverCallback(this);
			}
			_InternalHoverEvent();
		}
		else if (!hoverstate && oldHoverstate) {
			_InternalStopHoverEvent();
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
					childBlock |= (c->GetHover() && (c->GetDraggable() || c->GetBlockParentdragging())) || c->IsCurrentlyDragged();
				}
				isDragged = !childBlock;
			}
			if (draggmouse[2] == 0) {
				if (LeftCallback) {
					LeftCallback(this);
				}
				if (!inFocus) {
					inFocus = true;
					_Focus();
				}
				_InternalLeftclickEvent();
			}
		}
		else {
			isDragged = false;
		}
		if (GetRightclick() && draggmouse[3] == 0) {
			if (RightCallback) {
				RightCallback(this);
			}
			if (!inFocus) {
				inFocus = true;
				_Focus();
			}
			_InternalRightclickEvent();
		}

		if (inFocus && !GetHover() && (mouse[2] == 1 || mouse[3] == 1)) {
			inFocus = false;
			_LostFocus();
		}
		draggmouse = mouse;
		draggkey = (Uint8*)key;
		_Render();
		for (auto c : children) {
			c->Render();
		}
	}

	void GeneralElement::_Render()
	{
	}

	glm::vec4 GeneralElement::RelativePositionToParent() {
		glm::vec4 p;
		if (parent == nullptr) {
			p = pos;
		} else {
			glm::vec4 parentPos = parent->RelativePositionToParent();
			p = glm::vec4(parentPos[0] + pos[0] * parentPos[2], parentPos[1] + pos[1] * parentPos[3], pos[2] * parentPos[2], pos[3] * parentPos[3]); 
		}
		if(centered) {
			p.x = p.x - 0.5f*p[2];	
		}
		return p;
	}

	void GeneralElement::SetForgroundColor(glm::vec4 color)
	{
		fgcolor = color;
	}

	void GeneralElement::SetBackgroundColor(glm::vec4 color)
	{
		bgcolor = color;
	}

	void GeneralElement::SetEventColor(glm::vec4 color)
	{
		eventColor = color;
		usesEventColor = true;
	}

	glm::vec4 GeneralElement::GetForgroundColor()
	{
		return fgcolor;
	}

	glm::vec4 GeneralElement::GetBackgroundColor()
	{
		return bgcolor;
	}

	glm::vec4 GeneralElement::GetEventColor()
	{
		return eventColor;
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
		glm::vec4 mpos = Info::GetMouseInfo();
		return (mpos.x >= relativePos.x && mpos.y >= relativePos.y && mpos.x <= (relativePos.x + relativePos[2]) && mpos.y <= (relativePos.y + relativePos[3]));
	}

	bool GeneralElement::GetLeftclick()
	{
		glm::vec4 mpos = Info::GetMouseInfo();
		return (GetHover() && mpos[2] != 0.0);
	}

	bool GeneralElement::GetRightclick()
	{
		glm::vec4 mpos = Info::GetMouseInfo();
		return (GetHover() && mpos[3] != 0.0);
	}

	void GeneralElement::SetHoverCallback(ElementCallbackType c)
	{
		HoverCallback = c;
	}

	void GeneralElement::SetLeftclickCallback(ElementCallbackType c)
	{
		LeftCallback = c;
	}
	void GeneralElement::SetRightclickCallback(ElementCallbackType c)
	{
		RightCallback = c;
	}
	void GeneralElement::SetSpecialCallback(ElementCallbackType c)
	{
		specialCallback = c;
	}

	void GeneralElement::SetSmoothing(float s) 
	{
		smoothing = s;
	}

	float GeneralElement::GetSmoothing()
	{
		return smoothing;
	}

	void GeneralElement::SetRadiusParameter(float b) 
	{
		radiusParameter = b;
	}

	float GeneralElement::GetRadiusParameter()
	{
		return radiusParameter;
	}

	void GeneralElement::_InternalHoverEvent(){}
	void GeneralElement::_InternalStopHoverEvent(){}
	void GeneralElement::_InternalLeftclickEvent(){}
	void GeneralElement::_InternalRightclickEvent(){}

	void GeneralElement::_Focus() {}
	void GeneralElement::_LostFocus() {}

//Child element class functions


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


	void Root::Render()
	{
		GLint oldFBO;
		GLint oldTEX;
		GLint oldPRG;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFBO);
		glGetIntegerv(GL_CURRENT_PROGRAM, &oldPRG);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &oldTEX);
		
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		GeneralElement::Render();
	
		glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
		glUseProgram(oldPRG);
		glBindTexture(GL_TEXTURE_2D, oldTEX);
		
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
		"	UV = vec2(xscale, yscale);"
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
		"	float colorFactor=1.0f;"
		"	if(s!=0.0f) {"
		"		colorFactor=smoothstep(0.0f,s,UV.x)*smoothstep(0.0f,s,UV.y)*(1.0f-smoothstep(1.0f-s,1.0f,UV.x))*(1.0f-smoothstep(1.0f-s,1.0f,UV.y));"
		"	}"
		"	if (UV.x<=b) {"
		"		P.x=b;"
		"		if(UV.y<=b) {"
		"			P.y=b;"
		"			colorFactor *=  1.0f-smoothstep(r[0],r[0]+s,length(UV-P));"
		"		} else if(UV.y>=(1-b)) {"
		"			P.y=1-b;"
		"			colorFactor *= 1.0f-smoothstep(r[3],r[3]+s,length(UV-P));"
		"		}"
		"	}"
		"	else if (UV.x>=(1-b)) {"
		"		P.x=1-b;"
		"		if(UV.y<=b) {"
		"			P.y=b;"
		"			colorFactor *=  1.0f-smoothstep(r[1],r[1]+s,length(UV-P));"
		"		} else if(UV.y>=(1-b)) {"
		"			P.y=1-b;"
		"			colorFactor *=  1.0f-smoothstep(r[2],r[2]+s,length(UV-P));"
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
		"uniform sampler2D lastImage;"
		"void main(){"
		"   vec2 P;"
		"	float colorFactor=1.0f;"
		"	if(s!=0.0f) {"
		"		colorFactor=smoothstep(0.0f,s,UV.x)*smoothstep(0.0f,s,UV.y)*(1.0f-smoothstep(1.0f-s,1.0f,UV.x))*(1.0f-smoothstep(1.0f-s,1.0f,UV.y));"
		"	}"
		"	if (UV.x<=b) {"
		"		P.x=b;"
		"		if(UV.y<=b) {"
		"			P.y=b;"
		"			colorFactor *=  1.0f-smoothstep(r[0],r[0]+s,length(UV-P));"
		"		} else if(UV.y>=(1-b)) {"
		"			P.y=1-b;"
		"			colorFactor *= 1.0f-smoothstep(r[3],r[3]+s,length(UV-P));"
		"		}"
		"	}"
		"	else if (UV.x>=(1-b)) {"
		"		P.x=1-b;"
		"		if(UV.y<=b) {"
		"			P.y=b;"
		"			colorFactor *=  1.0f-smoothstep(r[1],r[1]+s,length(UV-P));"
		"		} else if(UV.y>=(1-b)) {"
		"			P.y=1-b;"
		"			colorFactor *=  1.0f-smoothstep(r[2],r[2]+s,length(UV-P));"
		"		}"
		"	}"
		"	color = incolor*colorFactor;"
		"}"
		;

	const char* fragmentShaderLine =
		"#version 330 core"
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

		//the read-in textures will have wrong y-axis if read directly into ogl. 
		//we need to mirror them!
		unsigned int pixelSize = (textureFormat == GL_RGBA || textureFormat==GL_BGRA) ? 4 : 3;
		unsigned char* mirrorData = new unsigned char[pixelSize*s->w*s->h];
		for (unsigned int y = 0; y < s->h; y++) {
			int invY = (s->h-1) - y;
			for (unsigned int x = 0; x < s->w*pixelSize; x++) {
					mirrorData[x + y*s->pitch ] = ((unsigned char*)s->pixels)[x + invY*s->pitch];
			}
		}

		//Throw at ogl
		GLuint TextureId = GL_INVALID_VALUE;
		glGenTextures(1, &TextureId);
		glBindTexture(GL_TEXTURE_2D, TextureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, s->w, s->h, 0, textureFormat, GL_UNSIGNED_BYTE, mirrorData);
		delete[] mirrorData;
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
	//	static GLuint lastRenderTex = 0;
		static GLuint posPos = 0;
		static GLuint texPos = 0;
		static GLuint rPos = 0;
		static GLuint bPos = 0;
		static GLuint sPos = 0;
		static GLuint quad = 0;
		if (programId == 0) {
			//glGenTextures(1, &lastRenderTex);
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
		glUniform4f(rPos,rFactor - radiusKomponent[0], rFactor - radiusKomponent[1], rFactor - radiusKomponent[2], rFactor - radiusKomponent[3]);
		glUniform1f(bPos, b);
		glUniform1f(sPos, s);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
	}

	void RenderSingleColor(glm::vec4 color, glm::vec4 pos, glm::vec4 radiusKomponent, float b, float s)
	{
		static GLuint programId = 0;
		//static GLuint lastRenderTex = 0;
		static GLuint posPos = 0;
		static GLuint colorPos = 0;
		static GLuint rPos = 0;
		static GLuint bPos = 0;
		static GLuint sPos = 0;
		static GLuint quad = 0;
		if (programId == 0) {
		//	glGenTextures(1, &lastRenderTex);
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

		//for future use
		//glReadBuffer(GL_FRONT);
		//glBindTexture(GL_TEXTURE_2D, lastRenderTex);
		//glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, Info::width, Info::height, 0);

		glUseProgram(programId);
		glUniform4f(posPos, pos[0], pos[1], pos[2], pos[3]);
		glUniform4f(colorPos, color[0], color[1], color[2], color[3]);
		float rFactor = sqrt(2 * b*b);
		glUniform4f(rPos,rFactor - radiusKomponent[0],rFactor - radiusKomponent[1], rFactor - radiusKomponent[2], rFactor - radiusKomponent[3]);
		glUniform1f(bPos, b);
		glUniform1f(sPos, s);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
	}

	void RenderLineColored(glm::vec4 color, glm::vec4 pos, float w, float s) {
		static GLuint programId = 0;
		static GLuint posPos = 0;
		static GLuint colorPos = 0;
		static GLuint sPos = 0;
		static GLuint wPos = 0;
		static GLuint quad = 0;
		if (programId == 0) {
			programId = _LoadProgram(vertexShader, fragmentShaderLine);
			posPos = glGetUniformLocation(programId, "position");
			colorPos = glGetUniformLocation(programId, "incolor");
			sPos = glGetUniformLocation(programId, "s");
			wPos = glGetUniformLocation(programId, "w");
			glGenBuffers(1, &quad);
			glBindBuffer(GL_ARRAY_BUFFER, quad);
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad), g_quad, GL_STATIC_DRAW);
		}


	}
}; 

