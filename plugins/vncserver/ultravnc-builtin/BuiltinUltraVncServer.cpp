/*
 * BuiltinUltraVncServer.cpp - implementation of BuiltinUltraVncServer class
 *
 * Copyright (c) 2017 Tobias Doerffel <tobydox/at/users/dot/sf/dot/net>
 *
 * This file is part of iTALC - http://italc.sourceforge.net
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program (see COPYING); if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#include "BuiltinUltraVncServer.h"
#include "UltraVncConfiguration.h"
#include "UltraVncConfigurationWidget.h"

extern bool Myinit( HINSTANCE hInstance );
extern int WinVNCAppMain();

static BuiltinUltraVncServer* vncServerInstance = nullptr;

extern HINSTANCE	hAppInstance;
extern DWORD		mainthreadId;


void ultravnc_italc_load_password( char* out, int size )
{
	QByteArray password = vncServerInstance->password().toLatin1();
	memcpy( out, password.constData(), std::min<int>( size, password.length() ) );
}



BOOL ultravnc_italc_load_int( LPCSTR valname, LONG *out )
{
	if( strcmp( valname, "LoopbackOnly" ) == 0 )
	{
		*out = 1;
		return true;
	}
	if( strcmp( valname, "DisableTrayIcon" ) == 0 )
	{
		*out = 1;
		return true;
	}
	if( strcmp( valname, "AuthRequired" ) == 0 )
	{
		*out = 1;
		return true;
	}
	if( strcmp( valname, "CaptureAlphaBlending" ) == 0 )
	{
		*out = vncServerInstance->configuration().vncCaptureLayeredWindows() ? 1 : 0;
		return true;
	}
	if( strcmp( valname, "PollFullScreen" ) == 0 )
	{
		*out = vncServerInstance->configuration().vncPollFullScreen() ? 1 : 0;
		return true;
	}
	if( strcmp( valname, "TurboMode" ) == 0 )
	{
		*out = vncServerInstance->configuration().vncLowAccuracy() ? 1 : 0;
		return true;
	}
	if( strcmp( valname, "NewMSLogon" ) == 0 )
	{
		*out = 0;
		return true;
	}
	if( strcmp( valname, "MSLogonRequired" ) == 0 )
	{
		*out = 0;
		return true;
	}
	if( strcmp( valname, "RemoveWallpaper" ) == 0 )
	{
		*out = 0;
		return true;
	}
	if( strcmp( valname, "FileTransferEnabled" ) == 0 )
	{
		*out = 0;
		return true;
	}
	if( strcmp( valname, "AllowLoopback" ) == 0 )
	{
		*out = 1;
		return true;
	}
	if( strcmp( valname, "AutoPortSelect" ) == 0 )
	{
		*out = 0;
		return true;
	}

	if( strcmp( valname, "HTTPConnect" ) == 0 )
	{
		*out = 0;
		return true;
	}

	if( strcmp( valname, "PortNumber" ) == 0 )
	{
		*out = vncServerInstance->serverPort();
		return true;
	}

	return false;
}



BuiltinUltraVncServer::BuiltinUltraVncServer() :
    m_configuration()
{
	vncServerInstance = this;

	// initialize global instance handler and main thread ID
	hAppInstance = GetModuleHandle( NULL );
	mainthreadId = GetCurrentThreadId();
}



BuiltinUltraVncServer::~BuiltinUltraVncServer()
{
	vncServerInstance = nullptr;
}



QWidget* BuiltinUltraVncServer::configurationWidget()
{
	return new UltraVncConfigurationWidget( m_configuration );
}



void BuiltinUltraVncServer::run( int serverPort, const QString& password )
{
	m_serverPort = serverPort;
	m_password = password;

	// run winvnc-server
	Myinit( GetModuleHandle( NULL ) );
	WinVNCAppMain();
}
