#include <Geode/Geode.hpp>
using namespace geode::prelude;
#include "ImCocos.hpp"

class DraggableWindow : public CCLayer {
  public:
	bool m_DragOnlyFromTopBar = true;

	static DraggableWindow *create(const CCSize &size, float topBarHeight,
	                               const ccColor4F &bgColor, const ccColor4F &topBarColor) {
		DraggableWindow *pRet = new DraggableWindow();
		if (pRet && pRet->init(size, topBarHeight, bgColor, topBarColor)) {
			pRet->autorelease();
			return pRet;
		}
		CC_SAFE_DELETE(pRet);
		return NULL;
	}

	bool init(const CCSize &size, float topBarHeight,
	          const ccColor4F &bgColor, const ccColor4F &topBarColor) {
		if (!CCLayer::init())
			return false;

		this->setAnchorPoint(ccp(0, 1));

		m_size = size;
		m_topBarHeight = topBarHeight;
		m_bgColor = bgColor;
		m_topBarColor = topBarColor;
		m_dragging = false;
		m_resizing = false;

		m_drawNode = CCDrawNode::create();
		this->addChild(m_drawNode);

		m_scrollLayer = ScrollLayer::create(CCRectMake(0, -m_size.height, m_size.width, m_size.height - m_topBarHeight));
		m_scrollLayer->setAnchorPoint(ccp(0, 1));
		m_scrollLayer->setPosition(ccp(0, -m_topBarHeight));
        this->getScrollLayer()->setContentSize(CCSizeMake(m_size.width, m_size.height - m_topBarHeight));
        this->getScrollLayer()->setLayout(
            RowLayout::create()
            ->setGrowCrossAxis(true)
            ->setCrossAxisAlignment(geode::AxisAlignment::Start)
            ->setAxisAlignment(geode::AxisAlignment::Start)
            ->setCrossAxisLineAlignment(geode::AxisAlignment::Start)
        );
		this->addChild(m_scrollLayer);

		redrawWindow();

		this->setTouchEnabled(true);

		return true;
	}
	CCContentLayer *getScrollLayer() const {
		return m_scrollLayer->m_contentLayer;
	}

	void drawResizeHandle() {
		float handleSize = 5.0f;
		CCPoint p1 = ccp(m_size.width, -m_size.height);
		CCPoint p2 = ccp(m_size.width - handleSize, -m_size.height);
		CCPoint p3 = ccp(m_size.width, -m_size.height + handleSize);
		CCPoint vertices[] = {p1, p2, p3};
		m_drawNode->drawPolygon(vertices, 3, ccc4f(0.5f, 0.5f, 0.5f, 0.5f), 1, ccc4f(0, 0, 0, 0));
	}

	void redrawWindow() {
		m_drawNode->clear();
		CCRect windowRect = CCRectMake(0, -m_size.height, m_size.width, m_size.height);
		IM_Cocos::drawModule::drawRectSquare(m_drawNode, windowRect, m_bgColor);

		CCRect topBarRect = CCRectMake(0, -m_topBarHeight, m_size.width, m_topBarHeight);
		IM_Cocos::drawModule::drawRectSquare(m_drawNode, topBarRect, m_topBarColor);

		drawResizeHandle();
		if (m_scrollLayer) {
			m_scrollLayer->setPositionY(-m_size.height);
			m_scrollLayer->setContentSize(CCSizeMake(m_size.width, m_size.height - m_topBarHeight));
		}
	}

	virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event) {
		CCPoint localPoint = this->convertTouchToNodeSpace(touch);
		float handleSize = 5.0f;
		CCRect resizeRect = CCRectMake(m_size.width - handleSize, -m_size.height, handleSize, handleSize);
		if (resizeRect.containsPoint(localPoint)) {
			m_resizing = true;
			m_resizeInitialLocal = localPoint;
			m_originalSize = m_size;
			return true;
		}

		if (localPoint.y < -m_topBarHeight) {
			return false;
		}

		CCRect draggableRect = CCRectMake(0, -m_topBarHeight, m_size.width, m_topBarHeight);
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

	CCSize m_size;
	float m_topBarHeight;
	bool m_dragging;
	bool m_resizing;
	CCPoint m_touchOffset;
	CCSize m_originalSize;
	CCPoint m_resizeInitialLocal;
	ccColor4F m_bgColor;
	ccColor4F m_topBarColor;
	CCDrawNode *m_drawNode;
	ScrollLayer *m_scrollLayer;
};
