#ifndef __NINA_H__
# define __NINA_H__

// NINA Base
# include "NinaDef.hpp"
# include "NinaTypes.hpp"

// NINA OS and utilities
# include "NinaOS.hpp"
# include "NinaSystemError.hpp"
# include "NinaCppUtils.hpp"
# include "NinaError.hpp"

// NINA Event handling
# include "NinaReactor.hpp"
# include "NinaReactorImplement.hpp"
# include "NinaEventHandler.hpp"
# include "NinaEventHandlerAdapter.hpp"

// NINA Service handling
# include "NinaServiceHandler.hpp"
# include "NinaAcceptor.hpp"

// NINA Socket wrapping
# include "NinaSAP.hpp"
# include "NinaSock.hpp"
# include "NinaSockAcceptor.hpp"
# include "NinaSockConnector.hpp"
# include "NinaSockDatagram.hpp"
# include "NinaSockIO.hpp"
# include "NinaSockStream.hpp"

// NINA Addressing
# include "NinaAddr.hpp"
# include "NinaInetAddr.hpp"

// NINA Policies
# if defined (NINA_LINUX)
#  include "NinaEpollPolicy.hpp"
# elif defined (NINA_BSD)
#  include "NinaKqueuePolicy.hpp"
# endif // !NINA_LINUX || NINA_BSD
# include "NinaPollPolicy.hpp"
# include "NinaSelectPolicy.hpp"

// NINA Packets
# include "NinaPacket.hpp"
# include "NinaPacketFactory.hpp"

// NINA Time
# include "NinaTime.hpp"

// NINA Container
# include "NinaIOContainer.hpp"

#endif /* !__NINA_H__ */
