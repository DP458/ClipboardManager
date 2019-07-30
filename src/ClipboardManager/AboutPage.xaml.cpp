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
package = Package::Current;
}


void ClipboardManager::AboutPage::OnNavigatedTo(NavigationEventArgs^ e)
{
SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Visible;
}

void ClipboardManager::AboutPage::ShowAppLogo()
{
BitmapImage^ bitmap_image = ref new BitmapImage();
try
{
bitmap_image->UriSource = package->Logo;
}
catch (Exception^)
{
ShowError("Failed to show logo of the app.", "Possibly is out of memory for this.", localSettings->Values);
return;
}
Logo->Source = bitmap_image;
}

void ClipboardManager::AboutPage::ShowAppVersion()
{
PackageVersion version = package->Id->Version;
String^ major = version.Major.ToString();
String^ minor = version.Minor.ToString();
String^ build = version.Build.ToString();
String^ revision = version.Revision.ToString();
VersionNumber->Text = major + "." + minor + "." + build + "." + revision;
}

void ClipboardManager::AboutPage::ShowDevModeCaption()
{
TextBlockDescription->Inlines->Append(ref new LineBreak());
Run^ r = ref new Run();
r->Text = "The app works in development mode under " + package->Id->Architecture.ToString() + " architecture.";
TextBlockDescription->Inlines->Append(r);
}

void ClipboardManager::AboutPage::Page_Loaded(Object^ sender, RoutedEventArgs^ e)
{
ShowAppLogo();
AppName->Text = package->DisplayName;
ShowAppVersion();
DeveloperName->Text = package->PublisherDisplayName;
if (package->IsDevelopmentMode)
	ShowDevModeCaption();
}

void ClipboardManager::AboutPage::ButtonBack_Click(Object^ sender, RoutedEventArgs^ e)
{
GoBack();
}