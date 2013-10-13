// Copyright (c) 2011, Jonathan Calmels <jbjcalmels@gmail.com>
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

/*!
 * @file NinaCppUtils.hpp
 * @brief C++ Utilities, provides some useful patterns for Nina classes
 * @author Jonathan Calmels
 * @date Tue Jun 07 2011
 */

#ifndef	__NINA_CPPUTILS_HPP__
# define __NINA_CPPUTILS_HPP__

# include <typeinfo>
# include "NinaDef.hpp"

NINA_BEGIN_NAMESPACE_DECL

/*! @class NonCopyable
 * @brief Defines a pattern which forbade the copy of a class
 *
 * @details Prevent the copy by privatizing the copy constructor and the assignement operator<br/>
 * Classes which has to be non copyable, then inherit from that class
 */
class NINA_DLLREQ NonCopyable
{
	protected:
		//! @brief Constructor
		NonCopyable() {}
		//! @brief Virtual destructor
		virtual ~NonCopyable() {}
	private:
		//! @brief Copy constructor
		NonCopyable(NonCopyable const& other);
		//! @brief Assignement operator
		NonCopyable& operator=(NonCopyable const& other);
};

/*! @class Singleton
 * @brief Defines the singleton pattern
 *
 * @details The singleton pattern restricts the instantiation of a class to one object<br/>
 * Classes which has to be unique, then inherit from that class
 */
template <class TYPE>
class Singleton : public NonCopyable
{
	public:
		//! @brief Constructor
		Singleton()
		{
			NINA_ASSERT_SINGLETON(!msSingleton, typeid(TYPE).name(), " in constructor");
			msSingleton = static_cast<TYPE*>(this);
		}
		//! @brief Destructor
		~Singleton()
		{
			NINA_ASSERT_SINGLETON(msSingleton, typeid(TYPE).name(), " in destructor");
			msSingleton = 0;
		}

	public:
		/*!
		 * @brief Get a reference on the object wrapped
		 * @details An assertion is done checking if the singleton conditions are respected<br/>
		 * If not, an abort is raised @see NINA_ASSERT_SINGLETON
		 * @return A reference to an object
		 */
		static TYPE& getSingleton()
		{
			NINA_ASSERT_SINGLETON(msSingleton, typeid(TYPE).name(), " in getSingleton");
			return *msSingleton;
		}
		/*!
		 * @brief Get a pointer on the object wrapped
		 * @details An assertion is done checking if the singleton conditions are respected<br/>
		 * If not, an abort is raised @see NINA_ASSERT_SINGLETON
		 * @return A pointer to an object
		 */
		static TYPE* getSingletonPtr()
		{
			NINA_ASSERT_SINGLETON(msSingleton, typeid(TYPE).name(), " in getSingletonPtr");
			return msSingleton;
		}

	protected: 
		static TYPE* msSingleton; //!< Static instance of a top class
};

template <class TYPE> TYPE* Singleton<TYPE>::msSingleton;

NINA_END_NAMESPACE_DECL

#endif // ! __NINA_CPPUTILS_HPP__
