#pragma once

#include "IDiagnostics.h"

#include <Logger/ILogger.h>

namespace Boggart
{
	class DiagnosticsBase : public IDiagnostics
	{
	private:
		std::string m_ModuleName;
		std::string m_Category;

	protected:
		Logger::ILogger* m_Logger;

	protected:
		DiagnosticsBase(std::string moduleName, std::string category);

	public:
		virtual ~DiagnosticsBase() override;

		void InjectLogger(Logger::ILogger* logger);

		virtual void Log(Logger::Level level, const char* format, ...) override;

	protected:
		void OnLoggerInjection();
		virtual void _OnLoggerInjection() = 0;

	protected:
		std::string ModuleName();
		std::string Category();
	};
}