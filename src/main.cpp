#include <Geode/Geode.hpp>

using namespace geode::prelude;
#include "ImCocos/ImcocosButton.hpp"
#include "ImCocos/ImCocosWindow.hpp"
#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		auto myButton = ImGuiButton::create("Test ImCocos",menu_selector(MyMenuLayer::onMyButton),this);

		CCSize windowSize = CCSizeMake(200, 100);
		float topBarHeight = 30.0f;
		ccColor4B windowColor = {51, 51, 51, 255};  
		ccColor4B topBarColor = {25, 25, 25, 255}; 


		DraggableWindow* window = DraggableWindow::create(windowSize, topBarHeight, windowColor, topBarColor);
		window->setTopBarState(true);
		window->setResizeState(true);
		window->setPosition(ccp(100, 100)); 
		window->setZOrder(1000);
		window->m_DragOnlyFromTopBar = false;
		window->getScrollLayer()->addChild(ImGuiButton::create("Test ImCocos",menu_selector(MyMenuLayer::onMyButton),this));
		window->getScrollLayer()->addChild(ImGuiButton::create("Test ImCocos",menu_selector(MyMenuLayer::onMyButton),this));
		window->getScrollLayer()->addChild(ImGuiButton::create("Test ImCocos",menu_selector(MyMenuLayer::onMyButton),this));
		window->getScrollLayer()->addChild(ImGuiButton::create("Test ImCocos",menu_selector(MyMenuLayer::onMyButton),this));
		window->getScrollLayer()->addChild(ImGuiButton::create("Test ImCocos",menu_selector(MyMenuLayer::onMyButton),this));
		window->getScrollLayer()->addChild(ImGuiButton::create("Test ImCocos",menu_selector(MyMenuLayer::onMyButton),this));
		window->getScrollLayer()->addChild(ImGuiButton::create("Test ImCocos",menu_selector(MyMenuLayer::onMyButton),this));
		window->getScrollLayer()->addChild(myButton);
		window->getScrollLayer()->updateLayout();
		this->addChild(window);

		return true;
	}

	void onMyButton(CCObject*) {
		FLAlertLayer::create("ImCocos", "Hello from my test button!", "OK")->show();
	}
};