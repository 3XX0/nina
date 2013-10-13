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
 * @file NinaSockAcceptor.hpp
 * @brief Defines a wrapper to a passive socket
 * @author Jonathan Calmels
 * @date Tue Aug 23 2011
 */

#ifndef __NINA_SOCKACCEPTOR_HPP__
# define __NINA_SOCKACCEPTOR_HPP__

# include "NinaSock.hpp"
# include "NinaSockStream.hpp"

NINA_BEGIN_NAMESPACE_DECL

/*! @class SockAcceptor
 * @brief An IPC participant to accept incoming connections
 *
 * @details This class serves as an IPC participant to acknowledge incoming connections<br/>
 * It can be used alone on connection oriented protocols or through a class NINA::Acceptor as a backend
 */ 
class NINA_DLLREQ SockAcceptor : public	Sock
{
	public:
		//! Address class to operate on peers
		typedef InetAddr PeerAddr;

	public:
		//! @brief Constructor
		SockAcceptor();
		//! @brief Destructor
		~SockAcceptor();
		//! @brief Copy constructor
		SockAcceptor(SockAcceptor const& sap);
		//! @brief Assignement operator
		SockAcceptor& operator=(SockAcceptor const& sap);
	
	public:
		/*!
		 * @brief Open the handle allowing to manage incoming connections
		 * @param[in] addr : the address which will listen for incoming connections
		 * @param[in] family : family used by the handle
		 * @param[in] protocol : protocol used by the handle
		 * @param[in] reuseAddr : specify if the binding will be reusable before the 2MSL
		 * @param[in] backlog : specify the backlog used by the SockAcceptor (length of the pending connections queue)
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int open(Addr const& addr, int family, int protocol, bool reuseAddr = false, int backlog = SOMAXCONN);
		/*!
		 * @brief Open the handle allowing to manage incoming connections
		 * @throw NINA::Error::SystemError if an error occured
		 * @param[in] addr : the address which will listen for incoming connections
		 * @param[in] family : family used by the handle
		 * @param[in] protocol : protocol used by the handle
		 * @param[in] reuseAddr : specify if the binding will be reusable before the 2MSL
		 * @param[in] backlog : specify the backlog used by the SockAcceptor (length of the pending connections queue)
		 */
		void open(Addr const& addr, int family, std::string const& protocol, bool reuseAddr = false, int backlog = SOMAXCONN);
		/*!
		 * @brief Accept an incoming connection
		 * @param[out] sock : a reference on a SockStream instance where the accepted peer handle will be stored
		 * @param[out] peerAddr : if specified, it is filled in with the address of the connecting entity
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int accept(SockStream& sock, Addr* peerAddr = 0);
};

NINA_END_NAMESPACE_DECL

#endif /* !__NINA_SOCKACCEPTOR_HPP__ */
