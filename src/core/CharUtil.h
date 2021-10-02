
#pragma once

void wtoc(char* Dest, const wchar_t* Source);
const char *wtoc_s(const wchar_t* Source);

void ctow(wchar_t* Dest, const char* Source);
const wchar_t *ctow_s(const char* Source);
