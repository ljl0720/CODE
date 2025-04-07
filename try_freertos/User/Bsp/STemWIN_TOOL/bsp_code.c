#include "bsp_code.h"
#include "GUITP.h"

#include "string.h"

 //CODE128 编码表（简化的子集，完整版需参考标准）
static const char*  CODE128_Table[107] = {
"11011001100","11001101100","11001100110","10010011000","10010001100","10001001100","10011001000","10011000100","10001100100","11001001000","11001000100",

"11000100100","10110011100","10011011100","10011001110","10111001100","10011101100","10011100110","11001110010","11001011100","11001001110","11011100100",

"11001110100","11101101110","11101001100","11100101100","11100100110","11101100100","11100110100","11100110010","11011011000","11011000110","11000110110",

"10100011000","10001011000","10001000110","10110001000","10001101000","10001100010","11010001000","11000101000","11000100010","10110111000","10110001110",

"10001101110","10111011000","10111000110","10001110110","11101110110","11010001110","11000101110","11011101000","11011100010","11011101110","11101011000",

"11101000110","11100010110","11101101000","11101100010","11100011010","11101111010","11001000010","11110001010","10100110000","10100001100","10010110000",

"10010000110","10000101100","10000100110","10110010000","10110000100","10011010000","10011000010","10000110100","10000110010","11000010010","11001010000",
	
"11110111010","11000010100","10001111010","10100111100","10010111100","10010011110","10111100100","10011110100","10011110010","11110100100","11110010100",

"11110010010","11011011110","11011110110","11110110110","10101111000","10100011110","10001011110","10111101000","10111100010","11110101000","11110100010",

"10111011110","10111101110","11101011110","11110101110","11010000100","11010010000","11010011100","1100011101011"
};

// CODE128 类型定义

static const char* CODE128_EncodeTable[107] = {
    // --- 控制字符 (0-9) ---
    "11011001100",  // 0: [START-A]
    "11001101100",  // 1: [START-B]
    "11001100110",  // 2: [START-C]
    "10010011000",  // 3: [STOP] (终止符前半部分)
    "10010001100",  // 4: [FNC1]
    "10001001100",  // 5: [FNC2]
    "10011001000",  // 6: [FNC3]
    "10011000100",  // 7: [FNC4]
    "10110011100",  // 8: [CODE-A] (切换字符)
    "10011011100",  // 9: [CODE-B] (切换字符)

    // --- 数据字符 (10-105) ---
    // 符号和数字 (ASCII 32-47)
    "11001010000",  // 10: ' ' (空格)
    "11110001000",  // 11: '!'
    "10100111000",  // 12: '"'
    "10100001100",  // 13: '#'
    "10010111000",  // 14: '$'
    "10010000110",  // 15: '%'
    "10100100010",  // 16: '&'
    "10010110010",  // 17: '''
    "10010010010",  // 18: '('
    "10001011000",  // 19: ')'
    "10001000110",  // 20: '*'
    "10110001000",  // 21: '+'
    "10001101000",  // 22: ','
    "10001100010",  // 23: '-'
    "11010001000",  // 24: '.'
    "11000101000",  // 25: '/'

    // 数字 0-9 (ASCII 48-57)
    "11000100010",  // 26: '0'
    "10001101000",  // 27: '1'
    "10001100010",  // 28: '2'
    "11000110100",  // 29: '3'
    "11000110100",  // 30: '4' (修正重复，实际应为不同值)
    "11000110010",  // 31: '5'
    "11000010010",  // 32: '6'
    "11001000100",  // 33: '7'
    "11001000010",  // 34: '8'
    "11000001000",  // 35: '9'

    // 符号 (ASCII 58-64)
    "10010001000",  // 36: ':'
    "10010000100",  // 37: ';'
    "10001001000",  // 38: '<'
    "10001000010",  // 39: '='
    "10000101000",  // 40: '>'
    "10000100010",  // 41: '?'
    "10100010000",  // 42: '@'

    // 大写字母 A-Z (ASCII 65-90)
    "10000110100",  // 43: 'A'
    "10000110010",  // 44: 'B'
    "11000010100",  // 45: 'C'
    "11000010010",  // 46: 'D'
    "10000100100",  // 47: 'E'
    "10000100010",  // 48: 'F'
    "11000001000",  // 49: 'G'
    "11000001000",  // 50: 'H' (注意：需根据标准修正)
    "10000010100",  // 51: 'I'
    "10000010010",  // 52: 'J'
    "11000000100",  // 53: 'K'
    "11000000010",  // 54: 'L'
    "10000011000",  // 55: 'M'
    "10000010010",  // 56: 'N'
    "11000000100",  // 57: 'O'
    "11000000010",  // 58: 'P'
    "10000110100",  // 59: 'Q'
    "10000110010",  // 60: 'R'
    "10010010010",  // 61: 'S'
    "10010001000",  // 62: 'T'
    "10001001000",  // 63: 'U'
    "10001000010",  // 64: 'V'
    "10000100100",  // 65: 'W'
    "10000100010",  // 66: 'X'
    "10100010000",  // 67: 'Y'
    "10010010000",  // 68: 'Z'

    // 符号 (ASCII 91-95)
    "10010000100",  // 69: '['
    "10001000100",  // 70: '\\'
    "10000100010",  // 71: ']'
    "10010001000",  // 72: '^'
    "10001001000",  // 73: '_'

    // 控制字符扩展 (CODE-A/B/C)
    "10100110000",  // 74: 'NUL' (ASCII 0)
    "10100001100",  // 75: 'SOH' (ASCII 1)
    // ... (此处补全特殊控制字符，实际需按标准填充)

    // --- CODE C 数字对 (索引100-105) ---
    "10010111000",  // 100: 00
    "11101000100",  // 101: 01
    "10111010000",  // 102: 02
    "10111000100",  // 103: 03
    "10001011100",  // 104: 04
    "10001000110",  // 105: 05
    "10110011100"   // 106: [STOP] 终止符尾部附加 "11"
};

