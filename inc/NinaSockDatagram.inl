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
 * @file NinaSockDatagram.inl
 * @brief Implementation of datagrams based socket operations (inline functions)
 * @author Jonathan Calmels
 * @date Wed Oct 19 2011
 */

NINA_BEGIN_NAMESPACE_DECL

NINA_INLINE int
SockDatagram::open(Addr const& addr, int family, int protocol, bool reuseAddr)
{
	return Sock::open(addr, family, protocol, SOCK_DGRAM, reuseAddr);
}

NINA_INLINE void
SockDatagram::open(Addr const& addr, int family, std::string const& protocol, bool reuseAddr)
{
	return Sock::open(addr, family, protocol, SOCK_DGRAM, reuseAddr);
}

NINA_INLINE int
SockDatagram::sendToPeer(std::string const& str, Addr const& peerAddr, uint8_t flag) const
{
	return sendToPeer(str.c_str(), str.size(), peerAddr, flag);
}

NINA_INLINE int
SockDatagram::exactSendToPeer(std::string const& str, Addr const& peerAddr, uint8_t flag) const
{
	return exactSendToPeer(str.c_str(), str.size(), peerAddr, flag);
}

NINA_INLINE int
SockDatagram::exactReceiveFromPeer(void* buf, size_t bufLen, Addr* peerAddr, uint8_t flags) const
{
	return receiveFromPeer(buf, bufLen, peerAddr, BUF_FULL | flags);
}

NINA_INLINE int
SockDatagram::exactReceiveFromPeer(std::string& str, Addr* peerAddr, uint8_t flags) const
{
	return receiveFromPeer(str, peerAddr, BUF_FULL | flags);
}

NINA_END_NAMESPACE_DECL
