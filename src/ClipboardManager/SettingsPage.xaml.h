#pragma once

#include "SettingsPage.g.h"
#include "CommonTools.h"

namespace ClipboardManager
{

[Windows::Foundation::Metadata::WebHostHidden]
public ref class SettingsPage sealed
{
public:
	SettingsPage();
protected:
	void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
private:
	Windows::UI::Xaml::Application^ app;
	Windows::Storage::ApplicationData^ appData;
	Windows::Storage::ApplicationDataContainer^ localSettings;
	Windows::ApplicationModel::Resources::ResourceLoader^ resourceLoader;
	void Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void SetFontSizeSwitcher(int size);
	template<class T> bool SetSetting(Platform::String^ name, T value);
	void ThemeSwitcher_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void OnSelectionChanged(Platform::Object ^sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs ^e);
	void LanguageSwitcherSelectionChanged(Platform::Object ^sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs ^e);
	void CheckBoxFileSavingNotify_StateChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void CheckBoxClearClipboardConfirm_StateChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void CheckBoxClearFavoritesConfirm_StateChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void CheckBoxSuccessAddContentNotify_StateChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ExitSwitcher_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void CheckBoxErrorMessages_StateChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void CheckBoxShowSuccessSaveFavoritesNotify_StateChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void FlyoutResetButton_Click();
	void CheckBoxTextWrapping_StateChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void CheckBoxTextWrappingTextEditPage_StateChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void CheckBoxTextWrappingShareTargetPage_StateChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void CheckBoxShowSuccessAddToFavoritesNotify_StateChanged(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^e);
	void CheckBoxShowSuccessClearFavoritesNotify_StateChanged(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^e);
	void CheckBoxMultiloadPageConfirmClear_StateChanged(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^e);
};

}