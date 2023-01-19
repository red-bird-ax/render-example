#pragma once

#include <string>
#include <vector>

std::string ReadText(const std::string& path);
std::vector<uint8_t> ReadBytes(const std::string& path);

std::string ClipFileName(const std::string& path);