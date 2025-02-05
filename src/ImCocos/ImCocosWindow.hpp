#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;
#include "ImCocos.hpp"
#include "ImScrollLayer.hpp"
#include "ImCCDrawNode.hpp"

class DraggableWindow : public CCLayer {
  public:
	static DraggableWindow *create(const CCSize &size, float topBarHeight,
	                               const ccColor4B &bgColor, const ccColor4B &topBarColor) {
		DraggableWindow *pRet = new DraggableWindow();
		if (pRet && pRet->init(size, topBarHeight, bgColor, topBarColor)) {
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_DELETE(pRet);
		return NULL;
	}

	float m_topBarHeight;
	bool m_resizing;
	bool m_DragOnlyFromTopBar = true;
	bool m_dragging;
	IMCCDrawNode *m_drawNode;
	IMCCDrawNode *m_drawNodeBar;
	IMCCDrawNode *m_drawTriangle;
	ImCocoScrollLayer *m_scrollLayer;
	ccColor4B m_bgColor;
	ccColor4B m_topBarColor;

	bool init(const CCSize &size, float topBarHeight,
	          const ccColor4B &bgColor, const ccColor4B &topBarColor) {
		if (!CCLayer::init())
			return false;

		this->setAnchorPoint(ccp(0, 1));

		m_size = size;
		m_topBarHeight = topBarHeight;
		m_bgColor = bgColor;
		m_topBarColor = topBarColor;
		m_dragging = false;
		m_resizing = false;
		m_topbar = true;
		m_Resizeable = true;

		m_drawNode = IMCCDrawNode::create(Shapes::RectSquare,bgColor,CCRectMake(0, -m_size.height, m_size.width, m_size.height));
		m_drawNode->setColor(bgColor);
		m_drawNode->setID("background");
		m_drawNodeBar = IMCCDrawNode::create(Shapes::RectSquare,topBarColor,CCRectMake(0, -m_topBarHeight, m_size.width, m_topBarHeight));
		m_drawNodeBar->setColor(topBarColor);
		m_drawNodeBar->setID("top-bar");
		m_drawTriangle = IMCCDrawNode::create(Shapes::Custom,{127,127,127,127},CCRectMake(0, 0, 0, 0));
		m_drawTriangle->setID("scale-triangle");
		this->addChild(m_drawNode);
		this->addChild(m_drawTriangle);
		this->addChild(m_drawNodeBar);

		m_scrollLayer = ImCocoScrollLayer::create(CCRectMake(0, -m_size.height, m_size.width, m_size.height - m_topBarHeight),false,true);
		m_scrollLayer->setAnchorPoint(ccp(0, 1));
		m_scrollLayer->setPosition(ccp(0, -m_topBarHeight));
        this->getScrollLayer()->setLayout(
            ColumnLayout::create()
			->setAutoScale(false)
            ->setCrossAxisAlignment(geode::AxisAlignment::Start)
            ->setAxisAlignment(geode::AxisAlignment::Start)
            ->setCrossAxisLineAlignment(geode::AxisAlignment::Start)
        );
		if (m_topbar) {
			this->getScrollLayer()->setContentSize(CCSizeMake(m_size.width, m_size.height - m_topBarHeight));
		} else {
			this->getScrollLayer()->setContentSize(CCSizeMake(m_size.width, m_size.height));
		};
		this->addChild(m_scrollLayer);

		redrawWindow();

		this->setTouchEnabled(true);

		return true;
	}
	ImCocoContent* getScrollLayer() const {
		return m_scrollLayer->m_ImCocoContent;
	}

	void drawResizeHandle() {
		float handleSize = 5.0f;
		CCPoint p1 = ccp(m_size.width, -m_size.height);
		CCPoint p2 = ccp(m_size.width - handleSize, -m_size.height);
		CCPoint p3 = ccp(m_size.width, -m_size.height + handleSize);
		m_drawTriangle->m_vertices = {p1, p2, p3};
		m_drawTriangle->m_size = CCRectMake(0, 0, m_size.width, m_size.height);
		m_drawTriangle->drawElement();
		if (m_topbar) {
			m_drawTriangle->setVisible(true);
		} else {
			m_drawTriangle->setVisible(false);
		}
	}

	void redrawWindow() {
		CCRect windowRect = CCRectMake(0, -m_size.height, m_size.width, m_size.height);
		m_drawNode->setColor(m_bgColor);
		m_drawNode->m_size = windowRect;
		m_drawNode->drawElement();
		CCRect topBarRect = CCRectMake(0, -m_topBarHeight, m_size.width, m_topBarHeight);
		m_drawNodeBar->setColor(m_topBarColor);
		m_drawNodeBar->m_size = topBarRect;
		m_drawNodeBar->drawElement();
		if (m_topbar) {
			m_drawNodeBar->setVisible(true);
		} else {
			m_drawNodeBar->setVisible(false);
		}
		drawResizeHandle();
		if (m_scrollLayer) {
			m_scrollLayer->setPositionY(-m_size.height);
			if (m_topbar) {
				m_scrollLayer->setContentSize(CCSizeMake(m_size.width, m_size.height - m_topBarHeight));
			} else {
				m_scrollLayer->setContentSize(CCSizeMake(m_size.width, m_size.height));
			};
		}
	}

	virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event) {
		CCPoint localPoint = this->convertTouchToNodeSpace(touch);
		if (m_Resizeable) {
			float handleSize = 5.0f;
			CCRect resizeRect = CCRectMake(m_size.width - handleSize, -m_size.height, handleSize, handleSize);
			if (resizeRect.containsPoint(localPoint)) {
				m_resizing = true;
				m_resizeInitialLocal = localPoint;
				m_originalSize = m_size;
				return true;
			}
		}
		CCRect draggableRect;
		if (m_DragOnlyFromTopBar) {
			if (localPoint.y < -m_topBarHeight) {
				return false;
			}
			draggableRect = CCRectMake(0, -m_topBarHeight, m_size.width, m_topBarHeight);
		} else {
			if (localPoint.y < -m_size.height) {
				return false;
			}
			draggableRect = CCRectMake(0, -m_size.height, m_size.width, m_size.height);
		}
		if (draggableRect.containsPoint(localPoint)) {
			m_dragging = true;
			m_touchOffset = ccpSub(this->getPosition(), touch->getLocation());
			return true;
		}
		return false;
	}

