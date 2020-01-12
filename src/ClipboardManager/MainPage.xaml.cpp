// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include "MainPage.xaml.Share.h"
#include "MainPage.xaml.h"
#include "MainPage.xaml.Save.h"
#include "MainPage.xaml.Open.h"

using namespace ClipboardManager;
using namespace ClipboardManager::CommonTools;

using namespace Platform;
using namespace Platform::Collections;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace Windows::ApplicationModel::Resources;
using namespace Windows::UI::Xaml::Documents;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Storage::Pickers;

ClipboardManager::MainPage::MainPage()
{
	InitializeComponent();

	ButtonSave->Command = SaveCommand;

	app = Application::Current;
	appData = ApplicationData::Current;
	historyFolder = nullptr;
	localSettings = appData->LocalSettings;

	if (static_cast<bool>(localSettings->Values->Lookup("DarkTheme")))
		RequestedTheme = ElementTheme::Dark;

	Scroller->FontSize = static_cast<int>(localSettings->Values->Lookup("FontSize"));
	IsConfirmClearEnabled = static_cast<bool>(localSettings->Values->Lookup("ConfirmClear"));
	resourceLoader = ResourceLoader::GetForViewIndependentUse();
	dataTransferManager = DataTransferManager::GetForCurrentView();
	dataTransferManager->DataRequested += ref new TypedEventHandler<DataTransferManager^, DataRequestedEventArgs^>(this, &ClipboardManager::MainPage::OnDataRequested);

	IsClipboardChanged = true;
	IsBackPage = false;
	clipboardDataType = ClipboardDataType::Nothing;
	data = nullptr;
}

#pragma region Properties

#pragma region WaiterState

bool ClipboardManager::MainPage::WaiterState::get()
{
	return Waiter->IsActive;
}

void ClipboardManager::MainPage::WaiterState::set(bool value)
{
	Waiter->IsActive = value;
}

#pragma endregion

#pragma region Properties

IPropertySet^ ClipboardManager::MainPage::Properties::get()
{
	return localSettings->Values;
}

#pragma endregion

#pragma region Res

ResourceLoader^ ClipboardManager::MainPage::Res::get()
{
	return this->resourceLoader;
}

#pragma endregion

#pragma region IsClipboardChanged

bool ClipboardManager::MainPage::IsClipboardChanged::get()
{
	return _isClipboardChanged;
}

void ClipboardManager::MainPage::IsClipboardChanged::set(bool value)
{
	_isClipboardChanged = value;
}

#pragma endregion

#pragma endregion

#pragma region Commands

#pragma region SaveCommand

void ClipboardManager::MainPage::Save()
{
	Waiter->IsActive = true;
	const ClipboardDataType type = clipboardDataType;
	Platform::Object^ obj = data;
	create_task(CreateFileSavePicker(type)->PickSaveFileAsync()).then(MainPagePickSaveFileClass(this, type, obj));
}

ICommand^ ClipboardManager::MainPage::SaveCommand::get()
{
	if (_saveCommand == nullptr)
	{
		StandardUICommand^ command = ref new StandardUICommand(StandardUICommandKind::Save);
		command->Command = ref new RelayCommand
		(
			ref new ExecuteHandler([this](Object^ parameter)->void {Save(); }),
			ref new CanExecuteHandler([](Object^ parameter)->bool {return true; })
		);
		_saveCommand = command;
	}
	return _saveCommand;
}

#pragma endregion

#pragma endregion

void ClipboardManager::MainPage::SetBottomBarButtonState(bool state)
{
	ButtonSave->IsEnabled = state;
	ButtonShare->IsEnabled = state;
	ButtonEdit->IsEnabled = state;
	ButtonClear->IsEnabled = state;
	ButtonFavorite->IsEnabled = state;
}

// Protected

