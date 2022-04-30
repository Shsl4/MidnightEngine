#pragma once

#include <Memory/String.h>

#include <fmt/format.h>
#include <fmt/color.h>

#include <utility>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>

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
    Logger(String id) : id(std::move(id)) {

    }

    /*!
     *  Logs a debug message.
     *
     *  \param[in] format The format string
     *  \param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void debug(String const &format, Args &&... args) const {
        this->log(fg(fmt::color::gray), "DEBUG", format, args...);
    }

    /*!
     *  Logs an info message.
     *
     *  \param[in] format The format string
     *  \param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void info(String const &format, Args &&... args) const {
        this->log(fg(fmt::color::dark_gray), "INFO", format, args...);
    }

    /*!
     *  Logs a success message.
     *
     *  \param[in] format The format string
     *  \param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void success(String const &format, Args &&... args) const {
        this->log(fg(fmt::color::light_green), "SUCCESS", format, args...);
    }

    /*!
     *  Logs a warning message.
     *
     *  \param[in] format The format string
     *  \param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void warning(String const &format, Args &&... args) const {
        this->log(fg(fmt::color::light_yellow), "WARNING", format, args...);
    }

    /*!
     *  Logs an error message.
     *
     *  \param[in] format The format string
     *  \param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void error(String const &format, Args &&... args) const {
        this->log(fg(fmt::color::red), "ERROR", format, args...);
    }

    /*!
     *  Logs a fatal error message and stops the program.
     *
     *  \param[in] format The format string
     *  \param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void fatal(String const &format, Args &&... args) const {
        this->log(fg(fmt::color::dark_red), "FATAL", format, args...);
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
    static void check(bool condition, String const &format, Args &&... args) {

        if (!condition) {
            assertLogger.fatal(format, args...);
        }

    }

    /*!
     *  Returns the logger's identifier.
     *
     *  \return The Logger id.
     */
    String getId() const {
        return this->id;
    }

private:

    /*!
     * Logs a message to the console.
     *
     *  \param[in] color The text color
     *  \param[in] type The log category to display
     *  \param[in] format The format string
     *  \param[in] args The variable arguments for the format string
     */
    template<typename ... Args>
    void log(const fmt::text_style color, String const &type, String const &format, Args &&...args) const {

        // Format the string.
        const auto formatArgs = fmt::make_format_args(args...);
        const std::string formatted = vformat(format.toCString(), formatArgs);

        // Print the formatted string.
        print(color, "[{}] ({}) | {}\n", type, this->getId(), formatted);

        // Get the time and put it into a string stream (cannot convert _Put_time directly to string).
        const auto now = std::chrono::system_clock::now();
        const std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::stringstream dateStream;
        std::tm tm;

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
    const String id;
    
    /*!
     * A static private logger used for assertion checks.
     */
    static Logger assertLogger;

    /*!
    * The log file where messages are dumped.
    */
    inline static std::ofstream file = std::ofstream("latest.log");

};