	virtual void ccTouchMoved(CCTouch *touch, CCEvent *event) {
		if (m_resizing) {
			CCPoint currentLocal = this->convertTouchToNodeSpace(touch);
			float dx = currentLocal.x - m_resizeInitialLocal.x;
			float dy = -(currentLocal.y - m_resizeInitialLocal.y);
			float newWidth = std::max(m_originalSize.width + dx, 15.0f);
			float newHeight = std::max(m_originalSize.height + dy, m_topBarHeight + 5);
			m_size = CCSizeMake(newWidth, newHeight);
			redrawWindow();
		} else if (m_dragging) {
			CCPoint newPos = ccpAdd(touch->getLocation(), m_touchOffset);
			this->setPosition(newPos);
		}
	}

	virtual void ccTouchEnded(CCTouch *touch, CCEvent *event) {
		m_dragging = false;
		m_resizing = false;
	}

	virtual void ccTouchCancelled(CCTouch *touch, CCEvent *event) {
		m_dragging = false;
		m_resizing = false;
	}

	virtual void registerWithTouchDispatcher() {
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -9999, true);
	}

	void setTopBarState(bool state) {
		m_topbar = state;
		redrawWindow();
	};
	bool getTopBarState() {
		return m_topbar;
	};
	void setResizeState(bool state) {
		m_Resizeable = state;
		redrawWindow();
	};
	bool getResizeState() {
		return m_Resizeable;
	};
	protected:
		bool m_topbar;
		bool m_Resizeable;
		CCSize m_size;
		CCPoint m_touchOffset;
		CCSize m_originalSize;
		CCPoint m_resizeInitialLocal;
};
