#pragma once

#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/bgfx.h>
#include <imgui/imgui.h>
#include <iconfontheaders/icons_kenney.h>
#include <iconfontheaders/icons_font_awesome.h>
#include <Core/Logging/Logger.h>

class PerformanceWindow {

public:

	struct SampleData
	{

		static constexpr uint32_t kNumSamples = 100;

		SampleData();

		void reset();

		void pushSample(float value);

		int32_t m_offset;
		float m_values[kNumSamples];

		float m_min;
		float m_max;
		float m_avg;

	};

	void render(const char* _errorText);

private:


	bool bar(float _width, float _maxWidth, float _height, const ImVec4& _color);

	void resourceBar(const char* _name, const char* _tooltip, uint32_t _num, uint32_t _max, float _maxWidth, float _height);

	bool s_showStats = false;
	SampleData s_frameTime;
	const ImVec4 s_resourceColor = { 0.5f, 0.5f, 0.5f, 1.0f };
	Logger logger = Logger("PerformanceWindow");

};