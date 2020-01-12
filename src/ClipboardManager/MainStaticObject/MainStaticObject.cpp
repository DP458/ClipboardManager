// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"

using namespace ClipboardManager;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Input;

ClipboardManager::MainStaticObject::MainStaticObject() {};

#pragma region Properties

String^ ClipboardManager::MainStaticObject::AppVersionString::get()
{
    PackageVersion version = Package::Current->Id->Version;
    return version.Major.ToString() + "." + version.Minor.ToString() + "." + version.Build.ToString() + "." + version.Revision.ToString();
}

#pragma endregion

#pragma region PageBackwardCommand

ICommand^ _pageBackwardCommand;

void ClipboardManager::MainStaticObject::GoBackward(Object^ parameter)
{
    Frame^ rootFrame = dynamic_cast<Frame^>(Window::Current->Content);
    if (rootFrame == nullptr)
        return;
    if (rootFrame->CanGoBack)
        rootFrame->GoBack();
};

ICommand^ ClipboardManager::MainStaticObject::PageBackwardCommand::get()
{
    if (_pageBackwardCommand == nullptr)
    {
        StandardUICommand^ command = ref new StandardUICommand(StandardUICommandKind::Backward);
        command->Command = ref new RelayCommand
        (
            ref new ExecuteHandler(GoBackward),
            ref new CanExecuteHandler([](Object^ parameter)->bool {return true; })
        );
        _pageBackwardCommand = command;
    }
	return _pageBackwardCommand;
}

#pragma endregion

#pragma region Methods

// Going to previous page
void ClipboardManager::MainStaticObject::GoBack()
{
    Frame^ rootFrame = dynamic_cast<Frame^>(Window::Current->Content);
    if (rootFrame == nullptr)
        return;
    if (rootFrame->CanGoBack)
        rootFrame->GoBack();
}

#pragma endregion