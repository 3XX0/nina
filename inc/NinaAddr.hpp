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
 * @file NinaAddr.hpp
 * @brief Defines the base system of addressing
 * @author Jonathan Calmels
 * @date Mon Jun 06 2011
 */

#ifndef __NINA_ADDR_HPP__
# define __NINA_ADDR_HPP__

#include <cstddef>
# include "NinaDef.hpp"

NINA_BEGIN_NAMESPACE_DECL

/*! @class Addr
 * @brief Base of addressing classes
 *
 * @details This class is the base class of the addressing hierarchy<br/>
 * It encapsulates the size and the family of an address
 */
class NINA_DLLREQ Addr
{
	protected:
		enum
		{
			AF_ANY = -1 //!< Any type of addresses
		};

	public:
		//! @brief Virtual Destructor
		virtual ~Addr() {};
	protected:
		/*! @brief Constructor
		 * @details Constructor of the class Addr
		 * @param[in] family : the address family, types supported are AF_INET/AF_INET6/AF_UNSPEC
		 * @param[in] size : size of address container
		 */
		Addr(int family = AF_ANY, int size = -1);
		//! @brief Copy constructor
		Addr(Addr const& sap);
		//! @brief Assignement operator
		Addr& operator=(Addr const& sap);

	public:
		//! @brief Check for address equality
		bool operator==(Addr const& sap) const;
		//! @brief Check for address inequality
		bool operator!=(Addr const& sap) const;
		//! @brief Get the family address
		//! @return Address family or -1 on error
		int getFamily() const;
		//! @brief Set the family address
		//! @param[in] family : the address family, types supported are AF_INET/AF_INET6/AF_UNSPEC
		void setFamily(int family);
		//! @brief Get the size of address container
		//! @return Size of address container or -1 on error
		int getSize() const;
		//! @brief Set the size of address container
		//! @param[in] size : size of address container
		void setSize(int size);
		//! @brief Get a pointer on the concrete address
		//! @return A pointer on the concrete address or 0 on error
		virtual void const* getAddr() const;
		/*!
		 * @brief Set a pointer on the concrete address
		 * @param[in] addr : a pointer on the concrete address
		 * @param[in] addrLen : size of the address pointed by addr
		 * @return 0 on success or -1 on error 
		 */
		virtual int setAddr(void* addr, size_t addrLen);
		//! @brief Reset the address to its default values
		virtual void reset();

	public:
		static Addr const sapAny; //!< A wilcard instance of Addr, used to avoid socket binding
	protected:
		int		mFamily; //!< Address family
		int		mSize; //!< Address size
};

NINA_END_NAMESPACE_DECL

# include "NinaAddr.inl"

#endif // !__NINA_ADDR_HPP__
