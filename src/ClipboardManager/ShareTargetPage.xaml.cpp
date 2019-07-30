// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include "ShareTargetPage.xaml.h"

using namespace ClipboardManager;
using namespace ClipboardManager::CommonTools;

using namespace Platform;
using namespace Platform::Collections;
using namespace concurrency;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace Windows::ApplicationModel::DataTransfer::ShareTarget;
using namespace Windows::ApplicationModel::Resources;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Documents;
using namespace Windows::Storage::Streams;
using namespace Windows::Storage;
using namespace Windows::UI::Notifications;
using namespace Windows::Data::Xml::Dom;

ClipboardManager::ShareTargetPage::ShareTargetPage()
{
InitializeComponent();
localSettings = ApplicationData::Current->LocalSettings;
resourceLoader = ResourceLoader::GetForViewIndependentUse();
if (static_cast<bool>(localSettings->Values->Lookup("DarkTheme")))
	RequestedTheme = ElementTheme::Dark;
Scroller->FontSize = static_cast<int>(localSettings->Values->Lookup("FontSize"));
dataType = ClipboardDataType::Nothing;
data = nullptr;
OperationCompleted = false;
}

void ClipboardManager::ShareTargetPage::OnNavigatedTo(NavigationEventArgs^ e)
{
shareOperation = dynamic_cast<ShareOperation^>(e->Parameter);
}

void ClipboardManager::ShareTargetPage::Page_Loaded(Object^ sender, RoutedEventArgs^ e)
{
if (shareOperation == nullptr)
{
ShowError("Failed to get share data", "", localSettings->Values);
OperationCompleted = true;
GoBack();
return;
}
ToggleButtonTextWrapping->IsChecked = static_cast<bool>(localSettings->Values->Lookup("TextWrappingShareTargetPage"));
ToggleButtonTextWrapping->Checked += ref new RoutedEventHandler(this, &ClipboardManager::ShareTargetPage::ToggleButtonTextWrapping_Checked);
ToggleButtonTextWrapping->Unchecked += ref new RoutedEventHandler(this, &ClipboardManager::ShareTargetPage::ToggleButtonTextWrapping_Unchecked);
DataPackageView^ dp_view = shareOperation->Data;
if (dp_view->Contains(StandardDataFormats::WebLink))
{
SetComboBoxItem(TypeSwitcher,ClipboardDataType::Link, resourceLoader->GetString("ShareTargetPageComboBoxItemLink"));
}
if (dp_view->Contains(StandardDataFormats::Text))
{
SetComboBoxItem(TypeSwitcher, ClipboardDataType::Text, resourceLoader->GetString("ShareTargetPageComboBoxItemText"));
}
if (dp_view->Contains(StandardDataFormats::StorageItems))
{
SetComboBoxItem(TypeSwitcher, ClipboardDataType::Files, resourceLoader->GetString("ShareTargetPageComboBoxItemFiles"));
}
if (dp_view->Contains(StandardDataFormats::Bitmap))
{
SetComboBoxItem(TypeSwitcher, ClipboardDataType::Image, resourceLoader->GetString("ShareTargetPageComboBoxItemImage"));
}
}

void ClipboardManager::ShareTargetPage::Page_Unloaded(Object^ sender, RoutedEventArgs^ e)
{
if (!OperationCompleted)
	shareOperation->ReportCompleted();
}

void ClipboardManager::ShareTargetPage::ButtonCopy_Click(Object^ sender, RoutedEventArgs^ e)
{
switch (dataType)
{
case ClipboardDataType::Link:
{
Uri^ link = dynamic_cast<Uri^>(data);
if (link == nullptr)
	break;
if (LoadLink(link))
	if (static_cast<bool>(localSettings->Values->Lookup("ShowSuccessAddContentNotify")))
		UserNotify(resourceLoader->GetString("NotifyCopyLinkToClipboard"), link->DisplayUri, NotificationGroup::CopyToClipboard);
}
break;
case ClipboardDataType::Text:
{
String^ text = dynamic_cast<String^>(data);
if (text == nullptr)
	break;
if (LoadText(text))
	if (static_cast<bool>(localSettings->Values->Lookup("ShowSuccessAddContentNotify")))
		UserNotify(resourceLoader->GetString("NotifyCopyTextToClipboard"), "", NotificationGroup::CopyToClipboard);
}
break;
case ClipboardDataType::Files:
{
IVectorView<IStorageItem^>^ items = dynamic_cast<IVectorView<IStorageItem^>^>(data);
if (items == nullptr)
	break;
Vector<IStorageItem^>^ vItems = ref new Vector<IStorageItem^>();
for (unsigned int i = 0; i < items->Size; i++)
	vItems->Append(items->GetAt(i));
if (LoadFiles(vItems->GetView()))
	if (static_cast<bool>(localSettings->Values->Lookup("ShowSuccessAddContentNotify")))
		UserNotify(resourceLoader->GetString("NotifyCopyFilesToClipboard"), "", NotificationGroup::CopyToClipboard);
}
break;
case ClipboardDataType::Image:
{
IRandomAccessStream^ BitmapStream = dynamic_cast<IRandomAccessStream^>(data);
if (BitmapStream == nullptr)
	break;
if (LoadImage(BitmapStream))
	if (static_cast<bool>(localSettings->Values->Lookup("ShowSuccessAddContentNotify")))
		UserNotify(resourceLoader->GetString("NotifyCopyImageToClipboard"), "", NotificationGroup::CopyToClipboard);
}
break;
default: 
break;
}
OperationCompleted = true;
shareOperation->ReportCompleted();
}

