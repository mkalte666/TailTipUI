#include "TailTipUI_xml.h"

#include "HoardXML.h"

namespace TailTipUI {

#define ELEMENT_SETTING(name, func, element, convert, cmp, content) \
	if (name == cmp) { element->func(convert(content)); }

#define SPECIAL_ELEMENT_SETTING(elementClassName, tagname, type, name, func, element, convert, cmp, content) \
	if (elementClassName == tagname) { \
		if (name == cmp) { dynamic_cast<type*>(element)->func(convert(content)); }}

#define ELEMENT_SETTINGS(name, content, tagname, dstElement) \
	ELEMENT_SETTING("name", SetName, dstElement, std::string, name, content) \
	ELEMENT_SETTING("id", SetId, dstElement, std::string, name, content) \
	ELEMENT_SETTING("pos", SetPos, dstElement, strToVec4, name, content) \
	ELEMENT_SETTING("font", SetFont, dstElement, strToFont, name, content) \
	ELEMENT_SETTING("color", SetForgroundColor, dstElement, strToVec4, name, content) \
	ELEMENT_SETTING("bgcolor", SetBackgroundColor, dstElement, strToVec4, name, content) \
	ELEMENT_SETTING("eventcolor", SetEventColor, dstElement, strToVec4, name, content) \
	ELEMENT_SETTING("hidden", SetHidden, dstElement, strToBool, name, content) \
	ELEMENT_SETTING("draggable", SetDraggable, dstElement, strToBool, name, content) \
	ELEMENT_SETTING("blockParentDragging", SetBlockParentdragging, dstElement, strToBool, name, content) \
	ELEMENT_SETTING("radius", SetRadius, dstElement, strToVec4, name, content) \
	ELEMENT_SETTING("radiusParameter", SetRadiusParameter, dstElement, strToFloat, name, content) \
	ELEMENT_SETTING("smoothing", SetSmoothing, dstElement, strToFloat, name, content) \
	SPECIAL_ELEMENT_SETTING("text", tagname, Text, "widthlock", SetWidthLock, dstElement, strToBool, name, content) \
	SPECIAL_ELEMENT_SETTING("button", tagname, Button, "widthlock", SetTextWidthlock, dstElement, strToBool, name, content) \
	SPECIAL_ELEMENT_SETTING("button", tagname, Button, "hovercolor", SetEventColor, dstElement, strToVec4, name, content) \
	SPECIAL_ELEMENT_SETTING("image", tagname, Image, "texturename", SetImage, dstElement, strToImage, name, content)

#define TAG_COMPARE(cmpname,name,classname,dstElement) else if(name==cmpname) { \
	dstElement = new classname(); \
	}

#define TAG_COMPARE_END }

