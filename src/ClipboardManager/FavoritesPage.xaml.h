#pragma once

#include "FavoritesPage.g.h"
#include "CommonTools.h"

namespace ClipboardManager
{

[Windows::Foundation::Metadata::WebHostHidden]
public ref class FavoritesPage sealed
{
public:
	FavoritesPage();
	void SetText(Platform::String^ text, Windows::Storage::StorageFile^ file);
	void SetLink(Windows::Foundation::Uri^ link, Windows::Storage::StorageFile^ file);
	void SetImage(Windows::Storage::Streams::IRandomAccessStream^ stream, Windows::Storage::StorageFile^ file);
	void SetEmptyPageStub();
	property Windows::Storage::StorageFolder^ HistoryFolder
	{

		Windows::Storage::StorageFolder^ get()
		{
			return historyFolder;
		}

		void set(Windows::Storage::StorageFolder^ value)
		{
			historyFolder = value;
		}

	}
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
	Windows::Storage::ApplicationData^ appData;
	Windows::Storage::StorageFolder^ historyFolder;
	Windows::Storage::ApplicationDataContainer^ localSettings;
	Windows::ApplicationModel::Resources::ResourceLoader^ resourceLoader;
	bool IsConfirmClearFavoritesEnabled;
	bool IsEmpty;
	void Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	Windows::Storage::Pickers::FolderPicker^ CreateFolderPicker();
	void ButtonSaveAll_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ButtonClearFavorites_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ButtonBack_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	Windows::UI::Xaml::Media::SolidColorBrush^ GetBrush(unsigned char A, unsigned char R, unsigned char G, unsigned char B);
	void LoadFavoriteItem(ClipboardManager::CommonTools::ClipboardDataType type, Platform::Object^ data);
	Windows::UI::Xaml::Controls::Grid^ CreateWindowHeader(Platform::String^ title, Windows::Storage::StorageFile^ file);
	Windows::UI::Xaml::UIElement^ CreateWindowBody(const ClipboardManager::CommonTools::ClipboardDataType type, Platform::Object^ data);
	Windows::UI::Xaml::Controls::Grid^ CreateFavoriteWindow(ClipboardManager::CommonTools::ClipboardDataType type, Platform::Object^ data, Windows::Storage::StorageFile^ file);
};

}