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
 * @file NinaSock.hpp
 * @brief Global socket manipulations
 * @author Jonathan Calmels
 * @date Fri Jun 10 2011
 */

#ifndef __NINA_SOCK_HPP__
# define __NINA_SOCK_HPP__

# include "NinaDef.hpp"

# if defined (NINA_POSIX)
#  include <netinet/in.h>
#  include <sys/socket.h>
# endif // !NINA_POSIX
# if defined (NINA_WIN32)
#  include <winsock2.h>
# endif // !NINA_WIN32

# include "NinaSAP.hpp"
# include "NinaAddr.hpp"

NINA_BEGIN_NAMESPACE_DECL

/*! @class Sock
 * @brief Global socket manipulations
 *
 * @details This class is designed to provide an interface to sub classes so as they can operate on the service access point<br/>
 * It also encapsulates general socket options in a portable manner
 */
class NINA_DLLREQ Sock : public SAP
{
	public:
		enum
		{
			ANY_PROTO = 0 //!< Specify any protocol
		};

	public:
		//! @brief Virtual destructor
		virtual ~Sock();

	protected:
		//! @brief Constructor
		Sock();
		//! @brief Copy constructor
		Sock(Sock const& sap);
		//! @brief Assignement operator
		Sock& operator=(Sock const& sap);
	
	public:
		/*!
		 * @brief Open and initialize a new transport endpoint
		 * @param[in] addr : address on which the socket will be bound or Addr::sapAny if binding is not suitable
		 * @param[in] family : socket family to set up
		 * @param[in] protocol : set a predefined protocol, #ANY_PROTO for a specific definiton by the operating system
		 * @param[in] sockType : defines the socket type to use
		 * @param[in] reuseAddr : set the transport endpoint to be reusable on the same port
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int open(Addr const& addr, int family, int protocol, int sockType, bool reuseAddr = false);
		/*!
		 * @brief Open and initialize a new transport endpoint
		 * @throw NINA::Error::SystemError if an error occured
		 * @param[in] addr : address on which the socket will be bound or Addr::sapAny if binding is not suitable
		 * @param[in] family : socket family to set up
		 * @param[in] protocol : set a predefined protocol, "any" for a specific definiton by the operating system
		 * @param[in] sockType : defines the socket type to use
		 * @param[in] reuseAddr : set the transport endpoint to be reusable on the same port
		 */
		void open(Addr const& addr, int family, std::string const& protocol, int sockType, bool reuseAddr = false);
		/*!
		 * @brief Close an existing endpoint
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int close();
		/*!
		 * @brief Get the socket options
		 * @param[in] level : specifies the protocol level at which the option resides (SOL_SOCKET/IPPROTO_IP/IPPROTO_TCP ...)
		 * @param[in] opt : specifies a single option to be retrieved
		 * @param[out] buf : user defined buffer, depending on the option
		 * @param[in, out] bufLen : a pointer to the size of the user defined buffer<br/>
		 * Note that this size could be modified if it not scale the amount requested
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int getOption(int level, int opt, void* buf, int* bufLen) const;
		/*!
		 * @brief Set the socket options
		 * @param[in] level : specifies the protocol level at which the option resides (SOL_SOCKET/IPPROTO_IP/IPPROTO_TCP ...)
		 * @param[in] opt : specifies a single option to be retrieved
		 * @param[in] buf : user defined buffer, depending on the option
		 * @param[in] bufLen : size of the user defined buffer<br/>
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int setOption(int level, int opt, void const* buf, int bufLen) const;
		/*!
		 * @brief Get the address currently bound to the transport endpoint
		 * @param[out] sap : reference on an address to fill
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int getLocalAddr(Addr& sap) const;
		/*!
		 * @brief Get the remote address to which the transport endpoint is currently connected
		 * @param[out] sap : reference on an address to fill
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int getRemoteAddr(Addr& sap) const;
	private:
		/*!
		 * @brief Retrieve the family used by an address
		 * @param[in] addr : address concerned
		 * @return The family on success or -1 on error
		 */
		int retrieveFamily(sockaddr_storage const* addr) const;
};

NINA_END_NAMESPACE_DECL

#endif // !__NINA_SOCK_HPP__
