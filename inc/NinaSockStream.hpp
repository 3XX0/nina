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
 * @file NinaSockStream.hpp
 * @brief Stream bytes based socket operations
 * @author Jonathan Calmels
 * @date Sat Jun 11 2011
 */

#ifndef __NINA_SOCKSTREAM_HPP__
# define __NINA_SOCKSTREAM_HPP__

# include "NinaSockIO.hpp"
# include "NinaIOContainer.hpp"
# include "NinaInetAddr.hpp"

NINA_BEGIN_NAMESPACE_DECL

/*! @class SockStream
 * @brief Stream bytes based socket operations
 *
 * @details This class is intended to wrap socket stream I/O operations in a portable manner<br/>
 * It provides especially certain specificities such as :<br/>
 * @arg Sending/receiving out of band datas
 * @arg Bringing scatter read and gather write methods to reduce overhead caused by syscalls
 * @todo Enabling timers on common methods :<br/>
 *	If EWOULDBLOCK, try to select again ...<br/>
 *	The select implementation should have a template describing the
 *	method used (epoll, poll, kqueue, select, wfmo)<br/>
 *	(Reactor implementation should be closed to that method)
 */
class NINA_DLLREQ SockStream : public SockIO
{
	public:
		//! Defines the prefered class used for addressing 
		typedef InetAddr PeerAddr;

	public:
		//! @brief Constructor
		SockStream();
		//! @brief Destructor
		~SockStream();
		//! @brief Copy constructor
		SockStream(SockStream const& sap);
		//! @brief Assignement operator
		SockStream& operator=(SockStream const& sap);

	public:
		/*!
		 * @brief Open and initialize a new transport endpoint
		 * @param[in] addr : address on which the socket will be bound or Addr::sapAny if binding is not suitable
		 * @param[in] family : socket family to set up
		 * @param[in] protocol : set a predefined protocol, #ANY_PROTO for a specific definiton by the operating system
		 * @param[in] reuseAddr : set the transport endpoint to be reusable on the same port
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int open(Addr const& addr, int family, int protocol = IPPROTO_TCP, bool reuseAddr = false);
		/*!
		 * @brief Open and initialize a new transport endpoint
		 * @details Throw an exception (NINA::Error::SystemError) if an error occured
		 * @param[in] addr : address on which the socket will be bound or Addr::sapAny if binding is not suitable
		 * @param[in] family : socket family to set up
		 * @param[in] protocol : set a predefined protocol, "any" for a specific definiton by the operating system
		 * @param[in] reuseAddr : set the transport endpoint to be reusable on the same port
		 */
		void open(Addr const& addr, int family, std::string const& protocol, bool reuseAddr = false);
		/*!
		 * @brief Send an urgent stream to the transport endpoint, it will be treated as out of band data
		 * @param[in] buf : buffer to be sent
		 * @param[in] bufLen : size of the buffer
		 * @param[in] flag : specifies additional option @see SockIO::send
		 * @return The number of bytes sent on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int urgentSend(void const* buf, size_t bufLen, uint8_t flag = 0) const;
		/*!
		 * @brief Send an urgent stream to the transport endpoint, it will be treated as out of band data
		 * @param[in] str : string to be sent
		 * @param[in] flag : specifies additional option @see SockIO::send
		 * @return The number of bytes sent on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int urgentSend(std::string const& str, uint8_t flag = 0) const;
		/*!
		 * @brief Receive an urgent stream from the transport endpoint, it will be treated as out of band data
		 * @param[out] buf : buffer to be filled
		 * @param[in] bufLen : size of the buffer
		 * @param[in] flags : specifies additional options @see SockIO::receive
		 * @return The number of bytes received on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int urgentReceive(void* buf, size_t bufLen, uint8_t flags = 0) const;
		/*!
		 * @brief Receive an urgent stream from the transport endpoint, it will be treated as out of band data
		 * @param[out] str : string to be filled, it has to be pre-allocated using std::string::reserve
		 * @param[in] flags : specifies additional options @see SockIO::receive
		 * @return The number of bytes received on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int urgentReceive(std::string& str, uint8_t flags = 0) const;
		/*!
		 * @brief Send exactly bufLen bytes to the transport endpoint
		 * @param[in] buf : buffer to be sent
		 * @param[in] bufLen : size of the buffer
		 * @param[in] flag : specifies additional option @see SockIO::send
		 * @return bufLen on success, the number of bytes sent until the window can't accept the data anymore or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int exactSend(void const* buf, size_t bufLen, uint8_t flag = 0) const;
		/*!
		 * @brief Send the string entirely to the transport endpoint
		 * @param[in] str : string to be sent
		 * @param[in] flag : specifies additional option @see SockIO::send
		 * @return size of the string on success, the number of bytes sent until the window can't accept the data anymore or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int exactSend(std::string const& str, uint8_t flag = 0) const;
		/*!
		 * @brief Receive exactly bufLen bytes from the transport endpoint
		 * @param[out] buf : buffer to be filled
		 * @param[in] bufLen : size of the buffer
		 * @param[in] flags : specifies additional options @see SockIO::receive (BUF_FULL is implicit, see below)
		 * @return bufLen on success, the number of bytes received until an EOF occured or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 * @remark This function is equivalent to SockIO::receive using the BUF_FULL flag
		 */
		int exactReceive(void* buf, size_t bufLen, uint8_t flags = 0) const;
		/*!
		 * @brief Receive exactly the string capacity from the transport endpoint
		 * @param[out] str : string to be filled, it has to be pre-allocated using std::string::reserve
		 * @param[in] flags : specifies additional options @see SockIO::receive (BUF_FULL is implicit, see below)
		 * @return the string capacity (see std::string::capacity) on success, the number of bytes received until an EOF occured or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 * @remark This function is equivalent to SockIO::receive using the BUF_FULL flag
		 */
		int exactReceive(std::string& str, uint8_t flags = 0) const;
		/*!
		 * @brief Send a stream from multiples buffers to the transport endpoint
		 * @param[in] ioc : the IOContainer previously filled
		 * @return The number of bytes sent on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int gatherWrite(IOContainer& ioc) const;
		/*!
		 * @brief Receive a stream from the transport endpoint and store it into multiples buffers
		 * @param[out] ioc : the IOContainer to be filled
		 * @param[in] flags : specifies additional options @see SockIO::receive<br/>
		 * @warning The parameter flag is only meaningful on WIN32 plateforms
		 * @return The number of bytes received on success, 0 on EOF or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int scatterRead(IOContainer& ioc, uint8_t flags = 0) const;
};

NINA_END_NAMESPACE_DECL

# include "NinaSockStream.inl"

#endif // !__NINA_SOCKSTREAM_HPP__
