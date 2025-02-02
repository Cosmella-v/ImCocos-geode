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
		ccColor4F windowColor = ccc4f(0.2f, 0.2f, 0.2f, 1.0f);  
		ccColor4F topBarColor = ccc4f(0.1f, 0.1f, 0.1f, 1.0f); 


		DraggableWindow* window = DraggableWindow::create(windowSize, topBarHeight, windowColor, topBarColor);
		window->setPosition(ccp(100, 100)); 
		window->setZOrder(1000);
		CCMenu* i = CCMenu::createWithItem(myButton);
		i->setLayout(
			RowLayout::create()
			->setCrossAxisAlignment(geode::AxisAlignment::Start)
            ->setAxisAlignment(geode::AxisAlignment::Start)
            ->setCrossAxisLineAlignment(geode::AxisAlignment::Start)
		);
		i->setContentSize(myButton->getScaledContentSize());
		i->updateLayout();
		window->getScrollLayer()->addChild(i);
		window->getScrollLayer()->updateLayout();
		this->addChild(window);

		return true;
	}

	void onMyButton(CCObject*) {
		FLAlertLayer::create("ImCocos", "Hello from my test button!", "OK")->show();
	}
};