void ClipboardManager::MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{

SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Collapsed;

if (e->NavigationMode == NavigationMode::Back)
	IsBackPage = true;

create_task(appData->LocalFolder->CreateFolderAsync("History", CreationCollisionOption::OpenIfExists)).then
(
[this](task<StorageFolder^> task_folder)
{
try
{
historyFolder = task_folder.get();
}
catch (Exception^ ex)
{
historyFolder = nullptr;
}
}
);

}

// Private

void ClipboardManager::MainPage::OnActivated(Object^ sender, WindowActivatedEventArgs^ e)
{

switch(e->WindowActivationState)
{
case CoreWindowActivationState::Deactivated:
IsActive=false;
break;

default:
IsActive=true;
if (IsClipboardChanged)
	OnContentChanged(nullptr, nullptr);
break;

}

e->Handled=true;

}

void ClipboardManager::MainPage::CreateHamburgerButtonFlyout()
{
	Flyout^ flyout = ref new Flyout();
	flyout->FlyoutPresenterStyle = ref new Windows::UI::Xaml::Style(TypeName(FlyoutPresenter::typeid));
	flyout->FlyoutPresenterStyle->Setters->Append(ref new Setter(FlyoutPresenter::PaddingProperty, Thickness(0, 0, 0, 0)));
	flyout->FlyoutPresenterStyle->Setters->Append(ref new Setter(FlyoutPresenter::BorderBrushProperty, dynamic_cast<Brush^>(app->Resources->Lookup("SystemControlBackgroundAccentBrush"))));
	flyout->FlyoutPresenterStyle->Setters->Append(ref new Setter(FlyoutPresenter::RequestedThemeProperty, RequestedTheme));
	StackPanel^ sp = ref new StackPanel();
	sp->Orientation = Orientation::Vertical;
	sp->Padding = Thickness(0, 10, 0, 10);

	MenuFlyoutCheckBoxTextWrapping = CreateMenuFlyoutCheckBox
	(
		app,
		resourceLoader->GetString("MainPageCheckBoxTextWrapping"),
		ref new RoutedEventHandler(this, &ClipboardManager::MainPage::MenuFlyoutCheckBoxTextWrapping_StateChanged),
		ref new RoutedEventHandler(this, &ClipboardManager::MainPage::MenuFlyoutCheckBoxTextWrapping_StateChanged),
		static_cast<bool>(localSettings->Values->Lookup("TextWrapping"))
	);
	sp->Children->Append(MenuFlyoutCheckBoxTextWrapping);

	sp->Children->Append(ref new Windows::UI::Xaml::Controls::MenuFlyoutSeparator());

	Button^ buttonSettings = CreateMenuFlyoutButton(app, L"\xE115", resourceLoader->GetString("MainPageButtonSettings"), nullptr);
	buttonSettings->Command = MainStaticObject::GoToPageCommand;
	buttonSettings->CommandParameter = ClipboardManagerPages::SettingsPage;
	sp->Children->Append(buttonSettings);

	Button^ buttonInfo = CreateMenuFlyoutButton(app, L"\xE946", resourceLoader->GetString("MainPageButtonInfo"), nullptr);
	buttonInfo->Command = MainStaticObject::GoToPageCommand;
	buttonInfo->CommandParameter = ClipboardManagerPages::AboutPage;
	sp->Children->Append(buttonInfo);

	sp->Children->Append(ref new Windows::UI::Xaml::Controls::MenuFlyoutSeparator());

	sp->Children->Append
	(
		CreateMenuFlyoutButton
		(
			app,
			L"\xE8BB",
			resourceLoader->GetString("MainPageButtonClose"),
			ref new RoutedEventHandler
			(
				[this](Object^ sender, RoutedEventArgs^ e)
				{
					this->app->Exit();
				}
			)
		)
	);
	flyout->Content = sp;
	ButtonHamburger->Flyout = flyout;
}

