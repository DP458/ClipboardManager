// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include "MultiloadPage.xaml.h"
#include <MultiloadPage.xaml.Open.h>

using namespace ClipboardManager;
using namespace ClipboardManager::CommonTools;

using namespace Platform;
using namespace Platform::Collections;
using namespace concurrency;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace Windows::ApplicationModel::Resources;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Documents;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;

// Public

MultiloadPage::MultiloadPage()
{
InitializeComponent();
app = Application::Current;
localSettings = ApplicationData::Current->LocalSettings;
if (static_cast<bool>(localSettings->Values->Lookup("DarkTheme")))
	RequestedTheme = ElementTheme::Dark;
resourceLoader = ResourceLoader::GetForViewIndependentUse();
IsConfirmClearEnabled = static_cast<bool>(localSettings->Values->Lookup("MultiloadPageConfirmClear"));
Files = ref new Vector<IStorageItem^>();
}

void ClipboardManager::MultiloadPage::SetText(String^ text)
{

Text = text;

if (IsEmpty)
{
	ItemsControl1->Items->Clear();
	IsEmpty = false;
}

ItemsControl1->Items->Append(CreatePreviewWindow(ClipboardDataType::Text, Text));

}

void ClipboardManager::MultiloadPage::SetFiles(IVectorView<StorageFile^>^ files)
{

for (unsigned int i = 0; i < files->Size; i++)
	Files->Append(files->GetAt(i));

if (IsEmpty)
{
	ItemsControl1->Items->Clear();
	IsEmpty = false;
}

ItemsControl1->Items->Append(CreatePreviewWindow(ClipboardDataType::Files, Files));

}

void ClipboardManager::MultiloadPage::SetImage(InMemoryRandomAccessStream^ stream)
{


BMP_Stream = stream;

if (IsEmpty)
{
	ItemsControl1->Items->Clear();
	IsEmpty = false;
}

ItemsControl1->Items->Append(CreatePreviewWindow(ClipboardDataType::Image, BMP_Stream));

}

void ClipboardManager::MultiloadPage::PostLoadingProc(ClipboardDataType type)
{

switch (type)
{

case ClipboardDataType::Text:
ButtonAddText->IsEnabled = false;
break;

case ClipboardDataType::Files:
ButtonAddFiles->IsEnabled = false;
break;

case ClipboardDataType::Image:
ButtonAddImage->IsEnabled = false;
break;

default:
return;

}

ElementCountNumber->Text = ItemsControl1->Items->Size.ToString();
ButtonLoad->IsEnabled = true;
ButtonClear->IsEnabled = true;
Waiter->IsActive = false;

}

// Protected

void ClipboardManager::MultiloadPage::OnNavigatedTo(NavigationEventArgs^ e)
{
SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Visible;
}

// Private

void ClipboardManager::MultiloadPage::CreateAddButtonFlyout()
{
Flyout^ flyout = ref new Flyout();
flyout->FlyoutPresenterStyle = ref new Windows::UI::Xaml::Style(TypeName(FlyoutPresenter::typeid));
flyout->FlyoutPresenterStyle->Setters->Append(ref new Setter(FlyoutPresenter::PaddingProperty,Thickness(0,0,0,0)));
flyout->FlyoutPresenterStyle->Setters->Append(ref new Setter(FlyoutPresenter::BorderBrushProperty, dynamic_cast<Brush^>(app->Resources->Lookup("SystemControlBackgroundAccentBrush"))));
flyout->FlyoutPresenterStyle->Setters->Append(ref new Setter(FlyoutPresenter::RequestedThemeProperty, RequestedTheme));
StackPanel^ sp = ref new StackPanel();
sp->Orientation = Orientation::Vertical;
sp->Padding = Thickness(0, 10, 0, 10);
ButtonAddText = CreateMenuFlyoutButton
(
app,
L"\xE1A5",
resourceLoader->GetString("MultiloadPageButtonAddText"),
ref new RoutedEventHandler(this, &ClipboardManager::MultiloadPage::MenuFlyoutItemAddText_Click)
);
sp->Children->Append(ButtonAddText);
ButtonAddFiles = CreateMenuFlyoutButton
(
app,
L"\xE16F",
resourceLoader->GetString("MultiloadPageButtonAddFiles"),
ref new RoutedEventHandler(this, &ClipboardManager::MultiloadPage::MenuFlyoutItemAddFiles_Click)
);
sp->Children->Append(ButtonAddFiles);
ButtonAddImage = CreateMenuFlyoutButton
(
app,
L"\xEB9F",
resourceLoader->GetString("MultiloadPageButtonAddImage"),
ref new RoutedEventHandler(this, &ClipboardManager::MultiloadPage::MenuFlyoutItemAddImage_Click)
);
sp->Children->Append(ButtonAddImage);
flyout->Content = sp;
ButtonAdd->Flyout = flyout;
}

