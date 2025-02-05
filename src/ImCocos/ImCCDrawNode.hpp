#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

enum Shapes {
	RectSquare,
	RoundedSquare,
	Custom, // Custom and PointBased are the same :troll:
	PointBased
};

class IMCCDrawNode : public CCDrawNode {
  public:
     std::vector<CCPoint> m_vertices;
	CCRect m_size;
	Shapes m_shapeType;
	float m_roundedcorner;
	static IMCCDrawNode *create(Shapes shape, ccColor4B color, CCRect size) {
		IMCCDrawNode *pRet = new IMCCDrawNode();
		if (pRet) {
			pRet->m_shapeType = shape;
			pRet->m_size = size;
			pRet->m_roundedcorner = 5;
			if (pRet->init()) {
				pRet->setColor(color);
				pRet->drawElement();
				pRet->autorelease();
				return pRet;
			}
		}
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
	void setColor(ccColor4B const & col) {
		CCDrawNode::setColor({col.r,col.g,col.b});
		this->setOpacity(col.a);
	}
	void drawElement() {
		this->clear();
		switch (m_shapeType) {
		case Shapes::RectSquare:
			this->drawRectSquare(m_size,  ccc4FFromccc4B({_displayedColor.r,_displayedColor.g,_displayedColor.b,_realOpacity}) );
			break;
		case Shapes::RoundedSquare:
			this->drawRoundedRect(m_size, m_roundedcorner, ccc4FFromccc4B({_displayedColor.r,_displayedColor.g,_displayedColor.b,_realOpacity}));
			break;
		default:
			if (m_vertices.size() > 0) {
				this->drawPolygon(m_vertices.data(), static_cast<unsigned int>(m_vertices.size()), ccc4FFromccc4B({_displayedColor.r,_displayedColor.g,_displayedColor.b,_realOpacity}), 0, cocos2d::ccColor4F{0, 0, 0, 0});
			};
			break;
		}
	}
	void drawRoundedRect(cocos2d::CCRect rect, float radius, cocos2d::ccColor4F color) {
		const int segments = 5; // Stupid drawing maths
		std::vector<cocos2d::CCPoint> vertices;
		addArcPoints(vertices,
		             ccp(rect.origin.x + radius, rect.origin.y + radius),
		             radius,
		             M_PI, 3 * M_PI / 2, segments);
		addArcPoints(vertices,
		             ccp(rect.origin.x + rect.size.width - radius, rect.origin.y + radius),
		             radius,
		             3 * M_PI / 2, 2 * M_PI, segments);

		addArcPoints(vertices,
		             ccp(rect.origin.x + rect.size.width - radius, rect.origin.y + rect.size.height - radius),
		             radius,
		             0, M_PI / 2, segments);

		addArcPoints(vertices,
		             ccp(rect.origin.x + radius, rect.origin.y + rect.size.height - radius),
		             radius,
		             M_PI / 2, M_PI, segments);

		vertices.push_back(vertices.front());
		this->drawPolygon(vertices.data(), static_cast<unsigned int>(vertices.size()),
		                  color, 0, cocos2d::ccColor4F{0, 0, 0, 0});
	}

	void drawRectSquare(cocos2d::CCRect rect, cocos2d::ccColor4F fillColor, float borderWidth = 1.0f, cocos2d::ccColor4F borderColor = cocos2d::ccc4f(0, 0, 0, 0)) {
		cocos2d::CCPoint vertices[4];
		vertices[0] = ccp(rect.origin.x, rect.origin.y);
		vertices[1] = ccp(rect.origin.x + rect.size.width, rect.origin.y);
		vertices[2] = ccp(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height);
		vertices[3] = ccp(rect.origin.x, rect.origin.y + rect.size.height);
		this->drawPolygon(vertices, 4, fillColor, borderWidth, borderColor);
	}

  protected:
	void addArcPoints(std::vector<cocos2d::CCPoint> &verts, cocos2d::CCPoint center, float radius, float startAngle, float endAngle, int segments) {
		float angleStep = (endAngle - startAngle) / segments;
		for (int i = 0; i <= segments; i++) {
			float angle = startAngle + i * angleStep;
			verts.push_back(ccp(
			    center.x + cosf(angle) * radius,
			    center.y + sinf(angle) * radius));
		}
	};
};