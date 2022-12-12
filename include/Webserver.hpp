#pragma once

#include <vector>
#include <Parser.hpp>
#include <Lexer.hpp>
#include <fstream>
#include <Socket.hpp>

int initWebserver(std::vector<Socket> *pSockets, std::string const &filePath);