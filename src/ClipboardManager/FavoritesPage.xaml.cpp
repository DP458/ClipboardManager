// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include "FavoritesPage.xaml.History.h"

using namespace ClipboardManager;
using namespace ClipboardManager::CommonTools;

using namespace Platform;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Documents;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Search;
using namespace Windows::Storage::Streams;
using namespace Windows::ApplicationModel::Resources;

// Public

ClipboardManager::FavoritesPage::FavoritesPage()
{
InitializeComponent();
app = Application::Current;
appData = ApplicationData::Current;
historyFolder = nullptr;
localSettings = appData->LocalSettings;
if (static_cast<bool>(localSettings->Values->Lookup("DarkTheme")))
RequestedTheme = ElementTheme::Dark;
resourceLoader = ResourceLoader::GetForViewIndependentUse();
IsConfirmClearFavoritesEnabled = static_cast<bool>(localSettings->Values->Lookup("ConfirmClearFavorites"));
IsEmpty = true;
}

void ClipboardManager::FavoritesPage::SetText(String^ text, StorageFile^ file)
{

if (IsEmpty)
{
	ItemsControl1->Items->Clear();
	IsEmpty = false;
}

ItemsControl1->Items->Append(CreateFavoriteWindow(ClipboardDataType::Text, text, file));
ElementCountNumber->Text = ItemsControl1->Items->Size.ToString();
ButtonSaveAll->IsEnabled = true;
ButtonClearFavorites->IsEnabled = true;

}

void ClipboardManager::FavoritesPage::SetLink(Uri^ link, StorageFile^ file)
{
	
if (IsEmpty)
{
	ItemsControl1->Items->Clear();
	IsEmpty = false;
}
	
ItemsControl1->Items->Append(CreateFavoriteWindow(ClipboardDataType::Link, link, file));
ElementCountNumber->Text = ItemsControl1->Items->Size.ToString();
ButtonSaveAll->IsEnabled = true;
ButtonClearFavorites->IsEnabled = true;

}

void ClipboardManager::FavoritesPage::SetImage(IRandomAccessStream^ stream, StorageFile^ file)
{

if (IsEmpty)
{
	ItemsControl1->Items->Clear();
	IsEmpty = false;
}

ItemsControl1->Items->Append(CreateFavoriteWindow(ClipboardDataType::Image, stream, file));
ElementCountNumber->Text = ItemsControl1->Items->Size.ToString();
ButtonSaveAll->IsEnabled = true;
ButtonClearFavorites->IsEnabled = true;

}

void ClipboardManager::FavoritesPage::SetEmptyPageStub()
{

ItemsControl1->Items->Clear();

IsEmpty = true;

ItemsControl1->Items->Append
(
	CreateStubCaption(resourceLoader->GetString("FavoritesPageEmptyStub"))
);

ElementCountNumber->Text = "0";
ButtonSaveAll->IsEnabled = false;
ButtonClearFavorites->IsEnabled = false;

}

// Protected

void ClipboardManager::FavoritesPage::OnNavigatedTo(NavigationEventArgs^ e)
{
SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Visible;
}

// Private

void ClipboardManager::FavoritesPage::Page_Loaded(Object^ sender, RoutedEventArgs^ e)
{

Waiter->IsActive = true;

if (IsConfirmClearFavoritesEnabled)
{
	ButtonClearFavorites->Flyout = CreateConfirmFlyout
	(
		RequestedTheme,
		resourceLoader->GetString("FavoritesPageFlyoutClearFavoritesMessage"),
		resourceLoader->GetString("FavoritesPageFlyoutClearFavoritesButton"),
		ref new FlyoutButtonClick
		(
		[this]()
		{

			auto t = create_task(historyFolder->GetFilesAsync(CommonFileQuery::DefaultQuery)).then(FavoritesPageClearFavoritesClass());

			t.then(FavoritesPageClearFavoritesResultsClass(this));

		}
		)
	);
}

SetEmptyPageStub();

auto t = create_task(appData->LocalFolder->CreateFolderAsync("History", CreationCollisionOption::OpenIfExists)).then(FavoritesPageGetHistoryFolderFilesClass(this));

auto t2 = t.then(FavoritesPageShowHistoryFilesClass(this));

t2.then(FavoritesPageShowHistoryFilesResultClass(this));

}

