#include <UI/PerformanceWindow.h>

#include <bx/timer.h>
#include <bx/math.h>
#include <bx/string.h>

PerformanceWindow::SampleData::SampleData() {
    reset();
}

void PerformanceWindow::SampleData::reset() {
    m_offset = 0;
    bx::memSet(m_values, 0, sizeof(m_values));

    m_min = 0.0f;
    m_max = 0.0f;
    m_avg = 0.0f;
}

void PerformanceWindow::SampleData::pushSample(float value) {
    m_values[m_offset] = value;
    m_offset = (m_offset + 1) % kNumSamples;

    float min = bx::kFloatMax;
    float max = -bx::kFloatMax;
    float avg = 0.0f;

    for (UInt32 ii = 0; ii < kNumSamples; ++ii) {
        const float val = m_values[ii];
        min = bx::min(min, val);
        max = bx::max(max, val);
        avg += val;
    }

    m_min = min;
    m_max = max;
    m_avg = avg / kNumSamples;
}

bool PerformanceWindow::bar(float _width, float _maxWidth, float _height, const ImVec4 &_color) {
    const ImGuiStyle &style = ImGui::GetStyle();

    ImVec4 hoveredColor(
            _color.x + _color.x * 0.1f, _color.y + _color.y * 0.1f, _color.z + _color.z * 0.1f, _color.w + _color.w * 0.1f
    );

    ImGui::PushStyleColor(ImGuiCol_Button, _color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoveredColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, _color);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, style.ItemSpacing.y));

    bool itemHovered = false;

    ImGui::Button("", ImVec2(_width, _height));
    itemHovered |= ImGui::IsItemHovered();

    ImGui::SameLine();
    ImGui::InvisibleButton("", ImVec2(bx::max(1.0f, _maxWidth - _width), _height));
    itemHovered |= ImGui::IsItemHovered();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);

    return itemHovered;
}

void PerformanceWindow::resourceBar(const char *_name, const char *_tooltip, UInt32 _num, UInt32 _max, float _maxWidth, float _height) {
    bool itemHovered = false;

    ImGui::Text("%s: %4d / %4d", _name, _num, _max);
    itemHovered |= ImGui::IsItemHovered();
    ImGui::SameLine();

    const float percentage = float(_num) / float(_max);

    itemHovered |= bar(bx::max(1.0f, percentage * _maxWidth), _maxWidth, _height, s_resourceColor);
    ImGui::SameLine();

    ImGui::Text("%5.2f%%", percentage * 100.0f);

    if (itemHovered) {
        ImGui::SetTooltip("%s %5.2f%%", _tooltip, percentage * 100.0f
        );
    }
}

void PerformanceWindow::render(const char *_errorText) {

    ImGui::SetNextWindowPos(
            ImVec2(10.0f, 15.0f), ImGuiCond_FirstUseEver
    );
    ImGui::SetNextWindowSize(
            ImVec2(250.0f, 140.0f), ImGuiCond_FirstUseEver
    );

    ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);

    ImGui::Begin("Performance", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    ImGui::PopStyleColor();

    if (NULL != _errorText) {
        const Int64 now = bx::getHPCounter();
        const Int64 freq = bx::getHPFrequency();
        const float time = float(now % freq) / float(freq);

        bool blink = time > 0.5f;

        ImGui::PushStyleColor(ImGuiCol_Text, blink
                ? ImVec4(1.0, 0.0, 0.0, 1.0)
                : ImVec4(1.0, 1.0, 1.0, 1.0)
        );
        ImGui::TextWrapped("%s", _errorText);
        ImGui::Separator();
        ImGui::PopStyleColor();
    }

    const bgfx::Stats *stats = bgfx::getStats();
    const double toMsCpu = 1000.0 / stats->cpuTimerFreq;
    const double toMsGpu = 1000.0 / stats->gpuTimerFreq;
    const double frameMs = double(stats->cpuTimeFrame) * toMsCpu;

    s_frameTime.pushSample(float(frameMs));

    char frameTextOverlay[256];
    bx::snprintf(frameTextOverlay, BX_COUNTOF(frameTextOverlay), "%s%.3fms, %s%.3fms\nAvg: %.3fms, %.1f FPS", ICON_FA_ARROW_DOWN, s_frameTime.m_min, ICON_FA_ARROW_UP, s_frameTime.m_max, s_frameTime.m_avg, 1000.0f / s_frameTime.m_avg
    );

    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImColor(0.0f, 0.5f, 0.15f, 1.0f).Value);
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::PlotHistogram("", s_frameTime.m_values, SampleData::kNumSamples, s_frameTime.m_offset, frameTextOverlay, 0.0f, 60.0f, ImVec2(0.0f, 45.0f)
    );
    ImGui::PopStyleColor();

    ImGui::Text("Submit CPU %0.3f, GPU %0.3f (L: %d)", double(stats->cpuTimeEnd - stats->cpuTimeBegin) * toMsCpu, double(stats->gpuTimeEnd - stats->gpuTimeBegin) * toMsGpu, stats->maxGpuLatency
    );

    if (-INT64_MAX != stats->gpuMemoryUsed) {
        char tmp0[64];
        bx::prettify(tmp0, BX_COUNTOF(tmp0), stats->gpuMemoryUsed);

        char tmp1[64];
        bx::prettify(tmp1, BX_COUNTOF(tmp1), stats->gpuMemoryMax);

        ImGui::Text("GPU mem: %s / %s", tmp0, tmp1);
    }

    ImGui::End();
}
