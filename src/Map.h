#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <fstream>

namespace FSR {

	struct Map
	{
		std::vector<std::vector<unsigned char>> Data;

		Map(const std::string& path)
			:Data(readBMP(path))
		{
		}

	private:
		std::vector<std::vector<unsigned char>> readBMP(const std::string& filename)
		{
			std::ifstream f;
			f.open(filename, std::ios_base::in | std::ios_base::binary);
			if (!f.is_open())
			{
				std::cout << "File Can not be opened \n";
				__debugbreak();
			}
			const int fileHeaderSize = 14;
			const int infoHeaderSize = 40;

			char fileHeader[fileHeaderSize];
			f.read(fileHeader, fileHeaderSize);

			if (fileHeader[0] != 'B' && fileHeader[1] != 'M')
			{
				std::cout << "Not a bitmap file \n";
				__debugbreak();
			}

			char infoHeader[infoHeaderSize];
			f.read(infoHeader, infoHeaderSize);

			int fileSize = (fileHeader[2] << 0) | (fileHeader[3] << 8) | (fileHeader[4] << 16) | (fileHeader[5] << 24);
			int width = (infoHeader[4] << 0) | (infoHeader[5] << 8) | (infoHeader[6] << 16) | (infoHeader[7] << 24);
			int height = (infoHeader[8] << 0) | (infoHeader[9] << 8) | (infoHeader[10] << 16) | (infoHeader[11] << 24);

			std::vector<std::vector<unsigned char>> colorData(height, std::vector<unsigned char>(width, 0));

			const int padding = ((4 - (width * 3) % 4) % 4);

			for (int y = 63; y >= 0; y--)
			{
				for (int x = 0; x < width; x++)
				{
					unsigned char color[3];
					f.read(reinterpret_cast<char*>(color), 3);
					colorData[y][x] = (static_cast<unsigned char>(color[2]));
				}
				f.ignore(padding);
			}
			f.close();
			return colorData;
		}
	};

}