FolderPicker^ ClipboardManager::FavoritesPage::CreateFolderPicker()
{

FolderPicker^ folderPicker = ref new FolderPicker();

folderPicker->FileTypeFilter->Append("*");
folderPicker->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;
folderPicker->ViewMode = PickerViewMode::List;

return folderPicker;

}

ref class SaveAllFaforitesData sealed
{

private:
	IVectorView<StorageFile^>^ files;
	StorageFolder^ folder;

	~SaveAllFaforitesData();

public:
	SaveAllFaforitesData();

	property IVectorView<StorageFile^>^ Files
	{

		IVectorView<StorageFile^>^ get()
		{
			return files;
		}

		void set(IVectorView<StorageFile^>^ value)
		{
			files = value;
		}

	};

	property StorageFolder^ Folder
	{

		StorageFolder^ get()
		{
			return folder;
		}

		void set(StorageFolder^ value)
		{
			folder = value;
		}

	};

};

SaveAllFaforitesData::~SaveAllFaforitesData()
{
files = nullptr;
folder = nullptr;
}

SaveAllFaforitesData::SaveAllFaforitesData()
{
files = nullptr;
folder = nullptr;
}

void ClipboardManager::FavoritesPage::ButtonSaveAll_Click(Object^ sender, RoutedEventArgs^ e)
{
SaveAllFaforitesData^ fv_data = ref new SaveAllFaforitesData();
auto t = create_task(historyFolder->GetFilesAsync(CommonFileQuery::DefaultQuery)).then
(
[this, fv_data](const task<IVectorView<StorageFile^>^>& task_files)->IAsyncOperation<StorageFolder^>^
{
try
{
fv_data->Files = task_files.get();
}
catch (Exception^)
{
fv_data->Files = nullptr;
return nullptr;
}
return CreateFolderPicker()->PickSingleFolderAsync();
}
);
auto t2 = t.then
(
[this, fv_data](const task<StorageFolder^>& t)->task<std::vector<StorageFile^>>
{
try
{
fv_data->Folder = t.get();
}
catch (Exception^)
{
fv_data->Folder = nullptr;
ShowError("Failed to save all your favorites", "Please try it again", localSettings->Values);
return task_from_result(std::vector<StorageFile^>());
}
if ((fv_data->Files == nullptr) || (fv_data->Folder == nullptr))
	return task_from_result(std::vector<StorageFile^>());
std::vector <task<StorageFile^>> taskFiles;
for (unsigned int i = 0; i < fv_data->Files->Size; i++)
	taskFiles.push_back(create_task(fv_data->Files->GetAt(i)->CopyAsync(fv_data->Folder, fv_data->Files->GetAt(i)->Name, NameCollisionOption::GenerateUniqueName)));
return when_all(taskFiles.begin(), taskFiles.end());
}
);
t2.then
(
[this, fv_data](const task<std::vector<StorageFile^>>& t)
{
std::vector<StorageFile^> vFiles;
try
{
vFiles = t.get();
}
catch (Exception^)
{
return;
}
if (vFiles.size() == 0U)
	return;
for (size_t i = 0U; i < vFiles.size(); i++)
	if (vFiles[i]->FileType == ".tlf")
		create_task(vFiles[i]->RenameAsync(vFiles[i]->Name + ".txt", NameCollisionOption::GenerateUniqueName));
if (static_cast<bool>(localSettings->Values->Lookup("ShowSuccessSaveFavoritesNotify")))
	UserNotify(resourceLoader->GetString("NotifySaveFavorites"), fv_data->Folder->Path, NotificationGroup::SaveData);
}
);
}

void ClipboardManager::FavoritesPage::ButtonClearFavorites_Click(Object^ sender, RoutedEventArgs^ e)
{

if (!IsConfirmClearFavoritesEnabled)
{

	auto t = create_task(historyFolder->GetFilesAsync(CommonFileQuery::DefaultQuery)).then(FavoritesPageClearFavoritesClass());

	t.then(FavoritesPageClearFavoritesResultsClass(this));

}

}

SolidColorBrush^ ClipboardManager::FavoritesPage::GetBrush(unsigned char A, unsigned char R, unsigned char G, unsigned char B)
{

Color color = Color();

color.A = A;
color.R = R;
color.G = G;
color.B = B;

return ref new SolidColorBrush(color);

}

