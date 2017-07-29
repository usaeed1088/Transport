#pragma once

#include <Logger/ILogger.h>
#include <Common/Patterns/DependencyInjection/IDependencyInjection.h>

#include <string>
#include <memory>

namespace Boggart
{
	class IDiagnostics : public std::enable_shared_from_this<IDiagnostics>
	{
	public:
		virtual ~IDiagnostics() {}

		virtual void InjectLogger(Logger::ILoggerPtr logger) = 0;

		virtual void ShareLogger(std::shared_ptr<class IDependencyInjection> dependentObject) = 0;
		virtual void Log(Logger::Level level, const char* format, ...) = 0;
	};

	typedef std::shared_ptr<IDiagnostics> IDiagnosticsPtr;
}