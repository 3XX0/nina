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
 * @file NinaAcceptor.hpp
 * @brief Defines the acceptor design pattern
 * @author Jonathan Calmels
 * @date Tue Aug 23 2011
 */

#ifndef __NINA_ACCEPTOR_HPP__
# define __NINA_ACCEPTOR_HPP__ 

# include "NinaDef.hpp"
# include "NinaEventHandler.hpp"
# include "NinaSockAcceptor.hpp"
# include "NinaReactor.hpp"
# include "NinaSystemError.hpp"
# include "NinaSAP.hpp"

NINA_BEGIN_NAMESPACE_DECL

/*! @class Acceptor
 * @brief Accept incoming connections and start the appropriate service
 *
 * @details This class accepts all incoming connections then calls the SVC_HANDLER::init function<br/>
 * It permits a separation between connection process and the services provided by the application
 * @arg SVC_HANDLER : service launched at the acceptance (see NINA::ServiceHandler)
 * @arg IPC_ACCEPTOR : underlying IPC class used by the Acceptor
 * @arg SYNC_POLICY : policy used by the dispatcher (see NINA::Reactor)
 * @throw NINA::Error::SystemError if THROW_ON_ERR is true and an error occured while accepting
 * (note that if an exception occured, it will be thrown during the event loop see: Reactor::handleEvents)
 */ 
template <class SVC_HANDLER, class IPC_ACCEPTOR = SockAcceptor,
		 class SYNC_POLICY = NINADefaultPolicy, bool THROW_ON_ERR = true>
class Acceptor : public EventHandler
{
	public:
		//! Adress class associated with the IPC_ACCEPTOR<br/>
		//! Note that the name _Addr isn't trivial, in fact all address classes descend from NINA::Addr
		typedef typename IPC_ACCEPTOR::PeerAddr _Addr;

	public:
		/*! @brief Constructor
		 * @details Constructor of the class Acceptor
		 * @param[in] reactor : a pointer on the dispatcher used to retrieve socket events
		 * @throw std::invalid_argument if the reactor is invalid
		 */
		Acceptor(Reactor<SYNC_POLICY>* reactor);
		//! @brief Virtual destructor
		virtual ~Acceptor();
		//! @brief Copy constructor
		Acceptor(Acceptor const& acceptor);
		//! @brief Assignment operator
		Acceptor& operator=(Acceptor const& acceptor);

	public:
		//! @brief Handle read events on the underlying socket
		virtual int handleRead(NINAHandle handle);
		//! @brief Handle write events on the underlying socket
		virtual int handleWrite(NINAHandle) {return 0;};
		//! @brief Handle urgent events on the underlying socket
		virtual int handleUrgent(NINAHandle) {return 0;};
		//! @brief Handle timeout events on the underlying socket
		virtual int handleTimeout(NINAHandle) {return 0;};
		//! @brief Handle signal events on the underlying socket
		virtual int handleSignal(NINAHandle) {return 0;};
		//! @brief Handle close events on the underlying socket
		//! @throw NINA::Error::SystemError if THROW_ON_ERR is true
		virtual int handleClose(NINAHandle handle);
		//! @brief Get the underlying handle to process double dispatching
		virtual NINAHandle getHandle() const;
		/*!
		 * @brief Open the IPC_ACCEPTOR and initialize the Acceptor
		 * @param[in] addr : the address which will listen for incoming connections
		 * @param[in] sockNonBlock : set the IPC_ACCEPTOR in non blocking mode
		 * @param[in] family : family used by the IPC_ACCEPTOR
		 * @param[in] protocol : protocol used by the IPC_ACCEPTOR
		 * @param[in] reuseAddr : specify if the binding will be reusable before the 2MSL
		 * @param[in] backlog : specify the backlog used by the Acceptor (length of the pending connections queue)
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		virtual int open(_Addr const& addr, bool sockNonBlock = false, int family = PF_UNSPEC,
				int protocol = IPC_ACCEPTOR::ANY_PROTO, bool reuseAddr = false, int backlog = SOMAXCONN);
		/*!
		 * @brief Accept an incoming connection and initialize the SVC_HANDLER if it succeed
		 * @return 0 on success or -1 on error<br/>
		 * If an error occurred errno will be set accordingly apart from the case where SVC_HANDLER::init produces the error
		 */
		virtual int accept();
		/*!
		 * @brief Suspend the acceptation of connections
		 * @remark This function removes all the Acceptor registered events (see EventHandler::handleRead)
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		virtual int suspend();
		/*!
		 * @brief Resume the acceptation of connections
		 * @return 0 on success or -1 on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		virtual int resume();	
	protected:
		//! @brief SVC_HANDLER factory method
		virtual SVC_HANDLER* makeServiceHandler() const;
		//! @brief Accept a connection and set the SVC_HANDLER handle
		virtual int acceptServiceHandler(SVC_HANDLER* serviceHandler);
		//! @brief Initialize the SVC_HANDLER
		virtual int initServiceHandler(SVC_HANDLER* serviceHandler) const;

	private:
		IPC_ACCEPTOR			mIPCAcceptor; //!< IPC_ACCEPTOR used by the Acceptor
		bool					mIsSuspended; //!< Suspend/Resume flag
		Reactor<SYNC_POLICY>*	mReactor; //!< Dispatcher used to retrieve events
};

NINA_END_NAMESPACE_DECL

# include "NinaAcceptor.imp"
# include "NinaAcceptor.inl"

#endif /* !__NINA_ACCEPTOR_HPP__ */
