#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>

class FileHelper {

public:

	static std::string getFileContents(std::string const& path) {

        std::string _path = std::filesystem::current_path().string() + "/" + path;
		std::ifstream fStream(_path);

		if (!fStream.is_open()) { return ""; }

		std::stringstream sStream;
		std::string string;

		while (fStream.good()) {
			std::getline(fStream, string);
			sStream << string << std::endl;
		}

		return sStream.str();

	}

};
