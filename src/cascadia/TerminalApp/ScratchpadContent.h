// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once
#include "ScratchpadContent.g.h"

namespace winrt::TerminalApp::implementation
{
    struct ScratchpadContent : ScratchpadContentT<ScratchpadContent>
    {
        ScratchpadContent();

        winrt::Windows::UI::Xaml::FrameworkElement GetRoot();

        winrt::Windows::Foundation::Size MinimumSize();

        void Focus(winrt::Windows::UI::Xaml::FocusState reason = winrt::Windows::UI::Xaml::FocusState::Programmatic);
        void Close();
        winrt::Microsoft::Terminal::Settings::Model::NewTerminalArgs GetNewTerminalArgs(const bool asContent) const;

        winrt::hstring Title() { return L"Scratchpad"; }
        uint64_t TaskbarState() { return 0; }
        uint64_t TaskbarProgress() { return 0; }
        bool ReadOnly() { return false; }

        til::typed_event<> ConnectionStateChanged;
        til::typed_event<IPaneContent> CloseRequested;
        til::typed_event<IPaneContent, winrt::TerminalApp::BellEventArgs> BellRequested;
        til::typed_event<IPaneContent> TitleChanged;
        til::typed_event<IPaneContent> TabColorChanged;
        til::typed_event<IPaneContent> TaskbarProgressChanged;
        til::typed_event<IPaneContent> ReadOnlyChanged;
        til::typed_event<IPaneContent> FocusRequested;

    private:
        winrt::Windows::UI::Xaml::Controls::Grid _root{ nullptr };
        winrt::Windows::UI::Xaml::Controls::TextBox _box{ nullptr };
    };
}
