#pragma once
#include <Geode/binding/CCContentLayer.hpp>
#include <Geode/binding/CCScrollLayerExt.hpp>
#include <Geode/utils/cocos.hpp>
using namespace geode::prelude;

class ImCocoContent : public CCContentLayer {
  public:
	void addChild(CCMenuItem* Child) {
		CCMenu * ch = CCMenu::createWithItem(Child);
		ch->setContentSize(Child->getContentSize());
		ch->setLayout(
			RowLayout::create()
			->setGap(0)
		);
		ch->setAnchorPoint({0,0.5});
		ch->updateLayout();
		ch->setTouchPriority(-10000);
		CCContentLayer::addChild(ch);
		CCContentLayer::setContentHeight(CCContentLayer::getContentHeight()+ch->getContentHeight());
	};
	static ImCocoContent *create(float width, float height) {
		auto ret = new ImCocoContent();
		if (ret->initWithColor({0, 0, 0, 0}, width, height)) {
			ret->autorelease();
			return ret;
		}
		delete ret;
		return nullptr;
	}

	void setPosition(CCPoint const &pos) {
		CCLayerColor::setPosition(pos);

		for (auto child : CCArrayExt<CCNode *>(m_pChildren)) {
			auto y = this->getPositionY() + child->getPositionY();
			child->setVisible(!((m_obContentSize.height < y) || (y < -child->getContentSize().height)));
		}
	}
};

class ImCocoScrollLayer : public CCScrollLayerExt {
  public:
	ImCocoContent *m_ImCocoContent;
	bool m_scrollWheelEnabled;
	void visit() {
		if (m_cutContent && this->isVisible()) {
			glEnable(GL_SCISSOR_TEST);

			if (this->getParent()) {
				auto const bottomLeft = this->convertToWorldSpace(ccp(0, 0));
				auto const topRight = this->convertToWorldSpace(this->getContentSize());
				CCSize const size = topRight - bottomLeft;

				CCEGLView::get()->setScissorInPoints(bottomLeft.x, bottomLeft.y, size.width, size.height);
			}
		}

		CCNode::visit();

		if (m_cutContent && this->isVisible()) {
			glDisable(GL_SCISSOR_TEST);
		}
	}

	void scrollWheel(float y, float) {
		if (m_scrollWheelEnabled) {
			this->scrollLayer(y);
		}
	}

	void enableScrollWheel(bool enable) {
		m_scrollWheelEnabled = enable;
	}

	bool ccTouchBegan(CCTouch *touch, CCEvent *event) {
		if (nodeIsVisible(this)) {
			return CCScrollLayerExt::ccTouchBegan(touch, event);
		}
		return false;
	}

	void scrollToTop() {
		auto listTopScrollPos = -m_contentLayer->getContentHeight() + this->getContentHeight();
		m_contentLayer->setPositionY(listTopScrollPos);
	}

	ImCocoScrollLayer(CCRect const &rect, bool scrollWheelEnabled, bool vertical) : CCScrollLayerExt(rect) {
		m_scrollWheelEnabled = scrollWheelEnabled;

		m_disableVertical = !vertical;
		m_disableHorizontal = vertical;
		m_cutContent = true;

		m_contentLayer->removeFromParent();
		m_ImCocoContent = ImCocoContent::create(rect.size.width, rect.size.height);
		m_contentLayer = m_ImCocoContent;
		m_contentLayer->setID("content-layer");
		m_contentLayer->setAnchorPoint({0, 0});
		this->addChild(m_contentLayer);

		this->setID("ScrollLayer");

		this->setMouseEnabled(true);
		this->setTouchEnabled(true);
	};

	static ImCocoScrollLayer *create(CCRect const &rect, bool scroll, bool vertical) {
		auto ret = new ImCocoScrollLayer(rect, scroll, vertical);
		ret->autorelease();
		return ret;
	}

	static ImCocoScrollLayer *create(CCSize const &size, bool scroll, bool vertical) {
		return ImCocoScrollLayer::create({0, 0, size.width, size.height}, scroll, vertical);
	}
};