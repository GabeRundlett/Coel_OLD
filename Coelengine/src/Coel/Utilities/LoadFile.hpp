#pragma once
#include "clpch.hpp"
#include <string.h>

#include "Coel/Utilities/Log.hpp"

namespace Coel
{
void load_text(const char *filepath, std::string &output)
{
	FILE *file = fopen(filepath, "rt");
	if (!file)
	{
		CL_LOG("ERROR::filepath '" << filepath << "' could not be opened.\n");
		return;
	}
	fseek(file, 0, SEEK_END);
	unsigned long length = ftell(file);
	char *data = new char[length + 1];
	memset(data, 0, length + 1);
	fseek(file, 0, SEEK_SET);
	fread(data, 1, length, file);
	fclose(file);

	output = std::string(data);
	delete[] data;
}
} // namespace Coel
