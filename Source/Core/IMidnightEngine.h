#pragma once

#include <Core/EngineTypes.h>
#include <Core/ErrorHandler.h>
#include <Core/Logging/Logger.h>
#include <Utilities/FileHelper.h>
#include <Math/MathUtils.h>
#include <vector>

class IMidnightEngine
{
public:

    IMidnightEngine() {
        
        instance = this;
        logger = std::make_unique<Logger>("MidnightEngine");

    }
    
    virtual int init(int argc, char** argv) = 0;
    virtual void loop() = 0;

    virtual FORCEINLINE double getDeltaTime() const = 0;
    virtual FORCEINLINE double getTime() const = 0;

    FORCEINLINE const Logger* getLogger() const { return logger.get(); };
    FORCEINLINE static IMidnightEngine* getInstance() { return instance; };

private:

    inline static IMidnightEngine* instance = nullptr;
    UniquePtr<Logger> logger;

};
