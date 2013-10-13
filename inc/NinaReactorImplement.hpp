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
 * @file NinaReactorImplement.hpp
 * @brief Defines Nina reactor implementation following the bridge pattern logic
 * @author Jonathan Calmels
 * @date Mon Jul 25 2011
 */

#ifndef __NINA_REACTORIMPLEMENT_HPP__
# define __NINA_REACTORIMPLEMENT_HPP__

# include "NinaDef.hpp"

# if defined (NINA_WIN32)
// Disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#  pragma warning(disable: 4251)
// Disable warnings on extern before template instantiation
#  pragma warning(disable: 4231)
# endif // !NINA_WIN32

# include <map>
# include <utility>
# include "NinaTypes.hpp"
# include "NinaEventHandler.hpp"
# include "NinaTime.hpp"
# include "NinaReactor.hpp"

// STL forward declaration as expected for dll processing
// see http://support.microsoft.com/kb/168958/en-us
// Note that it may be not require because STL members are private here
// Especially since the export of map container is not supported 
NINA_EXTERN template struct NINA_DLLREQ std::pair<NINA::EventHandler*, uint16_t>;
NINA_EXTERN template class NINA_DLLREQ std::map<NINA::NINAHandle, std::pair<NINA::EventHandler*, uint16_t> >;

NINA_BEGIN_NAMESPACE_DECL

/*! @class ReactorImplement
 * @brief Collaborator in the bridge pattern with the NINA::Reactor class
 *
 * @details This class provides an unified interface for NINA::Reactor policies<br/>
 * Each policy inherits from that class to implement its backend and thus takes part to the bridge pattern
 */ 
class NINA_DLLREQ ReactorImplement
{
	protected:
		//! Pair regrouping an EventHandler with its registered events
		typedef std::pair<EventHandler*, uint16_t> HandleInfo;
		//! Associate an handle with its corresponding informations pair
		typedef std::map<NINAHandle, HandleInfo> DemuxTable;
		//! An element of DemuxTable (see above)
		typedef std::pair<NINAHandle const, HandleInfo> DemuxTableElement;
		typedef int (EventHandler::*Handler)(NINAHandle);

	public:
		virtual ~ReactorImplement() {};

	public:
		virtual int registerHandler(EventHandler* eHandler, uint16_t eType) = 0;
		virtual int registerHandler(NINAHandle handle, EventHandler* eHandler, uint16_t eType) = 0;
		virtual int removeHandler(EventHandler* eHandler, uint16_t eType) = 0;
		virtual int removeHandler(NINAHandle handle, uint16_t eType) = 0;
		virtual int handleEvents(Time const* timeout) = 0;
	protected:
		//! @brief Calls the appropriate member function depending on the event and resolves its errors
		//! @return the handle which has to be skipped if there are no more events to be monitored or NINA_INVALID_HANDLE otherwise
		NINAHandle dispatchEvent(Events::Type event, DemuxTableElement const* elem) const
		{
			int			errCode;
			NINAHandle	handle;

			handle = elem->first;
			switch (event) {
				case Events::READ:
					errCode = elem->second.first->handleRead(handle);
					break;
				case Events::WRITE:
					errCode = elem->second.first->handleWrite(handle);
					break;
				case Events::URGENT:
					errCode = elem->second.first->handleUrgent(handle);
					break;
				case Events::SIGNAL:
					errCode = elem->second.first->handleSignal(handle);
					break;
				case Events::TIME_OUT:
					errCode = elem->second.first->handleTimeout(handle);
					break;
				default:
					errCode = 0;
			}
			if (errCode == -1)
				elem->second.first->handleClose(handle);
			else if (errCode == 1)
				return handle;
			return NINA_INVALID_HANDLE;
		}

	protected:
		DemuxTable mTable; //!< Demuxing table necessary to manage events
};

NINA_END_NAMESPACE_DECL

#endif // !__NINA_REACTORIMPLEMENT_HPP__
