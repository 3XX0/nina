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
 * @file NinaEventHandler.hpp
 * @brief Reactor event handling interface
 * @author Jonathan Calmels
 * @date Tue Jun 07 2011
 */

#ifndef __NINA_EVENTHANDLER_HPP__
# define __NINA_EVENTHANDLER_HPP__

# include "NinaTypes.hpp"
# include "NinaCppUtils.hpp"

NINA_BEGIN_NAMESPACE_DECL

/*! @class EventHandler
 * @brief Reactor event handling interface
 *
 * @details This class describes the principals methods used to handle events
 * product by a reactor<br/>
 * It is necessary to inherit from that class to implement a concret event handler
 */
class NINA_DLLREQ EventHandler
{
	protected:
		//! @brief Virtual destructor
		virtual ~EventHandler() {}

	public:
		/*!
		 * @brief Handle a read event such as data ready to be received, errors or connection terminaison
		 * @return 0 on success, 1 if the handling of event leads to the removal of all currently registered events or -1 on error (see handleClose above)<br/>
		 * Note that it is important to respect these values.<br/> Indeed, the Reactor pattern and its policies use the return value to do
		 * special manipulations such as discarding subsequent events in order to avoid memory corruptions
		 */
		virtual int handleRead(NINAHandle handle) = 0;
		/*!
		 * @brief Handle a write event such as data ready to be sent
		 * @return 0 on success, 1 if the handling of event leads to the removal of all currently registered events or -1 on error (see handleClose above)<br/>
		 * Note that it is important to respect these values.<br/> Indeed, the Reactor pattern and its policies use the return value to do
		 * special manipulations such as discarding subsequent events in order to avoid memory corruptions
		 */
		virtual int handleWrite(NINAHandle handle) = 0;
		/*! 
		 * @brief Handle out of band data (protocol specific)
		 * @warning Currently not supported by the NINA::KqueuePolicy
		 * @return 0 on success, 1 if the handling of event leads to the removal of all currently registered events or -1 on error (see handleClose above)<br/>
		 * Note that it is important to respect these values.<br/> Indeed, the Reactor pattern and its policies use the return value to do
		 * special manipulations such as discarding subsequent events in order to avoid memory corruptions
		 */
		virtual int handleUrgent(NINAHandle handle) = 0;
		/*!
		 * @brief Handle the dispatcher timeout
		 * @return 0 on success, 1 if the handling of event leads to the removal of all currently registered events or -1 on error (see handleClose above)<br/>
		 * Note that it is important to respect these values.<br/> Indeed, the Reactor pattern and its policies use the return value to do
		 * special manipulations such as discarding subsequent events in order to avoid memory corruptions
		 */
		virtual int handleTimeout(NINAHandle handle) = 0;
		/*!
		 * @brief Handle the case which a signal has been caught during reactor notifications
		 * @return 0 on success, 1 if the handling of event leads to the removal of all currently registered events or -1 on error (see handleClose above)<br/>
		 * Note that it is important to respect these values.<br/> Indeed, the Reactor pattern and its policies use the return value to do
		 * special manipulations such as discarding subsequent events in order to avoid memory corruptions
		 */
		virtual int handleSignal(NINAHandle handle) = 0;
		/*!
		 * @brief Handle two specific events
		 * @arg the removing of an EventHandler from the dispatcher
		 * @arg an error has occured on an handling method above (namely it has returned -1)
		 * @return 0 on success, 1 if the handling of event leads to the removal of all currently registered events or -1 on error<br/>
		 * Note that it is important to respect these values.<br/> Indeed, the Reactor pattern and its policies use the return value to do
		 * special manipulations such as discarding subsequent events in order to avoid memory corruptions
		 */
		virtual int handleClose(NINAHandle handle) = 0;
		//! @brief Get the underlying handle used by the EventHandler
		virtual NINAHandle getHandle() const = 0;
};

NINA_END_NAMESPACE_DECL

#endif // !__NINA_EVENTHANDLER_HPP__
