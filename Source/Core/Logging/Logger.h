#pragma once

#include <fmt/format.h>
#include <fmt/color.h>

class Logger
{
    
public:
    
    explicit Logger(const std::string id) : id(id) { }

    template<typename ... Args>
    void debug(std::string formatString, Args&&... args) const
    {
        this->log(fg(fmt::color::gray), "DEBUG", formatString, args...);
    }

    template<typename ... Args>
    void info(std::string formatString, Args&&... args) const
    {
        this->log(fg(fmt::color::dark_gray), "INFO", formatString, args...);
    }

    template<typename ... Args>
    void success(std::string formatString, Args&&... args) const
    {
        this->log(fg(fmt::color::light_green), "SUCCESS", formatString, args...);
    }

    template<typename ... Args>
    void warning(std::string formatString, Args&&... args) const
    {
        this->log(fg(fmt::color::light_yellow), "WARNING", formatString, args...);
    }

    template<typename ... Args>
    void error(std::string formatString, Args&&... args) const
    {
        this->log(fg(fmt::color::red), "ERROR", formatString, args...);
    }

    template<typename ... Args>
    void fatal(std::string formatString, Args&&... args) const
    {
        this->log(fg(fmt::color::dark_red), "FATAL", formatString, args...);
        abort();
    }
    
    template<typename ... Args>
    static void check(bool condition, std::string formatString, Args&&... args) {

        if (!condition) {
            assertLogger.fatal(formatString, args...);
        }

    }

    std::string getId() const { return this->id; }

private:
        
    template<typename ... Args>
    void log(const fmt::text_style color, const std::string type, const std::string format, Args&& ...args) const {
        
        const fmt::basic_format_args<fmt::format_context> formatArgs = fmt::make_format_args(args...);
        const std::string formatted = vformat(format, formatArgs) + '\n';
        print(color, "[{}] ({}) | ", type, this->getId());
        print(color, formatted);
        
    }

    const std::string id;
    static Logger assertLogger;

};