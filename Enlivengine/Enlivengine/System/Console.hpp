#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Singleton.hpp>

#include <functional>
#include <map>
#include <string>

#ifdef ENLIVE_DEBUG
#include <thread>
#endif // ENLIVE_DEBUG

namespace en
{

class Console;

class ConsoleInstance
{
public:
	ConsoleInstance();

	void RegisterConsole();
	void UnregisterConsole();
	bool IsRegisteredConsole() const;

	void SendCommand(const std::string& command);

	virtual void AddLine(const std::string& line) = 0;
};

class Console
{
	ENLIVE_SINGLETON(Console);

public:
	void AddLine(const std::string& line);
	void ReceiveCommand(const std::string& command);

	void SetConsoleInstance(ConsoleInstance* instance);
	ConsoleInstance* GetConsoleInstance() const;

	class Command
	{
	public:
		using OptionSplit = std::map<std::string, std::vector<std::string>>;
		using CommandFunction = std::function<void(OptionSplit options)>;

		Command();

		const std::string& GetName() const;
		const std::string& GetHelp() const;
		const std::string& GetManual() const;

		void Execute(const std::string& options = "");

		void SetName(const std::string& name);
		void SetHelp(const std::string& help);
		void SetManual(const std::string& manual);
		void SetFunction(CommandFunction function);

		static void Parse(const std::string& command, std::string& name, std::string& options);
		static OptionSplit ParseOptions(const std::string& options);

	private:
		std::string mName;
		std::string mHelp;
		std::string mManual;
		CommandFunction mFunction;
	};

	void RegisterCommand(const Command& command);

private:
	ConsoleInstance* mConsoleInstance;
	std::vector<Command> mCommands;
};

#ifdef ENLIVE_DEBUG
class DebugConsole : public ConsoleInstance
{
public:
	DebugConsole();
	~DebugConsole();

	virtual void AddLine(const std::string& line);

private:
	void Run(); // TODO : Make thread-safe

private:
	std::thread mThread;
	bool mRunning;
};
#endif // ENLIVE_DEBUG

} // namespace en
