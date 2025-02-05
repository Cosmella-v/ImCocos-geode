#pragma once
#include <Geode/Geode.hpp>


/*
Default theme colors:
- Normal: #265387 (38, 83, 135, 255)
- Hover:  #315996 (49, 89, 150, 255)
- Pressed: #153971 (21, 57, 113, 255)
*/


;
namespace IM_Cocos {
    namespace defaultTheme {
        namespace Buttons {  
            inline cocos2d::ccColor4B m_normalColor = {38, 83, 135, 255};
            inline cocos2d::ccColor4B m_hoverColor = {49, 89, 150, 255};
            inline cocos2d::ccColor4B m_pressedColor = {21, 57, 113, 255};
        };
    };
};

