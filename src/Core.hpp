//
//  Core.hpp
//  Snake
//
//  Created by Witek Cybulski on 27/10/2024.
//

#include <glm/glm.hpp>

typedef glm::vec4 Color;

struct Colors
{
    static constexpr Color BLACK =          glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    static constexpr Color WHITE =          glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    static constexpr Color RED =            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    static constexpr Color GREEN =          glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    static constexpr Color BLUE =           glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    static constexpr Color LIGHT_GREEN =    glm::vec4(167.0f / 255.0f, 216.0f / 255.0f, 73.0f / 255.0f, 1.0f);
    static constexpr Color DARK_GREEN =     glm::vec4(117.0f / 255.0f, 166.0f / 255.0f, 47.0f / 255.0f, 1.0f);
};