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
     *  @return The name of the class.
     */
    const char *getClassName() const {

        return typeid(*this).name();

    }

    /*!
     * Returns a debug description of the object.
     *
     *  @return The object description
     *
     */
    virtual const char *getDescription() const {

        return getClassName();

    }

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
     *  @return Whether this object inherits T
     */
    template<class T>
    bool instanceOf() {

        return cast<T>() != nullptr;

    }

};
