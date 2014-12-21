/*
 * highflyers-cpp-protocol
 *     Optional.h
 *
 *  Created on: 11 mar 2014
 *     Author: Marcin Kolny <marcin.kolny@gmail.com>
 */

// this file should be removed in the future. there is std::optional in c++14

#ifndef OPTIONAL_H_
#define OPTIONAL_H_

#include <type_traits>

namespace HighFlyers {
namespace Protocol {

struct	nullopt_t {};

template<typename T>
class Optional
{
private:
	T val;
	bool is_null;

	typedef typename std::remove_const<T>::type Type;
	
public:
	Optional():is_null(true){}
	Optional(const Type& value) : is_null(false), val(value) {}

	T& value() { return val; }
	const T& value() const { return val; }
	T& value_or(T&& value) { return is_null ? value : val; }
	const T& value_or(T&& value) const { return is_null ? value : val; }

	Optional& operator=(nullopt_t)
	{
		is_null = true;
	}
	Optional& operator=(T&& value)
	{
		val = value;

		return *this;
	}

	operator T() const
	{
		return val;
	}

	operator T()
	{
		return val;
	}

	operator bool()
	{
		return !is_null;
	}

	const Type* operator->() const
	{
		return is_null ? nullptr : &val;
	}

	Type* operator->() 
	{
		return is_null ? nullptr : &val;
	}
};

}
}

#endif