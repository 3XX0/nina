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
 * @file NinaPacketFactory.hpp
 * @brief Defines a packet manager to craft packets upon bytes stream protocols
 * @author Jonathan Calmels
 * @date Fri Nov 11 2011
 */

#ifndef __NINA_PACKETFACTORY_HPP__
# define __NINA_PACKETFACTORY_HPP__

# include <cstring>
# include <list>
# include "NinaDef.hpp"
# include "NinaCppUtils.hpp"
# include "NinaPacket.hpp"

NINA_BEGIN_NAMESPACE_DECL

/*! @class PacketFactory
 * @brief Creates packets upon bytes stream protocols
 *
 * @details This class acts as a factory, it analyses an incoming stream and tries to build
 * packets according to the delimiter sequence which is set
 * @arg IN_PACKET : type of the packet to build. Note that this class should inherit from NINA::InputPacket
 */ 
template <class IN_PACKET = InputPacket>
class PacketFactory : public NonCopyable
{
	private:
		enum
		{
			BUF_SIZE = 1024 //!< Size of the circular buffer
		};

		//! @struct CircularBuf
		//! @brief Definition of the circular buffer containing rd/wr indexes
		struct		CircularBuf
		{
			char	data[BUF_SIZE];
			size_t	wPtr;
			size_t	rPtr;
		};

		typedef std::list<IN_PACKET>				PacketList;
	public:
		//! Packet iterator
		typedef typename PacketList::iterator		iterator;
		//! Packet constant iterator
		typedef typename PacketList::const_iterator	const_iterator;

	public:
		//! @brief Constructor
		PacketFactory();
		//! @brief Constructor
		//! @param[in] sequence : sequence of characters delimiting an incoming packet
		PacketFactory(std::string const& sequence);
		//! @brief Destructor
		~PacketFactory();

	public:
		//! @brief Push a record so that it can be analysed by the PacketFactory, resulting sometimes in a new input packet
		//! @param[in] record : string to be pushed as a plain record
		void pushStream(std::string const& record);
		//! @brief Check whether input packets are pending or not
		//! @return true if the packet list is empty, false otherwise
		bool empty() const;
		//! @brief Return the number of pending packets
		//! @return size of the packet list
		size_t size() const;
		//! @brief Sort pending packets
		//! @details Sort input packets according to their inferiority operator (see InputPacket::operator<)
		void sort();
		//! @brief Clear the packet list, destroying all the input packets already created
		void clear();
		//! @brief Returns an iterator referring to the oldest input packet built
		//! @return an iterator to the beginning of the packet list
		iterator begin();
		//! @brief Returns an iterator referring to the newest input packet built
		//! @return an iterator to the ending of the packet list
		iterator end();
		//! @brief Returns a constant iterator referring to the oldest input packet built
		//! @return a constant iterator to the beginning of the packet list
		const_iterator begin() const;
		//! @brief Returns a constant iterator referring to the newest input packet built
		//! @return a constant iterator to the ending of the packet list
		const_iterator end() const;
		//! @brief Removes the packet pointed by the iterator pos
		//! @return an iterator on the following packet
		iterator remove(iterator pos);
		//! @brief Removes packets from the iterator first to the iterator last (first included)
		//! @return an iterator on the following packet, namely last
		iterator remove(iterator first, iterator last);
	private:
		//! @brief Update the payload with the circular buffer data
		void updatePayload();
		//! @brief Forge a new packet based on the current payload
		void forgePacket();

	private:
		CircularBuf	mWheel; //!< Wheel which analyses the data to find matching sequences
		std::string	mPayload; //!< Holds incomplete packet informations
		std::string mSequence; //!< End of packet delimiter
		PacketList	mList; //!< Pending packet list
};

NINA_END_NAMESPACE_DECL

# include "NinaPacketFactory.imp"
# include "NinaPacketFactory.inl"

#endif /* !__NINA_PACKETFACTORY_HPP__ */
