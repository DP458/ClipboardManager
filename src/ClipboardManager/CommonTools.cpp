// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include "CommonTools.h"

using namespace ClipboardManager::CommonTools;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Documents;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Security::Cryptography;
using namespace Windows::Security::Cryptography::Core;

void ClipboardManager::CommonTools::SetComboBoxItem(ComboBox^ comboBox, const ClipboardDataType type, String^ text)
{
ComboBoxItem^ item = ref new ComboBoxItem();
item->UseSystemFocusVisuals = true;
item->RequestedTheme = comboBox->RequestedTheme;
TextBlock^ tb = ref new TextBlock();
tb->IsTextSelectionEnabled = false;
tb->Text = text;
item->Content = tb;
item->Tag = type;
comboBox->Items->Append(item);
if (comboBox->SelectedItem == nullptr)
	comboBox->SelectedIndex = 0;
}

void ClipboardManager::CommonTools::SetLink(ScrollViewer^ scroller, Uri^ link, bool navigate)
{
TextBlock^ tb = ref new TextBlock();
tb->IsTextSelectionEnabled = false;
scroller->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;
tb->TextWrapping = TextWrapping::NoWrap;
Hyperlink^ hyperlink = ref new Hyperlink();
if (navigate)
	hyperlink->NavigateUri = link;
Run^ r = ref new Run();
r->Text = link->DisplayUri;
hyperlink->Inlines->Append(r);
tb->Inlines->Append(hyperlink);
scroller->Content = tb;
}

void ClipboardManager::CommonTools::SetText(ScrollViewer^ scroller, String^ text, bool isWrapped)
{
TextBlock^ tb = ref new TextBlock();
tb->IsTextSelectionEnabled = false;
if (isWrapped)
{
scroller->HorizontalScrollBarVisibility = ScrollBarVisibility::Disabled;
tb->TextWrapping = TextWrapping::Wrap;
}
else
{
scroller->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;
tb->TextWrapping = TextWrapping::NoWrap;
}
tb->Text = text;
scroller->Content = tb;
}

void ClipboardManager::CommonTools::SetFiles(ScrollViewer^ scroller, IVectorView<IStorageItem^>^ files)
{
TextBlock^ tb = ref new TextBlock();
tb->IsTextSelectionEnabled = false;
scroller->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;
tb->TextWrapping = TextWrapping::NoWrap;
Run^ r;
for (unsigned int i = 0; i < files->Size; i++)
{
r = ref new Run();
r->Text = files->GetAt(i)->Path;
tb->Inlines->Append(r);
tb->Inlines->Append(ref new LineBreak());
scroller->Content = tb;
}
}

void ClipboardManager::CommonTools::SetImage(ScrollViewer^ scroller, ImageSource^ img)
{
Image^ image = ref new Image();
image->Stretch = Stretch::None;
image->Source = img;
scroller->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;
scroller->Content = image;
}

Platform::String^ ClipboardManager::CommonTools::GenHashCode()
{
HashAlgorithmProvider^ provider = HashAlgorithmProvider::OpenAlgorithm(HashAlgorithmNames::Md5);
CryptographicHash^ hash = provider->CreateHash();
hash->Append(CryptographicBuffer::GenerateRandom(provider->HashLength));
hash->Append(CryptographicBuffer::GenerateRandom(provider->HashLength));
return CryptographicBuffer::EncodeToHexString(hash->GetValueAndReset());
}

bool ClipboardManager::CommonTools::ReloadCurrentPage(Frame^ frame, Object^ parameter)
{
bool result = frame->Navigate(frame->CurrentSourcePageType, parameter);
if (result)
	frame->BackStack->RemoveAt(frame->BackStack->Size - 1);
return result;
}

TextBlock^ ClipboardManager::CommonTools::CreateStubCaption(String^ caption)
{
TextBlock^ tb = ref new TextBlock();
tb->IsTextSelectionEnabled = false;
tb->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Top;
tb->Margin = Thickness(10, 0, 10, 0);
tb->FontSize = 20;
tb->TextWrapping = TextWrapping::Wrap;
tb->Text = caption;
return tb;
}

FileOpenPicker^ ClipboardManager::CommonTools::CreateFileOpenPicker(ClipboardDataType type)
{
FileOpenPicker^ openPicker = ref new FileOpenPicker();
switch (type)
{
case ClipboardDataType::Text:
openPicker->FileTypeFilter->Append(".txt");
openPicker->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;
openPicker->ViewMode = PickerViewMode::List;
break;
case ClipboardDataType::Image:
openPicker->FileTypeFilter->Append(".jpg");
openPicker->FileTypeFilter->Append(".bmp");
openPicker->FileTypeFilter->Append(".png");
openPicker->FileTypeFilter->Append(".gif");
openPicker->FileTypeFilter->Append(".tif");
openPicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;
openPicker->ViewMode = PickerViewMode::Thumbnail;
break;
default:
openPicker->FileTypeFilter->Append("*");
openPicker->SuggestedStartLocation = PickerLocationId::DocumentsLibrary;
openPicker->ViewMode = PickerViewMode::List;
break;
}
return openPicker;
}