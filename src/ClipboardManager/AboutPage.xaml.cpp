// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include "AboutPage.xaml.h"
#include "CommonTools.h"

using namespace ClipboardManager;
using namespace ClipboardManager::CommonTools;

using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Documents;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::ApplicationModel;
using namespace Windows::Storage;

ClipboardManager::AboutPage::AboutPage()
{
	InitializeComponent();
	localSettings = ApplicationData::Current->LocalSettings;
	if (static_cast<bool>(localSettings->Values->Lookup("DarkTheme")))
		RequestedTheme = ElementTheme::Dark;
}


void ClipboardManager::AboutPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Visible;
}

void ClipboardManager::AboutPage::Page_Loaded(Object^ sender, RoutedEventArgs^ e)
{
	if (Package::Current->IsDevelopmentMode)
	{
		TextBlockDescription->Inlines->Append(ref new LineBreak());
		Run^ r = ref new Run();
		r->Text = "The app works in development mode under " + Package::Current->Id->Architecture.ToString() + " architecture.";
		TextBlockDescription->Inlines->Append(r);
	}
}