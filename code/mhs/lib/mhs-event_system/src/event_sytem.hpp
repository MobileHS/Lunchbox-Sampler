#pragma once
#include <map>
#include <functional>
#include <cstdio>
#include "scheduler.hpp"
namespace mhs
{
    class EventSystem
    {
    public:
        class Events
        {
        public:
            enum class DIGITAL
            {
                ROTARY_L,
                ROTARY_R,
                BTN_ENTER,
                BTN_RETURN,
                BTN_TOGGLE,
                MIDI_IN,
            };
            enum class ANALOG
            {
                POT0 = 0,
                POT1 = 1,
                POT2 = 2,
                POT3 = 3,

                FSR0 = 0,
                FSR1 = 1,
                FSR2 = 2,
                FSR3 = 3
            };

        };

        struct EventInfo
        {
            u_int16_t pin;
            Events event;
        };

        EventSystem() = default;

        void attach( u_int16_t p, const Events &e, std::function<void()> f );

        void attach( EventInfo eventInfo, std::function<void()> f );

        void detach( EventInfo eventInfo );

        void detach( u_int16_t p );

    protected:
        friend class InterruptHandler;
        void enqueueDigital( Events::DIGITAL e);
        void enqueueAnalog( Events::ANALOG e);

    private:
        Scheduler scheduler;
        std::map<Events::DIGITAL, std::function<void()>> m_digMapping;
        std::map<Events::ANALOG, std::function<void()>> m_analogMapping;
    };
}