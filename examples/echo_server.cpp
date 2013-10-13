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

#include <nina.h>

typedef NINA::SelectPolicy POLICY;

class EchoReply : public NINA::ServiceHandler<>
{
	public:
		enum { BUFSIZE = 1023 };

	public:
		EchoReply() {}
		~EchoReply()
		{
			 NINA::Reactor<POLICY>::getSingleton().removeHandler(this, NINA::Events::ALL);
		}

	public:
		int init()
		{
			NINA::Reactor<POLICY>::getSingleton().registerHandler(this, NINA::Events::READ);
			std::cout << "Client joined : " << getRemoteAddr().getHostAddr() << std::endl;
			return 0;
		}

		int handleRead(NINA::NINAHandle)
		{
			mSize  = getPeer().receive(mBuf, BUFSIZE);
			if (mSize == 0) {
				std::cout << "Client " << getRemoteAddr().getHostAddr() << " disconnected" << std::endl;
				delete this;
				return 1;
			}
			else {
				mBuf[mSize] = '\0';
				std::cout << "Received : " << mBuf << std::endl;
				NINA::Reactor<POLICY>::getSingleton().registerHandler(this, NINA::Events::WRITE);
			}
			return 0;
		}

		int handleWrite(NINA::NINAHandle)
		{
			std::cout << "Sending reply ..." << std::endl;
			getPeer().send(mBuf, mSize);
			NINA::Reactor<POLICY>::getSingleton().removeHandler(this, NINA::Events::WRITE);
			return 0;
		}

	private:
		int		mSize;
		char	mBuf[BUFSIZE + 1];

};

int main(void)
{
	NINA::Reactor<POLICY>									reactor;
	NINA::InetAddr											local_address;
	NINA::Acceptor<EchoReply, NINA::SockAcceptor, POLICY>	acceptor(&reactor);

	NINA::Init						startup;

	local_address.wildcardQuery("1234", "tcp");
	acceptor.open(local_address, true);
	reactor.handleEventsLoop();
	return 0;
}
