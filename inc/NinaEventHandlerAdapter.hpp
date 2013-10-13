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
 * @file NinaEventHandlerAdapter.hpp
 * @brief Defines a class useful to handle events with functions pointers
 * @author Jonathan Calmels
 * @date Mon Jul 25 2011
 */

#ifndef __NINA_EVENTHANDLERADAPTER_HPP__
# define __NINA_EVENTHANDLERADAPTER_HPP__

# include "NinaEventHandler.hpp"
# include "NinaSock.hpp"

NINA_BEGIN_NAMESPACE_DECL

/*! @class EventHandlerAdapter
 * @brief Reactor event handling interface
 *
 * @details This class does essentially the same job as the EventHandler class, however
 * it is not necessary to inherit from that class to implement a concret event handler.<br/>
 * Indeed, by supplying IPC_STREAM you specify the class which will act as the transport endpoint (see NINA::SockStream / NINA::SockDatagram)<br/>
 * All event handlers are then set up by user's handling functions by calling set*Handler()
 */
template <class IPC_STREAM>
class EventHandlerAdapter : public EventHandler
{
	private:
		//! @brief User's handler definition
		//! @details A user should respect this prototype to bind its handling function to the EventHandlerAdapter
		typedef int (*Handler)(Sock& handle);

	public:
		/*!
		 * @brief Constructor
		 * @throw NINA::Error::SystemError if an error occured
		 * @param[in] addr : address on which the underlying IPC_STREAM will be bound or Addr::sapAny if binding is not suitable
		 * @param[in] family : IPC_STREAM family to set up
		 * @param[in] protocol : set a predefined protocol, Sock::ANY_PROTO for a specific definiton by the operating system
		 * @param[in] reuseAddr : set the transport endpoint to be reusable on the same port
		 */
		EventHandlerAdapter(Addr const& addr, int family, int protocol, bool reuseAddr = false);
		/*!
		 * @brief Constructor
		 * @throw NINA::Error::SystemError if an error occured
		 * @param[in] addr : address on which the underlying IPC_STREAM will be bound or Addr::sapAny if binding is not suitable
		 * @param[in] family : IPC_STREAM family to set up
		 * @param[in] protocol : set a predefined protocol, "any" for a specific definiton by the operating system
		 * @param[in] reuseAddr : set the transport endpoint to be reusable on the same port
		 */
		EventHandlerAdapter(Addr const& addr, int family, std::string const& protocol, bool reuseAddr = false);
		//! @brief Destructor
		~EventHandlerAdapter();
		//! @brief Copy constructor
		EventHandlerAdapter(EventHandlerAdapter const& eha);
		//! @brief Assignement operator
		EventHandlerAdapter& operator=(EventHandlerAdapter const& eha);

	public:
		//! @brief Bind a user's handling function to receive read events (see handleRead)
		//! @param[in] readHandler : a function pointer on a user's handler
		void setReadHandler(Handler readHandler);
		//! @brief Bind a user's handling function to receive write events (see handleWrite)
		//! @param[in] writeHandler : a function pointer on a user's handler
		void setWriteHandler(Handler writeHandler);
		//! @brief Bind a user's handling function to receive urgent events (see handleUrgent)
		//! @param[in] urgentHandler : a function pointer on a user's handler
		void setUrgentHandler(Handler urgentHandler);
		//! @brief Bind a user's handling function to receive timeout events (see handleTimeout)
		//! @param[in] timeoutHandler : a function pointer on a user's handler
		void setTimeoutHandler(Handler timeoutHandler);
		//! @brief Bind a user's handling function to receive signal events (see handleSignal)
		//! @param[in] signalHandler : a function pointer on a user's handler
		void setSignalHandler(Handler signalHandler);
		//! @brief Bind a user's handling function to receive close events (see handleClose)
		//! @param[in] closeHandler : a function pointer on a user's handler
		void setCloseHandler(Handler closeHandler);
		virtual int handleRead(NINAHandle handle);
		virtual int handleWrite(NINAHandle handle);
		virtual int handleUrgent(NINAHandle handle);
		virtual int handleTimeout(NINAHandle handle);
		virtual int handleSignal(NINAHandle handle);
		virtual int handleClose(NINAHandle handle);
		virtual NINAHandle getHandle() const;
		//! @brief Get the underlying IPC stream
		//! @return A reference on an IPC_STREAM instance
		IPC_STREAM& getPeer();

	private:
		Handler 	mReadHandler; //! Read handler pointer
		Handler 	mWriteHandler; //! Write handler pointer
		Handler 	mUrgentHandler; //! Urgent handler pointer
		Handler 	mTimeoutHandler; //! Timeout handler pointer
		Handler 	mSignalHandler; //! Signal handler pointer
		Handler 	mCloseHandler; //! Close handler pointer
		IPC_STREAM	mIPCStream; //!< Service endpoint
};

NINA_END_NAMESPACE_DECL

# include "NinaEventHandlerAdapter.imp"
# include "NinaEventHandlerAdapter.inl"

#endif // !__NINA_EVENTHANDLERADAPTER_HPP__
