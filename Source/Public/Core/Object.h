#pragma once

#include <typeinfo>

#include <Core/EngineMacros.h>
#include <Memory/String.h>

/*!
 * \brief The default class for all the objects created and used by the engine.
 * All classes but helpers should inherit this class.
 */
class ENGINE_API Object {

public:

    virtual ~Object() = default;
    
    /*!
     * Returns the name of the class.
     *
     *  \return The name of the class.
     */
    NODISCARD class String getClassName() const;
    
    /*!
     * Returns a debug description of the object.
     *
     *  \return The object description
     */
    NODISCARD virtual class String getDescription() const;

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
    bool inherits() {

        return cast<T>() != nullptr;

    }

    template<class T>
    static bool isObject(T* object)
    {
        return dynamic_cast<Object*>(object);
    }

};