void ClipboardManager::MainPage::CreateAddButtonFlyout()
{
	Flyout^ flyout = ref new Flyout();
	flyout->FlyoutPresenterStyle = ref new Windows::UI::Xaml::Style(TypeName(FlyoutPresenter::typeid));
	flyout->FlyoutPresenterStyle->Setters->Append(ref new Setter(FlyoutPresenter::PaddingProperty, Thickness(0, 0, 0, 0)));
	flyout->FlyoutPresenterStyle->Setters->Append(ref new Setter(FlyoutPresenter::BorderBrushProperty, dynamic_cast<Brush^>(app->Resources->Lookup("SystemControlBackgroundAccentBrush"))));
	flyout->FlyoutPresenterStyle->Setters->Append(ref new Setter(FlyoutPresenter::RequestedThemeProperty, RequestedTheme));
	StackPanel^ sp = ref new StackPanel();
	sp->Orientation = Orientation::Vertical;
	sp->Padding = Thickness(0, 10, 0, 10);

	Button^ multiAddButton = CreateMenuFlyoutButton(app, L"\xE898", resourceLoader->GetString("MainPageButtonMultiloadingText"), nullptr);
	multiAddButton->Command = MainStaticObject::GoToPageCommand;
	multiAddButton->CommandParameter = ClipboardManagerPages::MultiloadPage;
	sp->Children->Append(multiAddButton);

	sp->Children->Append(ref new Windows::UI::Xaml::Controls::MenuFlyoutSeparator());

	sp->Children->Append
	(
		CreateMenuFlyoutButton
		(
			app,
			L"\xE1A5",
			resourceLoader->GetString("MainPageButtonAddText"),
			ref new RoutedEventHandler(this, &ClipboardManager::MainPage::MenuFlyoutItemAddText_Click)
		)
	);

	sp->Children->Append
	(
		CreateMenuFlyoutButton
		(
			app,
			L"\xE16F",
			resourceLoader->GetString("MainPageButtonAddFiles"),
			ref new RoutedEventHandler(this, &ClipboardManager::MainPage::MenuFlyoutItemAddFiles_Click)
		)
	);

	sp->Children->Append
	(
		CreateMenuFlyoutButton
		(
			app,
			L"\xEB9F",
			resourceLoader->GetString("MainPageButtonAddImage"),
			ref new RoutedEventHandler(this, &ClipboardManager::MainPage::MenuFlyoutItemAddImage_Click)
		)
	);

	flyout->Content = sp;
	ButtonAdd->Flyout = flyout;
}

void ClipboardManager::MainPage::Page_Loaded(Object^ sender, RoutedEventArgs^ e)
{
	CreateHamburgerButtonFlyout();
	CreateAddButtonFlyout();
	if (IsConfirmClearEnabled)
	{
		ButtonClear->Flyout = CreateConfirmFlyout
		(
			RequestedTheme,
			resourceLoader->GetString("MainPageFlyoutClearClipboardMessage"),
			resourceLoader->GetString("MainPageFlyoutClearClipboardButton"),
			ref new FlyoutButtonClick
			(
				[this]()
				{
					if (!ClearClipboard())
						ShowError("Failed to clear the clipboard", "", localSettings->Values);
				}
			)
		);
	}
	if (IsBackPage)
	{
		IsActive = true;
		OnContentChanged(nullptr, nullptr);
		IsBackPage = false;
	}
	windowActivatedEventHandler = Window::Current->Activated += ref new WindowActivatedEventHandler(this, &ClipboardManager::MainPage::OnActivated);
	clipboardContentChangedEventHandler = Clipboard::ContentChanged += ref new EventHandler<Object^>(this, &ClipboardManager::MainPage::OnContentChanged);
}

void ClipboardManager::MainPage::Page_Unloaded(Object^ sender, RoutedEventArgs^ e)
{
	Clipboard::ContentChanged -= clipboardContentChangedEventHandler;
	Window::Current->Activated -= windowActivatedEventHandler;
}

void ClipboardManager::MainPage::ButtonShare_Click(Object^ sender, RoutedEventArgs^ e)
{
	dataTransferManager->ShowShareUI();
}

