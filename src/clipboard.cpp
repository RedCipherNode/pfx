#include <pfx/clipboard.hpp>

#include <windows.h>

#include <cstring>

namespace pfx::clipboard
{
    bool copy(std::string_view text)
    {
        if (!OpenClipboard(nullptr))
        {
            return false;
        }

        EmptyClipboard();

        HGLOBAL memory =
            GlobalAlloc(
                GMEM_MOVEABLE,
                text.size() + 1);

        if (memory == nullptr)
        {
            CloseClipboard();
            return false;
        }

        void *buffer =
            GlobalLock(memory);

        if (buffer == nullptr)
        {
            GlobalFree(memory);
            CloseClipboard();
            return false;
        }

        std::memcpy(
            buffer,
            text.data(),
            text.size());

        static_cast<char *>(buffer)[text.size()] = '\0';

        GlobalUnlock(memory);

        if (SetClipboardData(
                CF_TEXT,
                memory) == nullptr)
        {
            GlobalFree(memory);
            CloseClipboard();
            return false;
        }

        CloseClipboard();

        return true;
    }
}