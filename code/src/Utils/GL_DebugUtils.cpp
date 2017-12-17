#include "GL_DebugUtils.h"

void GLClearError()
{
    // Clears every error code, letting it empty
    while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    // Returns true if no error was found, false otherwise
    if(GLenum error_code = glGetError())
    {
        LOG("[OpenGL Error]: \n");
        std::cout << std::showbase << std::dec << "\t(" << error_code << ")" << std::endl;
        std::cout << std::hex                  << "\t(" << error_code << ")" << std::endl;
        std::cout                              << "\tpath: " << file << std::endl;
        std::cout                              << "\tfunction: " << function << std::endl;
        std::cout << std::dec                  << "\tl: " << line << std::endl;

        return false;
    }
    return true;
}