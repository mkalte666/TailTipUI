

#pragma once
#include <TailTipUI.h>
#include <TailTipUI_text.h>
#include <TailTipUI_area.h>
#include <TailTipUI_button.h>
#include <TailTipUI_image.h>

namespace TailTipUI {

	//class XMLLoader
	//info: Loads an xml-file and creates a Tree-structure.
	class XMLLoader
	{
	public:
		XMLLoader(GLuint destinationFramebuffer);
		XMLLoader(GLuint destinationFramebuffer, std::string infile);
		~XMLLoader();

		void Load(std::string infile);
	
		void RegisterCallback(std::string name, ElementCallbackType c);
		void RemoveCallback(std::string name);


		void RenderElements();

		void HoverCallbackEventHandler(GeneralElement* elem);
		void LeftclickCallbackEventHandler(GeneralElement* elem);
		void RightclickCallbackEventHandler(GeneralElement* elem);
		void AddHoverEvent(std::string id, std::string e);
		void AddLeftclickEvent(std::string id, std::string e);
		void AddRightclickEvent(std::string id, std::string e);

		GeneralElement* GetElementById(std::string id);
		
	private:
		GLuint framebuffer;
		Root* rootElelent;

		std::map<std::string, ElementCallbackType> callbacks;
		std::vector<GeneralElement*> elements;
		std::map<std::string, std::string> hoverEvents;
		std::map<std::string, std::string> lClickEvents;
		std::map<std::string, std::string> rClickEvents;
	protected:
		void _HandleGeneralCallback(GeneralElement* caller, std::string callbackString);
	};

};
