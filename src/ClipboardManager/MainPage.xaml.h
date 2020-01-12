#pragma once

#include "MainPage.g.h"
#include "CommonTools.h"

namespace ClipboardManager
{

public ref class MainPage sealed 
{
public:
	MainPage();

#pragma region Properties

#pragma region WaiterState

public:
	property bool WaiterState
	{
		bool get();
		void set(bool value);
	}

#pragma endregion

#pragma region Properties

public:
	property Windows::Foundation::Collections::IPropertySet^ Properties
	{
		Windows::Foundation::Collections::IPropertySet^ get();
	}

#pragma endregion

#pragma region Res

public:
	property Windows::ApplicationModel::Resources::ResourceLoader^ Res
	{
		Windows::ApplicationModel::Resources::ResourceLoader^ get();
	}

#pragma endregion

#pragma region IsClipboardChanged

private:
	bool _isClipboardChanged;

public:
	property bool IsClipboardChanged
	{
		bool get();
		void set(bool value);
	}

#pragma endregion

#pragma endregion

public:
	void SetBottomBarButtonState(bool state);

protected:
	void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

private:
	Windows::UI::Xaml::Application^ app;
	Windows::Storage::ApplicationData^ appData;
	Windows::Storage::StorageFolder^ historyFolder;
	Windows::Storage::ApplicationDataContainer^ localSettings;
	bool IsConfirmClearEnabled;
	Windows::ApplicationModel::Resources::ResourceLoader^ resourceLoader;
	Platform::Agile<Windows::ApplicationModel::DataTransfer::DataTransferManager> dataTransferManager;
	bool IsActive;
	bool IsBackPage;
	Windows::Foundation::EventRegistrationToken windowActivatedEventHandler;
	Windows::Foundation::EventRegistrationToken clipboardContentChangedEventHandler;
	ClipboardManager::CommonTools::ClipboardDataType clipboardDataType;
	Platform::Object^ data;
	Windows::UI::Xaml::Controls::CheckBox^ MenuFlyoutCheckBoxTextWrapping;
	void OnActivated(Platform::Object ^sender, Windows::UI::Core::WindowActivatedEventArgs ^e);
	void CreateHamburgerButtonFlyout();
	void CreateAddButtonFlyout();
	void Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void Page_Unloaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ButtonSave_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ButtonShare_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ButtonClear_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void ComboBox_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
	void ButtonFavorite_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void MenuFlyoutItemAddText_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void MenuFlyoutItemAddFiles_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void MenuFlyoutItemAddImage_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void MenuFlyoutCheckBoxTextWrapping_StateChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	void OnContentChanged(Platform::Object^ sender, Platform::Object^ args);
	void AddContent(const ClipboardManager::CommonTools::ClipboardDataType type);
	void AddFileType(Windows::Storage::Pickers::FileSavePicker^ picker, Platform::String^ label, Platform::String^ type);
	Windows::Storage::Pickers::FileSavePicker^ CreateFileSavePicker(ClipboardManager::CommonTools::ClipboardDataType type);
};

}