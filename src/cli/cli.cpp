// cli.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "libwfp/filterengine.h"
#include "libwfp/objectenumerator.h"
#include "libcommon/string.h"
#include <iostream>
#include <memory>
#include <vector>

std::wstring SessionFlags(UINT32 flags)
{
	std::vector<std::pair<UINT32, std::wstring> > definitions =
	{
		std::make_pair(FWPM_SESSION_FLAG_DYNAMIC, L"FWPM_SESSION_FLAG_DYNAMIC"),
		std::make_pair(FWPM_SESSION_FLAG_RESERVED, L"FWPM_SESSION_FLAG_RESERVED")
	};

	return common::string::FormatFlags(definitions, flags);
}

bool SessionCallback(const FWPM_SESSION0 &session)
{
	std::wcout << "Session" << std::endl;

	std::wcout << "  key:\t\t\t" << common::string::FormatGuid(session.sessionKey) << std::endl;
	std::wcout << "  name:\t\t\t" <<
		(session.displayData.name == nullptr ? L"n/a" : session.displayData.name) << std::endl;
	std::wcout << "  description:\t\t" <<
		(session.displayData.description == nullptr ? L"n/a" : session.displayData.description) << std::endl;
	std::wcout << "  flags:\t\t" << session.flags << L" = " << SessionFlags(session.flags) << std::endl;
	std::wcout << "  wait timeout:\t\t" << session.txnWaitTimeoutInMSec << std::endl;
	std::wcout << "  sid:\t\t\t" << common::string::FormatSid(*session.sid) << std::endl;
	std::wcout << "  username:\t\t" << session.username << std::endl;
	std::wcout << "  kernel:\t\t" <<
		(session.kernelMode ? L"true" : L"false") << std::endl;

	return true;
}

std::wstring ProviderFlags(UINT32 flags)
{
	std::vector<std::pair<UINT32, std::wstring> > definitions =
	{
		std::make_pair(FWPM_PROVIDER_FLAG_PERSISTENT, L"FWPM_PROVIDER_FLAG_PERSISTENT"),
		std::make_pair(FWPM_PROVIDER_FLAG_DISABLED, L"FWPM_PROVIDER_FLAG_DISABLED")
	};

	return common::string::FormatFlags(definitions, flags);
}

bool ProviderCallback(const FWPM_PROVIDER0 &provider)
{
	std::wcout << L"Provider" << std::endl;

	std::wcout << "  key:\t\t\t" << common::string::FormatGuid(provider.providerKey) << std::endl;
	std::wcout << "  name:\t\t\t" <<
		(provider.displayData.name == nullptr ? L"n/a" : provider.displayData.name) << std::endl;
	std::wcout << "  description:\t\t" <<
		(provider.displayData.description == nullptr ? L"n/a" : provider.displayData.description) << std::endl;
	std::wcout << "  flags:\t\t" << provider.flags << L" = " << ProviderFlags(provider.flags) << std::endl;
	std::wcout << "  data length:\t\t" << provider.providerData.size << std::endl;
	std::wcout << "  service name:\t\t" <<
		(provider.serviceName == nullptr ? L"n/a" : provider.serviceName) << std::endl;

	return true;
}

bool ConnectionCallback(const FWPM_CONNECTION0 &connection)
{
	std::wcout << L"Connection" << std::endl;

	std::wcout << L"  connection id:\t" << connection.connectionId << std::endl;

	if (connection.ipVersion == FWP_IP_VERSION_V4)
	{
		std::wcout << L"  local addr:\t" << common::string::FormatIpV4(connection.localV4Address) << std::endl;
		std::wcout << L"  remote addr:\t" << common::string::FormatIpV4(connection.remoteV4Address) << std::endl;
	}
	else
	{
		std::wcout << L"  local addr:\t" << common::string::FormatIpV6(connection.localV6Address) << std::endl;
		std::wcout << L"  remote addr:\t" << common::string::FormatIpV6(connection.remoteV6Address) << std::endl;
	}

	std::wcout << L"  provider:\t" << common::string::FormatGuid(*connection.providerKey) << std::endl;
	std::wcout << L"  bytes in:\t" << connection.bytesTransferredIn << std::endl;
	std::wcout << L"  bytes out:\t" << connection.bytesTransferredOut << std::endl;

	// TODO: format and include creation time

	return true;
}

std::wstring FormatIpProtocol(UINT8 protocol)
{
	switch (protocol)
	{
	case 0:		return L"IPPROTO_HOPOPTS";
	case 1:		return L"IPPROTO_ICMP";
	case 2:		return L"IPPROTO_IGMP";
	case 3:		return L"IPPROTO_GGP";
	case 4:		return L"IPPROTO_IPV4";
	case 5:		return L"IPPROTO_ST";
	case 6:		return L"IPPROTO_TCP";
	case 7:		return L"IPPROTO_CBT";
	case 8:		return L"IPPROTO_EGP";
	case 9:		return L"IPPROTO_IGP";
	case 12:	return L"IPPROTO_PUP";
	case 17:	return L"IPPROTO_UDP";
	case 22:	return L"IPPROTO_IDP";
	case 27:	return L"IPPROTO_RDP";
	case 41:	return L"IPPROTO_IPV6";
	case 43:	return L"IPPROTO_ROUTING";
	case 44:	return L"IPPROTO_FRAGMENT";
	case 50:	return L"IPPROTO_ESP";
	case 51:	return L"IPPROTO_AH";
	case 58:	return L"IPPROTO_ICMPV6";
	case 59:	return L"IPPROTO_NONE";
	case 60:	return L"IPPROTO_DSTOPTS";
	case 77:	return L"IPPROTO_ND";
	case 78:	return L"IPPROTO_ICLFXBM";
	case 103:	return L"IPPROTO_PIM";
	case 113:	return L"IPPROTO_PGM";
	case 115:	return L"IPPROTO_L2TP";
	case 132:	return L"IPPROTO_SCTP";
	case 255:	return L"IPPROTO_RAW";
	default:	return L"Unknown";
	};
}

