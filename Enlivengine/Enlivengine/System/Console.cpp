#include <Enlivengine/System/Console.hpp>

#include <Enlivengine/System/Log.hpp>

#ifdef ENLIVE_DEBUG
#include <iostream>
#endif // ENLIVE_DEBUG

namespace en
{

ConsoleInstance::ConsoleInstance()
{
}

void ConsoleInstance::RegisterConsole()
{
	if (IsRegisteredConsole())
	{
		return;
	}

	Console::GetInstance().SetConsoleInstance(this);
}

void ConsoleInstance::UnregisterConsole()
{
	if (!IsRegisteredConsole())
	{
		return;
	}

	Console::GetInstance().SetConsoleInstance(nullptr);
}

bool ConsoleInstance::IsRegisteredConsole() const
{
	return Console::GetInstance().GetConsoleInstance() == this;
}

void ConsoleInstance::SendCommand(const std::string& command)
{
	if (IsRegisteredConsole())
	{
		Console::GetInstance().ReceiveCommand(command);
	}
}

DebugConsole::DebugConsole()
	: ConsoleInstance()
	, mThread(&DebugConsole::Run, this)
{
}

DebugConsole::~DebugConsole()
{
	if (IsRegisteredConsole())
	{
		UnregisterConsole();
	}
	mRunning = false;
	mThread.join();
}

void DebugConsole::AddLine(const std::string& line)
{
	printf("%s\n", line.c_str());
}

void DebugConsole::Run()
{
	mRunning = true;
	while (mRunning)
	{
		printf("-> ");

		std::string cmd;
		std::getline(std::cin, cmd);

		SendCommand(cmd);

		if (mRunning)
		{
			printf("\n");
		}
	}
}

Console::Console()
	: mConsoleInstance(nullptr)
	, mCommands()
{
	Command help;
	help.SetName("help");
	help.SetHelp("See the list of commands with short description");
	help.SetManual("See the list of commands with short description");
	help.SetFunction([this](Command::OptionSplit options)
	{
                ENLIVE_UNUSED(options);
		for (auto command : mCommands)
		{
			AddLine(command.GetName() + " : " + command.GetHelp());
		}
	});
	RegisterCommand(std::move(help));

	Command man;
	man.SetName("man");
	man.SetHelp("See detailed information for a command");
	man.SetManual("See detailed information for a command\nOptions : commandName\nEx : man help");
	man.SetFunction([this](Command::OptionSplit options)
	{
		if (options["param"].size() >= 1)
		{
			std::string commandName = options["param"][0];
			for (auto command : mCommands)
			{
				if (command.GetName() == commandName)
				{
					AddLine(command.GetManual());
				}
			}
		}
		else
		{
			AddLine("Error with the options, ex : man help");
		}
	});
	RegisterCommand(std::move(man));
}

void Console::AddLine(const std::string& line)
{
	if (mConsoleInstance != nullptr)
	{
		mConsoleInstance->AddLine(line);
	}
}

void Console::ReceiveCommand(const std::string& command)
{
	AddLine("# " + command + '\n');

	std::string name;
	std::string options;
	Command::Parse(command, name, options);

	for (U32 i = 0; i < mCommands.size(); i++)
	{
		if (mCommands[i].GetName() == name)
		{
			mCommands[i].Execute(options);
		}
	}
}

void Console::SetConsoleInstance(ConsoleInstance* instance)
{
	mConsoleInstance = instance;
}

ConsoleInstance* Console::GetConsoleInstance() const
{
	return mConsoleInstance;
}

void Console::RegisterCommand(const Command& command)
{
	mCommands.push_back(command);
}

Console::Command::Command()
	: mName("")
	, mHelp("")
	, mManual("")
{
}

const std::string& Console::Command::GetName() const
{
	return mName;
}

const std::string& Console::Command::GetHelp() const
{
	return mHelp;
}

const std::string& Console::Command::GetManual() const
{
	return mManual;
}

void Console::Command::Execute(const std::string& options)
{
	if (mFunction)
	{
		mFunction(ParseOptions(options));
	}
	else
	{
		LogError(en::LogChannel::System, 6, "Command not properly set%s\n", "");
	}
}

void Console::Command::SetName(const std::string& name)
{
	mName = name;
}

void Console::Command::SetHelp(const std::string& help)
{
	mHelp = help;
}

void Console::Command::SetManual(const std::string& manual)
{
	mManual = manual;
}

void Console::Command::SetFunction(CommandFunction function)
{
	mFunction = function;
}

void Console::Command::Parse(const std::string& command, std::string& name, std::string& options)
{
	std::size_t found = command.find_first_of(" ");
	if (found != std::string::npos)
	{
		name = command.substr(0, found);
		options = command.substr(found + 1);
	}
	else
	{
		name = command;
		options.clear();
	}
}

Console::Command::OptionSplit Console::Command::ParseOptions(const std::string& options)
{
	OptionSplit split;
	if (options.size() > 0)
	{
		bool open = false;
		bool readParam = true;
		bool readOption = false;
		bool readArgs = false;
		std::size_t readPos = 0;
		std::string actualOption("");
		for (std::size_t i = 0; i < options.size(); i++)
		{
			if (options[i] == ' ' && readParam)
			{
				if (readPos == 0)
				{
					split["param"].push_back(options.substr(readPos, i - readPos));
				}
				else
				{
					split["param"].push_back(options.substr(readPos + 1, i - readPos - 1));
				}
				readPos = i;
			}
			else if (options[i] == '-' && !open)
			{
				readArgs = false;
				readParam = false;
				readOption = true;
				readPos = i;
			}
			else if (options[i] == ' ' && readOption == true)
			{
				actualOption = options.substr(readPos + 1, i - readPos - 1);
				split[actualOption] = {};
				readOption = false;
				readArgs = true;
				readPos = i;
			}
			else if (options[i] == ' ' && readArgs && !open)
			{
				std::string arg = options.substr(readPos + 1, i - readPos - 1);
				if (arg != "")
				{
					if (arg[0] == '\"' && arg[arg.size() - 1] == '\"')
					{
						arg = arg.substr(1, arg.size() - 2);
					}
					split[actualOption].push_back(arg);
				}
				readPos = i;
			}
			else if (options[i] == '\"' && readArgs)
			{
				open = !open;
			}
		}
		if (readParam)
		{
			split["param"].push_back(options.substr(readPos + ((readPos == 0) ? 0 : 1)));
		}
		else if (readOption)
		{
			split[options.substr(readPos + 1)] = {};
		}
		else if (readArgs)
		{
			std::string arg = options.substr(readPos + 1);
			if (arg != "")
			{
				if (arg[0] == '\"' && arg[arg.size() - 1] == '\"')
				{
					arg = arg.substr(1, arg.size() - 2);
				}
				split[actualOption].push_back(arg);
			}
		}
	}
	return split;
}

} // namespace en
