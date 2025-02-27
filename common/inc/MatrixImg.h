#pragma once

#include <cstdint>

typedef const uint8_t MATRIX_IMG[8];

// Big numbers
MATRIX_IMG img_big0 = {0x7E, 0xFF, 0xFF, 0xC3, 0xC3, 0xFF, 0xFF, 0x7E};
MATRIX_IMG img_big1 = {0x00, 0x04, 0x06, 0xFF, 0xFF, 0xFF, 0x00, 0x00};
MATRIX_IMG img_big2 = {0xE6, 0xE7, 0xF7, 0xF3, 0xDB, 0xDF, 0xCF, 0xCE};
MATRIX_IMG img_big3 = {0xC3, 0xC3, 0xDB, 0xDB, 0xDB, 0xFF, 0xFF, 0x66};
MATRIX_IMG img_big4 = {0x3F, 0x3F, 0x3F, 0x30, 0xFF, 0xFF, 0xFF, 0x30};
MATRIX_IMG img_big5 = {0x5F, 0xDF, 0xDF, 0xDB, 0xDB, 0xFB, 0xFB, 0x73};
MATRIX_IMG img_big6 = {0x7E, 0xFF, 0xFF, 0xDB, 0xDB, 0xFB, 0xFB, 0x72};
MATRIX_IMG img_big7 = {0x03, 0x03, 0xE3, 0xF3, 0xFB, 0x1F, 0x0F, 0x07};
MATRIX_IMG img_big8 = {0x66, 0xFF, 0xFF, 0xDB, 0xDB, 0xFF, 0xFF, 0x66};
MATRIX_IMG img_big9 = {0x5E, 0xDF, 0xDF, 0xDB, 0xDB, 0xFF, 0xFF, 0x7E};

// Small numbers
MATRIX_IMG img_left0 = {0x3E, 0x22, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00};
MATRIX_IMG img_left1 = {0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00};
MATRIX_IMG img_left2 = {0x3A, 0x2A, 0x2E, 0x00, 0x00, 0x00, 0x00, 0x00};
MATRIX_IMG img_left3 = {0x2A, 0x2A, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00};
MATRIX_IMG img_left4 = {0x0E, 0x08, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00};
MATRIX_IMG img_left5 = {0x2E, 0x2A, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00};
MATRIX_IMG img_left6 = {0x3E, 0x2A, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x00};
MATRIX_IMG img_left7 = {0x02, 0x02, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00};
MATRIX_IMG img_left8 = {0x3E, 0x2A, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00};
MATRIX_IMG img_left9 = {0x2E, 0x2A, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00};

// Letters
MATRIX_IMG img_LetterA = {0x00, 0x78, 0x24, 0x22, 0x24, 0x78, 0x00, 0x00};
MATRIX_IMG img_LetterB = {0x00, 0x7E, 0x4A, 0x4A, 0x4A, 0x34, 0x00, 0x00};
MATRIX_IMG img_LetterC = {0x00, 0x3C, 0x42, 0x42, 0x42, 0x24, 0x00, 0x00};
MATRIX_IMG img_LetterD = {0x00, 0x7E, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00};
MATRIX_IMG img_LetterE = {0x00, 0x7E, 0x4A, 0x4A, 0x4A, 0x42, 0x00, 0x00};
MATRIX_IMG img_LetterF = {0x00, 0x7E, 0x0A, 0x0A, 0x0A, 0x02, 0x00, 0x00};
MATRIX_IMG img_LetterG = {0x00, 0x3C, 0x42, 0x52, 0x52, 0x34, 0x00, 0x00};
MATRIX_IMG img_LetterH = {0x00, 0x7E, 0x10, 0x10, 0x10, 0x7E, 0x00, 0x00};
MATRIX_IMG img_LetterI = {0x00, 0x00, 0x42, 0x7E, 0x42, 0x00, 0x00, 0x00};
MATRIX_IMG img_LetterJ = {0x00, 0x20, 0x40, 0x42, 0x3E, 0x02, 0x00, 0x00};
MATRIX_IMG img_LetterK = {0x00, 0x7E, 0x08, 0x08, 0x14, 0x62, 0x00, 0x00};
MATRIX_IMG img_LetterL = {0x00, 0x7E, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00};
MATRIX_IMG img_LetterM = {0x00, 0x7E, 0x04, 0x08, 0x04, 0x7E, 0x00, 0x00};
MATRIX_IMG img_LetterN = {0x00, 0x7E, 0x04, 0x08, 0x10, 0x7E, 0x00, 0x00};
MATRIX_IMG img_LetterO = {0x00, 0x3C, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00};
MATRIX_IMG img_LetterP = {0x00, 0x7E, 0x12, 0x12, 0x12, 0x0C, 0x00, 0x00};
MATRIX_IMG img_LetterQ = {0x00, 0x3C, 0x42, 0x42, 0x22, 0x5C, 0x00, 0x00};
MATRIX_IMG img_LetterR = {0x00, 0x7E, 0x12, 0x12, 0x12, 0x6C, 0x00, 0x00};
MATRIX_IMG img_LetterS = {0x00, 0x44, 0x4A, 0x4A, 0x4A, 0x32, 0x00, 0x00};
MATRIX_IMG img_LetterT = {0x00, 0x02, 0x02, 0x7E, 0x02, 0x02, 0x00, 0x00};
MATRIX_IMG img_LetterU = {0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00, 0x00};
MATRIX_IMG img_LetterV = {0x00, 0x1E, 0x20, 0x40, 0x20, 0x1E, 0x00, 0x00};
MATRIX_IMG img_LetterW = {0x00, 0x3E, 0x40, 0x3C, 0x40, 0x3E, 0x00, 0x00};
MATRIX_IMG img_LetterX = {0x00, 0x42, 0x24, 0x18, 0x24, 0x42, 0x00, 0x00};
MATRIX_IMG img_LetterY = {0x00, 0x02, 0x04, 0x78, 0x04, 0x02, 0x00, 0x00};
MATRIX_IMG img_LetterZ = {0x00, 0x42, 0x62, 0x52, 0x4A, 0x46, 0x00, 0x00};

// Big Override letters
MATRIX_IMG img_BigLetterO = {0x3C, 0x7E, 0xC3, 0xC3, 0xC3, 0x7E, 0x3C, 0x00};
MATRIX_IMG img_BigLetterV = {0x3F, 0x7F, 0xE0, 0xC0, 0xE0, 0x7F, 0x3F, 0x00};
MATRIX_IMG img_BigLetterE = {0xFF, 0xFF, 0xDB, 0xDB, 0xDB, 0xC3, 0xC3, 0x00};
MATRIX_IMG img_BigLetterR = {0xFF, 0xFF, 0x33, 0x33, 0x33, 0xFF, 0xEF, 0x00};
MATRIX_IMG img_BigLetterI = {0xC3, 0xC3, 0xFF, 0xFF, 0xFF, 0xC3, 0xC3, 0x00};
MATRIX_IMG img_BigLetterD = {0xFF, 0xFF, 0xC3, 0xC3, 0xE7, 0x7E, 0x3C, 0x00};

