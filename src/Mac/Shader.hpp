//
//  Shader.hpp
//  Snake
//
//  Created by Witek Cybulski on 27/10/2024.
//

#pragma once

#include <string>
#include <vector>

class Shader
{
    public:
        static std::vector<char> readFile(const std::string& filepath);
};