#define ELEMENT_SPECIFICATION(cpname, dstElement) \
	TAG_COMPARE(cpname, "text",Text,dstElement) \
	TAG_COMPARE(cpname, "area", Area, dstElement) \
	TAG_COMPARE(cpname, "button", Button, dstElement) \
	TAG_COMPARE(cpname, "image", Image, dstElement) \
	TAG_COMPARE(cpname, "input", Input, dstElement)

	glm::vec4 strToVec4(std::string s)
	{
		static std::regex strRE("([\\d.]+),([\\d.]+),([\\d.]+),([\\d.]+)");
		std::smatch m;
		if (std::regex_search(s, m, strRE)) {
			return glm::vec4(atof(m[1].str().c_str()), atof(m[2].str().c_str()), atof(m[3].str().c_str()), atof(m[4].str().c_str()));
		}
		return glm::vec4(0);
	}

	TTF_Font* strToFont(std::string s)
	{
		static std::regex strRE("([\\w\\d.\\\\\\/]+)\\s*:\\s*([\\d]*)");
		std::smatch m;
		if (std::regex_search(s, m, strRE)) {
			return Info::GetFont(m[1], atoi(m[2].str().c_str()));
		}
		return nullptr;
	}

	float strToFloat(std::string s)
	{
		static std::regex strRE("([\\d.]+)");
		std::smatch m;
		if (std::regex_search(s, m, strRE)) {
			return (float)atof(s.c_str());
		}
		return 0.0f;
	}

	bool strToBool(std::string s)
	{
		static std::regex strRE("(\\s*true\\s*)");
		std::smatch m;
		if (std::regex_search(s, m, strRE)) {
			return true;
		}
		return false;
	}

	GLuint strToImage(std::string s)
	{
		return Info::GetImage(s);
	}


	//this is NOT a memeber fuction because i want XMLLoader completly out of the headers.
	GeneralElement* LoadTagIntoElement(XMLLoader* currentLoader, HoardXML::Tag t, GLuint rootFramebuffer = 0)
	{
		static int counter = 0;
		GeneralElement* newElement = nullptr;
		std::string tagName = t.GetName();
		std::map<std::string, std::string> tagAttributes = t.GetAttributes();
		if (tagName == "root") {
			newElement = new Root(rootFramebuffer);
		}
		ELEMENT_SPECIFICATION(tagName, newElement);

		if (newElement == nullptr) {
			return nullptr;
		}

		//Generate unique id for the thing.
		std::ostringstream s;
		s << counter;
		std::string newid = std::string("LOADED_GENERATED_") + tagName + s.str();
		newElement->SetId(newid);
		counter++;

		for (auto attributeIterator = tagAttributes.begin(); attributeIterator != tagAttributes.end(); attributeIterator++) {
			ELEMENT_SETTINGS(attributeIterator->first, attributeIterator->second, tagName, newElement);
		}
		//for callbacks we need to make shure id is set already, so wee loop through the attributes again.
		//callbacks are a little special
		for (auto attributeIterator = tagAttributes.begin(); attributeIterator != tagAttributes.end(); attributeIterator++) {
			if (attributeIterator->first == "onhover") {
				currentLoader->AddHoverEvent(newElement->GetId(), attributeIterator->second);
				newElement->SetHoverCallback(std::bind(&XMLLoader::HoverCallbackEventHandler, currentLoader, std::placeholders::_1));
			}
			else if (attributeIterator->first == "onrclick") {
				currentLoader->AddRightclickEvent(newElement->GetId(), attributeIterator->second);
				newElement->SetRightclickCallback(std::bind(&XMLLoader::RightclickCallbackEventHandler, currentLoader, std::placeholders::_1));
			}
			else if (attributeIterator->first == "onlclick") {
				currentLoader->AddLeftclickEvent(newElement->GetId(), attributeIterator->second);
				newElement->SetLeftclickCallback(std::bind(&XMLLoader::LeftclickCallbackEventHandler, currentLoader, std::placeholders::_1));
			}
			else if (attributeIterator->first == "onclick") {
				currentLoader->AddRightclickEvent(newElement->GetId(), attributeIterator->second);
				currentLoader->AddLeftclickEvent(newElement->GetId(), attributeIterator->second);
				newElement->SetRightclickCallback(std::bind(&XMLLoader::RightclickCallbackEventHandler, currentLoader, std::placeholders::_1));
				newElement->SetLeftclickCallback(std::bind(&XMLLoader::LeftclickCallbackEventHandler, currentLoader, std::placeholders::_1));
			}
			else if (attributeIterator->first == "onspecial") {
				currentLoader->AddSpecialEvent(newElement->GetId(), attributeIterator->second);
				newElement->SetSpecialCallback(std::bind(&XMLLoader::SpecialEventCallbackEventHandler, currentLoader, std::placeholders::_1));
			}
		}
		
		std::vector<HoardXML::Tag> childtags = t.GetChildren();
		for (auto t : childtags) {
			GeneralElement* newChildElement = LoadTagIntoElement(currentLoader, t, rootFramebuffer);
			if (newChildElement != nullptr) {
				newElement->AttatchChild(newChildElement);
			}
		}
		return newElement;
	}

	XMLLoader::XMLLoader(GLuint destinationFramebuffer)
		: framebuffer(destinationFramebuffer)
	{

	}

	XMLLoader::XMLLoader(GLuint destinationFramebuffer, std::string infile)
		: framebuffer(destinationFramebuffer)
	{
		Load(infile);
	}

	XMLLoader::~XMLLoader()
	{
		for (auto e : elements) {
			delete e;
		}
		elements.clear();
	}
	void XMLLoader::Load(std::string infile)
	{
		HoardXML::Document indoc(infile);
		std::vector<HoardXML::Tag> tags = indoc.GetChildren();

		for (auto t : tags) {
			GeneralElement* newElement = LoadTagIntoElement(this, t, framebuffer);
			if (newElement != nullptr) {
				elements.push_back(newElement);
			}
		}
	}

	GeneralElement* XMLLoader::GetElementById(std::string id)
	{
		for (auto e : elements) {
			GeneralElement* elem = e->GetElementById(id);
			if (elem != nullptr) {
				return elem;
			}
		}
		return nullptr;
	}

	void XMLLoader::RenderElements()
	{
		for (auto e : elements) {
			e->Render();
		}
	}

	void XMLLoader::_HandleGeneralCallback(GeneralElement* caller, std::string callbackString)
	{
		static std::regex callbackRE("([\\w]+).([\\w]+)=?(.*)");
		std::smatch m;
		if (std::regex_search(callbackString, m, callbackRE)) {
			if (m[1] == "callback") {
				if (callbacks[m[2]]) {
					callbacks[m[2]](caller, this);
				}
				return;
			}
			else {
				GeneralElement* e = GetElementById(m[1]);
				if (e != nullptr) {
					ELEMENT_SETTINGS(m[2], m[3], std::string(""), e);
				}
			}
		}
	}

	void XMLLoader::HoverCallbackEventHandler(GeneralElement* elem) 
	{
		_HandleGeneralCallback(elem, hoverEvents[elem->GetId()]);
	}
	void XMLLoader::LeftclickCallbackEventHandler(GeneralElement* elem)
	{
		_HandleGeneralCallback(elem, lClickEvents[elem->GetId()]);
	}
	void XMLLoader::RightclickCallbackEventHandler(GeneralElement* elem)
	{
		_HandleGeneralCallback(elem, rClickEvents[elem->GetId()]);
	}
	void XMLLoader::SpecialEventCallbackEventHandler(GeneralElement* elem)
	{
		_HandleGeneralCallback(elem, specialEvents[elem->GetId()]);
	}

	void XMLLoader::AddHoverEvent(std::string id, std::string e)
	{
		hoverEvents[id] = e;
	}
	void XMLLoader::AddLeftclickEvent(std::string id, std::string e)
	{
		lClickEvents[id] = e;
	}
	void XMLLoader::AddRightclickEvent(std::string id, std::string e)
	{
		rClickEvents[id] = e;
	}
	void XMLLoader::AddSpecialEvent(std::string id, std::string e)
	{
		specialEvents[id] = e;
	}

	void XMLLoader::RegisterCallback(std::string name, XMLLoaderEventCallback c)
	{
		callbacks[name] = c;
	}

	void XMLLoader::RemoveCallback(std::string name)
	{
		for (auto i = callbacks.begin(); i != callbacks.end(); i++) {
			if (i->first == name) {
				callbacks.erase(i);
				break;
			}
		}
	}

}; //namespace TailTipUI