void ClipboardManager::FavoritesPage::LoadFavoriteItem(ClipboardDataType type, Object^ data)
{
switch (type)
{
case ClipboardDataType::Link:
{
Uri^ link = dynamic_cast<Uri^>(data);
if (link == nullptr)
{
ShowError("Failed to load selected link into the clipboard", "", localSettings->Values);
break;
}
if (LoadLink(link))
{
if (static_cast<bool>(localSettings->Values->Lookup("ShowSuccessAddContentNotify")))
	UserNotify(resourceLoader->GetString("NotifyCopyLinkToClipboard"), link->DisplayUri, NotificationGroup::CopyToClipboard);
}
else
	ShowError("Failed to load selected link into the clipboard", "", localSettings->Values);
}
break;
case ClipboardDataType::Text:
{
String^ text = dynamic_cast<String^>(data);
if (text == nullptr)
{
ShowError("Failed to load selected text into the clipboard", "", localSettings->Values);
break;
}
if (LoadText(text))
{
if (static_cast<bool>(localSettings->Values->Lookup("ShowSuccessAddContentNotify")))
	UserNotify(resourceLoader->GetString("NotifyCopyTextToClipboard"), text, NotificationGroup::CopyToClipboard);
}
else
	ShowError("Failed to load selected text into the clipboard", "", localSettings->Values);
}
break;
case ClipboardDataType::Image:
{
IRandomAccessStream^ bitmapStream = dynamic_cast<IRandomAccessStream^>(data);
if (bitmapStream == nullptr)
{
ShowError("Failed to load selected image into the clipboard", "", localSettings->Values);
break;
}
if (LoadImage(bitmapStream))
{
if (static_cast<bool>(localSettings->Values->Lookup("ShowSuccessAddContentNotify")))
	UserNotify(resourceLoader->GetString("NotifyCopyImageToClipboard"), "", NotificationGroup::CopyToClipboard);
}
else
	ShowError("Failed to load selected image into the clipboard", "", localSettings->Values);
}
break;
}
}

Grid^ ClipboardManager::FavoritesPage::CreateWindowHeader(String^ title, StorageFile^ file)
{
Grid^ head = ref new Grid();
head->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Top;
head->Height = 28;
head->Background = dynamic_cast<Brush^>(app->Resources->Lookup("SystemControlBackgroundAccentBrush"));
head->BorderThickness = Thickness(0, 0, 0, 1);
head->BorderBrush = dynamic_cast<Brush^>(app->Resources->Lookup("SystemControlBackgroundAccentBrush"));
TextBlock^ tb = ref new TextBlock();
tb->Foreground = ref new SolidColorBrush(Colors::White);
tb->Margin = Thickness(10, 0, 0, 0);
tb->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
tb->FontSize = 16;
tb->FontFamily = ref new Windows::UI::Xaml::Media::FontFamily("Segoe MDL2 Assets");
tb->IsTextSelectionEnabled = false;
tb->Text = L"\xE16D";
head->Children->Append(tb);
TextBlock^ tb2 = ref new TextBlock();
tb2->Foreground = ref new SolidColorBrush(Colors::White);
tb2->Margin = Thickness(40, 0, 0, 0);
tb2->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
tb2->FontSize = 16;
tb2->IsTextSelectionEnabled = false;
tb2->Text = title;
head->Children->Append(tb2);
Button^ button = ref new Button();
button->Background = ref new SolidColorBrush(Colors::Transparent);
button->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Right;
button->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
button->BorderThickness = Thickness(0, 0, 0, 0);
button->RequestedTheme = ElementTheme::Dark;
TextBlock^ tb3 = ref new TextBlock();
tb3->Foreground = ref new SolidColorBrush(Colors::White);
tb3->FontSize = 16;
tb3->FontFamily = ref new Windows::UI::Xaml::Media::FontFamily("Segoe MDL2 Assets");
tb3->IsTextSelectionEnabled = false;
tb3->Text = L"\xE10A";
button->Content = tb3;
button->Click += ref new RoutedEventHandler
(
[this, head, file](Object^ sender, RoutedEventArgs^ e)
{
create_task(file->DeleteAsync(StorageDeleteOption::Default)).then
(
[this, head](task<void> t)
{
try
{
t.get();
Grid^ grid = dynamic_cast<Grid^>(head->Parent);
unsigned int index;
ItemsControl1->Items->IndexOf(grid, &index);
ItemsControl1->Items->RemoveAt(index);
if (ItemsControl1->Items->Size == 0)
{
SetEmptyPageStub();
if (static_cast<bool>(localSettings->Values->Lookup("ExitFavoritesPage")))
	MainStaticObject::GoBack();
return;
}
ElementCountNumber->Text = ItemsControl1->Items->Size.ToString();
}
catch (Exception^ ex)
{
ShowError("Failed to delete the favorite item", "Please try it again", localSettings->Values);
}
}
);
}
);
head->Children->Append(button);
return head;
}

