//
//  Core.hpp
//  Snake
//
//  Created by Witek Cybulski on 27/10/2024.
//

#pragma once

#include "Config.hpp"

#include <string>

constexpr NS::String* string(const std::string& str)
{
    return NS::String::string(str.c_str(), NS::StringEncoding::UTF8StringEncoding);
}