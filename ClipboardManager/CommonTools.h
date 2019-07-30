#pragma once

#include "Clipboard.h"
#include "Notification.h"
#include "Flyout.h"
#include "Functors.h"

namespace ClipboardManager
{

namespace CommonTools
{

void SetComboBoxItem(Windows::UI::Xaml::Controls::ComboBox^ comboBox, const ClipboardManager::CommonTools::ClipboardDataType type, Platform::String^ text);
void SetLink(Windows::UI::Xaml::Controls::ScrollViewer^ scroller, Windows::Foundation::Uri^ link, bool navigate=true);
void SetText(Windows::UI::Xaml::Controls::ScrollViewer^ scroller, Platform::String^ text, bool isWrapped = true);
void SetFiles(Windows::UI::Xaml::Controls::ScrollViewer^ scroller, Windows::Foundation::Collections::IVectorView<Windows::Storage::IStorageItem^>^ files);
void SetImage(Windows::UI::Xaml::Controls::ScrollViewer^ scroller, Windows::UI::Xaml::Media::ImageSource^ img);
void GoBack();
Platform::String^ GenHashCode();
bool ReloadCurrentPage(Windows::UI::Xaml::Controls::Frame^ frame, Platform::Object^ parameter=nullptr);
Windows::UI::Xaml::Controls::TextBlock^ CreateStubCaption(Platform::String^ caption);
Windows::Storage::Pickers::FileOpenPicker^ CreateFileOpenPicker(ClipboardManager::CommonTools::ClipboardDataType type);

}

}