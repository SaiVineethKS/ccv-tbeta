//
// SocketAcceptor.h
//
// $Id: //poco/svn/Net/include/Poco/Net/SocketAcceptor.h#2 $
//
// Library: Net
// Package: Reactor
// Module:  SocketAcceptor
//
// Definition of the SocketAcceptor class.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Net_SocketAcceptor_INCLUDED
#define Net_SocketAcceptor_INCLUDED


#include "Poco/Net/Net.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Observer.h"


namespace Poco {
namespace Net {


template <class ServiceHandler>
class SocketAcceptor
	/// This class implements the Acceptor part of the
	/// Acceptor-Connector design pattern.
	///
	/// The Acceptor-Connector pattern has been described in the book
	/// "Pattern Languages of Program Design 3", edited by Robert Martin,
	/// Frank Buschmann and Dirk Riehle (Addison Wesley, 1997).
	///
	/// The Acceptor-Connector design pattern decouples connection
	/// establishment and service initialization in a distributed system
	/// from the processing performed once a service is initialized.
	/// This decoupling is achieved with three components: Acceptors, 
	/// Connectors and Service Handlers.
	/// The SocketAcceptor passively waits for connection requests (usually
	/// from a remote Connector) and establishes a connection upon
	/// arrival of a connection requests. Also, a Service Handler is
	/// initialized to process the data arriving via the connection in
	/// an application-specific way.
	///
	/// The SocketAcceptor sets up a ServerSocket and registers itself
	/// for a ReadableNotification, denoting an incoming connection request.
	///
	/// When the ServerSocket becomes readable the SocketAcceptor accepts
	/// the connection request and creates a ServiceHandler to
	/// service the connection.
	///
	/// The ServiceHandler class must provide a constructor that
	/// takes a StreamSocket and a SocketReactor as arguments,
	/// e.g.:
	///     MyServiceHandler(const StreamSocket& socket, ServiceReactor& reactor)
	///
	/// When the ServiceHandler is done, it must destroy itself.
	///
	/// Subclasses can override the createServiceHandler() factory method
	/// if special steps are necessary to create a ServiceHandler object.
{
public:		
	SocketAcceptor(ServerSocket& socket):
		_socket(socket),
		_pReactor(0)
		/// Creates an SocketAcceptor, using the given ServerSocket.
	{
	}

	SocketAcceptor(ServerSocket& socket, SocketReactor& reactor):
		_socket(socket),
		_pReactor(0)
		/// Creates an SocketAcceptor, using the given ServerSocket.
		/// The SocketAcceptor registers itself with the given SocketReactor.
	{
		registerAcceptor(reactor);
	}

	virtual ~SocketAcceptor()
		/// Destroys the SocketAcceptor.
	{
		unregisterAcceptor();
	}
	
	virtual void registerAcceptor(SocketReactor& reactor)
		/// Registers the SocketAcceptor with a SocketReactor.
		///
		/// A subclass can override this and, for example, also register
		/// an event handler for a timeout event.
		///
		/// The overriding method must call the baseclass implementation first.
	{
		_pReactor = &reactor;
		_pReactor->addEventHandler(_socket, Poco::Observer<SocketAcceptor, ReadableNotification>(*this, &SocketAcceptor::onAccept));
	}
	
	virtual void unregisterAcceptor()
		/// Unregisters the SocketAcceptor.
		///
		/// A subclass can override this and, for example, also unregister
		/// its event handler for a timeout event.
		///
		/// The overriding method must call the baseclass implementation first.
	{
		if (_pReactor)
		{
			_pReactor->removeEventHandler(_socket, Poco::Observer<SocketAcceptor, ReadableNotification>(*this, &SocketAcceptor::onAccept));
		}
	}
	
	void onAccept(ReadableNotification* pNotification)
	{
		pNotification->release();
		StreamSocket sock = _socket.acceptConnection();
		createServiceHandler(sock);
	}
	
protected:
	virtual ServiceHandler* createServiceHandler(StreamSocket& socket)
		/// Create and initialize a new ServiceHandler instance.
		///
		/// Subclasses can override this method.
	{
		return new ServiceHandler(socket, *_pReactor);
	}

	SocketReactor* reactor()
		/// Returns a pointer to the SocketReactor where
		/// this SocketAcceptor is registered.
		///
		/// The pointer may be null.
	{
		return _pReactor;
	}

	Socket& socket()
		/// Returns a reference to the SocketAcceptor's socket.
	{
		return _socket;
	}

private:
	SocketAcceptor();
	SocketAcceptor(const SocketAcceptor&);
	SocketAcceptor& operator = (const SocketAcceptor&);
	
	ServerSocket   _socket;
	SocketReactor* _pReactor;
};


} } // namespace Poco::Net


#endif // Net_SocketAcceptor_INCLUDED