void ClipboardManager::MultiloadPage::SetEmptyPageStub()
{
ItemsControl1->Items->Clear();
IsEmpty = true;
Text = nullptr;
Files->Clear();
BMP_Stream = nullptr;
TextBlock^ tb = CreateStubCaption(resourceLoader->GetString("MultiloadPageEmptyStub"));
ItemsControl1->Items->Append(tb);
ElementCountNumber->Text = "0";
ButtonAddText->IsEnabled = true;
ButtonAddFiles->IsEnabled = true;
ButtonAddImage->IsEnabled = true;
ButtonClear->IsEnabled = false;
ButtonLoad->IsEnabled = false;
}

void ClipboardManager::MultiloadPage::Page_Loaded(Object^ sender, RoutedEventArgs^ e)
{
CreateAddButtonFlyout();
SetEmptyPageStub();
if (IsConfirmClearEnabled)
{
ButtonClear->Flyout=CreateConfirmFlyout
(
RequestedTheme,
resourceLoader->GetString("MultiloadPageFlyoutClearMessage"),
resourceLoader->GetString("MultiloadPageFlyoutClearButton"),
ref new FlyoutButtonClick
(
[this]()
{
SetEmptyPageStub();
}
)
);
}
}

Grid^ ClipboardManager::MultiloadPage::CreateWindowHeader(const ClipboardDataType type, String^ title)
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
[this, type, head](Object^ sender, RoutedEventArgs^ e)
{
Grid^ grid = dynamic_cast<Grid^>(head->Parent);
if (grid == nullptr)
{
ShowError("Failed to delete the item", "", localSettings->Values);
return;
}
unsigned int index;
ItemsControl1->Items->IndexOf(grid, &index);
ItemsControl1->Items->RemoveAt(index);
if (ItemsControl1->Items->Size == 0)
{
SetEmptyPageStub();
return;
}
ElementCountNumber->Text = ItemsControl1->Items->Size.ToString();
switch (type)
{
case ClipboardDataType::Text:
Text = nullptr;
ButtonAddText->IsEnabled = true;
break;
case ClipboardDataType::Files:
Files->Clear();
ButtonAddFiles->IsEnabled = true;
break;
case ClipboardDataType::Image:
BMP_Stream = nullptr;
ButtonAddImage->IsEnabled = true;
break;
}
}
);
head->Children->Append(button);
return head;
}

