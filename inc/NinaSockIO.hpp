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
 * @file NinaSockIO.hpp
 * @brief Common socket Input/Output operations
 * @author Jonathan Calmels
 * @date Fri Jun 10 2011
 */

#ifndef __NINA_SOCKIO_HPP__
# define __NINA_SOCKIO_HPP__

# include "NinaDef.hpp"
# include "NinaSock.hpp"
# include "NinaOS.hpp"

NINA_BEGIN_NAMESPACE_DECL

/*! @class SockIO
 * @brief Common socket Input/Output operations
 *
 * @details This class is intended to wrap common socket I/O operations such as send/recv in a portable manner
 * @todo Enabling timers on common methods :<br/>
 *	If EWOULDBLOCK, try to select again ...<br/>
 *	The select implementation should have a template describing the
 *	method used (epoll, poll, kqueue, select, wfmo)<br/>
 *	(Reactor implementation should be closed to that method)
 */
class NINA_DLLREQ SockIO : public Sock
{
	public:
		enum
		{
			UNCONSUMED = MSG_PEEK, //!< Underlying buffer stay unconsumed after a read operation
			BUF_FULL = MSG_WAITALL, //!< Wait until the buffer is fully filled
			DONT_ROUTE = MSG_DONTROUTE //!< Only use the local network
		};

	public:
		//! @brief Virtual destructor
		virtual ~SockIO() {};

	protected:
		//! @brief Constructor
		SockIO();
		//! @brief Copy constructor
		SockIO(SockIO const& sap);
		//! @brief Assignement operator
		SockIO& operator=(SockIO const& sap);

	public:
		/*!
		 * @brief Receive data from the transport endpoint
		 * @param[out] buf : buffer to be filled
		 * @param[in] bufLen : size of the buffer
		 * @param[in] flags : specify some modes to customize the reception, it is a bit set composed using the following flags : #UNCONSUMED/#BUF_FULL
		 * @return The number of bytes received on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int receive(void* buf, size_t bufLen, uint8_t flags = 0) const;
		/*!
		 * @brief Receive data from the transport endpoint
		 * @param[out] str : string to be filled, it has to be pre-allocated using std::string::reserve
		 * @param[in] flags : specify some modes to customize the reception, it is a bit set composed using the following flags : #UNCONSUMED/#BUF_FULL
		 * @return The number of bytes received on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int receive(std::string& str, uint8_t flags = 0) const;
		/*!
		 * @brief Send data to the transport endpoint
		 * @param[in] buf : buffer to be sent
		 * @param[in] bufLen : size of the buffer
		 * @param[in] flag : specify a mode to customize the sending, it can take the following flag : #DONT_ROUTE
		 * @return The number of bytes sent on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int send(void const* buf, size_t bufLen, uint8_t flag = 0) const;
		/*!
		 * @brief Send data to the transport endpoint
		 * @param[in] str : string to be sent
		 * @param[in] flag : specify a mode to customize the sending, it can take the following flag : #DONT_ROUTE
		 * @return The number of bytes sent on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int send(std::string const& str, uint8_t flag = 0) const;
};

NINA_END_NAMESPACE_DECL

# include "NinaSockIO.inl"

#endif // !__NINA_SOCKIO_HPP__
