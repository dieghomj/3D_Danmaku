#pragma once

class CTime
{
public:
    CTime();
    ~CTime();

    bool Init(double fps); // fps is the desired *fixed* update rate (e.g., 60.0)

    // New: Calculate the time accumulated for fixed updates
    void Tick();

    // Corrected: Returns true if a fixed step *should* occur.
    // The main loop will call this repeatedly until it returns false.
    bool FixedTick(double& fixedDeltaTime);

    // Standard variable time update for rendering
    void VariableTick();

    // Getters for external use
    float GetDeltaTime() const { return deltaTime; }
    double GetTotalTime() const { return m_totalTime; }

private:
    float deltaTime;        // Variable time (for rendering/animation)
    double m_totalTime;     // Total elapsed time

    LARGE_INTEGER m_lastTime;   // Last recorded counter value
    LARGE_INTEGER m_qpFreq;     // Performance counter frequency

    // Fixed Time Step Variables
    double m_fixedStepDuration; // The target time for a single fixed update (e.g., 16.666 ms)
    double m_timeAccumulator;   // Time accumulated since the last fixed update
};