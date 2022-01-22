#pragma once

#include <fmt/format.h>
#include <fmt/color.h>

/*!
 * A Logging class allowing you to log messages to the console using different severities.
 */
class Logger {

public:

    explicit Logger(const std::string id) : id(id) {
    }

    /*!
     *  Logs a debug message.
     *
     *  @param[in] format The format string
     *  @param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void debug(std::string format, Args &&... args) const {
        this->log(fg(fmt::color::gray), "DEBUG", format, args...);
    }

    /*!
     *  Logs an info message.
     *
     *  @param[in] format The format string
     *  @param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void info(std::string format, Args &&... args) const {
        this->log(fg(fmt::color::dark_gray), "INFO", format, args...);
    }

    /*!
     *  Logs a success message.
     *
     *  @param[in] format The format string
     *  @param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void success(std::string format, Args &&... args) const {
        this->log(fg(fmt::color::light_green), "SUCCESS", format, args...);
    }

    /*!
     *  Logs a warning message.
     *
     *  @param[in] format The format string
     *  @param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void warning(std::string format, Args &&... args) const {
        this->log(fg(fmt::color::light_yellow), "WARNING", format, args...);
    }

    /*!
     *  Logs an error message.
     *
     *  @param[in] format The format string
     *  @param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void error(std::string format, Args &&... args) const {
        this->log(fg(fmt::color::red), "ERROR", format, args...);
    }

    /*!
     *  Logs a fatal error message and stops the program.
     *
     *  @param[in] format The format string
     *  @param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void fatal(std::string format, Args &&... args) const {
        this->log(fg(fmt::color::dark_red), "FATAL", format, args...);
        abort();
    }

    template<typename ... Args>
    static void check(bool condition, std::string format, Args &&... args) {

        if (!condition) {
            assertLogger.fatal(format, args...);
        }

    }

    std::string getId() const {
        return this->id;
    }

private:

    /*!
     * Logs a message to the console.
     *
     *  @param[in] color The text color
     *  @param[in] type The log category to display
     *  @param[in] format The format string
     *  @param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void log(const fmt::text_style color, std::string const &type, std::string const &format, Args &&...args) const {

        const fmt::basic_format_args<fmt::format_context> formatArgs = fmt::make_format_args(args...);
        const std::string formatted = vformat(format, formatArgs) + '\n';
        print(color, "[{}] ({}) | ", type, this->getId());
        print(color, formatted);

    }

    const std::string id;
    static Logger assertLogger;

};