void ClipboardManager::MainPage::ButtonClear_Click(Object^ sender, RoutedEventArgs^ e)
{
	if (!IsConfirmClearEnabled)
		if (!ClearClipboard())
			ShowError("Failed to clear the clipboard", "", localSettings->Values);
}

void ClipboardManager::MainPage::ComboBox_SelectionChanged(Object^ sender, SelectionChangedEventArgs^ e)
{

	if (TypeSwitcher->SelectedItem == nullptr)
	{
		TypeSwitcher->Items->Clear();
		return;
	}

	Waiter->IsActive = true;
	data = nullptr;
	Scroller->Content = nullptr;

	SetBottomBarButtonState(false);

	ComboBoxItem^ item = dynamic_cast<ComboBoxItem^>(TypeSwitcher->SelectedItem);
	clipboardDataType = static_cast<ClipboardDataType>(item->Tag);

	if
		(
		(clipboardDataType == ClipboardDataType::Nothing) || (clipboardDataType == ClipboardDataType::Unknown)
			)
	{

		Scroller->HorizontalScrollBarVisibility = ScrollBarVisibility::Disabled;

		if (clipboardDataType == ClipboardDataType::Nothing)
			Scroller->Content = CreateStubCaption(resourceLoader->GetString("MainPageEmptyClipboardStub"));
		else
		{
			Scroller->Content = CreateStubCaption(resourceLoader->GetString("MainPageUnknownClipboardDataStub"));
			ButtonClear->IsEnabled = true;
		}

		Waiter->IsActive = false;

		return;

	}

	DataPackageView^ dp_view = GetClipboardContent();

	if (dp_view == nullptr)
	{
		ButtonClear->IsEnabled = true;
		Waiter->IsActive = false;
		ShowError(resourceLoader->GetString("MainPageShowDataError"), "", localSettings->Values);
		return;
	}

	switch (clipboardDataType)
	{

	case ClipboardDataType::Link:

		if (!dp_view->Contains(StandardDataFormats::WebLink))
		{
			ButtonClear->IsEnabled = true;
			Waiter->IsActive = false;
			ShowError(resourceLoader->GetString("MainPageShowLinkError"), "", localSettings->Values);
			return;
		}

		create_task(dp_view->GetWebLinkAsync()).then
		(
			[this](const task<Uri^>& task_link)
			{

				Uri^ link;

				try
				{
					link = task_link.get();
				}
				catch (Exception^)
				{
					ButtonClear->IsEnabled = true;
					Waiter->IsActive = false;
					ShowError(resourceLoader->GetString("MainPageShowLinkError"), "", localSettings->Values);
					return;
				}

				data = link;
				SetLink(Scroller, link);

				ButtonSave->IsEnabled = true;
				ButtonShare->IsEnabled = true;
				ButtonClear->IsEnabled = true;
				ButtonFavorite->IsEnabled = true;
				Waiter->IsActive = false;

				return;

			}
		);
		return;

	case ClipboardDataType::Text:

		if (!dp_view->Contains(StandardDataFormats::Text))
		{
			ButtonClear->IsEnabled = true;
			Waiter->IsActive = false;
			ShowError(resourceLoader->GetString("MainPageShowTextError"), "", localSettings->Values);
			return;
		}

		create_task(dp_view->GetTextAsync(StandardDataFormats::Text)).then
		(
			[this](const task<String^>& task_text)
			{

				String^ text;

				try
				{
					text = task_text.get();
				}
				catch (Exception^)
				{
					ButtonClear->IsEnabled = true;
					Waiter->IsActive = false;
					ShowError(resourceLoader->GetString("MainPageShowTextError"), "", localSettings->Values);
					return;
				}

				data = text;
				SetText(Scroller, text, MenuFlyoutCheckBoxTextWrapping->IsChecked->Value);

				SetBottomBarButtonState(true);

				Waiter->IsActive = false;

				return;

			}
		);
		return;

	case ClipboardDataType::Files:

		if (!dp_view->Contains(StandardDataFormats::StorageItems))
		{
			ButtonClear->IsEnabled = true;
			Waiter->IsActive = false;
			ShowError(resourceLoader->GetString("MainPageShowFilesError"), "", localSettings->Values);
			return;
		}

		create_task(dp_view->GetStorageItemsAsync()).then
		(
			[this](const task<IVectorView<IStorageItem^>^>& task_items)
			{

				IVectorView<IStorageItem^>^ items;

				try
				{
					items = task_items.get();
				}
				catch (Exception^)
				{
					ButtonClear->IsEnabled = true;
					Waiter->IsActive = false;
					ShowError(resourceLoader->GetString("MainPageShowFilesError"), "", localSettings->Values);
					return;
				}

				data = items;
				SetFiles(Scroller, items);

				ButtonSave->IsEnabled = true;
				ButtonShare->IsEnabled = true;
				ButtonClear->IsEnabled = true;
				Waiter->IsActive = false;

				return;

			}
		);
		return;

	case ClipboardDataType::Image:

		if (!dp_view->Contains(StandardDataFormats::Bitmap))
		{
			ButtonClear->IsEnabled = true;
			Waiter->IsActive = false;
			ShowError(resourceLoader->GetString("MainPageShowImageError"), "", localSettings->Values);
			return;
		}

		{

			auto t2 = create_task(dp_view->GetBitmapAsync()).then
			(
				[](const task<RandomAccessStreamReference^>& t)->task<IRandomAccessStreamWithContentType^>
				{

					RandomAccessStreamReference^ bitmap_stream_ref;

					try
					{
						bitmap_stream_ref = t.get();
					}
					catch (Exception^)
					{
						return task<IRandomAccessStreamWithContentType^>();
					}

					return create_task(bitmap_stream_ref->OpenReadAsync());

				}
			);

			BitmapImage^ bitmap_image = ref new BitmapImage();

			auto t3 = t2.then
			(
				[this, bitmap_image](const task<IRandomAccessStreamWithContentType^>& task_image_stream)->task<void>
				{

					IRandomAccessStreamWithContentType^ image_stream;

					try
					{
						image_stream = task_image_stream.get();
					}
					catch (Exception ^ ex)
					{
						return task<void>();
					}

					data = image_stream->CloneStream();

					return create_task(bitmap_image->SetSourceAsync(image_stream));

				}
			);

			t3.then
			(
				[this, bitmap_image](const task<void>& t)
				{

					try
					{
						t.get();
					}
					catch (Exception^)
					{
						ButtonClear->IsEnabled = true;
						Waiter->IsActive = false;
						ShowError(resourceLoader->GetString("MainPageShowImageError"), "", localSettings->Values);
						return;
					}

					SetImage(Scroller, bitmap_image);

					ButtonSave->IsEnabled = true;
					ButtonShare->IsEnabled = true;
					ButtonClear->IsEnabled = true;
					ButtonFavorite->IsEnabled = true;
					Waiter->IsActive = false;

					return;

				}
			);

		}
		return;

	default:
		return;

	}

}

