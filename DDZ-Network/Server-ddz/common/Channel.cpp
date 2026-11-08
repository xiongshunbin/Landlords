#include "Channel.h"

Channel::Channel(int fd, FDEvent events, handleFunc readFunc,
	handleFunc writeFunc, handleFunc destroyFunc, void* arg)
	: m_fd(fd), m_events(static_cast<int>(events)), m_arg(arg), 
	readCallback(readFunc), writeCallback(writeFunc), destroyCallback(destroyFunc)
{
}

Channel::~Channel()
{
}

void Channel::writeEventEnable(bool flag)
{
	if (flag)
	{
		m_events |= static_cast<int>(FDEvent::WriteEvent);
	}
	else
	{
		m_events &= ~static_cast<int>(FDEvent::WriteEvent);
	}
}

void Channel::setCurrentEvent(FDEvent event)
{
	m_events = static_cast<int>(event);
}

bool Channel::isWriteEventEnable() const
{
	return m_events & static_cast<int>(FDEvent::WriteEvent);
}

bool Channel::isReadEventEnable() const
{
	return m_events & static_cast<int>(FDEvent::ReadEvent);
}
