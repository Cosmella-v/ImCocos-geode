#pragma once
#include <Geode/Geode.hpp>
#include <vector>

namespace IM_Cocos {
namespace defaultTheme {
	namespace Buttons {
		extern cocos2d::ccColor4B m_normalColor;
		extern cocos2d::ccColor4B m_hoverColor;
		extern cocos2d::ccColor4B m_pressedColor;
	} 
} 
namespace drawModule {
    	inline void addArcPoints(std::vector<cocos2d::CCPoint> &verts, cocos2d::CCPoint center, float radius, float startAngle, float endAngle, int segments) {
            float angleStep = (endAngle - startAngle) / segments;
            for (int i = 0; i <= segments; i++) {
                float angle = startAngle + i * angleStep;
                verts.push_back(ccp(
                    center.x + cosf(angle) * radius,
                    center.y + sinf(angle) * radius));
            }
        };
        inline void drawRectSquare(cocos2d::CCDrawNode* node, cocos2d::CCRect rect, cocos2d::ccColor4F fillColor, float borderWidth = 1.0f, cocos2d::ccColor4F borderColor = cocos2d::ccc4f(0, 0, 0, 0)) {
            cocos2d::CCPoint vertices[4];
            vertices[0] = ccp(rect.origin.x, rect.origin.y);
            vertices[1] = ccp(rect.origin.x + rect.size.width, rect.origin.y);
            vertices[2] = ccp(rect.origin.x + rect.size.width, rect.origin.y + rect.size.height);
            vertices[3] = ccp(rect.origin.x, rect.origin.y + rect.size.height);
            node->drawPolygon(vertices, 4, fillColor, borderWidth, borderColor);
        }

	inline void drawRoundedRect(cocos2d::CCDrawNode *node, cocos2d::CCRect rect, float radius, cocos2d::ccColor4F color) {
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
		node->drawPolygon(vertices.data(), static_cast<unsigned int>(vertices.size()),
		                  color, 0, cocos2d::ccColor4F{0, 0, 0, 0});
	}
} 
} 
