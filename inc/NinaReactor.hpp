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
 * @file NinaReactor.hpp
 * @brief Nina reactor interface using the bridge pattern and a policy system
 * @author Jonathan Calmels
 * @date Mon Jul 25 2011
 */

#ifndef __NINA_REACTOR_HPP__
# define __NINA_REACTOR_HPP__

# include "NinaDef.hpp"
# include "NinaCppUtils.hpp"
# include "NinaTypes.hpp"
# include "NinaTime.hpp"
# include "NinaSystemError.hpp"
# include "NinaEventHandler.hpp"

NINA_BEGIN_NAMESPACE_DECL

/*! @struct Events
 * @brief Events associated with the NINA::Reactor class
 *
 * @details These events are used in a bit set mask style in order to inform the dispatcher
 * with the events we are intersted in
 */ 
struct NINA_DLLREQ Events
{
	enum Type
	{
		NONE = 0, //!< No events
		ACCEPT = 1, //!< Event of connection acceptance
		READ = 1, //!< Event of incoming data
		WRITE = 2, //!< Event of outgoing data
		URGENT = 4, //!< Out of band data (protocol specific)
		SIGNAL = 8, //!< Interrupted by a signal (always enabled)
		TIME_OUT = 16, //!< Time out reached (always enabled)
		ALL = 7 //!< All the events above
	};
};

NINA_END_NAMESPACE_DECL

# include "NinaSelectPolicy.hpp"
# include "NinaPollPolicy.hpp"

# if defined (NINA_HAS_KQUEUE)
# include "NinaKqueuePolicy.hpp"
# endif // !NINA_HAS_KQUEUE
# if defined (NINA_HAS_EPOLL)
# include "NinaEpollPolicy.hpp"
#endif // !NINA_HAS_EPOLL

NINA_BEGIN_NAMESPACE_DECL

/*! @class Reactor
 * @brief Dispatch events to appropriate EventHandlers
 *
 * @details This class is one of the main classes used in event driven environements<br/>
 * It provides a dispatcher allowing users to register their handles and wait for their triggering<br/>
 * When an handle is triggered, its associated EventHandler calls the appropriate method to manage the evenement occurred
 * @arg SYNC_POLICY : represents the backend used by the Reactor (see NINA::PollPolicy, NINA::EpollPolicy, NINA::KqueuePolicy, NINA::SelectPolicy)
 */ 
template <class SYNC_POLICY = NINADefaultPolicy>
class Reactor : public Singleton<Reactor<SYNC_POLICY> >
{
	public:
		//! Constructor
		Reactor();
		//! Destructor
		~Reactor();

	public:
		/*!
		 * @brief Register an EventHandler to the reactor
		 * @details After that operation, events on eHandler handle described by eType will be monitored by a call to handleEvents[Loop]<br/>
		 * Here the handle is obtained by a double dispatching, namely that EventHandler::getHandle has been called
		 * @param[in] eHandler : EventHandler that will be used to get the handle and process the events
		 * @param[in] eType : types of events to monitor, it is a bit set composed using the NINA::Events flags
		 * @return 0 on success or -1 on error<br/>
		 * Depending on the policy used, errno could be set to provide more information about the error
		 */
		int registerHandler(EventHandler* eHandler, uint16_t eType);
		/*!
		 * @brief Register an EventHandler to the reactor
		 * @details After that operation, events on handle described by eType will be monitored by a call to handleEvents[Loop]<br/>
		 * Here the handle is obtained by a user paramater. Indeed, it allows an application to register 
		 * the same EventHandler for multiple handles which may help to conserve memory
		 * @param[in] handle : handle to monitor
		 * @param[in] eHandler : EventHandler that will be used to process the events
		 * @param[in] eType : types of events to monitor, it is a bit set composed using the NINA::Events flags
		 * @return 0 on success or -1 on error<br/>
		 * Depending on the policy used, errno could be set to provide more information about the error
		 */
		int registerHandler(NINAHandle handle, EventHandler* eHandler, uint16_t eType);
		/*!
		 * @brief Remove an EventHandler from the reactor
		 * @details After that operation, events on eHandler handle described by eType will be suspended and ignored by a call to handleEvents[Loop]<br/>
		 * Here the handle is obtained by a double dispatching, namely that EventHandler::getHandle has been called
		 * @param[in] eHandler : EventHandler that will be used to get the handle and process the events
		 * @param[in] eType : types of events to suspend, it is a bit set composed using the NINA::Events flags
		 * @return 0 on success or -1 on error<br/>
		 * Depending on the policy used, errno could be set to provide more information about the error
		 */
		int removeHandler(EventHandler* eHandler, uint16_t eType);
		/*!
		 * @brief Remove an EventHandler from the reactor
		 * @details After that operation, events on handle described by eType will be suspended and ignored by a call to handleEvents<br/>
		 * @param[in] handle : handle to suspend
		 * @param[in] eType : types of events to suspend, it is a bit set composed using the NINA::Events flags
		 * @return 0 on success or -1 on error<br/>
		 * Depending on the policy used, errno could be set to provide more information about the error
		 */
		int removeHandler(NINAHandle handle, uint16_t eType);
		/*!
		 * @brief Monitor registered handles and dispatch an event
		 * @details Wait that an event occurs, then call the appropriate method on the EventHandler concerned<br/>
		 * If no event occurred until the timeout is reached, EventHandler::handleTimeout is dispatched to all EventHandlers registered<br/>
		 * If a signal is caught during the processing, EventHandler::handleSignal is dispatched to all EventHandlers registered
		 * @param[in] timeout : time before giving up the monitoring
		 * @arg 0 = wait forever
		 * @arg Time::timeNull = effect a polling and return immediately
		 * @return 0 on success or -1 on error<br/>
		 * If an error occurred errno will be set accordingly
		 */
		int handleEvents(Time const* timeout);
		//! @brief Monitor registered handles and dispatch events until an error occurred on the Reactor (see handleEvents above)
		//! @details Also print the errors NINA::Error::SystemError on the error output which occurred while handling events, useful for the Acceptor pattern with THROW_ON_ERR enabled
		void handleEventsLoop();

	private:
		ReactorImplement* mReactImplement; //!< Real reactor implementation (policy)
};

NINA_END_NAMESPACE_DECL

# include "NinaReactor.imp"
# include "NinaReactor.inl"

#endif // !__NINA_REACTOR_HPP__
