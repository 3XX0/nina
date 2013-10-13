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
 * @file NinaServiceHandler.hpp
 * @brief Defines an half of an end-to-end service
 * @author Jonathan Calmels
 * @date Sat Sep 03 2011
 */

#ifndef __NINA_SERVICEHANDLER_HPP__
# define __NINA_SERVICEHANDLER_HPP__

# include "NinaDef.hpp"
# include "NinaEventHandler.hpp"
# include "NinaSockStream.hpp"

NINA_BEGIN_NAMESPACE_DECL

/*! @class ServiceHandler
 * @brief An abstract class describing a service, useful for networked patterns
 *
 * @details This class describes a service, it participates to the Acceptor pattern 
 * and may be used in the Reactor pattern dynamics<br/>
 * It is necessary to inherit from that class to implement a concret service handler
 * @arg IPC_STREAM : concrete IPC stream providing an endpoint to the service
 */
template <class IPC_STREAM = SockStream>
class ServiceHandler : public EventHandler
{
	public:
		//! Address class related to the IPC_STREAM<br/>
		//! Note that the name _Addr isn't trivial, in fact all address classes descend from NINA::Addr
		typedef typename IPC_STREAM::PeerAddr _Addr;

	public:
		//! @brief Virtual destructor
		virtual ~ServiceHandler() {};
	protected:
		//! @brief Destructor
		ServiceHandler();
		//! @brief Copy constructor
		ServiceHandler(ServiceHandler const& service);
		//! @brief Assignement operator
		ServiceHandler& operator=(ServiceHandler const& service);

	public:
		virtual int handleRead(NINAHandle) {return 0;};
		virtual int handleWrite(NINAHandle) {return 0;};
		virtual int handleUrgent(NINAHandle) {return 0;};
		virtual int handleTimeout(NINAHandle) {return 0;};
		virtual int handleSignal(NINAHandle) {return 0;};
		virtual int handleClose(NINAHandle) {return 0;};
		virtual NINAHandle getHandle() const;
		/*!
		 * @brief Initialize the service
		 * @details This function is used by the Acceptor pattern to start a new service<br/>
		 * This function is dedicated to the end user so that it can do specific manipulations on service initialization
		 * @return 0 on success, -1 on error<br/>
		 * Note that it is important to respect these values.<br/>Indeed, the Acceptor pattern uses the return value to
		 * judge whether an acceptation succeeded and has started a new service or if it has failed and must be aborted
		 */
		virtual int init() = 0;
		//! @brief Get the underlying IPC stream
		//! @return A reference on an IPC_STREAM instance
		IPC_STREAM& getPeer();
		//! @brief Get the remote peer address we are connected to
		//! @return A reference on an address instance associated to IPC_STREAM
		_Addr& getRemoteAddr();

	private:
		IPC_STREAM	mIPCStream; //!< Service endpoint
		_Addr		mIPCAddr; //!< Hold the remote peer address which is connected to our service endpoint
};

NINA_END_NAMESPACE_DECL

# include "NinaServiceHandler.imp"
# include "NinaServiceHandler.inl"

#endif /* !__NINA_SERVICEHANDLER_HPP__ */
