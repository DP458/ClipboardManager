#pragma once

#include "ShareTargetPage.g.h"
#include "CommonTools.h"

namespace ClipboardManager
{

[Windows::Foundation::Metadata::WebHostHidden]
public ref class ShareTargetPage sealed
{
public:
	ShareTargetPage();
protected:
	void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
private:
	Windows::Storage::ApplicationDataContainer^ localSettings;
	Windows::ApplicationModel::Resources::ResourceLoader^ resourceLoader;
	Platform::Agile<Windows::ApplicationModel::DataTransfer::ShareTarget::ShareOperation> shareOperation;
	ClipboardManager::CommonTools::ClipboardDataType dataType;
	Platform::Object^ data;
	bool OperationCompleted;
	void Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void Page_Unloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ButtonCopy_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ButtonCancel_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void TypeSwitcher_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
	void ToggleButtonTextWrapping_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ToggleButtonTextWrapping_Unchecked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
};

}