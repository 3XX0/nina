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
 * @file NinaInetAddr.hpp
 * @brief Defines the internet domain addressing
 * @author Jonathan Calmels
 * @date Mon Jun 06 2011
 */

#ifndef __NINA_INETADDR_HPP__
# define __NINA_INETADDR_HPP__

# include "NinaDef.hpp"

# if defined (NINA_WIN32)
// Disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#  pragma warning(disable: 4251)
# endif // !NINA_WIN32

# include <cstring>
# include <iostream>

# if defined (NINA_POSIX)
#  include <netinet/in.h>
#  include <sys/socket.h>
# endif // !NINA_POSIX
# if defined (NINA_WIN32)
#  include <winsock2.h>
#  include <ws2tcpip.h>
# endif // !NINA_WIN32

# include "NinaAddr.hpp"

NINA_BEGIN_NAMESPACE_DECL

/*! @class InetAddr
 * @brief Internet domain addressing
 *
 * @details This class is intended to wrap internet domain addressing<br/>
 * It provides all the functions necessary to manipulate internet addresses
 */ 
class NINA_DLLREQ InetAddr : public Addr
{ 
	public:
		enum
		{
			ANY_PROTO = 0, //!< Specify any protocol
			ANY_SOCK = 0, //!< Specify any socket type
			IPV4_MAPPED = 1, //!< Enable ipv4 to ipv6 mapping (ignored in OpenBSD and NetBSD Operating systems)
			NAME_RESOLUTION = 2, //!< Enable dynamic name resolution
			SERV_RESOLUTION = 4, //!< Enable service resolution
			DEFAULT_MODE = 7 //!< Default mode, equivalent to all options enabled
		};
	private:
		//! Name info buffer
		typedef char NameInfoBuf[2048];

	public:
		/*! @brief Constructor
		 * @details Constructor of the class InetAddr
		 * @param[in] family : the address family of the internet address, types supported are AF_INET/AF_INET6/AF_UNSPEC
		 * @throw std::invalid_argument if the address family is unknown or not supported
		 */
		InetAddr(int family = AF_UNSPEC);
		//! @brief Destructor
		~InetAddr();
		//! @brief Copy constructor
		InetAddr(InetAddr const& sap);
		//! @brief Assignment operator
		InetAddr& operator=(InetAddr const& sap);

