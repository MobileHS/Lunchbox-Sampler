#include "event_sytem.hpp"

using namespace lbs;

EventSystem::EventSystem()
{
	/* we can't call nullptr in the consumer so init to real void func! */
	for( int i = static_cast<int>(Events::DIGITAL::ROTARY_L); static_cast<Events::DIGITAL>(i) < Events::DIGITAL::LAST; i++ ) {
		m_digMapping[ static_cast<Events::DIGITAL>(i) ] = []() { };
	}
	
	for( int i = static_cast<int>(Events::Analog::POTS::POT_0); static_cast<Events::Analog::POTS>(i) < Events::Analog::POTS::LAST; i++ ) {
		m_analogMapping[ static_cast<Events::Analog::POTS>(i) ] = []( AnalogData ) { };
	}
	
	for( int i = static_cast<int>(Events::Analog::FSR::FSR_0); static_cast<Events::Analog::FSR>(i) < Events::Analog::FSR::LAST; i++ ) {
		m_fsrMapping[ static_cast<Events::Analog::FSR>(i) ] = []( AnalogData ) { };
	}
}

void EventSystem::enqueueDigital( Events::DIGITAL e )
{
	m_scheduler.m_digitalListener.send( m_digMapping.find( e )->second );
}

void EventSystem::enqueueAnalog( Events::Analog::POTS e, AnalogData value )
{
	auto f = m_analogMapping.find( e )->second;
	std::function< void() > binder = std::bind( m_analogMapping.find( e )
	                                                           ->second, value );
	m_scheduler.m_analogListener.send( binder );
}

void EventSystem::enqueueAnalog( Events::Analog::FSR e, AnalogData value )
{
	auto f = m_fsrMapping.find( e )->second;
	std::function< void() > binder = std::bind( m_fsrMapping.find( e )
	                                                        ->second, value );
	m_scheduler.m_analogListener.send( binder );
}

void EventSystem::attachDigital( Events::DIGITAL e, std::function< void() > f )
{
	m_digMapping[ e ] = std::move( f );
}

void EventSystem::detachDigital( Events::DIGITAL e )
{
	m_digMapping[ e ] = []() { };
}

void EventSystem::attachAnalog( Events::Analog::POTS e, std::function< void( AnalogData ) > f )
{
	m_analogMapping[ e ] = std::move( f );
}

void EventSystem::detachAnalog( Events::Analog::POTS e )
{
	m_analogMapping[ e ] = []( AnalogData ) { };
}

void EventSystem::attachAnalog( Events::Analog::FSR e, std::function< void( AnalogData ) > f )
{
	m_fsrMapping[ e ] = std::move( f );
}

void EventSystem::detachAnalog( Events::Analog::FSR e )
{
	m_fsrMapping[ e ] = []( AnalogData ) { };
}