#pragma once
#include <Windows.h>
#include <vector>

size_t IntLength(int value) {
    int count = 0;
    for (int i = 0; i < 10; i++) {
        count++;
        value /= 10;
        if (value == 0)
            break;
    }
    return count;
}

size_t StringLength(const char* text) {
    return strlen(text);
}

char IntToChar(int value) {
    if (value < 0 || value > 9) {
        return '0';
    }

    return (char)(value + 48);
}

int CharToInt(char value) {
    if (value < 48 || value > 57) {
        return 0;
    }
    return (int)(value - 48);
}

void IntToString(int value, char* string, int& intSize) {
    std::vector<int> splitValue;
    for (int i = 0; i < 10; i++) {
        splitValue.insert(splitValue.begin(), value % 10);
        value /= 10;
        if (value == 0)
            break;
    }
    intSize = splitValue.size();
    for (int i = 0; i < intSize; i++) {
        string[i] = IntToChar(splitValue[i]);
    }
    splitValue.clear();
}

int ConcatInt(int a, int b) {
    int times = 1;
    while (times <= b) {
        times *= 10;
    }
    return a * times + b;
}

int StringToInt(const char* str, int strLen) {
    int val = 0;
    for (int i = 0; i < strLen; i++) {
        int value = CharToInt(str[i]);
        val = ConcatInt(val, value);
    }
    return val;
}

void repeat(char* text, char* out, char repeat = ' ', int textLen = 0, int maxLength = 10) {
    if (textLen == 0)
        textLen = strlen(text);
    int repeatLength = maxLength - textLen;
    if (repeatLength < 1)
        return;

    for (int i = textLen - 1; i < repeatLength; i++) {
        out[i] = repeat;
    }
}

void ClampSize(const char* text, char* out, size_t maxLength, char replaceChar = ' ') {
    size_t textLength = StringLength(text);
    if (textLength > maxLength)
        return;
    size_t additionalLength = maxLength - textLength;
    for (int i = 0; i < textLength; i++) {
        out[i] = text[i];
    }
    for (int i = textLength; i < additionalLength; i++) {
        out[i] = replaceChar;
    }
}