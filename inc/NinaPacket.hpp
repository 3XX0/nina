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
 * @file NinaPacket.hpp
 * @brief Defines the packets wrapping of NINA api
 * @author Jonathan Calmels
 * @date Fri Nov 11 2011
 */

#ifndef __NINA_PACKET_HPP__
# define __NINA_PACKET_HPP__

# include "NinaDef.hpp"

# if defined (NINA_WIN32)
// Disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#  pragma warning(disable: 4251)
# endif // !NINA_WIN32

# include <sstream>
# include <cstdio>

# if defined (NINA_POSIX)
#  include <netinet/in.h>
# endif // !NINA_POSIX
# if defined (NINA_WIN32)
#  include <winsock2.h>
#endif // !NINA_WIN32

NINA_BEGIN_NAMESPACE_DECL

/*! @struct CDR
 * @brief NINA Common data representation
 *
 * @details These types are used to build or retrieve packet informations in an homogeneous manner
 */
struct NINA_DLLREQ CDR
{
	//! Type boolean (1 byte)
	typedef bool Boolean;
	//! Type Char (1 signed byte)
	typedef int8_t Char;
	//! Type UChar (1 unsigned byte)
	typedef uint8_t UChar;
	//! Type Short (2 signed bytes)
	typedef int16_t Short;
	//! Type UShort (2 unsigned bytes)
	typedef uint16_t UShort;
	//! Type Integer (4 signed bytes)
	typedef int32_t Integer;
	//! Type UInteger (4 unsigned bytes)
	typedef uint32_t UInteger;
	//! Type Long (8 signed bytes)
	typedef int64_t Long;
	//! Type Long (8 unsigned bytes)
	typedef uint64_t ULong;
	//! Type String (equivalent to the STL string)
	typedef std::string String;
};

/*! @class OutputPacket
 * @brief Marshalling for output operations
 *
 * @details This class provides simple operators to marshal data in order to
 * do output operations (such as sending informations to a remote peer)
 */
class NINA_DLLREQ OutputPacket
{
	public:
		//! @brief Constructor
		//! @param[in] str : an initial string to build the packet
		OutputPacket(std::string const& str = "");
		//! @brief Virtual destructor
		virtual ~OutputPacket() {};
		//! @brief Copy constructor
		OutputPacket(OutputPacket const& opack);
		//! @brief Assignement operator
		OutputPacket& operator=(OutputPacket const& opack);

	public:
		//! @brief Check for packet equality
		bool operator==(OutputPacket const& opack) const;
		//! @brief Check for packet inequality
		bool operator!=(OutputPacket const& opack) const;
		//! @brief Adds a boolean field to the packet
		//! @param[in] value : value to add (see NINA::CDR)
		OutputPacket& operator<<(CDR::Boolean value);
		//! @brief Adds a char field to the packet
		//! @param[in] value : value to add (see NINA::CDR)
		OutputPacket& operator<<(CDR::Char value);
		//! @brief Adds an unsigned char field to the packet
		//! @param[in] value : value to add (see NINA::CDR)
		OutputPacket& operator<<(CDR::UChar value);
		//! @brief Adds a short field to the packet
		//! @param[in] value : value to add (see NINA::CDR)
		OutputPacket& operator<<(CDR::Short value);
		//! @brief Adds an unsigned short field to the packet
		//! @param[in] value : value to add (see NINA::CDR)
		OutputPacket& operator<<(CDR::UShort value);
		//! @brief Adds an integer field to the packet
		//! @param[in] value : value to add (see NINA::CDR)
		OutputPacket& operator<<(CDR::Integer value);
		//! @brief Adds an unsigned integer field to the packet
		//! @param[in] value : value to add (see NINA::CDR)
		OutputPacket& operator<<(CDR::UInteger value);
		//! @brief Adds a long field to the packet
		//! @param[in] value : value to add (see NINA::CDR)
		OutputPacket& operator<<(CDR::Long value);
		//! @brief Adds an unsigned long field to the packet
		//! @param[in] value : value to add (see NINA::CDR)
		OutputPacket& operator<<(CDR::ULong value);
		//! @brief Adds a string field to the packet
		//! @param[in] value : value to add (see NINA::CDR)
		OutputPacket& operator<<(CDR::String const& str);
		//! @brief Dump the packet in an unformatted string
		//! @return a copy of the packet in a string format
		std::string dump() const;
		//! @brief Assign a string to the packet, thus replacing its whole content
		//! @param[in] str : the input string to replace packet informations
		void assign(std::string const& str);
		//! @brief Clear the packet, destroying all of its content
		void clear();

	private:
		std::stringbuf	mStream; //!< Underlying stream
};

/*! @class InputPacket
 * @brief Demarshalling for input operations
 *
 * @details This class provides simple operators to demarshal data in order to
 * do input operations (such as receiving informations from a remote peer)
 */
class NINA_DLLREQ InputPacket
{
	public:
		//! @brief Constructor
		//! @param[in] str : an initial string to build the packet
		InputPacket(std::string const& str = "");
		//! @brief Virtual destructor
		virtual ~InputPacket() {};
		//! @brief Copy constructor
		InputPacket(InputPacket const& ipack);
		//! @brief Assignement operator
		InputPacket& operator=(InputPacket const& ipack);

