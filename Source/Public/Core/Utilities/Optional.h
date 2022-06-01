#pragma once

#include <stdexcept>

#include <Memory/Allocator.h>
#include <Core/EngineTypes.h>

/*!
 *  A class used to safely encapsulate a value. All comparison operators are implemented, so
 *  safe comparisons can be made with direct values or other optionals.
 */
template<typename T>
class Optional final {

public:

    /*!
     *  The default Optional constructor.
     */
    Optional() = default;

    /*!
     *  Constructs an Optional and moves the input object in it.
     *
     *  \param[in] v The value to move.
     */
    Optional(T &&v);

    /*!
     *  Constructs an Optional and copies the input optional in it.
     *
     *  \param[in] opt The optional to copy.
     */
    Optional(Optional<T> const &opt);

    /*!
     *  Constructs an Optional and copies the input object in it.
     *
     *  \param[in] v The value to copy.
     */
    Optional(T const &v);

    /*!
     *  Constructs an Optional and copies the input object in it.
     *
     *  \param[in] v A pointer to the value to copy.
     */
    Optional(T *v);

    /*!
     *  Constructs an Optional and constructs a T object with the input arguments.
     *
     *  \tparam Args A variadic template.
     *  \param[in] args The arguments to forward to the constructor.
     */
    template<typename ... Args>
    Optional(Args ...args);

    /*!
     *  Returns an empty Optional.
     *
     *  \return An empty Optional.
     */
    static Optional<T> empty();

    /*!
     *  Returns whether this Optional contains a value.
     *
     *  \return Whether this Optional contains a value.
     */
    bool hasValue() const;

    /*!
     *  Returns a reference to the optional value.
     *
     *  \return The value contained in this Optional.
     *  \throws An std::runtime_error if the optional does not contain a value.
     */
    T& getValue() const;

    /*!
     *  Gets a reference to the value contained in the Optional or a default value if it is empty.
     *
     *  \param[in] def  A reference to default value.
     *  \return The appropriate object.
     */
    T& getValueElse(T &def) const;
    
    /*!
     *  Gets a copy of the value contained in the Optional or a default value if it is empty.
     *
     *  \param[in] def  The default value.
     *  \return The appropriate object.
     */
    T getValueElse(T def) const;

    /*!
     *  Returns a pointer to optional value.
     *
     *  \return The value contained in this Optional.
     *  \throws An std::runtime_error if the optional does not contain a value.
     */
    T* operator->();
    
    /*!
     *  Shorthand for getValue.
     *
     *  \return The value contained in this Optional.
     *  \throws An std::runtime_error if the optional does not contain a value.
     */
    T& operator()() const;
    
    /*!
     *  The not operator. Checks whether this Optional contains a value.
     *
     *  \return Whether this Optional contains a value.
     */
    bool operator!() const;
    
    /*!
     *  The Optional copy assignment operator.
     *
     *  \param[in] other The Optional to copy.
     *  \return This Optional.
     */
    Optional<T> &operator=(Optional<T> const &other);

    /*!
     *  The Optional move assignment operator.
     *
     *  \param[in] other The Optional to move.
     *  \return This Optional.
     */
    Optional<T> &operator=(Optional<T> &&other);

    /*!
     *  The T move assignment operator.
     *
     *  \param[in] other The T to move.
     *  \return This Optional.
     */
    Optional<T> &operator=(T &&other);

    /*!
     *  The T copy assignment operator.
     *
     *  \param[in] other The T to copy.
     *  \return This Optional.
     */
    Optional<T> &operator=(T const &other);

    /*!
     *  The Optional equality operator.
     *
     *  \tparam U The type of object to compare
     *  \param[in] other The Optional to compare with.
     *  \return Whether the value of this optional matches the other.
     */
    template<typename U>
    bool operator==(Optional<U> const &other) const;

    /*!
     *  The Optional lower than operator.
     *
     *  \tparam U The type of object to compare
     *  \param[in] other The Optional to compare with.
     *  \return Whether the value of this optional is lower than the other.
     */
    template<typename U>
    bool operator<(Optional<U> const &other) const;

    /*!
     *  The Optional greater than operator.
     *
     *  \tparam U The type of object to compare
     *  \param[in] other The Optional to compare with.
     *  \return Whether the value of this optional is greater than the other.
     */
    template<typename U>
    bool operator>(Optional<U> const &other) const;

    /*!
     *  The Optional lower or equal than operator.
     *
     *  \tparam U The type of object to compare
     *  \param[in] other The Optional to compare with.
     *  \return Whether the value of this optional is lower or equal to the other.
     */
    template<typename U>
    bool operator<=(Optional<U> const &other) const;

    /*!
     *  The Optional greater or equal than operator.
     *
     *  \tparam U The type of object to compare
     *  \param[in] other The Optional to compare with.
     *  \return Whether the value of this optional is greater or equal to the other.
     */
    template<typename U>
    bool operator>=(Optional<U> const &other) const;

    /*!
     *  The T equality operator.
     *
     *  \tparam U The type of object to compare
     *  \param[in] other The U to compare with.
     *  \return Whether the value of this optional matches other.
     */
    template<typename U>
    bool operator==(U const &other) const;

    /*!
     *  The T lower than operator.
     *
     *  \tparam U The type of object to compare
     *  \param[in] other The U to compare with.
     *  \return Whether the value of this optional is lower than other.
     */
    template<typename U>
    bool operator<(U const &other) const;

    /*!
     *  The T greater than operator.
     *
     *  \tparam U The type of object to compare
     *  \param[in] other The U to compare with.
     *  \return Whether the value of this optional is greater than other.
     */
    template<typename U>
    bool operator>(U const &other) const;

    /*!
     *  The T lower or equal than operator.
     *
     *  \tparam U The type of object to compare
     *  \param[in] other The U to compare with.
     *  \return Whether the value of this optional is lower or equal to other.
     */
    template<typename U>
    bool operator<=(U const &other) const;

    /*!
     *  The T greater or equal than operator.
     *
     *  \tparam U The type of object to compare
     *  \param[in] other The U to compare with.
     *  \return Whether the value of this optional is greater or equal to other.
     */
    template<typename U>
    bool operator>=(U const &other) const;

    /*!
     * The optional destructor.
     */
    ~Optional();

private:

    /// The optional value. Is nullptr if empty.
    T *value = nullptr;

    /// The allocator used to manage value.
    Allocator<T> allocator = Allocator<T>();

};

#define __OPT_INCL

#include <Utilities/Optional.inl>

#undef __OPT_INCL
