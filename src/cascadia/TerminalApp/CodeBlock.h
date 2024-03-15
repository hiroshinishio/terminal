// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once

#include "CodeBlock.g.h"
#include "RequestRunCommandsArgs.g.h"
#include "../../../src/cascadia/inc/cppwinrt_utils.h"
#include <til/hash.h>

namespace winrt::TerminalApp::implementation
{
    struct CodeBlock : CodeBlockT<CodeBlock>
    {
        CodeBlock(const winrt::hstring& initialCommandlines);

        til::property<winrt::hstring> Commandlines;

        // TODO! this should just be til::property_changed_event but I don't havfe tht commit here
        til::event<winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler> PropertyChanged;
        til::typed_event<TerminalApp::CodeBlock, RequestRunCommandsArgs> RequestRunCommands;

    private:
        friend struct CodeBlockT<CodeBlock>; // for Xaml to bind events

        void _playPressed(const Windows::Foundation::IInspectable& sender, const Windows::UI::Xaml::Input::TappedRoutedEventArgs& e);
    };

    struct RequestRunCommandsArgs : RequestRunCommandsArgsT<RequestRunCommandsArgs>
    {
        RequestRunCommandsArgs(const winrt::hstring& commandlines) :
            Commandlines{ commandlines } {};

        til::property<winrt::hstring> Commandlines;
    };
}

namespace winrt::TerminalApp::factory_implementation
{
    BASIC_FACTORY(CodeBlock);
}