//
// LayeredConfiguration.cpp
//
// $Id: //poco/1.3/Util/src/LayeredConfiguration.cpp#2 $
//
// Library: Util
// Package: Configuration
// Module:  LayeredConfiguration
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/Util/LayeredConfiguration.h"
#include "Poco/Exception.h"
#include <set>


using Poco::AutoPtr;
using Poco::RuntimeException;


namespace Poco {
namespace Util {


LayeredConfiguration::LayeredConfiguration()
{
}


LayeredConfiguration::~LayeredConfiguration()
{
}


void LayeredConfiguration::add(AbstractConfiguration* pConfig)
{
	add(pConfig, highest(), false, true);
}


void LayeredConfiguration::add(AbstractConfiguration* pConfig, bool shared)
{
	add(pConfig, highest(), false, shared);
}


void LayeredConfiguration::add(AbstractConfiguration* pConfig, int priority)
{
	add(pConfig, priority, false, true);
}


void LayeredConfiguration::add(AbstractConfiguration* pConfig, int priority, bool shared)
{
	add(pConfig, priority, false, shared);
}


void LayeredConfiguration::addFront(AbstractConfiguration* pConfig)
{
	add(pConfig, lowest(), false, true);
}


void LayeredConfiguration::addFront(AbstractConfiguration* pConfig, bool shared)
{
	add(pConfig, lowest(), false, shared);
}


void LayeredConfiguration::addWriteable(AbstractConfiguration* pConfig, int priority)
{
	add(pConfig, priority, true, true);
}


void LayeredConfiguration::addWriteable(AbstractConfiguration* pConfig, int priority, bool shared)
{
	add(pConfig, priority, true, shared);
}


void LayeredConfiguration::add(AbstractConfiguration* pConfig, int priority, bool writeable, bool shared)
{
	ConfigItem item;
	item.pConfig   = ConfigPtr(pConfig, shared);
	item.priority  = priority;
	item.writeable = writeable;
	
	ConfigList::iterator it = _configs.begin();
	while (it != _configs.end() && it->priority < priority)
		++it;
		
	_configs.insert(it, item);
}


bool LayeredConfiguration::getRaw(const std::string& key, std::string& value) const
{
	for (ConfigList::const_iterator it = _configs.begin(); it != _configs.end(); ++it)
	{
		if (it->pConfig->getRaw(key, value))
			return true;
	}
	return false;
}


void LayeredConfiguration::setRaw(const std::string& key, const std::string& value)
{
	for (ConfigList::iterator it = _configs.begin(); it != _configs.end(); ++it)
	{
		if (it->writeable)
		{
			it->pConfig->setRaw(key, value);
			return;
		}
	}
	throw RuntimeException("No writeable configuration object to store the property", key);
}


void LayeredConfiguration::enumerate(const std::string& key, Keys& range) const
{
	std::set<std::string> keys;
	for (ConfigList::const_iterator itc = _configs.begin(); itc != _configs.end(); ++itc)
	{
		Keys partRange;
		itc->pConfig->enumerate(key, partRange);
		for (Keys::const_iterator itr = partRange.begin(); itr != partRange.end(); ++itr)
		{
			if (keys.find(*itr) == keys.end())
			{
				range.push_back(*itr);
				keys.insert(*itr);
			}
		}
	}
}


int LayeredConfiguration::lowest() const
{
	if (_configs.empty())
		return 0;
	else
		return _configs.front().priority - 1;
}


int LayeredConfiguration::highest() const
{
	if (_configs.empty())
		return 0;
	else
		return _configs.back().priority + 1;
}


} } // namespace Poco::Util
