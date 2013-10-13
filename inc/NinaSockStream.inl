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
 * @file NinaSockStream.inl
 * @brief Stream bytes based socket operations (inline functions)
 * @author Jonathan Calmels
 * @date Wed Oct 19 2011
 */

NINA_BEGIN_NAMESPACE_DECL

NINA_INLINE int
SockStream::open(Addr const& addr, int family, int protocol, bool reuseAddr)
{
	return Sock::open(addr, family, protocol, SOCK_STREAM, reuseAddr);
}

NINA_INLINE void
SockStream::open(Addr const& addr, int family, std::string const& protocol, bool reuseAddr)
{
	return Sock::open(addr, family, protocol, SOCK_STREAM, reuseAddr);
}

NINA_INLINE int
SockStream::urgentSend(std::string const& str, uint8_t flags) const
{
	return urgentSend(str.c_str(), str.size(), flags);
}

NINA_INLINE int
SockStream::exactSend(std::string const& str, uint8_t flags) const
{
	return exactSend(str.c_str(), str.size(), flags);
}

NINA_INLINE int
SockStream::exactReceive(void* buf, size_t bufLen, uint8_t flags) const
{
	return receive(buf, bufLen, BUF_FULL | flags);
}

NINA_INLINE int
SockStream::exactReceive(std::string& str, uint8_t flags) const
{
	return receive(str, BUF_FULL | flags);
}

NINA_END_NAMESPACE_DECL