class MainPageWriteTextToHistoryFolderClass
{

private: 
	String^ text;

public:
	explicit MainPageWriteTextToHistoryFolderClass(String^ str);

	~MainPageWriteTextToHistoryFolderClass();

	IAsyncAction^ operator()(const task<StorageFile^>& task_file)const;

};

MainPageWriteTextToHistoryFolderClass::MainPageWriteTextToHistoryFolderClass(String^ str) : text(str)
{

}

MainPageWriteTextToHistoryFolderClass::~MainPageWriteTextToHistoryFolderClass()
{
text = nullptr;
}

IAsyncAction^ MainPageWriteTextToHistoryFolderClass::operator()(const task<StorageFile^>& task_file)const
{

StorageFile^ file;

try
{
	file = task_file.get();
}
catch (Exception^)
{
	return nullptr;
}

if (text == nullptr)
	return nullptr;

return FileIO::WriteTextAsync(file, text);

}

class MainPageAddToFavoritesTextClass
{

private:
	MainPage^ main_page;
	String^ res_successful;
	String^ res_failed;

public:
	explicit MainPageAddToFavoritesTextClass(MainPage^ page, String^ successful, String^ failed);

	~MainPageAddToFavoritesTextClass();

	void operator()(const task<void>& t)const;

};

MainPageAddToFavoritesTextClass::MainPageAddToFavoritesTextClass(MainPage^ page, String^ successful, String^ failed) : main_page(page), res_successful(successful), res_failed(failed)
{

}

