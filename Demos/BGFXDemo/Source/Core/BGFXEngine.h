#pragma once

#include <Core/IMidnightEngine.h>
#include <Core/EngineTypes.h>
#include <Core/ErrorHandler.h>
#include <Core/Logging/Logger.h>
#include <Utilities/FileHelper.h>
#include <Math/MathUtils.h>
#include <vector>
#include <bgfx/bgfx.h>
#include <bgfx/bgfx_utils.h>
#include <UI/PerformanceWindow.h>

class BGFXEngine : public IMidnightEngine {

public:

	BGFXEngine();

	~BGFXEngine();

	virtual int init(int argc, const char** argv) override;
	virtual void loop() override;

	virtual void stop();
	virtual void restart();

	virtual FORCEINLINE double getDeltaTime() const override { return 0.0f; };
	virtual FORCEINLINE double getTime() const override { return 0.0f; };

	std::string getNiceRendererName();
	std::string getNiceGPUName();

	inline static bgfx::RendererType::Enum renderer = bgfx::RendererType::Vulkan;

	FORCEINLINE void selectGPU(uint16_t deviceId, uint16_t vendorId) {
		this->deviceId = deviceId;
		this->vendorId = vendorId;
	}

private:

	void cleanup();

	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;
	int64_t timeOffset;

	uint16_t deviceId = BGFX_PCI_ID_NONE;
	uint16_t vendorId = 0;

	bool shouldRun = true;
	bool shouldRestart = false;

	bgfx::VertexBufferHandle triangleBuffer;
	bgfx::ProgramHandle program;
	PerformanceWindow perfWindow = PerformanceWindow();

};
