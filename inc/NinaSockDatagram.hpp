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
 * @file NinaSockDatagram.hpp
 * @brief Datagrams based socket operations
 * @author Jonathan Calmels
 * @date Wed Oct 19 2011
 */

#ifndef __NINA_SOCKDATAGRAM_HPP__
# define __NINA_SOCKDATAGRAM_HPP__

# include "NinaSockIO.hpp"

NINA_BEGIN_NAMESPACE_DECL

/*! @class SockDatagram
 * @brief Datagram based socket operations
 *
 * @details This class is intended to wrap datagram socket I/O operations in a portable manner
 */
class NINA_DLLREQ SockDatagram : public SockIO
{
	public:
		//! @brief Constructor
		SockDatagram();
		//! @brief Destructor
		~SockDatagram();
		//! @brief Copy constructor
		SockDatagram(SockDatagram const& sap);
		//! @brief Assignement operator
		SockDatagram& operator=(SockDatagram const& sap);

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
		int open(Addr const& addr, int family, int protocol = IPPROTO_UDP, bool reuseAddr = false);
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
		 * @brief Receive data from the transport endpoint and store the remote peer address
		 * @param[out] buf : buffer to be filled
		 * @param[in] bufLen : size of the buffer
		 * @param[out] peerAddr : if specified, it is filled in with the address of the sending entity
		 * @param[in] flags : specify some modes to customize the reception, it is a bit set composed using the following flags : #UNCONSUMED/#BUF_FULL
		 * @return The number of bytes received on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int receiveFromPeer(void* buf, size_t bufLen, Addr* peerAddr = 0, uint8_t flags = 0) const;
		/*!
		 * @brief Receive data from the transport endpoint and store the remote peer address
		 * @param[out] str : string to be filled, it has to be pre-allocated using std::string::reserve
		 * @param[out] peerAddr : if specified, it is filled in with the address of the sending entity
		 * @param[in] flags : specify some modes to customize the reception, it is a bit set composed using the following flags : #UNCONSUMED/#BUF_FULL
		 * @return The number of bytes received on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int receiveFromPeer(std::string& str, Addr* peerAddr = 0, uint8_t flags = 0) const;
		/*!
		 * @brief Send data to the transport endpoint at the address specified
		 * @param[in] buf : buffer to be sent
		 * @param[in] bufLen : size of the buffer
		 * @param[in] peerAddr : the address of the peer which will receive datas
		 * @param[in] flag : specify a mode to customize the sending, it can take the following flag : #DONT_ROUTE
		 * @return The number of bytes sent on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int sendToPeer(void const* buf, size_t bufLen, Addr const& peerAddr, uint8_t flag = 0) const;
		/*!
		 * @brief Send data to the transport endpoint at the address specified
		 * @param[in] str : string to be sent
		 * @param[in] peerAddr : the address of the peer which will receive datas
		 * @param[in] flag : specify a mode to customize the sending, it can take the following flag : #DONT_ROUTE
		 * @return The number of bytes sent on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int sendToPeer(std::string const& str, Addr const& peerAddr, uint8_t flag = 0) const;
		/*!
		 * @brief Send exactly bufLen bytes to the transport endpoint at the address specified
		 * @param[in] buf : buffer to be sent
		 * @param[in] bufLen : size of the buffer
		 * @param[in] peerAddr : the address of the peer which will receive datas
		 * @param[in] flag : specifies additional option @see SockIO::send
		 * @return bufLen on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int exactSendToPeer(void const* buf, size_t bufLen, Addr const& peerAddr, uint8_t flag = 0) const;
		/*!
		 * @brief Send the string entirely to the transport endpoint at the address specified
		 * @param[in] str : string to be sent
		 * @param[in] peerAddr : the address of the peer which will receive datas
		 * @param[in] flag : specifies additional option @see SockIO::send
		 * @return size of the string on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int exactSendToPeer(std::string const& str, Addr const& peerAddr, uint8_t flag = 0) const;
		/*!
		 * @brief Receive exactly bufLen bytes from the transport endpoint and store the remote peer address
		 * @param[out] buf : buffer to be filled
		 * @param[in] bufLen : size of the buffer
		 * @param[out] peerAddr : if specified, it is filled in with the address of the sending entity
		 * @param[in] flags : specifies additional options @see SockIO::receive (BUF_FULL is implicit, see below)
		 * @return bufLen on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 * @remark This function is equivalent to receiveFromPeer using the BUF_FULL flag
		 */
		int exactReceiveFromPeer(void* buf, size_t bufLen, Addr* peerAddr = 0, uint8_t flags = 0) const;
		/*!
		 * @brief Receive exactly the string capacity from the transport endpoint and store the remote peer address
		 * @param[out] str : string to be filled, it has to be pre-allocated using std::string::reserve
		 * @param[out] peerAddr : if specified, it is filled in with the address of the sending entity
		 * @param[in] flags : specifies additional options @see SockIO::receive (BUF_FULL is implicit, see below)
		 * @return the string capacity (see std::string::capacity) on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 * @remark This function is equivalent to receiveFromPeer using the BUF_FULL flag
		 */
		int exactReceiveFromPeer(std::string& str, Addr* peerAddr = 0, uint8_t flags = 0) const;
};

NINA_END_NAMESPACE_DECL

# include "NinaSockDatagram.inl"

#endif /* !__NINA_SOCKDATAGRAM_HPP__ */
