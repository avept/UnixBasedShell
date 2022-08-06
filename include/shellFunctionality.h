#pragma once

#include <string>
#include <vector>

void ShellLoop();
std::string GetLineFromShell();
std::vector<std::string> SplitLineForTokens(const std::string& line);
int LaunchShell(std::vector<std::string>& args);

int CommandShellCd(std::vector<std::string>& args);
int CommandShellHelp(std::vector<std::string>& args);
int CommandShellExit(std::vector<std::string>& args);

int ShellExecute(std::vector<std::string>& args);