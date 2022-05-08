#pragma once

#include <Memory/String.h>
#include <iostream>

#include <fmt/format.h>
#include <fmt/color.h>

#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

#include "fmt/ostream.h"

class Colors
{

public:
    
    static const inline char* Black = "\033[30m";
    static const inline char* Red = "\033[31m";
    static const inline char* Green = "\033[32m";
    static const inline char* Yellow = "\033[33m";
    static const inline char* Blue = "\033[34m";
    static const inline char* Magenta = "\033[35m";
    static const inline char* Cyan = "\033[36m";
    static const inline char* LightGray = "\033[37m";
    static const inline char* DarkGray = "\033[90m";
    static const inline char* LightRed = "\033[91m";
    static const inline char* LightGreen = "\033[92m";
    static const inline char* LightYellow = "\033[93m";
    static const inline char* LightBlue = "\033[94m";
    static const inline char* LightMagenta = "\033[95m";
    static const inline char* LightCyan = "\033[96m";
    static const inline char* Reset = "\033[0m";
    
};

template <>
struct fmt::formatter<String> : formatter<std::string>{
    
    template <typename FormatContext>
    auto format(const String& p, FormatContext& ctx) -> decltype(ctx.out()) {

        const char* c = p.toCString() ? p.toCString() : "";
        return formatter<std::string>::format(c, ctx);
      
    }
    
};

/*!
 * A Logging class allowing you to log messages to the console using different severities.
 */
class ENGINE_API Logger {

public:

    /*!
     * The default logger constructor. Initializes the id.
     *
     * \param[in] id The Logger identifier.
     */
    Logger(const char* id) : id(id) {

    }

    String prompt()
    {
        std::getline(std::cin, lastInput);

        if(lastInput.empty())
        {
            fmt::print("\033[A> ");
        }
        
        return lastInput;
    }

    /*!
     *  Logs a debug message.
     *
     *  \param[in] format The format string
     *  \param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void debug(const char* format, Args &&... args) const {
        this->log(fg(fmt::color::gray), "DEBUG", format, args...);
    }

    /*!
     *  Logs an info message.
     *
     *  \param[in] format The format string
     *  \param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void info(const char* format, Args &&... args) const {
        this->log(fg(fmt::color::dark_gray), "INFO", format, args...);
    }

    /*!
     *  Logs a success message.
     *
     *  \param[in] format The format string
     *  \param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void success(const char* format, Args &&... args) const {
        this->log(fg(fmt::color::light_green), "SUCCESS", format, args...);
    }

    /*!
     *  Logs a warning message.
     *
     *  \param[in] format The format string
     *  \param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void warning(const char* format, Args &&... args) const {
        this->log(fg(fmt::color::light_yellow), "WARNING", format, args...);
    }

    /*!
     *  Logs an error message.
     *
     *  \param[in] format The format string
     *  \param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void error(const char* format, Args &&... args) const {
        this->log(fg(fmt::color::red), "ERROR", format, args...);
    }

    /*!
     *  Logs a fatal error message and stops the program.
     *
     *  \param[in] format The format string
     *  \param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void fatal(const char*format, Args &&... args) const {
        this->log(fg(fmt::color::dark_red), "FATAL", format, args...);
        std::fflush(stdout);
        abort();
    }

    /*!
     *  An assertion function. If the input condition is not met, the message is printed and a fatal error is thrown.
     *
     *  \param[in] condition The condition to check
     *  \param[in] format The format string
     *  \param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    static void check(bool condition, const char* format, Args &&... args) {

        if (!condition) {
            assertLogger.fatal(format, args...);
        }

    }

    /*!
     *  Returns the logger's identifier.
     *
     *  \return The Logger id.
     */
    NODISCARD const char* getId() const {
        return this->id;
    }

private:

    enum class State
    {
        Idle,
        Logging,
        Prompting
    };

    /*!
     * Logs a message to the console.
     *
     *  \param[in] color The text color
     *  \param[in] type The log category to display
     *  \param[in] format The format string
     *  \param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void log(const fmt::text_style color, const char* type, const char*format, Args &&...args) const {
        
        // Format the string.
        const auto formatArgs = fmt::make_format_args(args...);
        const std::string formatted = vformat(format, formatArgs);
        
        fmt::print("\r");

        // Print the formatted string.
        print(color, "[{}] ({}) | {}\n", type, this->getId(), formatted);

        fmt::print("> ");

        // Get the time and put it into a string stream (cannot convert _Put_time directly to string).
        const auto now = std::chrono::system_clock::now();
        const std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::stringstream dateStream;
        std::tm tm{};

#ifdef _WIN64
        localtime_s(&tm, &t);
#else
         tm = *std::localtime(&t);
#endif
        
        dateStream << std::put_time(&tm, "%c");

        // Write to the log file.
        file << fmt::format("[{}] [{}] ({}) | {}\n", dateStream.str(), type, this->getId(), formatted);

        // Flush the data to the file.
        file.flush();

    }

    /*!
     * The logger id
     */
    const char* id;
    
    /*!
     * A static private logger used for assertion checks.
     */
    static Logger assertLogger;

    /*!
    * The log file where messages are dumped.
    */
    inline static std::ofstream file = std::ofstream("latest.log");

    State state = State::Idle;
    std::string lastInput;

};
