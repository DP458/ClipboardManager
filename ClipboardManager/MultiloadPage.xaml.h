#pragma once

#include "MultiloadPage.g.h"
#include "CommonTools.h"

namespace ClipboardManager
{

[Windows::Foundation::Metadata::WebHostHidden]
public ref class MultiloadPage sealed
{
public:
	MultiloadPage();
	void SetText(Platform::String^ text);
	void SetFiles(Windows::Foundation::Collections::IVectorView<Windows::Storage::StorageFile^>^ files);
	void SetImage(Windows::Storage::Streams::InMemoryRandomAccessStream^ stream);
	void PostLoadingProc(ClipboardManager::CommonTools::ClipboardDataType type);
	property bool WaiterState
	{

		bool get()
		{
			return Waiter->IsActive;
		}

		void set(bool value)
		{
			Waiter->IsActive = value;
		}

	}
	property Windows::Foundation::Collections::IPropertySet^ Properties
	{

		Windows::Foundation::Collections::IPropertySet^ get()
		{
			return localSettings->Values;
		}

	}
	property Windows::ApplicationModel::Resources::ResourceLoader^ Res
	{

		Windows::ApplicationModel::Resources::ResourceLoader^ get()
		{
			return this->resourceLoader;
		}

	}
protected:
	void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
private:
	Windows::UI::Xaml::Application^ app;
	Windows::Storage::ApplicationDataContainer^ localSettings;
	Windows::ApplicationModel::Resources::ResourceLoader^ resourceLoader;
	bool IsConfirmClearEnabled;
	bool IsEmpty;
	Platform::String^ Text;
	Platform::Collections::Vector<Windows::Storage::IStorageItem^>^ Files;
	Windows::Storage::Streams::InMemoryRandomAccessStream^ BMP_Stream;
	Windows::UI::Xaml::Controls::Button^ ButtonAddText;
	Windows::UI::Xaml::Controls::Button^ ButtonAddFiles;
	Windows::UI::Xaml::Controls::Button^ ButtonAddImage;
	void CreateAddButtonFlyout();
	void SetEmptyPageStub();
	void Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	Windows::UI::Xaml::Controls::Grid^ CreateWindowHeader(const ClipboardManager::CommonTools::ClipboardDataType type, Platform::String^ title);
	Windows::UI::Xaml::UIElement^ CreateWindowBody(const ClipboardManager::CommonTools::ClipboardDataType type, Platform::Object^ data);
	Windows::UI::Xaml::Controls::Grid^ CreatePreviewWindow(const ClipboardManager::CommonTools::ClipboardDataType type, Platform::Object^ data);
	void MenuFlyoutItemAddText_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void MenuFlyoutItemAddFiles_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void MenuFlyoutItemAddImage_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ButtonClear_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ButtonLoad_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ButtonBack_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void AddContent(const ClipboardManager::CommonTools::ClipboardDataType type);
};

}