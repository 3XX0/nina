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
 * @file NinaPollPolicy.hpp
 * @brief Defines the sychroneous event demultiplexer 'poll' as concrete reactor implementation
 * @author Jonathan Calmels
 * @date Mon Jul 25 2011
 */

#ifndef __NINA_POLLPOLICY_HPP__
# define __NINA_POLLPOLICY_HPP__

# include <stack>
# include <map>
# include "NinaTypes.hpp"
# include "NinaReactorImplement.hpp"
# include "NinaCppUtils.hpp"

// STL forward declaration as expected for dll processing
// see http://support.microsoft.com/kb/168958/en-us
// Note that it may be not require because STL members are private here
// Especially since the export of map container is not supported 
NINA_EXTERN template class NINA_DLLREQ std::stack<size_t>;
NINA_EXTERN template class NINA_DLLREQ std::map<NINA::NINAHandle, size_t>;

NINA_BEGIN_NAMESPACE_DECL

/*! @class PollPolicy
 * @brief Implement the sychroneous event demultiplexer 'poll'
 *
 * @details This class serves as a reactor policy, it permits to the reactor to use 'poll' as its backend
 * @see NINA::Reactor
 */
class NINA_DLLREQ PollPolicy : public ReactorImplement, public NonCopyable
{
	template <class SYNC_POLICY> friend class Reactor;

	private:
		//! A stack of indexes
		typedef std::stack<size_t> IndexStack;
		//! Table associating an handle with its index
		typedef std::map<NINAHandle, size_t> AssocTable;

	private:
		//! @brief Constructor
		PollPolicy();
		//! @brief Destructor
		~PollPolicy();

	public:
		//! @brief Register the handle to poll backend using double dispatching
		virtual int registerHandler(EventHandler* eHandler, uint16_t eType);
		//! @brief Register the handle to poll backend
		virtual int registerHandler(NINAHandle handle, EventHandler* eHandler, uint16_t eType);
		//! @brief Remove the handle from poll backend using double dispatching
		virtual int removeHandler(EventHandler* eHandler, uint16_t eType);
		//! @brief Remove the handle from poll backend 
		virtual int removeHandler(NINAHandle handle, uint16_t eType);
		//! @brief Process to the event poll
		virtual int handleEvents(Time const* timeout = 0);

	private:
		OS::NINAPollData*	mFdSet; //!< Poll specifics data
		size_t				mFdSetIndex; //!< Current index on Poll fd set
		IndexStack			mAvailableIndex; //!< Rooms free into Poll fd set
		AssocTable			mHandleToIndex; //!< Table associating an handle with an index into the Poll fd set
		long				mOpenMax; //!< Maximum open file descriptors
};

NINA_END_NAMESPACE_DECL

# include "NinaPollPolicy.inl"

#endif // !__NINA_POLLPOLICY_HPP__
