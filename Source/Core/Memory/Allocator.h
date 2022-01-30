#pragma once

#include <Core/EngineTypes.h>
#include <Core/Object.h>

/*!
 *  An object allowing to manage memory for objects of type T.
 *  The object is only one byte as it does not have any member variables.
 *
 *  \tparam T Type of object to manage
 */
template<typename T>
class Allocator : public Object {

public:

    /*!
     * The default constructor.
     */
    Allocator();

    /*!
     *  Constructs an object of type T.
     *
     *  \tparam Args A variadic template.
     *  \param[in] args The arguments to pass to the constructor.
     *  \return The newly constructed object.
     */
    template<typename ... Args>
    T *construct(Args &&... args);

    /*!
     *  Allocates a buffer containing \p size number of objects of type T.
     *  If T is a class, objects are constructed using the empty constructor.
     *
     *  \warning If T is a class, it must have a public empty constructor.
     *
     *  \param[in] size The size of the buffer.
     *  \return The newly allocated buffer.
     */
    T *alloc(size_t size);

    /*!
     *  Destroys a single object allocated with construct.
     *
     *  \param[in,out] pointer The object to release
     */
    void destroy(T *&pointer);

    /*!
     *  Destroys a single object allocated with construct.
     *
     *  \tparam U The type of object to destroy.
     *  \param[in,out] pointer The object to release
     */
    template<typename U>
    void autoDestroy(U *&pointer);
    
    /*!
     *  Releases a buffer allocated with allocate.
     *
     *  \param[in,out] pointer The buffer to release.
     */
    void release(T *&pointer);

    /*!
     *  Extends or shrinks a memory buffer.
     *
     *  \param[in,out] pointer The buffer to reallocate.
     *  \param[in] oldSize The old size of the buffer.
     *  \param[in] size The new size of the buffer.
     */
    void realloc(T *&pointer, size_t oldSize, size_t size);

    /*!
     * The Allocator destructor. It only serves debugging purposes.
     */
    ~Allocator();

    /*!
     *  Moves memory forwards or backwards automatically.
     *
     *  \param[in] from Where to start reading data
     *  \param[in] to Where to stop reading data
     *  \param[out] dest Where move the data.
     */
    void move(T *from, T *to, T *dest);

    /*!
     *  Copies memory forwards or backwards automatically.
     *
     *  \param[in] from Where to start reading data
     *  \param[in] to Where to stop reading data
     *  \param[out] dest Where copy the data.
     */
    void copy(const T *from, const T *to, T *dest);

    /*!
     * Swaps the contents of the two pointers by moving (no copying).
     */
    void swapMove(T *a, T *b);

private:

    /*!
     *  Moves memory forward.
     *
     *  \param[in] from Where to start reading data
     *  \param[in] to Where to stop reading data
     *  \param[out] dest Where move the data.
     */
    FORCEINLINE void moveForward(T *from, T *to, T *dest);

    /*!
     *  Moves memory backwards.
     *
     *  \param[in] from Where to start reading data
     *  \param[in] to Where to stop reading data
     *  \param[out] dest Where move the data.
     */
    FORCEINLINE void moveBack(T *from, T *to, T *dest);

    /*!
     *  Copies memory forwards.
     *
     *  \param[in] from Where to start reading data
     *  \param[in] to Where to stop reading data
     *  \param[out] dest Where copy the data.
     */
    FORCEINLINE void copyForward(const T *from, const T *to, T *dest);

    /*!
     *  Copies memory backwards.
     *
     *  \param[in] from Where to start reading data
     *  \param[in] to Where to stop reading data
     *  \param[out] dest Where copy the data.
     */
    FORCEINLINE void copyBack(const T *from, const T *to, T *dest);

};

#ifdef DEBUG

#include <Logging/Logger.h>

namespace DebugAlloc {

    struct AllocInfo {

        int64_t ratio() const{
            return allocCount - releaseCount;
        }

        uint32_t constructCount = 0;
        uint32_t allocCount = 0;
        uint32_t releaseCount = 0;
        uint32_t reallocCount = 0;

    };

    const static inline Logger allocLogger = Logger("DebugAlloc");
    static inline AllocInfo info = AllocInfo();

    static void stat(){

        if (info.ratio() && (info.constructCount == 0)) {

            allocLogger.warning("Unbalanced allocator ratio! {} objects may have been leaked.");
        
        }

        allocLogger.debug("Total allocs: {}", info.allocCount);
        allocLogger.debug("Total releases: {}", info.releaseCount);
        allocLogger.debug("Total reallocs: {}", info.reallocCount);
        allocLogger.debug("Total instances left: {}", info.constructCount);

    }

}

#endif

#define __ALLOC_INCL

#include <Memory/Allocator.inl>

#undef __ALLOC_INCL
