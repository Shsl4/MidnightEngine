#include <Memory/String.h>
#include <fmt/format.h>
#include <Exception/ParseError.h>
#include <ostream>
#include <cmath>

String::String(Array<char> const &array) {
    *this = array;
}

String::String(Array<char> &&array) {
    *this = std::move(array);
}

String::String(const char *str) {

    // Get the string data.
    this->size = std::strlen(str) + 1;
    this->capacity = this->size;

    // Allocate our data.
    this->data = allocator.alloc(capacity);

    // Copy the string.
    allocator.copy(str, str + size, this->data);

    // Set the terminating null byte
    this->data[size - 1] = '\0';

}

String::String(const char *mem, size_t limit) {

    size_t strSize = strlen(mem);

    // If the limit is above the array size,
    if (limit > strSize) {

        // Set the new size to be the array size.
        limit = strSize;

    }

    // Set the size and capacity
    this->size = limit + 1;
    this->capacity = limit + 1;

    // Allocate our data.
    this->data = allocator.alloc(capacity);

    // Copy the string.
    allocator.copy(mem, mem + limit, this->data);

    this->data[limit] = '\0';

}

String::String(std::string const &string) : String(string.c_str()) {

}

String String::substring(size_t from, size_t to) const {

    // If the end index is higher than the string
    // length, set it to the string length.
    if (to >= size) {to = size - 1;}

    // If the input parameters are invalid, return an empty string.
    if (size == 0 || from >= size || from >= to) {
        return "";
    }

    // Return the substring.
    return {data + from, to - from };

}

Int64 String::toInteger() const {

    ParseError::throwIf(getSize() == 0, "Cannot parse an empty string");

    const size_t max = getSize();
    Int64 value = 0;
    const bool negative = data[0] == '-';
    
    for(size_t i = negative; i < max; ++i){

        ParseError::throwIf(!isInteger(data[i]), "Failed to parse. \"{}\" does not represent an integer", *this);
        
        const UInt8 digit = charToInteger(data[i]);
        
        value += static_cast<Int64>(digit * pow(10, size - i - 2));
        
    }
    
    if (negative) {
        value = -value;
    }
    
    return value;
    
}

double String::toDouble() const {
    
    ParseError::throwIf(getSize() == 0, "Cannot parse an empty string");

    const Int64 max = static_cast<Int64>(getSize());
    double value = 0;
    const bool negative = data[0] == '-';

    Int64 dotIndex = -1;
    Int64 counter = 0;
    
    for(const auto& e : *this){
        
        if (e == '.' || e == ',') {
            dotIndex = counter;
            break;
        }
        
        ++counter;
        
    }

    if(dotIndex == -1)
    {
        return static_cast<double>(toInteger());
    }

    for(Int64 i = negative; i < max; ++i){
        
        if(dotIndex == i) { continue; }
        
        ParseError::throwIf(!isInteger(data[i]), "Failed to parse. \"{}\" does not represent a floating point number", *this);

        const UInt8 digit = charToInteger(data[i]);
        
        if(i < dotIndex){
            value += digit * pow(10, dotIndex - i - 1);
        }
        else{
            const auto v = dotIndex - i;
            value += digit * pow(10, v);
        }
        
    }
    
    if (negative) {
        value = -value;
    }
    
    return value;
    
}

bool String::toBool() const
{
    
    ParseError::throwIf(getSize() == 0, "Cannot parse an empty string");
    
    if(*this == "true" || *this == "1")
    {
        return true;
    }

    if(*this == "false" || *this == "0")
    {
        return false;
    }

    ParseError::throwError("Failed to parse. \"{}\" does not represent a boolean", *this);
    
}

Array<String> String::split(const char separator) const {
    
    auto arr = Array<String>(10);

    if(getSize() == 0) { return arr; }
    
    size_t from = 0;
    size_t to = 0;

    const size_t sz = getSize();

    for (size_t i = 0; i < sz; ++i) {

        if (data[i] == separator) {
            to = i;
            arr += substring(from, to);
            from = to + 1;
        }
        
    }

    if(from == 0 && to == 0) {
        arr += *this;
    }
    
    if (from == to + 1) {
        to = sz;
        arr += substring(from, to);
    }

    return arr;

}

String &String::operator=(Array<char> const &other) {

    // Deallocate the data in this array
    allocator.release(data);

    // If the other array is empty, set this array to empty.
    if (!other.data) {

        this->data = nullptr;
        this->size = 0;
        this->capacity = 0;

    } else {

        // Otherwise, allocate a new data buffer.
        this->data = allocator.alloc(other.capacity);

        // Copy the array contents.
        allocator.copy(other.data, other.data + other.size, this->data);

        // Store the new information.
        this->capacity = other.capacity;
        this->size = other.size;

    }

    return *this;

}

String &String::operator=(Array<char> &&other) noexcept {

    // If the other array is not terminated by a null byte
    if (other.getAt(other.size - 1) != '\0') {

        // If it is full
        if (other.size >= other.capacity) {

            // Do not move the data, copy it instead and empty the other array.
            // Resize the data buffer to fit the contents and an additional null byte.
            allocator.realloc(data, capacity, other.size + 1);
            this->capacity = other.size + 1;
            this->size = capacity;

            // Copy the array contents.
            allocator.copy(other.data, other.data + other.size, this->data);

            // Set the terminating null byte.
            this->data[size - 1] = '\0';

            // Clear the other array.
            other.clear();

            return *this;

        }

        // Else, just set the last byte of the array to a null byte.
        other.data[other.size] = '\0';
        ++other.size;

    }

    // Move the data
    this->data = other.data;
    this->size = other.size;
    this->capacity = other.capacity;

    // Clear the other data.
    other.data = nullptr;
    other.size = 0;
    other.capacity = 0;

    return *this;

}

bool String::operator==(const char *other) const {

    // If the sizes are different, return false.
    if (strlen(other) != getSize()) {
        return false;
    }

    // Compare each character.
    for (size_t i = 0; i < getSize(); ++i) {
        if (data[i] != other[i]) {
            return false;
        }
    }

    return true;

}

// Compare each character.
bool String::operator==(std::string const &other) const {

    return *this == other.c_str();

}

bool String::operator==(String const &other) const {

    return *this == other.toCString();

}

bool String::operator<(String const &other) const {

    // Compare the sizes of the two strings.
    return getSize() < other.getSize();

}

std::ostream &operator<<(std::ostream &os, String const &str) {
    return os << std::string(str.toCString());
}

void String::resize(const size_t newCap) {

    // Call the original function with + 1
    Array<char>::resize(newCap + 1);

    // Set the last element as null byte.
    data[getSize()] = '\0';

}
