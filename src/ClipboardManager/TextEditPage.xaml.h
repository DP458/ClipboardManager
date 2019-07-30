#pragma once

#include "TextEditPage.g.h"
#include "CommonTools.h"

namespace ClipboardManager
{

[Windows::Foundation::Metadata::WebHostHidden]
public ref class TextEditPage sealed
{
public:
	TextEditPage();
protected:
	void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
private:
	Windows::Storage::ApplicationDataContainer^ localSettings;
	Windows::ApplicationModel::Resources::ResourceLoader^ resourceLoader;
	Platform::String^ text;
	void Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ButtonAccept_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ButtonCancel_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ButtonSelectAll_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void RichTextEditor_TextChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void RichTextEditor_SelectionChanged(Platform::Object ^sender, Windows::UI::Xaml::RoutedEventArgs ^e);
	void ButtonUndo_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ButtonRedo_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ButtonDelete_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ToggleButtonTextWrapping_StateChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
};

}