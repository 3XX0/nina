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
 * @file NinaSAP.hpp
 * @brief Service access point, encapsulates transport endpoint
 * @author Jonathan Calmels
 * @date Tue Jun 07 2011
 */

#ifndef __NINA_SAP_HPP__
# define __NINA_SAP_HPP__

# include "NinaDef.hpp"
# include "NinaTypes.hpp"

NINA_BEGIN_NAMESPACE_DECL

// Forward declaration
template <class IPC_STREAM>
class EventHandlerAdapter;

/*! @class SAP
 * @brief Base of socket classes
 *
 * @details This class is the base class of the socket hierarchy<br/>
 * Its encapsulates the transport endpoint of a connection and its general options
 */
class NINA_DLLREQ SAP
{
	// Requires friendship because of the use of copy constructor and equality operator
	// in order to copy the concret SAP into the EventHandlerAdapter
	template <class IPC_STREAM>
	friend class EventHandlerAdapter;

	public:
		enum
		{
			NON_BLOCK = 1, //!< Non blocking transport endpoint
			SIGNAL_DRIVEN = 2, //!< Signals handling (SIGIO)
			CLOSE_ON_EXEC = 4, //!< Duplicates the transport endpoint on exec() calls
			OWNERSHIP = 8 //!< Ownership for signals handling (SIGIO/SIGURG)
		};

	public:
		//! @brief Virtual destructor
		virtual ~SAP() {};

	protected:
		//! @brief Constructor
		SAP();
		//! @brief Copy constructor
		SAP(SAP const& sap);
		//! @brief Assignement operator
		SAP& operator=(SAP const& sap);
	
	public:
		/*!
		 * @brief Enable an option on the transport endpoint
		 * @param opt : It is a bit set composed using the following flag : #NON_BLOCK/#SIGNAL_DRIVEN/#CLOSE_ON_EXEC/#OWNERSHIP
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int	enable(uint8_t opt);
		/*!
		 * @brief Disable an option on the transport endpoint
		 * @param opt : It is a bit set composed using the following flag : #NON_BLOCK/#SIGNAL_DRIVEN/#CLOSE_ON_EXEC/#OWNERSHIP
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int disable(uint8_t opt);
		/*!
		 * @brief Interface which serves ioctl() backend
		 * @param[in] cmd : ioctl command
		 * @param[in,out] args : a pointer to an ioctl argument
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 * @see <a href="http://linux.die.net/man/2/ioctl">ioctl POSIX man page</a>
		 * @see <a href="http://msdn.microsoft.com/en-us/library/ms738573(v=vs.85).aspx">Miscrosoft Windows ioctlsocket</a>
		 */
		int control(int cmd, void* args);
		//! @brief Get the underlying transport endpoint
		//! @return A transport endpoint or NINA::NINA_INVALID_HANDLE on error
		NINAHandle getHandle() const;
		//! @brief Set the underlying transport endpoint
		//! @param[in] handle : the transport endpoint to set up
		void setHandle(NINAHandle handle);
	
	protected:
		NINAHandle	mTransportEndpoint; //!< Concrete transport endpoint
	private:
		int			mIOFlags; //!< IO options cache
		int			mClFlag; //!< Close option cache
};

NINA_END_NAMESPACE_DECL

# include "NinaSAP.inl"

#endif // !__NINA_SAP_HPP__
