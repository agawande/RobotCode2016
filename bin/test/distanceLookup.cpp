#include "laserProcessor.hpp"

const int distanceLookup[] = {
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //   1 through 10
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0, //  11 through 20
     0,  46,   0,  42,   0,  40,  38,  37,  36,  35, //  21 through 30
    34,  33,  33,  32,  31,  30,  29,  29,  28,  28, //  31 through 40
    27,  27,  26,  26,  25,  24,  24,  23,  23,  22, //  41 through 50
    22,  22,  21,  21,  20,  20,  20,  20,  19,  19, //  51 through 60
    19,  18,  18,  18,  17,  17,  17,  17,  17,  16, //  61 through 70
    16,  16,  16,  15,  15,  15,  15,  15,  15,  14, //  71 through 80
    14,  14,  14,  14,  14,  14,  13,  13,  13,  13, //  81 through 90
    13,  13,  13,  12,  12,  12,  12,  12,  12,  12, //  91 through 100
    12,  12,  11,  11,  11,  11,  11,  11,  11,  11, // 101 through 110
    11,  10,  10,  10,  10,  10,  10,  10,  10,  10, // 111 through 120
    10,  10,   9,   9,   9,   9,   9,   9,   9,   9, // 121 through 130
     9,   9,   9,   9,   9,   9,   9,   9,   9,   9, // 131 through 140
     8,   8,   8,   8,   8,   8,   8,   8,   8,   8, // 141 through 150
     8,   8,   8,   8,   8,   8,   8,   8,   8,   8, // 151 through 160
     7,   7,   7,   7,   7,   7,   7,   7,   7,   7, // 161 through 170
     7,   7,   7,   7,   7,   7,   7,   7,   7,   7, // 171 through 180
     7,   6,   6,   6,   6,   6,   6,   6,   6,   6, // 181 through 190
     6,   6,   6,   6,   6,   6,   6,   6,   6,   6  // 191 through 200 
};