#pragma once

#include <Memory/Array.h>
#include <fmt/format.h>

/*!
 *  An efficient String class implemented on top of Array with lots of built-in functions.
 */
class ENGINE_API String : public Array<char> {

public:

    /*!
     * The default String constructor.
     */
    String() = default;

    /*!
     * Constructs a String by copying a char Array.
     */
    String(Array<char> const &array);

    /*!
     * Constructs a String by moving a char Array.
     */
    String(Array<char> &&array);

    /*!
     * Constructs a String copying a C string.
     */
    String(const char *str);

    /*!
     * Constructs a String copying a C string with a size limit.
     */
    String(const char *mem, size_t limit);

    /*!
     * Constructs a String copying a standard string.
     */
    String(std::string const &string);

    /*!
     *  Returns a string within the input range.
     *
     *  \param[in] from The index to start copying.
     *  \param[in] to The index to stop copying.
     *  \return The substring.
     */
    String substring(size_t from, size_t to) const;
    
    /*!
     *  Gets the Integer representation of this String or 0 if it is not a number.
     *
     *  \return The integer represented by the string.
     */
    Int64 toInteger() const;
    
    /*!
     *  Gets the floating point number representation of this String or 0 if it is not a number.
     *
     *  \return The floating point number represented by the string.
     */
    double toDouble() const;
    
    /*!
     *  Returns an array containing all the substrings separated by the separator.
     *
     *  \param separator The split separator.
     *  \return An array containing the substrings.
     */
    Array<String> split(char separator) const;
    
    /*!
     *  The Array copy assignment operator.
     *
     *  \param[in] other The array to copy.
     *  \return This object.
     */
    String &operator=(Array<char> const &other) override;

    /*!
     *  The Array move assignment operator.
     *
     *  \param[in] other The array to move.
     *  \return This object.
     */
    String &operator=(Array<char> &&other) override;

    /*!
     *  The standard string equality operator.
     *
     *  \param[in] other The string to compare with.
     *  \return Whether the strings are equal.
     */
    bool operator==(std::string const &other) const;

    /*!
     *  The C string equality operator.
     *
     *  \param[in] other The string to compare with.
     *  \return Whether the strings are equal.
     */
    bool operator==(const char *other) const;

    /*!
     *  The String equality operator.
     *
     *  \param[in] other The string to compare with.
     *  \return Whether the strings are equal.
     */
    bool operator==(String const &other) const;

    /*!
     *  The String lower than operator.
     *
     *  \param[in] other The string to compare with.
     *  \return Whether this string is shorter than the other.
     */
    bool operator<(String const &other) const;

    /*!
     *  The String output stream operator.
     *
     *  \param[in] os The stream.
     *  \param[in] str The string to print.
     *  \return The output stream.
     */
    friend std::ostream &operator<<(std::ostream &os, String const &str);

    void resize(size_t newCap) override;

    /*!
     *  Returns the size of the string without the terminating null byte.
     *
     *  \return The string size.
     */
    FORCEINLINE size_t getSize() const override {
        return (size == 0 ? 0 : (size - 1));
    }
    
    /*!
     * Returns whether the string is empty.
     * If the size equals 1, the string necessarily only contains the null byte and is considered empty.
     * \return Whether the string is empty.
     */
    FORCEINLINE bool isEmpty() const override {
        return size <= 1;
    }

    /*!
     *  Returns the C string contained in this array.
     *
     *  \return The C string
     */
    FORCEINLINE const char *toCString() const {
        return this->data;
    }
    
    /*!
     *  Returns whether the input character represents an integer.
     *
     *  \return Whether the input character represents an integer.
     */
    FORCEINLINE static bool isInteger(const char c) {
        return c >= '0' && c <= '9';
    }
    
    /*!
     *  Converts a char to its integer representation.
     *
     *  \return The represented integer.
     */
    FORCEINLINE static UInt8 charToInteger(const char c) {
        return c - '0';
    }

};

template <>
struct fmt::formatter<String> : formatter<std::string>{
    
  template <typename FormatContext>
  auto format(const String& p, FormatContext& ctx) -> decltype(ctx.out()) {

      return formatter<std::string>::format(p.toCString(), ctx);
      
  }
    
};
