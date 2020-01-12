// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include "AboutPage.xaml.h"
#include "SettingsPage.xaml.h"
#include "TextEditPage.xaml.h"
#include "FavoritesPage.xaml.h"
#include "MultiloadPage.xaml.h"

using namespace ClipboardManager;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;

ClipboardManager::MainStaticObject::MainStaticObject() {};

#pragma region Properties

String^ ClipboardManager::MainStaticObject::AppVersionString::get()
{
    PackageVersion version = Package::Current->Id->Version;
    return version.Major.ToString() + "." + version.Minor.ToString() + "." + version.Build.ToString() + "." + version.Revision.ToString();
}

#pragma endregion

#pragma region Commands

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

#pragma region GoToPageCommand

ICommand^ _goToPageCommand;

void ClipboardManager::MainStaticObject::GoToPage(Object^ parameter)
{
    ClipboardManagerPages page = static_cast<ClipboardManagerPages>(parameter);
    Frame^ rootFrame = dynamic_cast<Frame^>(Window::Current->Content);
    if (rootFrame == nullptr)
        return;
    switch (page)
    {
    case ClipboardManagerPages::AboutPage:
        rootFrame->Navigate(TypeName(ClipboardManager::AboutPage::typeid));
        break;

    case ClipboardManagerPages::SettingsPage:
        rootFrame->Navigate(TypeName(ClipboardManager::SettingsPage::typeid));
        break;

    case ClipboardManagerPages::TextEditPage:
        rootFrame->Navigate(TypeName(ClipboardManager::TextEditPage::typeid));
        break;

    case ClipboardManagerPages::FavoritesPage:
        rootFrame->Navigate(TypeName(ClipboardManager::FavoritesPage::typeid));
        break;

    case ClipboardManagerPages::MultiloadPage:
        rootFrame->Navigate(TypeName(ClipboardManager::MultiloadPage::typeid));
        break;
    }
}

ICommand^ ClipboardManager::MainStaticObject::GoToPageCommand::get()
{
    if (_goToPageCommand == nullptr)
    {
        _goToPageCommand = ref new RelayCommand
        (
            ref new ExecuteHandler(GoToPage),
            ref new CanExecuteHandler([](Object^ parameter)->bool {return true; })
        );
    }
    return _goToPageCommand;
}

#pragma endregion

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