bool EventCallback(const FWPM_NET_EVENT0 &event)
{
	std::wcout << L"Event" << std::endl;

	std::wcout << L"  timestamp:\t" << common::string::FormatTime(event.header.timeStamp) << std::endl;

	if (0 != (event.header.flags & FWPM_NET_EVENT_FLAG_IP_PROTOCOL_SET))
	{
		std::wcout << L"  ip proto:\t" << FormatIpProtocol(event.header.ipProtocol) << std::endl;
	}

	if (0 != (event.header.flags & FWPM_NET_EVENT_FLAG_IP_VERSION_SET)
		&& 0 != (event.header.flags & FWPM_NET_EVENT_FLAG_LOCAL_ADDR_SET))
	{
		if (event.header.ipVersion == FWP_IP_VERSION_V4)
		{
			std::wcout << L"  local addr:\t" << common::string::FormatIpV4(event.header.localAddrV4) << std::endl;
		}
		else
		{
			std::wcout << L"  local addr:\t" << common::string::FormatIpV6(event.header.localAddrV6.byteArray16) << std::endl;
		}
	}

	if (0 != (event.header.flags & FWPM_NET_EVENT_FLAG_IP_VERSION_SET)
		&& 0 != (event.header.flags & FWPM_NET_EVENT_FLAG_REMOTE_ADDR_SET))
	{
		if (event.header.ipVersion == FWP_IP_VERSION_V4)
		{
			std::wcout << L"  remote addr:\t" << common::string::FormatIpV4(event.header.remoteAddrV4) << std::endl;
		}
		else
		{
			std::wcout << L"  remote addr:\t" << common::string::FormatIpV6(event.header.remoteAddrV6.byteArray16) << std::endl;
		}
	}

	if (0 != (event.header.flags & FWPM_NET_EVENT_FLAG_LOCAL_PORT_SET))
	{
		std::wcout << L"  local port:\t" << event.header.localPort << std::endl;
	}

	if (0 != (event.header.flags & FWPM_NET_EVENT_FLAG_REMOTE_PORT_SET))
	{
		std::wcout << L"  remote port:\t" << event.header.remotePort << std::endl;
	}

	if (0 != (event.header.flags & FWPM_NET_EVENT_FLAG_APP_ID_SET))
	{
		std::wcout << L"  app id:\tPresent" << std::endl;
	}

	if (0 != (event.header.flags & FWPM_NET_EVENT_FLAG_USER_ID_SET))
	{
		std::wcout << L"  user id:\tPresent" << std::endl;
	}

	if (0 != (event.header.flags & FWPM_NET_EVENT_FLAG_SCOPE_ID_SET))
	{
		std::wcout << L"  scope id:\tPresent" << std::endl;
	}

	switch (event.type)
	{
	case FWPM_NET_EVENT_TYPE_IKEEXT_MM_FAILURE:
	{
		std::wcout << L"  type: IKEEXT_MM_FAILURE" << std::endl;
		break;
	}
	case FWPM_NET_EVENT_TYPE_IKEEXT_QM_FAILURE:
	{
		std::wcout << L"  type: IKEEXT_QM_FAILURE" << std::endl;
		break;
	}
	case FWPM_NET_EVENT_TYPE_IKEEXT_EM_FAILURE:
	{
		std::wcout << L"  type: IKEEXT_EM_FAILURE" << std::endl;
		break;
	}
	case FWPM_NET_EVENT_TYPE_CLASSIFY_DROP:
	{
		std::wcout << L"  type: CLASSIFY_DROP" << std::endl;
		break;
	}
	case FWPM_NET_EVENT_TYPE_IPSEC_KERNEL_DROP:
	{
		std::wcout << L"  type: IPSEC_KERNEL_DROP" << std::endl;
		break;
	}
	case FWPM_NET_EVENT_TYPE_IPSEC_DOSP_DROP:
	{
		std::wcout << L"  type: IPSEC_DOSP_DROP" << std::endl;
		break;
	}
	case FWPM_NET_EVENT_TYPE_CLASSIFY_ALLOW:
	{
		std::wcout << L"  type: CLASSIFY_ALLOW" << std::endl;
		break;
	}
	case FWPM_NET_EVENT_TYPE_CAPABILITY_DROP:
	{
		std::wcout << L"  type: CAPABILITY_DROP" << std::endl;
		break;
	}
	case FWPM_NET_EVENT_TYPE_CAPABILITY_ALLOW:
	{
		std::wcout << L"  type: CAPABILITY_ALLOW" << std::endl;
		break;
	}
	case FWPM_NET_EVENT_TYPE_CLASSIFY_DROP_MAC:
	{
		std::wcout << L"  type: CLASSIFY_DROP_MAC" << std::endl;
		break;
	}
	default:
	{
		std::wcout << L"  type: Unknown" << std::endl;
	}
	};

	return true;
}

int main()
{
	std::shared_ptr<FilterEngine> engine(new FilterEngine(false));

	//wfp::ObjectEnumerator::Sessions(engine, SessionCallback);
	//wfp::ObjectEnumerator::Providers(engine, ProviderCallback);

	// Turns out this kind of connection tracking is not enabled by default
	//wfp::ObjectEnumerator::Connections(engine, ConnectionCallback);

	// Only four events are recorded for this system after 6 days (?) of uptime
	wfp::ObjectEnumerator::Events(engine, EventCallback);

    return 0;
}