UIElement^ ClipboardManager::FavoritesPage::CreateWindowBody(const ClipboardDataType type, Object^ data)
{
switch (type)
{
case ClipboardDataType::Link:
{
TextBlock^ body = ref new TextBlock();
body->Margin = Thickness(0, 28, 0, 0);
body->IsTextSelectionEnabled = false;
body->FontSize = 12;
body->Padding = Thickness(12);
body->TextWrapping = TextWrapping::Wrap;
body->TextTrimming = TextTrimming::CharacterEllipsis;
body->Tapped += ref new TappedEventHandler
(
[this, type, data](Object^ sender, TappedRoutedEventArgs^ e)
{
LoadFavoriteItem(type, data);
MainStaticObject::GoBack();
}
);
Uri^ link = dynamic_cast<Uri^>(data);
body->Text = link == nullptr ? "" : link->DisplayUri;
return body;
}
case ClipboardDataType::Text:
{
TextBlock^ body = ref new TextBlock();
body->Margin = Thickness(0, 28, 0, 0);
body->IsTextSelectionEnabled = false;
body->FontSize = 12;
body->Padding = Thickness(12);
body->TextWrapping = TextWrapping::Wrap;
body->TextTrimming = TextTrimming::CharacterEllipsis;
body->Tapped += ref new TappedEventHandler
(
[this, type, data](Object^ sender, TappedRoutedEventArgs^ e)
{
LoadFavoriteItem(type, data);
MainStaticObject::GoBack();
}
);
String^ text = dynamic_cast<String^>(data);
if (text == nullptr)
{
body->Text = "";
return body;
}
if (text->Length() > 165)
{
wchar_t s_arr[166];
wcsncpy_s(s_arr, text->Data(), 165);
s_arr[165] = L'\0';
body->Text = ref new String(s_arr);
}
else
	body->Text = text;
return body;
}
case ClipboardDataType::Image:
{
IRandomAccessStream^ stream = dynamic_cast<IRandomAccessStream^>(data);
IRandomAccessStream^ bitmapStream = stream == nullptr ? nullptr : stream->CloneStream();
BitmapImage^ bitmapImage = ref new BitmapImage();
bitmapImage->SetSource(bitmapStream);
Image^ img = ref new Image();
img->Margin = Thickness(0, 28, 0, 0);
img->Tapped += ref new TappedEventHandler
(
[this, type, data](Object^ sender, TappedRoutedEventArgs^ e)
{
LoadFavoriteItem(type, data);
MainStaticObject::GoBack();
}
);
img->Source = bitmapImage;
return img;
}
default: return nullptr;
}
}

Grid^ ClipboardManager::FavoritesPage::CreateFavoriteWindow(ClipboardDataType type, Object^ data, StorageFile^ file)
{
Grid^ grid = ref new Grid();
grid->Height = 185;
grid->Width = 185;
grid->Margin = Thickness(10, 10, 0, 0);
grid->BorderThickness = Thickness(1, 1, 1, 1);
grid->BorderBrush = dynamic_cast<Brush^>(app->Resources->Lookup("SystemControlBackgroundAccentBrush"));
Grid^ head;
UIElement^ body;
switch (type)
{
case ClipboardDataType::Link:
head = CreateWindowHeader(resourceLoader->GetString("FavoritesPageWindowHeaderLink"), file);
break;
case ClipboardDataType::Text:
head = CreateWindowHeader(resourceLoader->GetString("FavoritesPageWindowHeaderText"), file);
break;
case ClipboardDataType::Image:
head = CreateWindowHeader(resourceLoader->GetString("FavoritesPageWindowHeaderImage"), file);
break;
}
grid->Children->Append(head);
grid->Children->Append(CreateWindowBody(type, data));
return grid;
}