	public:
		//! @brief Check for address equality
		bool operator==(InetAddr const& sap) const;
		//! @brief Check for address inequality
		bool operator!=(InetAddr const& sap) const;
		//! @brief Get a pointer on the concrete address
		//! @return A pointer on the concrete address or 0 on error
		virtual void const* getAddr() const;
		/*!
		 * @brief Set a pointer on the concrete address
		 * @param[in] addr : a pointer on the concrete address
		 * @param[in] addrLen : size of the address pointed by addr
		 * @return 0 on success or -1 on error
		 */
		virtual int setAddr(void* addr, size_t addrLen);
		/*! @brief Set a specific internet address remotely
		 * @throw NINA::Error::SystemError if an error occured
		 * @param[in] host : address of the remote host or FQDN if #NAME_RESOLUTION is enabled
		 * @param[in] port : port of the remote host or string service if #SERV_RESOLUTION is enabled
		 * @param[in] protocol : protocol used for the connection, IPPROTO_* or #ANY_PROTO for a specific definiton by the operating system
		 * @param[in] mode : specify the mode used to retreive the address, it is a bit set composed using the following flags : #IPV4_MAPPED/#NAME_RESOLUTION/#SERV_RESOLUTION
		 */
		void remoteQuery(std::string const& host, std::string const& port,
				int protocol, uint8_t mode = DEFAULT_MODE);
		/*! @brief Set a specific internet address remotely
		 * @throw NINA::Error::SystemError if an error occured
		 * @param[in] host : address of the remote host or FQDN if #NAME_RESOLUTION is enabled
		 * @param[in] port : port of the remote host
		 * @param[in] protocol : protocol used for the connection, IPPROTO_* or #ANY_PROTO for a specific definiton by the operating system
		 * @param[in] mode : specify the mode used to retreive the address, it is a bit set composed using the following flags : #IPV4_MAPPED/#NAME_RESOLUTION
		 */
		void remoteQuery(std::string const& host, uint16_t port,
				int protocol, uint8_t mode = DEFAULT_MODE);
		/*! @brief Set a specific internet address remotely
		 * @throw NINA::Error::SystemError if an error occured
		 * @param[in] host : address of the remote host or FQDN if #NAME_RESOLUTION is enabled
		 * @param[in] port : port of the remote host or string service if #SERV_RESOLUTION is enabled
		 * @param[in] protocol : string protocol used for the connection, or "any" for a specific definiton by the operating system
		 * @param[in] mode : specify the mode used to retreive the address, it is a bit set composed using the following flags : #IPV4_MAPPED/#NAME_RESOLUTION/#SERV_RESOLUTION
		 */
		void remoteQuery(std::string const& host, std::string const& port,
				std::string const& protocol, uint8_t mode = DEFAULT_MODE);
		/*! @brief Set a specific internet address remotely
		 * @throw NINA::Error::SystemError if an error occured
		 * @param[in] host : address of the remote host or FQDN if #NAME_RESOLUTION is enabled
		 * @param[in] port : port of the remote host
		 * @param[in] protocol : string protocol used for the connection, or "any" for a specific definiton by the operating system
		 * @param[in] mode : specify the mode used to retreive the address, it is a bit set composed using the following flags : #IPV4_MAPPED/#NAME_RESOLUTION
		 */
		void remoteQuery(std::string const& host, uint16_t port,
				std::string const& protocol, uint8_t mode = DEFAULT_MODE);
		/*! @brief Set a specific address locally
		 * @throw NINA::Error::SystemError if an error occured
		 * @param[in] host : address on the local host or an hostname if #NAME_RESOLUTION is enabled
		 * @param[in] port : port on the local host or string service if #SERV_RESOLUTION is enabled
		 * @param[in] protocol : protocol used for the connection, IPPROTO_* or #ANY_PROTO for a specific definiton by the operating system
		 * @param[in] mode : specify the mode used to retreive the address, it is a bit set composed using the following flags : #NAME_RESOLUTION/#SERV_RESOLUTION
		 */
		void localQuery(std::string const& host, std::string const& port,
				int protocol, uint8_t mode = DEFAULT_MODE);
		/*! @brief Set a specific address locally
		 * @throw NINA::Error::SystemError if an error occured
		 * @param[in] host : address on the local host or an hostname if #NAME_RESOLUTION is enabled
		 * @param[in] port : port on the local host
		 * @param[in] protocol : protocol used for the connection, IPPROTO_* or #ANY_PROTO for a specific definiton by the operating system
		 * @param[in] mode : specify the mode used to retreive the address, it is a bit set composed using the following flags : #NAME_RESOLUTION
		 */
		void localQuery(std::string const& host, uint16_t port,
				int protocol, uint8_t mode = DEFAULT_MODE);
		/*! @brief Set a specific address locally
		 * @throw NINA::Error::SystemError if an error occured
		 * @param[in] host : address on the local host or an hostname if #NAME_RESOLUTION is enabled
		 * @param[in] port : port on the local host or string service if #SERV_RESOLUTION is enabled
		 * @param[in] protocol : string protocol used for the connection, or "any" for a specific definiton by the operating system
		 * @param[in] mode : specify the mode used to retreive the address, it is a bit set composed using the following flags : #NAME_RESOLUTION/#SERV_RESOLUTION
		 */
		void localQuery(std::string const& host, std::string const& port,
				std::string const& protocol, uint8_t mode = DEFAULT_MODE);
		/*! @brief Set a specific address locally
		 * @throw NINA::Error::SystemError if an error occured
		 * @param[in] host : address on the local host or an hostname if #NAME_RESOLUTION is enabled
		 * @param[in] port : port on the local host
		 * @param[in] protocol : string protocol used for the connection, or "any" for a specific definiton by the operating system
		 * @param[in] mode : specify the mode used to retreive the address, it is a bit set composed using the following flags : #NAME_RESOLUTION
		 */
		void localQuery(std::string const& host, uint16_t port,
				std::string const& protocol, uint8_t mode = DEFAULT_MODE);
		/*! @brief Set the wildcard address on local host
		 * @details This function is useful for server applications which use passive sockets
		 * @throw NINA::Error::SystemError if an error occured
		 * @param[in] port : port on the local host or string service if #SERV_RESOLUTION is enabled
		 * @param[in] protocol : protocol used for the connection, IPPROTO_* or #ANY_PROTO for a specific definiton by the operating system
		 * @param[in] mode : specify the mode used to retreive the address, it is a bit set composed using the following flag : #SERV_RESOLUTION
		 */
		void wildcardQuery(std::string const& port, int protocol, uint8_t mode = SERV_RESOLUTION);
		/*! @brief Set the wildcard address on local host
		 * @throw NINA::Error::SystemError if an error occured
		 * This function is useful for server applications which use passive sockets
		 * @param[in] port : port on the local host
		 * @param[in] protocol : protocol used for the connection, IPPROTO_* or #ANY_PROTO for a specific definiton by the operating system
		 */
		void wildcardQuery(uint16_t port, int protocol);
		/*! @brief Set the wildcard address on local host
		 * @details This function is useful for server applications which use passive sockets
		 * @throw NINA::Error::SystemError if an error occured
		 * @param[in] port : port on the local host or string service if #SERV_RESOLUTION is enabled
		 * @param[in] protocol : string protocol used for the connection, or "any" for a specific definiton by the operating system
		 * @param[in] mode : specify the mode used to retreive the address, it is a bit set composed using the following flag : #SERV_RESOLUTION
		 */
		void wildcardQuery(std::string const& port, std::string const& protocol, uint8_t mode = SERV_RESOLUTION);
		/*! @brief Set the wildcard address on local host
		 * @details This function is useful for server applications which use passive sockets
		 * @throw NINA::Error::SystemError if an error occured
		 * @param[in] port : port on the local host
		 * @param[in] protocol : string protocol used for the connection, or "any" for a specific definiton by the operating system
		 */
		void wildcardQuery(uint16_t port, std::string const& protocol);
		/*!
		 * @brief Force the socket type used for the connection
		 * @details Calling this function must be done before any subsequent call to remoteQuery(), localQuery() or wildcardQuery()
		 * @param[in] sockType : socket type to use (SOCK_*)
		 */
		void forceSockType(int sockType);
		/*!
		 * @brief Get the name of the port currently used by the address
		 * @details The port name is retreived using services file of the operating system
		 * @throw NINA::Error::SystemError if an error occured
		 * @return string service or an empty string on error
		 */
		std::string getPortByName() const;
		 //! @brief Get the number of the port currently used by the address
		 //! @return the port number or -1 on error
		uint16_t getPortByNumber() const;
		/*!
		 * @brief Set the port of the address using string service format
		 * @details The port number is retreived using services file of the operating system
		 * @throw NINA::Error::SystemError if an error occured
		 * @param[in] port : string service to set up
		 */
		void setPort(std::string const& port);
		/*!
		 * @brief Set the port of the address
		 * @param[in] port : port number to set up
		 * @return 0 on success or -1 on error
		 */
		int setPort(uint16_t port);
		/*!
		 * @brief Get the host name
		 * @details Lookup for the  host name<br/>
		 * Return value may be either a FQDN or a netbios/hostname
		 * depending on the operating system and the requested address
		 * @return A string containing the host name or an empty string on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		std::string getHostName();
		/*!
		 * @brief Get the host address
		 * @return A string containing the host address or an empty string on error<br/>
		 * If an error occured errno will be set accordingly
		 */
		std::string getHostAddr();
		//! @brief Get the socket type currently used by the address
		//! @return The socket type or -1 on error
		int getSockType() const;
		//! @brief Get the protocol number currently used by the address
		//! @return The protocol number or -1 on error
		int getProtocolByNumber() const;
		/*!
		 * @brief Get the protocol string currently used by the address
		 * @details The protocol string is retreived using protocol file of the operating system
		 * @throw NINA::Error::SystemError if an error occured
		 * @return A string containing the protocol string or an empty string on error
		 */
		std::string getProtocolByName() const;
		//! @brief Test whether the address is an ipv4 address or not
		bool hasIpv4Addr() const;
		//! @brief Test whether the address is an ipv6 address or not
		bool hasIpv6Addr() const;
		//! @name Designed only for ipv6 addresses
		//! @{

