/**
 *	Copyright 2009-2014 Barrett Technology <support@barrett.com>
 *
 *	This file is part of libbarrett.
 *
 *	This version of libbarrett is free software: you can redistribute it
 *	and/or modify it under the terms of the GNU General Public License as
 *	published by the Free Software Foundation, either version 3 of the
 *	License, or (at your option) any later version.
 *
 *	This version of libbarrett is distributed in the hope that it will be
 *	useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License along
 *	with this version of libbarrett.  If not, see
 *	<http://www.gnu.org/licenses/>.
 *
 *
 *	Barrett Technology Inc.
 *	73 Chapel Street
 *	Newton, MA 02458
 *
 */

/**
 * @file os.h
 * @date 03/28/2012
 * @author Dan Cody
 *
 */

#ifndef BARRETT_DETAIL_OS_H_
#define BARRETT_DETAIL_OS_H_

#include <boost/format.hpp>
#include <string>

namespace barrett {
namespace detail {

class LogFormatter : public boost::format {
  public:
	LogFormatter(const std::string &fmt, bool outputToStderr)
	    : boost::format(fmt), ose(outputToStderr), printed(false) {}
	~LogFormatter() { print(); }

	template <typename ExceptionType> void raise(bool alsoPrint = false) {
		if (alsoPrint) {
			print();
		} else {
			printed = true;
		}

		// It's necessary to do this in a member function because it's unsafe to
		// throw exceptions from a dtor.
		throw ExceptionType(str());
	}

	// Reimplemented in order to return LogFormatter instead of boost::format
	template <class T> LogFormatter &operator%(const T &x) {
		boost::format::operator%(x); // Call super
		return *this;
	}

  protected:
	void print();

	bool ose;
	bool printed;
};
}
}

#endif /* BARRETT_DETAIL_OS_H_ */
