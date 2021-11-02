#pragma once

#include <Core/IMidnightEngine.h>

class SDLEngine : public IMidnightEngine {

public:

    virtual int init(int argc, char** argv) override;
    virtual void loop() override;

    virtual FORCEINLINE double getDeltaTime() const override { return 0.0f; }
    virtual FORCEINLINE double getTime() const override { return 0.0f; }

};