void ClipboardManager::ShareTargetPage::ButtonCancel_Click(Object^ sender, RoutedEventArgs^ e)
{
OperationCompleted = true;
shareOperation->ReportCompleted();
}

void ClipboardManager::ShareTargetPage::TypeSwitcher_SelectionChanged(Object^ sender, SelectionChangedEventArgs^ e)
{
if (TypeSwitcher->SelectedItem == nullptr)
	return;
Waiter->IsActive = true;
Scroller->Content = nullptr;
ComboBoxItem^ item = dynamic_cast<ComboBoxItem^>(TypeSwitcher->SelectedItem);
dataType = static_cast<ClipboardDataType>(item->Tag);
task<bool> t;
switch (dataType)
{
case ClipboardDataType::Link:
{
t = create_task(shareOperation->Data->GetWebLinkAsync()).then
(
[this](const task<Uri^>& task_link)->bool
{
Uri^ link;
try
{
link = task_link.get();
}
catch (Exception^)
{
return false;
}
data = link;
SetLink(Scroller, link, false);
Waiter->IsActive = false;
return true;
}
);
}
break;
case ClipboardDataType::Text:
{
t = create_task(shareOperation->Data->GetTextAsync(StandardDataFormats::Text)).then
(
[this](const task<String^>& task_text)->bool
{
String^ text;
try
{
text = task_text.get();
}
catch (Exception^)
{
return false;
}
data = text;
SetText(Scroller, text, ToggleButtonTextWrapping->IsChecked->Value);
Waiter->IsActive = false;
return true;
}
);
}
break;
case ClipboardDataType::Files:
{
t = create_task(shareOperation->Data->GetStorageItemsAsync()).then
(
[this](const task<IVectorView<IStorageItem^>^>& task_items)->bool
{
IVectorView<IStorageItem^>^ items;
try
{
items = task_items.get();
}
catch (Exception^ ex)
{
return false;
}
data = items;
SetFiles(Scroller, items);
Waiter->IsActive = false;
return true;
}
);
}
break;
case ClipboardDataType::Image:
{
auto t2 = create_task(shareOperation->Data->GetBitmapAsync()).then
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
catch (Exception^)
{
return task<void>();
}
data = image_stream->CloneStream();
return create_task(bitmap_image->SetSourceAsync(image_stream));
}
);
t = t3.then
(
[this, bitmap_image](const task<void>& t2)->bool
{
try
{
t2.get();
}
catch (Exception^)
{
return false;
}
SetImage(Scroller, bitmap_image);
Waiter->IsActive = false;
return true;
}
);
}
break;
default:
t = task_from_result(true);
break;
}
t.then
(
[this](bool successful)
{
if (successful)
	return;
switch (dataType)
{
case ClipboardDataType::Link:
ShowError(resourceLoader->GetString("ShareTargetPageShowLinkError"), "", localSettings->Values);
break;
case ClipboardDataType::Text:
ShowError(resourceLoader->GetString("ShareTargetPageShowTextError"), "", localSettings->Values);
break;
case ClipboardDataType::Files:
ShowError(resourceLoader->GetString("ShareTargetPageShowFilesError"), "", localSettings->Values);
break;
case ClipboardDataType::Image:
ShowError(resourceLoader->GetString("ShareTargetPageShowImageError"), "", localSettings->Values);
break;
default:
break;
}
ButtonCopy->IsEnabled = false;
Waiter->IsActive = false;
}
);
}

void ClipboardManager::ShareTargetPage::ToggleButtonTextWrapping_Checked(Object^ sender, RoutedEventArgs^ e)
{
localSettings->Values->Insert("TextWrappingShareTargetPage", true);
if (dataType != ClipboardDataType::Text)
	return;
TextBlock^ tb = dynamic_cast<TextBlock^>(Scroller->Content);
if (tb == nullptr)
	return;
Scroller->HorizontalScrollBarVisibility = ScrollBarVisibility::Disabled;
tb->TextWrapping = TextWrapping::Wrap;
}

void ClipboardManager::ShareTargetPage::ToggleButtonTextWrapping_Unchecked(Object^ sender, RoutedEventArgs^ e)
{
localSettings->Values->Insert("TextWrappingShareTargetPage", false);
if (dataType != ClipboardDataType::Text)
	return;
TextBlock^ tb = dynamic_cast<TextBlock^>(Scroller->Content);
if (tb == nullptr)
	return;
Scroller->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;
tb->TextWrapping = TextWrapping::NoWrap;
}