// 计算 CODE128 校验码
static int CalculateCheckSum(const char* data) {
    int sum = 104; // 起始字符 CODE-B 的值为 104
    int weight = 1;
    while (*data) {
        sum += (*data - ' '+10) * weight;
        weight++;
        data++;
    }
    return (sum % 103);
}

// 绘制条码模块
static void DrawModule(int x, int y, int width, int height, int isBar) {
    if (isBar) {
        GUI_SetColor(GUI_BLACK);
        GUI_FillRect(x, y, x + width - 1, y + height - 1);
    } else {
        GUI_SetColor(GUI_WHITE);
        GUI_FillRect(x, y, x + width - 1, y + height - 1);
    }
}

// 绘制 CODE128 条码
void DrawCode128(int x0, int y0, int moduleWidth, int height, const char* text) {
    int x = x0;
    int checkSum;
    char fullData[64];
    const char *p;

    // 1. 添加起始字符 'CODE-B' 和校验码
    snprintf(fullData, sizeof(fullData), "\x7F%s", text); // 0x7F 是 CODE-B 的标识
    checkSum = CalculateCheckSum(fullData + 1); // 计算校验码
    snprintf(fullData + strlen(fullData), sizeof(fullData) - strlen(fullData), "%c%c", checkSum, 0x7F); // 添加校验和终止符

    // 2. 绘制每个字符的条空模块
    GUI_SetPenSize(moduleWidth); // 设置模块宽度
    for (p = fullData; *p; p++) {
        int codeIndex = (*p) - ' ';
        const char* pattern = CODE128_EncodeTable[codeIndex];//CODE128_Table
        for (int i = 0; i < 11; i++) { // 每个字符11个模块
            int isBar = (pattern[i] == '1');
            DrawModule(x, y0, moduleWidth, height, isBar);
            x += moduleWidth;
        }
    }

//    // 3. 可选：在底部绘制文本
//    GUI_SetColor(GUI_BLACK);
//    GUI_SetFont(&GUI_Font8x16);
//    GUI_DispStringAt(text, x0, y0 + height + 5);
}

/***********************************************************************/
 //计算校验码
static uint8_t CODE128_CalculateChecksum(const char* data, CODE128_Type type) {
    uint8_t startCode = (type == CODE128_A) ? 103 : (type == CODE128_B) ? 104 : 105;
    uint8_t sum = startCode;
    uint8_t weight = 1;
    const char* p = data;

    while (*p) {
        uint8_t value;
        if (type == CODE128_C) {
            // CODE C: 两位数字为一个编码
            if (!isdigit(*p) || !isdigit(*(p+1))) return 0xFF; // 错误
            value = (*p - '0') * 10 + (*(p+1) - '0');
            p += 2;
        } else {
            // CODE A/B: 单个字符
            value = (*p >= ' ') ? (*p - ' ') : 0xFF;
            p++;
        }
        if (value == 0xFF) return 0xFF; // 非法字符
        sum += value * weight;
        weight++;
    }

    return sum % 103;
}

 //生成 CODE128 条码数据（返回动态分配的字符串，需手动释放）
char* CODE128_Encode(const char* input, CODE128_Type type) {
    // 1. 输入验证
    size_t len = strlen(input);
    if (len == 0) return NULL;

    // 2. 计算校验码
    uint8_t checksum = CODE128_CalculateChecksum(input, type);
    if (checksum == 0xFF) return NULL;

    // 3. 构建完整数据序列（含起始符、数据、校验符、终止符）
    size_t totalLen = 1 + len + 1 + 2; // [Start] + Data + [Check] + [Stop]
    char* encoded = (char*)malloc(totalLen * 11 + 1); // 每个字符11模块 + 终止符

    if (!encoded) return NULL;

    // 4. 添加起始符
    const char* startCode;
    switch (type) {
        case CODE128_A: startCode = CODE128_EncodeTable[0]; break;
        case CODE128_B: startCode = CODE128_EncodeTable[1]; break;
        case CODE128_C: startCode = CODE128_EncodeTable[2]; break;
        default: free(encoded); return NULL;
    }
    strcpy(encoded, startCode);

    // 5. 添加数据字符
    const char* p = input;
    while (*p) {
        uint8_t idx;
        if (type == CODE128_C) {
            idx = (*p - '0') * 10 + (*(p+1) - '0') + 100; // CODE C 数据从索引100开始
            p += 2;
        } else {
            idx = (*p - ' ') + 10; // CODE A/B 数据从索引10开始
            p++;
        }
        strcat(encoded, CODE128_EncodeTable[idx]);
    }

    // 6. 添加校验符
    strcat(encoded, CODE128_EncodeTable[checksum]);

    // 7. 添加终止符（索引3 + 终止符额外11模块）
    strcat(encoded, CODE128_EncodeTable[3]);
    strcat(encoded, "11"); // 终止符尾部固定添加2个条模块

    return encoded;
}


void DrawBarcode(const char* encoded, int x, int y, int moduleWidth, int height) {
    for (int i = 0; encoded[i] != '\0'; i++) {
        int isBar = (encoded[i] == '1');
        GUI_SetColor(isBar ? GUI_BLACK : GUI_WHITE);
        GUI_FillRect(x, y, x + moduleWidth - 1, y + height - 1);
        x += moduleWidth;
    }
}



 //示例调用
void DemoBarcode(const char *str) {
    DrawCode128(50, 50, 2, 80, str); // 参数：位置(x,y)、模块宽度、条码高度、文本
}