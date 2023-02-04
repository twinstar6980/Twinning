#pragma once

using uint8 = unsigned char;

void decompressBlockETC2c (unsigned int block_part1, unsigned int block_part2, uint8 * img, int width, int height, int startx, int starty, int channels);

void decompressBlockAlphaC (uint8 * data, uint8 * img, int width, int height, int ix, int iy, int channels);
