#pragma once

#include <Core/Logging/ConsoleColors.h>

#ifdef _WIN64
#include <format>
#endif

class Logger
{
    
public:
    
    Logger(std::string id) : id(id) { }

    template<typename ... Args>
    inline void debug(std::string formatString, Args&&... args)
    {
        this->log(ConsoleColors::getDarkGrey(), "DEBUG", formatString, args...);
    }

    template<typename ... Args>
    inline void info(std::string formatString, Args&&... args)
    {
        this->log(ConsoleColors::getGrey(), "INFO", formatString, args...);
    }

    template<typename ... Args>
    inline void success(std::string formatString, Args&&... args)
    {
        this->log(ConsoleColors::getLightGreen(), "SUCCESS", formatString, args...);
    }

    template<typename ... Args>
    inline void warning(std::string formatString, Args&&... args)
    {
        this->log(ConsoleColors::getLightYellow(), "WARNING", formatString, args...);
    }

    template<typename ... Args>
    inline void error(std::string formatString, Args&&... args)
    {
        this->log(ConsoleColors::getLightRed(), "ERROR", formatString, args...);
    }

    template<typename ... Args>
    void fatal(std::string formatString, Args&&... args)
    {
        this->log(ConsoleColors::getDarkRed(), "FATAL", formatString, args...);
    }

    std::string getId() { return this->id; }
    
private:
        
    template<typename ... Args>
    inline void log(std::string color, std::string type, std::string format, Args&& ...args) {
        
#ifdef _WIN64

        auto preFormat = std::format("{}[{}] ({}) | ", color, type, this->getId());
        std::cout << preFormat + std::vformat(format, std::make_format_args(args...)) + ConsoleColors::getReset() << std::endl;

#endif
        /*
        auto preFormat = fmt::format("{}[{}] ({}) | ", color, type, this->getId());
        fmt::print(preFormat + fmt::vformat(format, fmt::make_format_args(args...)) + ConsoleColors::getReset());*/
    }

    const std::string id;
    
};
