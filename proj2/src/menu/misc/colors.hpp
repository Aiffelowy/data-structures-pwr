#ifndef COLORS_H
#define COLORS_H


namespace Colors {

#if defined(_WIN32) || defined(__WIN32__) || defined(_MSC_VER)
    constexpr const char * WHITE = "";
    constexpr const char * RED = "";
    constexpr const char * GREEN = "";
    constexpr const char * YELLOW = "";
    constexpr const char * BLUE = "";
    constexpr const char * MAGENTA = "";
    constexpr const char * CYAN = "";
    constexpr const char * BLACK = "";
    
    constexpr const char * WHITE_BOLD = "";
    constexpr const char * RED_BOLD = "";
    constexpr const char * GREEN_BOLD = "";
    constexpr const char * YELLOW_BOLD = "";
    constexpr const char * BLUE_BOLD = "";
    constexpr const char * MAGENTA_BOLD = "";
    constexpr const char * CYAN_BOLD = "";
    constexpr const char * BLACK_BOLD = "";

    constexpr const char * RESET = "";
    constexpr const char * ITALIC = "";
    constexpr const char * UNDERLINE = "";

#else
    constexpr const char * WHITE = "\x1b[30m";
    constexpr const char * RED = "\x1b[31m";
    constexpr const char * GREEN = "\x1b[32m";
    constexpr const char * YELLOW = "\x1b[33m";
    constexpr const char * BLUE = "\x1b[34m";
    constexpr const char * MAGENTA = "\x1b[35m";
    constexpr const char * CYAN = "\x1b[36m";
    constexpr const char * BLACK = "\x1b[37m";
    
    constexpr const char * WHITE_BOLD = "\x1b[30;1m";
    constexpr const char * RED_BOLD = "\x1b[31;1m";
    constexpr const char * GREEN_BOLD = "\x1b[32;1m";
    constexpr const char * YELLOW_BOLD = "\x1b[33;1m";
    constexpr const char * BLUE_BOLD = "\x1b[34;1m";
    constexpr const char * MAGENTA_BOLD = "\x1b[35;1m";
    constexpr const char * CYAN_BOLD = "\x1b[36;1m";
    constexpr const char * BLACK_BOLD = "\x1b[37;1m";

    constexpr const char * RESET = "\x1b[30;0m";
    constexpr const char * ITALIC = "\x1b[30;3m";
    constexpr const char * UNDERLINE = "\x1b[30;4m";
#endif


}

#endif