MainPageAddToFavoritesTextClass::~MainPageAddToFavoritesTextClass()
{
main_page = nullptr;
res_successful = nullptr;
res_failed = nullptr;
}

void MainPageAddToFavoritesTextClass::operator()(const task<void>& t)const
{

try
{
	t.get();
}
catch (Exception^)
{
	ShowError(main_page->Res->GetString(res_failed), "", main_page->Properties);
	return;
}

if (static_cast<bool>(main_page->Properties->Lookup("ShowSuccessAddToFavoritesNotify")))
	UserNotify(main_page->Res->GetString(res_successful), "", NotificationGroup::None);

}

class MainPageHistoryOpenBitmapClass
{

public:
	IAsyncOperation<IRandomAccessStream^>^ operator()(const task<StorageFile^>& task_file)const;

};

IAsyncOperation<IRandomAccessStream^>^ MainPageHistoryOpenBitmapClass::operator()(const task<StorageFile^>& task_file)const
{

	StorageFile^ file;

	try
	{
		file = task_file.get();
	}
	catch (Exception^)
	{
		return nullptr;
	}

	return file->OpenAsync(FileAccessMode::ReadWrite);

}

class MainPageHistoryCopyStreamToBitmapFileClass
{

private:
	IRandomAccessStream^ bitmapStream;

public:
	explicit MainPageHistoryCopyStreamToBitmapFileClass(IRandomAccessStream^ stream);

	~MainPageHistoryCopyStreamToBitmapFileClass();

	IAsyncOperationWithProgress<unsigned long long, unsigned long long>^ operator()(const task<IRandomAccessStream^>& t)const;

};

MainPageHistoryCopyStreamToBitmapFileClass::MainPageHistoryCopyStreamToBitmapFileClass(IRandomAccessStream^ stream) : bitmapStream(stream)
{

}

MainPageHistoryCopyStreamToBitmapFileClass::~MainPageHistoryCopyStreamToBitmapFileClass()
{
bitmapStream = nullptr;
}

IAsyncOperationWithProgress<unsigned long long, unsigned long long>^ MainPageHistoryCopyStreamToBitmapFileClass::operator()(const task<IRandomAccessStream^>& t)const
{

IRandomAccessStream^ fileStream;

try
{
	fileStream = t.get();
}
catch (Exception^)
{
	return nullptr;
}

if (bitmapStream == nullptr)
	return nullptr;

return RandomAccessStream::CopyAndCloseAsync(bitmapStream->GetInputStreamAt(0ULL), fileStream->GetOutputStreamAt(0ULL));

}

class MainPageAddToFavoritesImageClass
{

private:
	MainPage^ main_page;

public:
	explicit MainPageAddToFavoritesImageClass(MainPage^ page);

	~MainPageAddToFavoritesImageClass();

	void operator()(const task<unsigned long long>& t)const;

};

MainPageAddToFavoritesImageClass::MainPageAddToFavoritesImageClass(MainPage^ page) : main_page(page)
{

}

MainPageAddToFavoritesImageClass::~MainPageAddToFavoritesImageClass()
{
main_page = nullptr;
}

