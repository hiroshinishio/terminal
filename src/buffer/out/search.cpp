// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#include "precomp.h"
#include "search.h"

#include "textBuffer.hpp"
#include "../types/inc/GlyphWidth.hpp"

using namespace Microsoft::Console::Types;

// Routine Description:
// - Constructs a Search object.
// - Make a Search object then call .FindNext() to locate items.
// - Once you've found something, you can perform actions like .Select() or .Color()
// Arguments:
// - textBuffer - The screen text buffer to search through (the "haystack")
// - renderData - The IRenderData type reference, it is for providing selection methods
// - str - The search term you want to find (the "needle")
// - reverse - True when searching backward/upwards in the buffer
// - caseInsensitive - As the name indicates: case insensitivity
Search::Search(Microsoft::Console::Render::IRenderData& renderData, const std::wstring_view str, bool reverse, bool caseInsensitive) :
    _renderData{ &renderData },
    _step{ reverse ? -1 : 1 }
{
    const auto& textBuffer = _renderData->GetTextBuffer();

    _results = textBuffer.SearchText(str, caseInsensitive);
    _mutationCount = textBuffer.GetMutationCount();

    if (_results.empty())
    {
        return;
    }

    const auto highestIndex = gsl::narrow_cast<ptrdiff_t>(_results.size()) - 1;
    auto firstIndex = reverse ? highestIndex : 0;

    if (_renderData->IsSelectionActive())
    {
        const auto anchor = textBuffer.ScreenToBufferPosition(_renderData->GetSelectionAnchor());
        if (reverse)
        {
            for (; firstIndex >= 0 && til::at(_results, firstIndex).start >= anchor; --firstIndex)
            {
            }
        }
        else
        {
            for (; firstIndex <= highestIndex && til::at(_results, firstIndex).start <= anchor; ++firstIndex)
            {
            }
        }
    }

    // We reverse the _index by 1 so that the first call to FindNext() moves it to the firstIndex we found.
    _index = firstIndex - _step;
}

bool Search::IsStale() const noexcept
{
    return !_renderData || _renderData->GetTextBuffer().GetMutationCount() != _mutationCount;
}

// Routine Description:
// - Takes the found word and selects it in the screen buffer
bool Search::SelectNext()
{
    if (_results.empty())
    {
        return false;
    }

    const auto count = gsl::narrow_cast<ptrdiff_t>(_results.size());
    const auto& textBuffer = _renderData->GetTextBuffer();

    _index = (_index + _step + count) % count;

    const auto& s = til::at(_results, _index);
    // Convert buffer selection offsets into the equivalent screen coordinates
    // required by SelectNewRegion, taking line renditions into account.
    const auto selStart = textBuffer.BufferToScreenPosition(s.start);
    const auto selEnd = textBuffer.BufferToScreenPosition(s.end);

    _renderData->SelectNewRegion(selStart, selEnd);
    return true;
}

// Routine Description:
// - Applies the supplied TextAttribute to all search results.
// Arguments:
// - attr - The attribute to apply to the result
void Search::ColorAll(const TextAttribute& attr) const
{
    for (const auto& s : _results)
    {
        _renderData->ColorSelection(s.start, s.end, attr);
    }
}
