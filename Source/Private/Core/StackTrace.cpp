#include <Core/StackTrace.h>
#include <Core/EngineMacros.h>
#include <Exception/Exception.h>
#include <fmt/format.h>

#ifdef _WIN64

#include <Windows.h>
#include <DbgHelp.h>
#include <winnt.h>

#else

#include <execinfo.h>
#include <cxxabi.h>

#endif

StackFrame::StackFrame(const char* function, const char* file, const char* library, size_t line) : lineNumber(file ? line : -1) {

    String string;
    Array<String> storage;
    
    if (function) {
        string = function;
        storage = string.split('\\');
        this->functionName = storage.getAt(storage.getSize() - 1);
    }
    else {
        this->functionName = "External function";
    }

    if (file) {
        string = file;
        storage = string.split('\\');
        this->fileName = storage.getAt(storage.getSize() - 1);
    }
    else {
        this->fileName = "External file";
    }

    if (library) {
        string = library;
        storage = string.split('\\');
        this->libraryName = storage.getAt(storage.getSize() - 1);
    }
    else {
        this->libraryName = "External library";
    }
    
}

bool StackFrame::operator==(const StackFrame& other) const {
    
    return this->functionName == other.functionName
        && this->fileName == other.fileName
        && this->libraryName == other.libraryName
        && this->lineNumber == other.lineNumber;
    
}

bool StackFrame::operator!=(const StackFrame& other) const { return !(*this == other); }

bool StackFrame::operator<(const StackFrame& other) const {
    
    return false;
    
}

#ifdef _WIN64

Array<StackFrame> StackTrace::getStackTrace() {

    Array<StackFrame> stack;
    Allocator<char> allocator;
    constexpr DWORD machineType = IMAGE_FILE_MACHINE_AMD64;
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();
    STACKFRAME64 frame{};
    CONTEXT context;
    DWORD64 displacement = 0;
    DWORD offset = 0;

    char* name = allocator.alloc(MAX_PATH);
    char* symData = allocator.alloc(sizeof(IMAGEHLP_SYMBOL64) + MAX_PATH);
    char* lineData = allocator.alloc(sizeof(IMAGEHLP_LINE64));
    char* moduleData = allocator.alloc(sizeof(IMAGEHLP_MODULE64));

    const auto symbol = reinterpret_cast<IMAGEHLP_SYMBOL64*>(symData);
    const auto line = reinterpret_cast<IMAGEHLP_LINE64*>(lineData);
    const auto mod = reinterpret_cast<IMAGEHLP_MODULE64*>(moduleData);

    symbol->MaxNameLength = MAX_PATH;
    symbol->SizeOfStruct = sizeof(*symbol);

    line->SizeOfStruct = sizeof(IMAGEHLP_LINE64);

    mod->SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
    
    RtlCaptureContext(&context);

    frame.AddrPC.Offset = context.Rip;
    frame.AddrStack.Offset = context.Rsp;
    frame.AddrFrame.Offset = context.Rbp;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrStack.Mode = AddrModeFlat;
    frame.AddrFrame.Mode = AddrModeFlat;
    
    if (SymInitialize(process, nullptr, true)) {
                    
        while (StackWalk64(machineType, process, thread, &frame, &context, nullptr,
            SymFunctionTableAccess64, SymGetModuleBase64, nullptr)) {
        
            const bool foundSymbol = SymGetSymFromAddr64(process, frame.AddrPC.Offset, &displacement, symbol);
        
            const bool foundModule = SymGetModuleInfo64(process, frame.AddrPC.Offset, mod);
        
            const bool foundLine = SymGetLineFromAddr64(process, frame.AddrPC.Offset, &offset, line);

            UnDecorateSymbolName(symbol->Name, name, sizeof(name), UNDNAME_COMPLETE);

            stack += StackFrame(foundSymbol ? symbol->Name : nullptr,
                                foundLine ? line->FileName : nullptr,
                                foundModule ? mod->ImageName : nullptr,
                                foundLine ? line->LineNumber : 0);
            
        }
        
    }

    allocator.release(name);
    allocator.release(symData);
    allocator.release(lineData);
    allocator.release(moduleData);

    return stack;

}

#else

Array<StackFrame> StackTrace::getStackTrace() {
    
    constexpr Int32 maxSize = 1024;
    Array<StackFrame> stack = Array<StackFrame>(maxSize);
    Allocator<char> allocator;
    
    char* allocData = allocator.alloc(maxSize);
    void* data = reinterpret_cast<void*>(allocData);

    Int32 frameCount = backtrace(&data, maxSize);
    char** symbols = backtrace_symbols(&data, frameCount);
    
    Array<String> symArray;
    
    for (size_t i = 0; i < frameCount; ++i) {
        symArray += symbols[i];
    }
    
    for (const auto& sym : symArray) {
        
        Array<String> parts = sym.split(' ');
        
        String libName = parts[1];
        String symName = parts[3];
        
        int status = -1;

        char* demangled = abi::__cxa_demangle(symName.toCString(), nullptr, nullptr, &status);

        stack += StackFrame(status == 0 ? demangled : symName.toCString(), nullptr, libName.toCString(), 0);
        
        if (status == 0) {
            free(demangled);
        }
        
    }
        
    return stack;
    
}

#endif
