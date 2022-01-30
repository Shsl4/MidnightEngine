#pragma once

#include <typeinfo>

/*!
 * The default class for all the objects created and used by the engine.
 * All classes but helpers should inherit this class.
 */
class Object {

public:

    virtual ~Object() = default;
    
    /*!
     * Returns the name of the class.
     *
     *  \return The name of the class.
     */
    class String getClassName() const;
    
    /*!
     * Returns a debug description of the object.
     *
     *  \return The object description
     */
    virtual class String getDescription() const;

    /*!
     * Casts this object to the type T.
     */
    template<class T>
    T *cast() {

        return dynamic_cast<T *>(this);

    }

    /*!
     * Checks whether this object inherits T.
     *
     *  \return Whether this object inherits T
     */
    template<class T>
    bool instanceOf() {

        return cast<T>() != nullptr;

    }

};
