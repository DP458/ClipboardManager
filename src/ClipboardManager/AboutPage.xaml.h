#pragma once

#include "AboutPage.g.h"

namespace ClipboardManager
{

[Windows::Foundation::Metadata::WebHostHidden]
public ref class AboutPage sealed
{
public:
	AboutPage();
protected:
	void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
private:
	Windows::Storage::ApplicationDataContainer^ localSettings;
	Windows::ApplicationModel::Package^ package;
	void ShowAppLogo();
	void ShowAppVersion();
	void ShowDevModeCaption();
	void Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ButtonBack_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
};

}