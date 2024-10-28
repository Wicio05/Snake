//
//  Shader.cpp
//  Snake
//
//  Created by Witek Cybulski on 27/10/2024.
//

#include "Shader.hpp"

#include <fstream>
#include <iostream>

std::vector<char> Shader::readFile(const std::string & filepath)
{
    // open file stream at the end (ate)
    std::ifstream file(filepath, std::ios::ate);

    // check if file is open
    if(!file.is_open())
    {
        // throw error if file is not open
        std::cerr << "Failed to open file: " << filepath << '\n';
        exit(EXIT_FAILURE);
    }

    // get the current position (end of the file) and assign it to fileSize variable
    size_t fileSize = static_cast<size_t>(file.tellg());

    // create vector with specified size
    std::vector<char> buffer(fileSize);

    // go to the begining of the file
    file.seekg(0);

    // read file with specified size
    file.read(buffer.data(), fileSize);

    // close file stream
    file.close();

    // return populated buffer
    return buffer;
}
