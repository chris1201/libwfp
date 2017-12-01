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

int main()
{
	std::shared_ptr<FilterEngine> engine(new FilterEngine(false));

	wfp::ObjectEnumerator::Sessions(engine, SessionCallback);
	wfp::ObjectEnumerator::Providers(engine, ProviderCallback);

    return 0;
}