void MainPageAddToFavoritesImageClass::operator()(const task<unsigned long long>& t)const
{

unsigned long long size;

try
{
	size = t.get();
}
catch (Exception^)
{
	ShowError(main_page->Res->GetString("NotifyFailedAddImageToFavorites"), "", main_page->Properties);
	return;
}

if (size == 0ULL)
{
	ShowError(main_page->Res->GetString("NotifyFailedAddImageToFavorites"), "", main_page->Properties);
	return;
}

if (static_cast<bool>(main_page->Properties->Lookup("ShowSuccessAddToFavoritesNotify")))
	UserNotify(main_page->Res->GetString("NotifyAddImageToFavorites"), "", NotificationGroup::None);

}


void ClipboardManager::MainPage::ButtonFavorite_Click(Object^ sender, RoutedEventArgs^ e)
{

const ClipboardDataType type = clipboardDataType;

Object^ obj = data;

if ((historyFolder == nullptr) || (obj == nullptr))
	switch (type)
	{

	case ClipboardDataType::Link:
		ShowError(resourceLoader->GetString("NotifyFailedAddLinkToFavorites"), "", localSettings->Values);
	return;

	case ClipboardDataType::Text:
		ShowError(resourceLoader->GetString("NotifyFailedAddTextToFavorites"), "", localSettings->Values);
	return;

	case ClipboardDataType::Image:
		ShowError(resourceLoader->GetString("NotifyFailedAddImageToFavorites"), "", localSettings->Values);
	return;

	}

if (type == ClipboardDataType::Link)
{

	Uri^ link = dynamic_cast<Uri^>(obj);

	if (link == nullptr)
	{
		ShowError(resourceLoader->GetString("NotifyFailedAddLinkToFavorites"), "", localSettings->Values);
		return;
	}

	auto t = create_task(historyFolder->CreateFileAsync(GenHashCode() + ".tlf", CreationCollisionOption::OpenIfExists)).then(MainPageWriteTextToHistoryFolderClass(link->DisplayUri));

	t.then(MainPageAddToFavoritesTextClass(this, "NotifyAddLinkToFavorites", "NotifyFailedAddLinkToFavorites"));

	return;

}

if (type == ClipboardDataType::Text)
{

	auto t = create_task(historyFolder->CreateFileAsync(GenHashCode() + ".txt", CreationCollisionOption::OpenIfExists)).then
	(
		MainPageWriteTextToHistoryFolderClass(dynamic_cast<String^>(obj))
	);

	t.then(MainPageAddToFavoritesTextClass(this, "NotifyAddTextToFavorites", "NotifyFailedAddTextToFavorites"));

	return;

}

if (type == ClipboardDataType::Image)
{

	auto t = create_task(historyFolder->CreateFileAsync(GenHashCode() + ".bmp", CreationCollisionOption::OpenIfExists)).then(MainPageHistoryOpenBitmapClass());

	auto t2 = t.then
	(
		MainPageHistoryCopyStreamToBitmapFileClass(dynamic_cast<IRandomAccessStream^>(obj))
	);

	t2.then(MainPageAddToFavoritesImageClass(this));

}

}

void ClipboardManager::MainPage::MenuFlyoutItemAddText_Click(Object^ sender, RoutedEventArgs^ e)
{
AddContent(ClipboardDataType::Text);
}

void ClipboardManager::MainPage::MenuFlyoutItemAddFiles_Click(Object^ sender, RoutedEventArgs^ e)
{
AddContent(ClipboardDataType::Files);
}

void ClipboardManager::MainPage::MenuFlyoutItemAddImage_Click(Object^ sender, RoutedEventArgs^ e)
{
AddContent(ClipboardDataType::Image);
}

void ClipboardManager::MainPage::MenuFlyoutCheckBoxTextWrapping_StateChanged(Object^ sender, RoutedEventArgs^ e)
{

localSettings->Values->Insert("TextWrapping", MenuFlyoutCheckBoxTextWrapping->IsChecked->Value);

if (clipboardDataType != ClipboardDataType::Text)
	return;

TextBlock^ tb = dynamic_cast<TextBlock^>(Scroller->Content);

if (tb == nullptr)
	return;

if (MenuFlyoutCheckBoxTextWrapping->IsChecked->Value)
{
	Scroller->HorizontalScrollBarVisibility = ScrollBarVisibility::Disabled;
	tb->TextWrapping = TextWrapping::Wrap;
}
else
{
	Scroller->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;
	tb->TextWrapping = TextWrapping::NoWrap;
}

}

