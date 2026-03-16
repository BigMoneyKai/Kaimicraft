#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

namespace Util {
    void quit(int code = 0);
}

#define COLOR_RESET     "\033[0m"
#define COLOR_GREEN     "\033[32m"
#define COLOR_PURPLE    "\033[35m"
#define COLOR_RED       "\033[91m"
#define COLOR_DARK_RED  "\033[31m"

#define EMOJI_SMILE      "😄"
#define EMOJI_WARNING   "⚠️"
#define EMOJI_ERROR     "❌"
#define EMOJI_FATAL     "💥"
#define EMOJI_OK        "✅"
#define EMOJI_DEBUG     "🐛"
#define EMOJI_LEXER     "🔤"
#define EMOJI_PARSER    "🧩"
#define EMOJI_AST       "🌳"
#define EMOJI_IR        "🧠"
#define EMOJI_CODEGEN   "⚙️ "
#define EMOJI_LINKER    "🔗"
#define EMOJI_RUN       "🚀"
#define EMOJI_DEVIL     "😈"
#define EMOJI_ALERT     "🚨"

typedef enum {
    DBG_INFO,
    DBG_WARNING,
    DBG_ERROR,
    DBG_FATAL,

} DebugLevel;

#define INFO(...) \
    do{\
        fprintf(stdout, EMOJI_OK EMOJI_SMILE " " COLOR_GREEN "info" COLOR_RESET": ");\
        fprintf(stdout, __VA_ARGS__);\
        fprintf(stdout, "\n");\
    } while(0)

#define WARNING(...) \
    do{\
        fprintf(stderr, EMOJI_DEVIL EMOJI_WARNING " in \"%s\" %s:%d" COLOR_PURPLE " warning" COLOR_RESET ": ", __func__, __FILE__, __LINE__);\
        fprintf(stderr, __VA_ARGS__);\
        fprintf(stderr, "\n");\
    } while(0)

#define ERROR(...) \
    do{\
        fprintf(stderr, EMOJI_ERROR EMOJI_DEBUG " in \"%s\" %s:%d" COLOR_RED " error" COLOR_RESET ": ", __func__, __FILE__, __LINE__);\
        fprintf(stderr, __VA_ARGS__);\
        fprintf(stderr, "\n");\
    } while(0)

#define FATAL(...) \
    do{\
        fprintf(stderr, EMOJI_ALERT EMOJI_FATAL " in \"%s\" %s:%d" COLOR_DARK_RED " fatal" COLOR_RESET ": ", __func__, __FILE__, __LINE__);\
        fprintf(stderr, __VA_ARGS__);\
        fprintf(stderr, "\n");\
        Util::quit(EXIT_FAILURE);\
    } while(0)

// Debug shader
void printShaderLog(unsigned int shader);
void printProgramLog(int program);

// Debug OpenGL
bool checkOpenGLError();
