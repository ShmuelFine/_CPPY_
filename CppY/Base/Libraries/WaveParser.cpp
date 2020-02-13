//
// Created by fine on 12/3/19.
//

#include "WaveParser.h"
//#include "../../../../../../Android/Sdk/ndk/20.1.5948944/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/c++/v1/stdexcept"

#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <exception>
#include <stdexcept>

using std::cin;
using std::cout;
using std::endl;
using std::fstream;
using std::string;



// taken from https://stackoverflow.com/questions/13660777/c-reading-the-data-part-of-a-wav-file

bool LoadWaveMetaData(std::string const& filePath, wav_hdr& wavHeader)
{
	FILE* wavFile = fopen(filePath.c_str(), "rb");
	if (wavFile == nullptr) {
		throw std::runtime_error(("Non existing file " + filePath).c_str());
	}

	//Read the header
	size_t bytesRead = fread(&wavHeader, 1, sizeof(wav_hdr), wavFile);
	return (bytesRead > 0);
}
