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
 * @file NinaIOContainer.hpp
 * @brief Input/Output container
 * @author Jonathan Calmels
 * @date Wed Jun 08 2011
 */

#ifndef __NINA_IOCONTAINER_HPP__
# define __NINA_IOCONTAINER_HPP__

# include "NinaDef.hpp"

# if defined (NINA_WIN32)
// Disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#  pragma warning(disable: 4251)
// Disable warnings on extern before template instantiation
#  pragma warning(disable: 4231)
# endif // !NINA_WIN32

# if defined (NINA_POSIX)
#  include <sys/uio.h>
# endif // !NINA_POSIX
# if defined (NINA_WIN32)
#  include <winsock2.h>
# endif // !NINA_WIN32

# include <vector>
# include <utility>
# include "NinaOS.hpp"

// STL forward declaration as expected for dll processing
// see http://support.microsoft.com/kb/168958/en-us
// Note that it may be not require because STL members are private here
NINA_EXTERN template struct NINA_DLLREQ std::pair<void*, size_t>;
NINA_EXTERN template class NINA_DLLREQ std::vector<std::pair<void*, size_t> >;

NINA_BEGIN_NAMESPACE_DECL

/*! @class IOContainer
 * @brief Input/Output container
 *
 * @details This class is a wrapper to manipulate scatter read and gather write datas<br/>
 * It stores a vector of buffer associated with their size and provides useful overloaded operators
 * to manipulate it
 */
class NINA_DLLREQ IOContainer
{
	friend int OS::scatterRead(NINAHandle sock, IOContainer& ioc, unsigned long* flags);
	friend int OS::gatherWrite(NINAHandle sock, IOContainer& ioc, unsigned long flags);	

	public:
		//! Pair which wrap buffers informations
		typedef std::pair<void*, size_t> IOPair;
	private:
		//! Vector of #IOPair
		typedef std::vector<IOPair> IOVector;

	public:
		//! @brief Constructor
		IOContainer();
		//! @brief Destructor
		~IOContainer();
		//! @brief Copy constructor
		IOContainer(IOContainer const& ioc);
		//! @brief Assignement operator
		IOContainer& operator=(IOContainer const& ioc);
	
	public:
		/*!
		 * @brief Get a reference to the #IOPair at the index specified
		 * @details If the IOPair doesn't exist, the container won't be affected
		 * @param[in] idx : index of the element wanted
		 * @return A reference on an #IOPair
		 */
		IOPair& operator[](size_t idx);
		//! @brief Push an #IOPair into the container
		//! @param[in] iop : pair to be pushed
		void operator<<(IOPair const& iop);
		/*!
		 * @brief Retrieve the first #IOPair from the container
		 * @details After this operation, the pair will no longer exist in the container
		 * @param[out] iop : a reference on the pair to fill in
		 */
		void operator>>(IOPair& iop);
		//! @brief Get the size of the container
		//! @return Size of the container
		size_t getSize() const;
		//! @brief Remove the #IOPair at the index specified
		//! @param[in] idx : index of the element to remove
		void remove(size_t idx);
		//! @brief Clear the container, removing all the elements
		void clear();
	private:
# if defined (NINA_WIN32)
		//! @brief Get the underlying I/O vector
		//! @return A pointer on a LPWSABUF
		LPWSABUF operator()();
# else
		//! @brief Get the underlying I/O vector
		//! @return A pointer on a iovec structure
		iovec const* operator()();
# endif // !NINA_WIN32
		//! @brief Clean the underlying I/O vector after a call to operator()
		void cleanup();

	private:
		IOVector	mContainer; //!< Stores I/O buffers associated with their size
# if defined (NINA_WIN32)
		LPWSABUF	mConcreteContainer; //!< Microsoft Windows concrete I/O vector
# else
		iovec*		mConcreteContainer; //!< POSIX concrete I/O vector
# endif // !NINA_WIN32
};

NINA_END_NAMESPACE_DECL

# include "NinaIOContainer.inl"

#endif // !__NINA_IOCONTAINER_HPP__