		//! @brief Get flow informations from the address
		//! @return The flow information or (uint32_t) -1 on error
		uint32_t getFlowInfo() const;
		//! @brief Get the scope id from the address
		//! @return The scope id or (uint32_t) -1 on error
		uint32_t getScopeId() const;
		//! @brief Set flow informations
		//! @param[in] flow : flow information to set up
		void setFlowInfo(uint32_t flow);
		//! @brief Set the scope id of the address
		//! @param[in] scopeId : scope id to set up
		void setScopeId(uint32_t scopeId);
		/*!
		 * @brief Set the scope id of the address using an interface name
		 * @details Pretty usefull to set the scope id for link local addresses
		 * @param ifaceName : name of the interface
		 * @return 0 on success or -1 on error<br/>
		 * @warning Here errno may vary depending on the operating systems, but note that if it failed it's mainly that the interface doesn't exist
		 */
		int setScopeId(std::string const& ifaceName);
		//! @brief Test whether the address is a loopback address or not
		bool isLoopback() const;
		//! @brief Test whether the address is a multicast address or not
		bool isMulticast() const;
		//! @brief Test whether the address is a link local address or not
		bool isLinkLocal() const;
		//! @brief Test whether the address is an ipv4 mapped address or not
		bool isV4Mapped() const;
		//! @brief Test whether the address is an ipv4 compatible address or not
		bool isV4Compat() const;
		//! @brief Reset the address to its default values
		virtual void reset();
		//! @}
	private:
		//! @brief Get the real size of the address
		//! @return Size of the address or -1 on error
		int getRealSize() const;
		//! @brief Get the real family of the address
		//! @return The family of the address or -1 on error
		int getRealFamily() const;
		/*!
		 * @brief Determine the size of the address using its family
		 * @throw std::invalid_argument if the family is unknown
		 * @param[in] family : the address family
		 * @return Size of the address or -1 on error
		 */
		int determineSize(int family) const;

	private:
		sockaddr_storage		mInetAddr; //!< Addressing datas
		std::string				mHostAddr; //!< Host address cache
		std::string				mHostName; //!< Host name cache
		int						mForcedSockType; //!< Socket type requested
		int						mSockType; //!< Socket type optained
		int						mProtocol; //!< Protocol used
};

NINA_END_NAMESPACE_DECL

# include "NinaInetAddr.inl"

#endif // !__NINA_INETADDR_HPP__
