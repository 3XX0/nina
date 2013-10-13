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
 * @file NinaSockConnector.hpp
 * @brief Defines a connection wrapper
 * @author Jonathan Calmels
 * @date Tue Aug 30 2011
 */

#ifndef __NINA_SOCKCONNECTOR_HPP__
# define __NINA_SOCKCONNECTOR_HPP__

# include "NinaAddr.hpp"
# include "NinaSockStream.hpp"
# include "NinaSockDatagram.hpp"
# include "NinaTime.hpp"

NINA_BEGIN_NAMESPACE_DECL

/*! @class SockConnector
 * @brief An IPC participant to connect to a remote endpoint
 *
 * @details This class serves as an IPC participant to connect to remote entities<br/>
 * It can be used alone on connection oriented protocols or through a class NINA::Connector as a backend
 */
class NINA_DLLREQ SockConnector
{
	public:
		enum Process
		{
			SHUT_READ = NINA_SHUT_READ, //!< Further receives will be disallowed
			SHUT_WRITE = NINA_SHUT_WRITE, //!< Send all data queued then send a FIN (close the write end of the connection)
			SHUT_ALL = NINA_SHUT_ALL //!< Shutdown the both sides described above, so close the connection
		};

	public:
		//! @brief Constructor
		SockConnector();
		//! @brief Destructor
		~SockConnector();
		//! @brief Copy constructor
		SockConnector(SockConnector const& sap);
		//! @brief Assignement operator
		SockConnector& operator=(SockConnector const& sap);

	public:
		/*!
		 * @brief Start a connection attempt
		 * @details This function is used to connect an I/O handle to a remote peer<br/>
		 * On connection oriented protocols (such as TCP) an handshake will be negociated,
		 * otherwise on connectionless protocols (such as UDP) the kernel will acknowledge the destination
		 * address and copy it so it will not be required anymore<br/>
		 * Note that it may be also necessary to retrieve assynchroneous errors from lost datagrams
		 * @param[in] sock : a reference on a local endpoint we want to connect
		 * @param[in] addr : the address of the remote peer we want to connect to
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int connect(SockIO const& sock, Addr const& addr) const;
		/*!
		 * @brief Disconnect a datagram endpoint
		 * @details Remove previously registered destination address from the kernel (see connect above),
		 * thus it will be necessary to fill the address on all subsequent call to I/O operations
		 * @param[in] sock : a reference on a SockDatagram instance which is "connected"
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int disconnect(SockDatagram const& sock) const;
		/*!
		 * @brief Disconnect a connection oriented endpoint
		 * @details Process to an end-to-end terminaiton according to the flag how<br/>
		 * @param[in] sock : a reference on a SockStream instance which is connected
		 * @param[in] how : the termination manner. Possible values are : #SHUT_READ/#SHUT_WRITE/#SHUT_ALL
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int disconnect(SockStream const& sock, Process how) const;
		/*!
		 * @brief Abort the connection on a connection oriented endpoint
		 * @details Force the connection to abort by sending an RST segment to the remote peer
		 * @param[in] sock : a reference on a SockStream instance which is connected
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int abort(SockStream& sock) const;
		/*!
		 * @brief Check whether an asynchronous connection attempt is completed or not
		 * @details If a connect has been issued on a non blocking handle, the asynchronous connection must be monitored via calls to complete<br/>
		 * A call to complete consists of checking if the connection succeeded during the time specified by timeout.
		 * If not, any other calls to complete may be done as long as the complete function do not report an error
		 * @param[in] sock : a reference on a SockStream instance which is trying to connect asynchronously
		 * @param[in] timeout : time to wait for the completion before giving up
		 * @return 1 on success, 0 if nothing happened or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		int complete(SockStream& sock, Time const* timeout) const;
};

NINA_END_NAMESPACE_DECL

#endif /* !__NINA_SOCKCONNECTOR_HPP__ */
