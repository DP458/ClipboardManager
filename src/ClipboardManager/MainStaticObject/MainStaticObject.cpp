// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
//#include "MainStaticObject.h"

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

[Windows::Foundation::Metadata::WebHostHiddenAttribute]
ref class PageBackwardCommandClass sealed : public ICommand
{
public:
    virtual bool CanExecute(Object^ parameter)
    {
        return true;
    };

    virtual void Execute(Object^ parameter)
    {
        Frame^ rootFrame = dynamic_cast<Frame^>(Window::Current->Content);
        if (rootFrame == nullptr)
            return;
        if (rootFrame->CanGoBack)
            rootFrame->GoBack();
    };
    virtual event EventHandler<Object^>^ CanExecuteChanged;
};

ICommand^ _pageBackwardCommand;

ICommand^ ClipboardManager::MainStaticObject::PageBackwardCommand::get()
{
    if (_pageBackwardCommand == nullptr)
        _pageBackwardCommand = ref new PageBackwardCommandClass();
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