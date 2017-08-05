#include "Server.h"

#include <Lib/Instantiator.h>

namespace Boggart
{
	namespace Transport
	{
		namespace TCP
		{
			Server::Server(std::string myId, std::int32_t port):
				TransportBase(std::string("TCPServer"), myId),
				m_Port(port),
				m_Socket(PAL::Instantiator::ObjectFactory()->CreateSocket(PAL::Object::SocketType::Stream, PAL::Object::SocketFamily::Internetwork)),
				m_ProcessingTimer(nullptr),
				m_IncomingQueue(),
				m_Clients()
			{
				
			}

			Server::~Server()
			{
				
			}

			bool Server::OnOpen()
			{
				bool ret = true;

				ret &= m_Socket->Open();
				ret &= m_Socket->SetNonBlocking();
				ret &= m_Socket->Bind(m_Port);
				ret &= m_Socket->Listen();

				// TODO: Get span from configuration or not. Think about it?
				m_ProcessingTimer = m_TimerManager->Create(Timer::Span_t(50), Timer::Type_t::Periodic, std::bind(&Server::OnProcessingTimerExpired, this));
				m_TimerManager->Start(m_ProcessingTimer);

				return ret;
			}

			bool Server::OnClose()
			{
				bool ret = true;

				for (std::size_t i = 0; i < m_Clients.size(); i++)
				{
					ret &= m_Clients[i]->Close();
				}

				return ret;
			}

			bool Server::OnSend(const std::vector<unsigned char>& data)
			{
				bool ret = true;

				for (std::size_t i = 0; i < m_Clients.size(); i++)
				{
					ret &= m_Clients[i]->Send(data);
				}

				return ret;
			}

			std::vector<unsigned char> Server::OnReceive()
			{
				if (m_IncomingQueue.empty())
				{
					return std::vector<unsigned char>();
				}

				std::vector<unsigned char> data = m_IncomingQueue.front();
				m_IncomingQueue.pop();
				return data;
			}

			void Server::OnProcessingTimerExpired()
			{
				ProcessAsynchronousIncoming();
				ProcessIncomingConnections();
			}

			void Server::ProcessAsynchronousIncoming()
			{
				for (std::size_t i = 0; i < m_Clients.size(); i++)
				{
					std::vector<unsigned char> data = m_Clients[i]->Receive();
					if (data.empty())
					{
						continue;
					}

					m_IncomingQueue.push(data);
				}
			}

			void Server::ProcessIncomingConnections()
			{
				PAL::Object::ISocketPtr client = m_Socket->Accept();
				if (client == nullptr)
				{
					return;
				}

				m_Clients.push_back(client);
			}
		}
	}
}