void ClipboardManager::MainPage::OnContentChanged(Object^ sender, Object^ args)
{

	IsClipboardChanged = true;

	if (!IsActive)
		return;

	TypeSwitcher->SelectedItem = nullptr;
	IsClipboardChanged = false;

	DataPackageView^ dp_view = GetClipboardContent();

	if (dp_view == nullptr)
	{
		ShowError(resourceLoader->GetString("MainPageShowDataError"), "", localSettings->Values);
		SetComboBoxItem(TypeSwitcher, ClipboardDataType::Unknown, resourceLoader->GetString("MainPageComboBoxItemUnknown"));
		return;
	}

	if (dp_view->AvailableFormats->Size == 0)
	{
		SetComboBoxItem(TypeSwitcher, ClipboardDataType::Nothing, resourceLoader->GetString("MainPageComboBoxItemNothing"));
		return;
	}

	if (dp_view->Contains(StandardDataFormats::WebLink))
		SetComboBoxItem(TypeSwitcher, ClipboardDataType::Link, resourceLoader->GetString("MainPageComboBoxItemLink"));

	if (dp_view->Contains(StandardDataFormats::Text))
		SetComboBoxItem(TypeSwitcher, ClipboardDataType::Text, resourceLoader->GetString("MainPageComboBoxItemText"));

	if (dp_view->Contains(StandardDataFormats::StorageItems))
		SetComboBoxItem(TypeSwitcher, ClipboardDataType::Files, resourceLoader->GetString("MainPageComboBoxItemFiles"));

	if (dp_view->Contains(StandardDataFormats::Bitmap))
		SetComboBoxItem(TypeSwitcher, ClipboardDataType::Image, resourceLoader->GetString("MainPageComboBoxItemImage"));

	if (TypeSwitcher->SelectedItem == nullptr)
		SetComboBoxItem(TypeSwitcher, ClipboardDataType::Unknown, resourceLoader->GetString("MainPageComboBoxItemUnknown"));

}

void ClipboardManager::MainPage::AddContent(const ClipboardDataType type)
{

if (type == ClipboardDataType::Files)
{

Waiter->IsActive = true;

create_task(CreateFileOpenPicker(type)->PickMultipleFilesAsync()).then(MainPageLoadFilesClass(this));

return;

}

Waiter->IsActive = true;

create_task(CreateFileOpenPicker(type)->PickSingleFileAsync()).then(MainPageLoadDataClass(this, type));

}

void ClipboardManager::MainPage::AddFileType(FileSavePicker^ picker, String^ label, String^ type)
{

Vector<String^>^ file_type = ref new Vector<String^>();

file_type->Append(type);

picker->FileTypeChoices->Insert(label, file_type);

}

FileSavePicker^ ClipboardManager::MainPage::CreateFileSavePicker(ClipboardDataType type)
{

FileSavePicker^ picker = ref new FileSavePicker();

switch (type)
{

case ClipboardDataType::Link:case ClipboardDataType::Text: case ClipboardDataType::Files:
picker->SuggestedFileName = resourceLoader->GetString("FileSavePickerText");
picker->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;
AddFileType(picker, "txt-file", ".txt");
AddFileType(picker, "All files", ".");
break;

case ClipboardDataType::Image:
picker->SuggestedFileName = resourceLoader->GetString("FileSavePickerImage");
picker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;
AddFileType(picker, "jpg-file", ".jpg");
AddFileType(picker, "bmp-file", ".bmp");
AddFileType(picker, "png-file", ".png");
AddFileType(picker, "gif-file", ".gif");
AddFileType(picker, "tiff-file", ".tiff");
break;

default:
break;

}

return picker;

}