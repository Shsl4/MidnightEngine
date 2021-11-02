#pragma once

#include <fmt/format.h>
#include <fmt/color.h>

class Logger
{
    
public:
    
    Logger(std::string id) : id(id) { }

    template<typename ... Args>
    inline void debug(std::string formatString, Args&&... args) const
    {
        this->log(fg(fmt::color::gray), "DEBUG", formatString, args...);
    }

    template<typename ... Args>
    inline void info(std::string formatString, Args&&... args) const
    {
        this->log(fg(fmt::color::dark_gray), "INFO", formatString, args...);
    }

    template<typename ... Args>
    inline void success(std::string formatString, Args&&... args) const
    {
        this->log(fg(fmt::color::light_green), "SUCCESS", formatString, args...);
    }

    template<typename ... Args>
    inline void warning(std::string formatString, Args&&... args) const
    {
        this->log(fg(fmt::color::light_yellow), "WARNING", formatString, args...);
    }

    template<typename ... Args>
    inline void error(std::string formatString, Args&&... args) const
    {
        this->log(fg(fmt::color::red), "ERROR", formatString, args...);
    }

    template<typename ... Args>
    inline void fatal(std::string formatString, Args&&... args) const
    {
        this->log(fg(fmt::color::dark_red), "FATAL", formatString, args...);
    }

    std::string getId() const { return this->id; }
    
private:
        
    template<typename ... Args>
    inline void log(fmt::text_style color, std::string type, std::string format, Args&& ...args) const {
        
        auto preFormat = fmt::format("[{}] ({}) | ", type, this->getId());
        const auto formatArgs = fmt::make_format_args(args...);
        std::string formatted = preFormat + fmt::vformat(format, formatArgs) + '\n';
        fmt::print(color, formatted);
        
    }

    const std::string id;
    
};