UIElement^ ClipboardManager::MultiloadPage::CreateWindowBody(const ClipboardDataType type, Object^ data)
{
switch (type)
{
case ClipboardDataType::Text:
{
TextBlock^ body = ref new TextBlock();
body->Margin = Thickness(0, 28, 0, 0);
body->IsTextSelectionEnabled = false;
body->FontSize = 12;
body->Padding = Thickness(12);
body->TextWrapping = TextWrapping::Wrap;
body->TextTrimming = TextTrimming::CharacterEllipsis;
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
case ClipboardDataType::Files:
{
TextBlock^ body = ref new TextBlock();
body->Margin = Thickness(0, 28, 0, 0);
body->IsTextSelectionEnabled = false;
body->FontSize = 12;
body->Padding = Thickness(12);
body->TextWrapping = TextWrapping::NoWrap;
body->TextTrimming = TextTrimming::CharacterEllipsis;
Vector<IStorageItem^>^ files = dynamic_cast<Vector<IStorageItem^>^>(data);
if (files == nullptr)
{
body->Text = "";
return body;
}
Run^ r;
for (unsigned int i = 0; i < files->Size; i++)
{
r = ref new Run();
r->Text = files->GetAt(i)->Path;
body->Inlines->Append(r);
body->Inlines->Append(ref new LineBreak());
}
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
img->Source = bitmapImage;
return img;
}
default: return nullptr;
}
}

Grid^ ClipboardManager::MultiloadPage::CreatePreviewWindow(const ClipboardDataType type, Object^ data)
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
case ClipboardDataType::Text:
head = CreateWindowHeader(type, resourceLoader->GetString("MultiloadPageWindowHeaderText"));
break;
case ClipboardDataType::Files:
head = CreateWindowHeader(type, resourceLoader->GetString("MultiloadPageWindowHeaderFiles"));
break;
case ClipboardDataType::Image:
head = CreateWindowHeader(type, resourceLoader->GetString("MultiloadPageWindowHeaderImage"));
break;
}
grid->Children->Append(head);
grid->Children->Append(CreateWindowBody(type, data));
return grid;
}

void ClipboardManager::MultiloadPage::MenuFlyoutItemAddText_Click(Object^ sender, RoutedEventArgs^ e)
{
AddContent(ClipboardDataType::Text);
}

void ClipboardManager::MultiloadPage::MenuFlyoutItemAddFiles_Click(Object^ sender, RoutedEventArgs^ e)
{
AddContent(ClipboardDataType::Files);
}

void ClipboardManager::MultiloadPage::MenuFlyoutItemAddImage_Click(Object^ sender, RoutedEventArgs^ e)
{
AddContent(ClipboardDataType::Image);
}

void ClipboardManager::MultiloadPage::ButtonClear_Click(Object^ sender, RoutedEventArgs^ e)
{
if (!IsConfirmClearEnabled)
	SetEmptyPageStub();
}

void ClipboardManager::MultiloadPage::ButtonLoad_Click(Object^ sender, RoutedEventArgs^ e)
{
DataPackage^ dataPackage = ref new DataPackage();
try
{
if (Text != nullptr)
	dataPackage->SetText(Text);
if (Files->Size > 0)
{
dataPackage->RequestedOperation = DataPackageOperation::Copy;
dataPackage->SetStorageItems(Files, true);
}
if (BMP_Stream != nullptr)
	dataPackage->SetBitmap(RandomAccessStreamReference::CreateFromStream(BMP_Stream));
}
catch (Exception^)
{
ShowError("Failed to load the items into the clipboard", "", localSettings->Values);
GoBack();
return;
}
if (!LoadData(dataPackage))
{
ShowError("Failed to load the items into the clipboard", "", localSettings->Values);
GoBack();
return;
}
if (static_cast<bool>(localSettings->Values->Lookup("ShowSuccessAddContentNotify")))
	UserNotify(resourceLoader->GetString("MultiloadPageSuccessAddContentText"), "", NotificationGroup::CopyToClipboard);
GoBack();
}

void ClipboardManager::MultiloadPage::ButtonBack_Click(Object^ sender, RoutedEventArgs^ e)
{
GoBack();
}

void ClipboardManager::MultiloadPage::AddContent(const ClipboardDataType type)
{

if (type == ClipboardDataType::Files)
{

	Waiter->IsActive = true;

	create_task(CreateFileOpenPicker(type)->PickMultipleFilesAsync()).then(MultiloadPageLoadFilesClass(this));

	return;

}

Waiter->IsActive = true;

create_task(CreateFileOpenPicker(type)->PickSingleFileAsync()).then(MultiloadPageLoadDataClass(this, type));

}