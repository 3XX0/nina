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
 * @file NinaOS.hpp
 * @brief Defines operating systems specific mechanisms
 * @author Jonathan Calmels
 * @date Mon Jun 06 2011
 */

#ifndef __NINA_OS_HPP__
# define __NINA_OS_HPP__

# include "NinaDef.hpp"

# if defined (NINA_POSIX)
#  include <netinet/in.h>
#  include <netdb.h>
#  include <sys/types.h>
# endif // !NINA_POSIX
# if defined (NINA_WIN32)
#  include <winsock2.h>
#  undef POLLPRI
#  define POLLPRI POLLRDBAND
# endif // !NINA_WIN32

# include <string>
# include "NinaCppUtils.hpp"
# include "NinaTypes.hpp"
# include "NinaTime.hpp"

NINA_BEGIN_NAMESPACE_DECL

// Forward declaration
class IOContainer;

/*! @class Init
 * @brief Framework initialization class
 *
 * @details This class is used to initialize the Nina framework<br/>
 * Cleanup is automatically done at its destruction
 *
 * Note that this class is a Singleton
 */
class NINA_DLLREQ Init : public Singleton<Init>
{
	public:
		/*!
		 * @brief Constructor
		 * @details Constructor of the class Init
		 * @param[in] WSAVersion : version of WSA to use (Microsoft Windows specific)
		 */
		Init(std::string const& WSAVersion = "2.2");
		//! @brief Destructor
		~Init();

	private:
# if defined (NINA_WIN32)
		int startWSA(std::string const& wsaVersion) const;
# endif // !NINA_WIN32
};

 NINA_BEGIN_NAMESPACE_OS

//! @name Error handling global functions
//! @{
void setLastError(int errCode);
int getLastError();
int setErrnoToWSALastError();
int setErrnoToLastError();
//! @}

//! @name Communication global functions
//! @{
int IOCtl(NINAHandle handle, int cmd, void* args);
int sockClose(NINAHandle sock);
int setSockOpt(NINAHandle sock, int level, int opt, void const* buf, int bufLen);
int getSockOpt(NINAHandle sock, int level, int opt, void* buf, int* bufLen);
int getPeerName(NINAHandle sock, sockaddr* addr, int* addrLen);
int getSockName(NINAHandle sock, sockaddr* addr, int* addrLen);
//! @warning This function allocates its return value
protoent* getProtoByNumber(int protocol, NINAProtoBuf* pBuf);
//! @warning This function allocates its return value
protoent* getProtoByName(char const* name, NINAProtoBuf* pBuf);
//! @warning This function allocates its return value
servent* getServByPort(int port, char const* protocol, NINAServBuf* sBuf);
//! @warning This function allocates its return value
servent* getServByName(char const* name, char const* protocol, NINAServBuf* sBuf);
int recv(NINAHandle sock, void* buf, size_t bufLen, int flags);
int send(NINAHandle sock, void const* buf, size_t bufLen, int flags);
int scatterRead(NINAHandle sock, IOContainer& ioc, unsigned long* flags);
int gatherWrite(NINAHandle sock, IOContainer& ioc, unsigned long flags);
int select(int fdLen, fd_set* readFds, fd_set* writeFds, fd_set* exceptFds, Time const* timeout);
int poll(NINAPollData* fdSet, unsigned long fdSetLen, Time const* timeout);
int accept(NINAHandle sock, sockaddr* addr, int* addrLen);
int connect(NINAHandle sock, sockaddr const* addr, int addrLen);
int recvFrom(NINAHandle sock, void* buf, size_t bufLen, int flags, sockaddr* addr, int* addrLen);
int sendTo(NINAHandle sock, void const* buf, size_t bufLen, int flags, sockaddr const* addr, int addrLen);
//! @}

//! @name System informations global functions
//! @{
long getOpenMax();
int	getCurrentTime(NINATimeval* tv);
//! @}

 NINA_END_NAMESPACE_OS
NINA_END_NAMESPACE_DECL

# include "NinaOS.inl"

#endif // !__NINA_OS_HPP__