	public:
		//! @brief Check for packet equality
		bool operator==(InputPacket const& ipack) const;
		//! @brief Check for packet inequality
		bool operator!=(InputPacket const& ipack) const;
		//! @brief Check for packet inferiority
		//! @details This function is actually useful to sort packets (see PacketFactory::sort)
		bool operator<(InputPacket const& ipack) const;
		//! @brief Retrieves a boolean field from the packet and consumes it
		//! @param[out] value : a reference on a boolean to store the returned value (see NINA::CDR)
		InputPacket& operator>>(CDR::Boolean& value);
		//! @brief Retrieves a char field from the packet and consumes it
		//! @param[out] value : a reference on a char to store the returned value (see NINA::CDR)
		InputPacket& operator>>(CDR::Char& value);
		//! @brief Retrieves an unsigned char field from the packet and consumes it
		//! @param[out] value : a reference on an unsigned char to store the returned value (see NINA::CDR)
		InputPacket& operator>>(CDR::UChar& value);
		//! @brief Retrieves a short field from the packet and consumes it
		//! @param[out] value : a reference on a short to store the returned value (see NINA::CDR)
		InputPacket& operator>>(CDR::Short& value);
		//! @brief Retrieves an unsigned short field from the packet and consumes it
		//! @param[out] value : a reference on an unsigned short to store the returned value (see NINA::CDR)
		InputPacket& operator>>(CDR::UShort& value);
		//! @brief Retrieves an integer field from the packet and consumes it
		//! @param[out] value : a reference on an integer to store the returned value (see NINA::CDR)
		InputPacket& operator>>(CDR::Integer& value);
		//! @brief Retrieves an unsigned integer field from the packet and consumes it
		//! @param[out] value : a reference on an unsigned integer to store the returned value (see NINA::CDR)
		InputPacket& operator>>(CDR::UInteger& value);
		//! @brief Retrieves a long field from the packet and consumes it
		//! @param[out] value : a reference on a long to store the returned value (see NINA::CDR)
		InputPacket& operator>>(CDR::Long& value);
		//! @brief Retrieves an unsigned long field from the packet and consumes it
		//! @param[out] value : a reference on an unsigned long to store the returned value (see NINA::CDR)
		InputPacket& operator>>(CDR::ULong& value);
		//! @brief Retrieves a string field from the packet and consumes it
		//! @param[out] value : a reference on a string to store the returned value (see NINA::CDR)
		InputPacket& operator>>(CDR::String& str);
		//! @brief Peek a boolean field from the packet
		//! @param[out] value : a reference on a boolean to store the returned value (see NINA::CDR)
		void peek(CDR::Boolean& value) const;
		//! @brief Peek a char field from the packet
		//! @param[out] value : a reference on a char to store the returned value (see NINA::CDR)
		void peek(CDR::Char& value) const;
		//! @brief Peek an unsigned char field from the packet
		//! @param[out] value : a reference on an unsigned char to store the returned value (see NINA::CDR)
		void peek(CDR::UChar& value) const;
		//! @brief Peek a short field from the packet
		//! @param[out] value : a reference on a short to store the returned value (see NINA::CDR)
		void peek(CDR::Short& value) const;
		//! @brief Peek an unsigned short field from the packet
		//! @param[out] value : a reference on an unsigned short to store the returned value (see NINA::CDR)
		void peek(CDR::UShort& value) const;
		//! @brief Peek an integer field from the packet
		//! @param[out] value : a reference on an integer to store the returned value (see NINA::CDR)
		void peek(CDR::Integer& value) const;
		//! @brief Peek an unsigned integer field from the packet
		//! @param[out] value : a reference on an unsigned integer to store the returned value (see NINA::CDR)
		void peek(CDR::UInteger& value) const;
		//! @brief Peek a long field from the packet
		//! @param[out] value : a reference on a long to store the returned value (see NINA::CDR)
		void peek(CDR::Long& value) const;
		//! @brief Peek an unsigned long field from the packet
		//! @param[out] value : a reference on an unsigned long to store the returned value (see NINA::CDR)
		void peek(CDR::ULong& value) const;
		//! @brief Peek a string field from the packet
		//! @param[out] value : a reference on a string to store the returned value (see NINA::CDR)
		void peek(CDR::String& str) const;
		//! @brief Dump the packet in an unformatted string
		//! @return a copy of the packet in a string format
		std::string dump() const;
		//! @brief Assign a string to the packet, thus replacing its whole content
		//! @param[in] str : the input string to replace packet informations
		void assign(std::string const& str);
		/*!
		 * @brief Retrieves a string field from the packet and consumes it according to its length
		 * @param[out] str : a reference on a string to store the returned value (see NINA::CDR)
		 * @param[in] len : length of the string
		 */
		void get(CDR::String& str, size_t len);
		//! @brief Clear the packet, destroying all of its content
		void clear();

	private:
		mutable std::stringbuf	mStream; //!< Underlying stream
};

NINA_END_NAMESPACE_DECL

# include "NinaPacket.inl"

#endif /* !__NINA_PACKET_HPP__ */

