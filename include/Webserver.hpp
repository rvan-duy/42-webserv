#pragma once

#include <vector>
#include <Parser.hpp>
#include <Lexer.hpp>
#include <fstream>

int initWebserver(std::vector<Server> *pServers, std::string const &filePath);