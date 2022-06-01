#pragma once

/*!
 * \brief The reference counter class used by SharedPointer and WeakPointer.
 */
class ENGINE_API RefCounter final {
        
public:

    RefCounter() = default;

    ~RefCounter() = default;
        
    RefCounter(const RefCounter& other) = delete;
        
    RefCounter(RefCounter&& other) noexcept = delete;
        
    RefCounter& operator=(const RefCounter& other) = delete;

    RefCounter& operator=(RefCounter&& other) = delete;
    
    FORCEINLINE void increase() noexcept { ++this->ref; }
        
    FORCEINLINE void decrease() noexcept { --this->ref; }

    FORCEINLINE void increaseWeak() noexcept { ++this->weak; }
        
    FORCEINLINE void decreaseWeak() noexcept { --this->weak; }
        
    FORCEINLINE void reset() noexcept { this->ref = 0; this->weak = 0; }
        
    FORCEINLINE size_t refCount() const noexcept { return this->ref; }
    
    FORCEINLINE size_t weakCount() const noexcept { return this->weak; }

    FORCEINLINE bool exhausted() const noexcept { return this->ref == 0; }
    
    FORCEINLINE bool exhaustedWeak() const noexcept { return this->weak == 0; }

private:
        
    size_t ref = 0;
    size_t weak = 0;
        
};