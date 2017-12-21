#include "Debug.h"

void glClearError()
{
    // Clears every error code, letting it empty
    while(glGetError() != GL_NO_ERROR);
}

bool glLogCall(const char* function, const char* file, int line)
{
    // Returns true if no error was found, false otherwise
    if(GLenum error_code = glGetError())
    {
        LOGEL << "[OPENGL]:";
        LOGE  << std::showbase << std::dec << "code: (" << error_code << ")" << std::hex << ", (" << error_code << ")";
        LOGE                              << "path: " << file;
        LOGE                              << "function: " << function;
        LOGE  << std::dec                  << "l: " << line;

        crank::Log::Flush(); // making sure we don't lose information
        return false;
    }
    return true;
}