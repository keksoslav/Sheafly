# Fetches prebuilt PDFium at configure time, pinned by version and hash.
#
# Source: https://github.com/bblanchon/pdfium-binaries (the standard prebuilt
# distribution of PDFium). Non-V8 build: no embedded JavaScript engine, which
# is what we want. This runs at build/configure time only; the app itself
# makes no network calls.
#
# Defines the imported target `pdfium` and sets PDFIUM_DLL to the runtime DLL
# that must sit next to any executable linking it.

if(NOT WIN32)
    message(FATAL_ERROR "PDFium fetch is Windows-x64 only for now. Add the URL/hash for this platform in cmake/FetchPdfium.cmake.")
endif()

set(SHEAFLY_PDFIUM_VERSION "152.0.7947.0")  # chromium/7947

include(FetchContent)
FetchContent_Declare(pdfium_prebuilt
    URL https://github.com/bblanchon/pdfium-binaries/releases/download/chromium%2F7947/pdfium-win-x64.tgz
    URL_HASH SHA256=75df6802fc090ad7c76ccc29ed80c3fcb1a375c775bbf8e522189174647b101f
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(pdfium_prebuilt)

add_library(pdfium INTERFACE)
target_include_directories(pdfium INTERFACE "${pdfium_prebuilt_SOURCE_DIR}/include")
# MinGW links directly against the DLL's exported C symbols; no import lib needed.
target_link_libraries(pdfium INTERFACE "${pdfium_prebuilt_SOURCE_DIR}/bin/pdfium.dll")

set(PDFIUM_DLL "${pdfium_prebuilt_SOURCE_DIR}/bin/pdfium.dll")
