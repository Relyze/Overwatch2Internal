#pragma once

#include <Windows.h>
#include <future>
#include <type_traits>
#include <winternl.h>
#include <TlHelp32.h>
#include <codecvt>
#include <locale>
#include <string>
#include <sstream>
#include <cstdint>
#include <vector>
#include <memory>
#include <iostream>
#include <stdexcept>  // Include the exception header

struct HandleData {
    unsigned long processId;
    HWND windowHandle;
};

inline bool IsValidPtr(uint64_t address) { return !IsBadReadPtr((const void*)address, sizeof(uint64_t)); }

inline std::string AReadStringFromMemory(uint64_t address, SIZE_T size)
{
    if (IsValidPtr(address))
    {
        const char* buffer = reinterpret_cast<const char*>(address);
        return std::string(buffer, size);
    }
    return "None";
}

template <typename T>
inline T RPM(uint64_t Address) {
    try
    {
        if (IsValidPtr(Address))
            return *(T*)Address;
    }
    catch (...) {}
    return T{};
}

template <typename T>
inline void WPM(uint64_t Address, T Buffer) {
    try
    {
        if (IsValidPtr(Address))
            *(T*)Address = Buffer;
    }
    catch (...) {}
    return;
}

inline void BeginThread(void (*fptr)()) {
    static int count = 0;
    static std::future<void> list[50];
    list[count] = std::async(std::launch::async, fptr);
    count++;
}

inline DWORD GetProcessIdWithName(LPCSTR szProc) {
    DWORD pid = 0;
    PROCESSENTRY32 pe32{ sizeof(pe32) };
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE) {
        if (Process32First(hSnap, &pe32)) {
            do {
                if (!strcmp(szProc, pe32.szExeFile)) {
                    pid = pe32.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &pe32));
        }
        CloseHandle(hSnap);
    }
    return pid;
}


template<typename type>
static std::vector<uintptr_t> arrayscan(const std::string& arrays, type start_address, size_t size) {
    std::vector<uintptr_t> result;
    std::vector<std::pair<uint8_t, bool>> splits;

    //splits
    char delimiter = ' ';
    std::stringstream ss(arrays);

    std::string temp;
    while (std::getline(ss, temp, delimiter)) {
        uint8_t value = 0xCC;
        bool mask = temp == "?" || temp == "??";
        if (!mask) {
            value = (uint8_t)strtol(temp.c_str(), nullptr, 16);
        }
        splits.push_back({ value, mask });
    }

    std::vector<uint32_t> allows = {
        PAGE_EXECUTE, PAGE_EXECUTE_READ, PAGE_EXECUTE_READWRITE, PAGE_EXECUTE_WRITECOPY,
        PAGE_READONLY, PAGE_READWRITE, PAGE_WRITECOPY
    };
    uintptr_t start = (uintptr_t)start_address;
    uintptr_t end = start + size;
    while (start < end) {
        MEMORY_BASIC_INFORMATION mbi = {};
        VirtualQuery((const void*)start, &mbi, sizeof(mbi));
        
        if ((mbi.BaseAddress) &&
            (mbi.RegionSize) &&
            (mbi.State == MEM_COMMIT) &&
            (std::find(allows.begin(), allows.end(), mbi.Protect) != allows.end())) {
            for (uintptr_t n = (uintptr_t)mbi.BaseAddress; n < (uintptr_t)mbi.BaseAddress + mbi.RegionSize - splits.size(); n++) {
                if (mbi.Protect & PAGE_GUARD) {
                    decrypt::force_decryption(n);
                    VirtualQuery((const void*)start, &mbi, sizeof(mbi));

                    if (!(mbi.Protect & PAGE_GUARD))
                        printf("Decrypted page at %llx\n", mbi.BaseAddress);
                }

                if (std::equal(splits.begin(), splits.end(), (uint8_t*)n, [](const auto& find, uint8_t original) {
                    return find.second || find.first == original;
                    })) {
                    result.push_back(n);
                }
            }
        }
        uintptr_t next_address = (uintptr_t)mbi.BaseAddress + mbi.RegionSize;
        start = next_address > start ? next_address : end;
    }

    return result;
}

template<typename type>
static std::vector<uintptr_t> arrayscan_module(const std::string& arrays, type module_address) {
    IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*)module_address;
    IMAGE_NT_HEADERS* nt_headers = (IMAGE_NT_HEADERS*)((uintptr_t)module_address + dos_header->e_lfanew);
    return arrayscan(arrays, module_address, nt_headers->OptionalHeader.SizeOfImage);
}

inline DWORD FindProcessId(LPTSTR szProcessName) {
    HANDLE hSnapShot;
    PROCESSENTRY32 pe = { sizeof(pe) };
    hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapShot == INVALID_HANDLE_VALUE)
        return 0;
    if (!Process32First(hSnapShot, &pe))
        return 0;
    do {
        if (!lstrcmpi(pe.szExeFile, szProcessName)) {
            CloseHandle(hSnapShot);
            return pe.th32ProcessID;
        }
    } while (Process32Next(hSnapShot, &pe));
    CloseHandle(hSnapShot);
    return 0;
}


inline uint64_t ImageBase = *reinterpret_cast<uint64_t*>(__readgsqword(0x60) + 0x10);