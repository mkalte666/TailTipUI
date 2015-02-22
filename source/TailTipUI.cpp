#include "TailTipUI.h"

namespace TailTipUI {

	GeneralElement::GeneralElement() 
	{
		pos = glm::vec4(0);
		hidden = false;
		parent = nullptr;
		font = nullptr;
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

	void GeneralElement::Render() 
	{
		if (hidden) return;
		for (auto c : children) {
			c->Render();
		}
	}

	glm::vec4 GeneralElement::RelativePositionToParent() {
		if (parent == nullptr) {
			return pos;
		}
		glm::vec4 parentPos = parent->GetPos();
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

	void GeneralElement::SetFont(TTF_Font* newfont) 
	{
		newfont = font;
	}

	TTF_Font* GeneralElement::GetFont()
	{
		return font;
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
		glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
		GeneralElement::Render();
		glBindFramebuffer(GL_FRAMEBUFFER, oldFBO);
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
		"void main(){"
		"	color = texture2D(TextureSampler, UV).rgba;"
		"}"
		;

	const char* fragmentShaderSingleColor =
		"#version 330 core\n"
		"in vec2 UV;"
		"out vec4 color;"
		"uniform vec4 incolor;"
		"void main(){"
		"	color = incolor;"
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

	void RenderElementByTexture(GLuint tex, glm::vec4 pos) {
		static GLuint programId = 0;
		static GLuint posPos = 0;
		static GLuint texPos = 0;
		static GLuint quad = 0;
		if (programId == 0) {
			programId = _LoadProgram(vertexShader, fragmentShaderTexture);
			posPos = glGetUniformLocation(programId, "position");
			texPos = glGetUniformLocation(programId, "TextureSampler");
			glGenBuffers(1, &quad);
			glBindBuffer(GL_ARRAY_BUFFER, quad);	
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad), g_quad, GL_STATIC_DRAW);
		}

		glUseProgram(programId);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(texPos, 0);
		
		glUniform4f(posPos, pos[0], pos[1], pos[2], pos[3]);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
	}

	void RenderSingleColor(glm::vec4 color, glm::vec4 pos)
	{
		static GLuint programId = 0;
		static GLuint posPos = 0;
		static GLuint colorPos = 0;
		static GLuint quad = 0;
		if (programId == 0) {
			programId = _LoadProgram(vertexShader, fragmentShaderSingleColor);
			posPos = glGetUniformLocation(programId, "position");
			colorPos = glGetUniformLocation(programId, "incolor");
			glGenBuffers(1, &quad);
			glBindBuffer(GL_ARRAY_BUFFER, quad);
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad), g_quad, GL_STATIC_DRAW);
		}
		glUseProgram(programId);
		glUniform4f(posPos, pos[0], pos[1], pos[2], pos[3]);
		glUniform4f(colorPos, color[0], color[1], color[2], color[3]);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, quad);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(0);
	}
}; 

