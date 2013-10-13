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
 * @file NinaInetAddr.inl
 * @brief Implements the internet domain addressing (inline functions)
 * @author Jonathan Calmels
 * @date Mon Jun 06 2011
 */

#include <stdexcept>

NINA_BEGIN_NAMESPACE_DECL

NINA_INLINE bool
InetAddr::operator==(InetAddr const& sap) const
{
	if (::memcmp(&mInetAddr, &sap.mInetAddr, sizeof mInetAddr) == 0)
		return true;
	return false;
}

NINA_INLINE bool
InetAddr::operator!=(InetAddr const& sap) const
{
	if (::memcmp(&mInetAddr, &sap.mInetAddr, sizeof mInetAddr) != 0)
		return true;
	return false;
}

NINA_INLINE void const*
InetAddr::getAddr() const
{
	return &mInetAddr;
}

NINA_INLINE void
InetAddr::forceSockType(int sockType)
{
	mForcedSockType = sockType;
}

NINA_INLINE void
InetAddr::setFlowInfo(uint32_t flow)
{
	if (mFamily == AF_INET6
				|| (mFamily == AF_UNSPEC && hasIpv6Addr())) {
		sockaddr_in6* sa = reinterpret_cast<sockaddr_in6*> (&mInetAddr);
		sa->sin6_flowinfo = flow;
	}
}

NINA_INLINE void
InetAddr::setScopeId(uint32_t scopeId)
{ 
	if (mFamily == AF_INET6
			|| (mFamily == AF_UNSPEC && hasIpv6Addr())) {
		sockaddr_in6* sa = reinterpret_cast<sockaddr_in6*> (&mInetAddr);
		sa->sin6_scope_id = scopeId;
	}
}

NINA_INLINE int
InetAddr::getSockType() const
{
	return mSockType;
}

NINA_INLINE int
InetAddr::getProtocolByNumber() const
{
	return mProtocol;
}

NINA_INLINE bool
InetAddr::hasIpv4Addr() const
{
	if (mInetAddr.ss_family == AF_INET)
		return true;
	return false;
}

NINA_INLINE bool
InetAddr::hasIpv6Addr() const
{
	if (mInetAddr.ss_family == AF_INET6)
		return true;
	return false;
} 

NINA_INLINE bool
InetAddr::isLoopback() const
{
	if (mFamily == AF_INET6
				|| (mFamily == AF_UNSPEC && hasIpv6Addr())) {
		sockaddr_in6 const* sa = reinterpret_cast<sockaddr_in6 const*> (&mInetAddr);
		if (IN6_IS_ADDR_LOOPBACK(&sa->sin6_addr) != 0)
			return true;		
	}
	return false;
}

NINA_INLINE bool
InetAddr::isMulticast() const
{
	if (mFamily == AF_INET6
				|| (mFamily == AF_UNSPEC && hasIpv6Addr())) {
		sockaddr_in6 const* sa = reinterpret_cast<sockaddr_in6 const*> (&mInetAddr);
		if (IN6_IS_ADDR_MULTICAST(&sa->sin6_addr) != 0)
			return true;
	}
	return false;
}

NINA_INLINE bool
InetAddr::isLinkLocal() const
{
	if (mFamily == AF_INET6
				|| (mFamily == AF_UNSPEC && hasIpv6Addr())) {
		sockaddr_in6 const* sa = reinterpret_cast<sockaddr_in6 const*> (&mInetAddr);
		if (IN6_IS_ADDR_LINKLOCAL(&sa->sin6_addr) != 0)
			return true;
	}
	return false;
}

NINA_INLINE bool
InetAddr::isV4Mapped() const
{
	if (mFamily == AF_INET6
				|| (mFamily == AF_UNSPEC && hasIpv6Addr())) {
		sockaddr_in6 const* sa = reinterpret_cast<sockaddr_in6 const*> (&mInetAddr);
		if (IN6_IS_ADDR_V4MAPPED(&sa->sin6_addr) != 0)
			return true;
	}
	return false;
}

NINA_INLINE bool
InetAddr::isV4Compat() const
{
	if (mFamily == AF_INET6
				|| (mFamily == AF_UNSPEC && hasIpv6Addr())) {
		sockaddr_in6 const* sa = reinterpret_cast<sockaddr_in6 const*> (&mInetAddr);
		if (IN6_IS_ADDR_V4COMPAT(&sa->sin6_addr) != 0)
			return true;
	}
	return false;
}

NINA_END_NAMESPACE_DECL
