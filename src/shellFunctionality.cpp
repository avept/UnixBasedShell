#include "../include/shellFunctionality.h"
#include <iostream>
#include <sstream>
#include <exception>
#include <cstdlib>
#include <regex>
#include <unistd.h>
#include <functional>
#include <string>
#include <sys/wait.h>

namespace 
{
    inline constexpr auto TokenBufferSize = 64;
    inline constexpr auto TokenDelim =  " \t\r\n\a";
    inline constexpr auto CountOfCommands = 3;
}

void ShellLoop()
{
    bool flagForExit = true;

    while(flagForExit)
    {
      std::cout << std::endl;

      std::string line_from_shell = GetLineFromShell();
      auto args = SplitLineForTokens(line_from_shell);
      int status = ShellExecute(args);
    }
}

std::string GetLineFromShell()
{
    std::string line_from_shell = " ";
    size_t bufferSize = 1;

    getline(std::cin, line_from_shell);

    if(line_from_shell == "")
    {
        std::cout << "You could not enter empty string\n enter smth command-like" << std::endl;
    }

    return line_from_shell;
}

std::vector<std::string> SplitLineForTokens(const std::string& line)
{
    std::regex re("[ \\|,:]");
    std::sregex_token_iterator first{line.begin(), line.end(), re, -1}, last;
    std::vector<std::string> tokens{first, last};

    return tokens;
}

int LaunchShell(std::vector<std::string>& args)
{
    pid_t pid, wpid;
    int status = 0;
    pid = fork();

    std::vector<char*> cstrings;
    cstrings.reserve(args.size());

    for(size_t i = 0; i < args.size(); ++i)
    {
        cstrings.push_back(const_cast<char*>(args[i].c_str()));
    }

    if (pid == 0) 
    {
        if(execvp(args[0].c_str(), &cstrings[0]) == -1) 
        {
            std::cout << "error" << std::endl;
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // continue;
    } 
    else 
    {
        do 
        {
        wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int CommandShellCd(std::vector<std::string>& args)
{
  if (args[1].empty()) 
  {
    std::cout << "OwnShell: expected argument to \"cd\"\n" << std::endl;
  } 
  else
  {
    if (chdir(args[1].c_str()) != 0) 
    {
      std::cout << "error" << std::endl;
    }
  }
  return 1;
}

int CommandShellHelp(std::vector<std::string>& args)
{
  std::cout << "You can use man for additional informations about commands\n \"man cd\".\n" << std::endl;
  return 1;
}

int CommandShellExit(std::vector<std::string>& args)
{
  return 0;
}

int ShellExecute(std::vector<std::string>& args)
{
  if(args[0].empty())
  {
    return 1;
  }

  std::string ShellCommands[CountOfCommands] = { "cd", "help", "exit" };
  std::vector<std::function<int(std::vector<std::string>& )>> builtinFunctions = { CommandShellCd, CommandShellHelp, CommandShellExit };

  for (int i = 0; i < CountOfCommands; i++) 
  {
      if(args[0] == ShellCommands[i]) 
      {
        return (builtinFunctions.at(i))(args);
      }
  }

  return LaunchShell(args);
}
