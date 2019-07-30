// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include "MainPage.xaml.h"
#include "ShareTargetPage.xaml.h"

using namespace ClipboardManager;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Storage;

App::App()
{
InitializeComponent();
Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);
Resuming += ref new Windows::Foundation::EventHandler<Platform::Object ^>(this, &ClipboardManager::App::OnResuming);
ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;
InitSetting(localSettings->Values, "DarkTheme", true);
InitSetting(localSettings->Values, "ConfirmClear", true);
InitSetting(localSettings->Values, "FontSize", 16);
InitSetting(localSettings->Values, "ShowSuccessNotify", true);
InitSetting(localSettings->Values, "ConfirmClearFavorites", true);
InitSetting(localSettings->Values, "ShowSuccessAddContentNotify", true);
InitSetting(localSettings->Values, "ExitFavoritesPage", true);
InitSetting(localSettings->Values, "ShowErrors", true);
InitSetting(localSettings->Values, "ShowSuccessSaveFavoritesNotify", true);
InitSetting(localSettings->Values, "TextWrapping", true);
InitSetting(localSettings->Values, "TextWrappingTextEditPage", true);
InitSetting(localSettings->Values, "TextWrappingShareTargetPage", true);
InitSetting(localSettings->Values, "ShowSuccessAddToFavoritesNotify", true);
InitSetting(localSettings->Values, "ShowSuccessClearFavoritesNotify", true);
InitSetting(localSettings->Values, "MultiloadPageConfirmClear", true);
}

template<class T> void ClipboardManager::App::InitSetting(IPropertySet^ values, String^ parameter, T value)
{
if (!values->HasKey(parameter))
	values->Insert(parameter, value);
}

void App::OnLaunched(LaunchActivatedEventArgs^ e)
{
#if _DEBUG
    // Show graphics profiling information while debugging.
    if (IsDebuggerPresent())
    {
        // Display the current frame rate counters
         DebugSettings->EnableFrameRateCounter = true;
    }
#endif
	SystemNavigationManager::GetForCurrentView()->BackRequested += ref new EventHandler<BackRequestedEventArgs ^>(this, &ClipboardManager::App::OnBackRequested);
    auto rootFrame = dynamic_cast<Frame^>(Window::Current->Content);

    // Do not repeat app initialization when the Window already has content,
    // just ensure that the window is active
    if (rootFrame == nullptr)
    {
        // Create a Frame to act as the navigation context and associate it with
        // a SuspensionManager key
        rootFrame = ref new Frame();

        rootFrame->NavigationFailed += ref new NavigationFailedEventHandler(this, &App::OnNavigationFailed);

        if (e->PreviousExecutionState == ApplicationExecutionState::Terminated)
        {
            // TODO: Restore the saved session state only when appropriate, scheduling the
            // final launch steps after the restore is complete

        }

        if (e->PrelaunchActivated == false)
        {
            if (rootFrame->Content == nullptr)
            {
                // When the navigation stack isn't restored navigate to the first page,
                // configuring the new page by passing required information as a navigation
                // parameter
                rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
            }
            // Place the frame in the current Window
            Window::Current->Content = rootFrame;
            // Ensure the current window is active
            Window::Current->Activate();
        }
    }
    else
    {
        if (e->PrelaunchActivated == false)
        {
            if (rootFrame->Content == nullptr)
            {
                // When the navigation stack isn't restored navigate to the first page,
                // configuring the new page by passing required information as a navigation
                // parameter
                rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
            }
            // Ensure the current window is active
            Window::Current->Activate();
        }
    }
}

void App::OnShareTargetActivated(ShareTargetActivatedEventArgs^ e)
{
auto rootFrame = dynamic_cast<Frame^>(Window::Current->Content);
if (rootFrame == nullptr)
{
rootFrame = ref new Frame();
rootFrame->NavigationFailed += ref new NavigationFailedEventHandler(this, &App::OnNavigationFailed);
rootFrame->Navigate(TypeName(ShareTargetPage::typeid), e->ShareOperation);
Window::Current->Content = rootFrame;
Window::Current->Activate();
}
else
{
if (rootFrame->Content == nullptr)
	rootFrame->Navigate(TypeName(ShareTargetPage::typeid), e->ShareOperation);
Window::Current->Activate();
}
}

void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{
(void) sender;  // Unused parameter
(void) e;   // Unused parameter

//TODO: Save application state and stop any background activity
}

void ClipboardManager::App::OnResuming(Object^ sender,Object^ args)
{
auto rootFrame = dynamic_cast<Frame^>(Window::Current->Content);
MainPage^ mainPage = dynamic_cast<MainPage^>(rootFrame->Content);
if (mainPage != nullptr)
	mainPage->SetClipboardChangedFlag(true);
}

void App::OnNavigationFailed(Object ^sender, NavigationFailedEventArgs ^e)
{
    throw ref new FailureException("Failed to load Page " + e->SourcePageType.Name);
}

void ClipboardManager::App::OnBackRequested(Object ^sender, BackRequestedEventArgs ^args)
{
Frame^ rootFrame = dynamic_cast<Frame^>(Window::Current->Content);
if (rootFrame == nullptr)
	return;
if (rootFrame->CanGoBack && args->Handled == false)
{
args->Handled = true;
rootFrame->GoBack();
}
}