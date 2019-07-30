// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include "TextEditPage.xaml.h"

using namespace ClipboardManager;
using namespace ClipboardManager::CommonTools;

using namespace Platform;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;
using namespace Windows::UI::Text;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Storage;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace Windows::ApplicationModel::Resources;

ClipboardManager::TextEditPage::TextEditPage()
{
InitializeComponent();
localSettings = ApplicationData::Current->LocalSettings;
if (static_cast<bool>(localSettings->Values->Lookup("DarkTheme")))
RequestedTheme = ElementTheme::Dark;
resourceLoader = ResourceLoader::GetForViewIndependentUse();
RichTextEditor->FontSize = static_cast<int>(localSettings->Values->Lookup("FontSize"));
}

void ClipboardManager::TextEditPage::OnNavigatedTo(NavigationEventArgs^ e)
{
SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Visible;
}

void ClipboardManager::TextEditPage::Page_Loaded(Object^ sender, RoutedEventArgs^ e)
{
Waiter->IsActive = true;
ToggleButtonTextWrapping->IsChecked = static_cast<bool>(localSettings->Values->Lookup("TextWrappingTextEditPage"));
if (ToggleButtonTextWrapping->IsChecked->Value)
{
Scroller->HorizontalScrollBarVisibility = ScrollBarVisibility::Disabled;
RichTextEditor->TextWrapping = TextWrapping::Wrap;
}
else
{
Scroller->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;
RichTextEditor->TextWrapping = TextWrapping::NoWrap;
}
ToggleButtonTextWrapping->Checked += ref new RoutedEventHandler(this, &ClipboardManager::TextEditPage::ToggleButtonTextWrapping_StateChanged);
ToggleButtonTextWrapping->Unchecked += ref new RoutedEventHandler(this, &ClipboardManager::TextEditPage::ToggleButtonTextWrapping_StateChanged);
//
RichTextEditor->Focus(Windows::UI::Xaml::FocusState::Programmatic);
RichTextEditor->Document->UndoLimit = 0;
DataPackageView^ dp_view = GetClipboardContent();
if (dp_view == nullptr)
{
ShowError("Failed to get string from the clipboard", "", localSettings->Values);
GoBack();
return;
}
if (!dp_view->Contains(StandardDataFormats::Text))
{
ShowError("Failed to get string from the clipboard", "", localSettings->Values);
GoBack();
return;
}
create_task(dp_view->GetTextAsync(StandardDataFormats::Text)).then
(
[this](const task<String^>& t)
{
try
{
this->text = t.get();
}
catch (Exception^)
{
Waiter->IsActive = false;
ShowError("Failed to get string from the clipboard", "", localSettings->Values);
GoBack();
return;
}
RichTextEditor->Document->SetText(TextSetOptions::None, this->text);
RichTextEditor->Document->UndoLimit = 10;
ButtonUndo->IsEnabled = RichTextEditor->Document->CanUndo();
ButtonRedo->IsEnabled = RichTextEditor->Document->CanRedo();
RichTextEditor->TextChanged += ref new RoutedEventHandler(this, &ClipboardManager::TextEditPage::RichTextEditor_TextChanged);
Waiter->IsActive = false;
}
);
}

void ClipboardManager::TextEditPage::ButtonAccept_Click(Object^ sender, RoutedEventArgs^ e)
{
if (LoadText(text))
{
if (static_cast<bool>(localSettings->Values->Lookup("ShowSuccessAddContentNotify")))
	UserNotify(resourceLoader->GetString("NotifyCopyTextToClipboard"), text, NotificationGroup::CopyToClipboard);
}
else
	ShowError("Failed to load text into the clipboard", "", localSettings->Values);
GoBack();
}

void ClipboardManager::TextEditPage::ButtonCancel_Click(Object^ sender, RoutedEventArgs^ e)
{
GoBack();
}

void ClipboardManager::TextEditPage::ButtonSelectAll_Click(Object^ sender, RoutedEventArgs^ e)
{
RichTextEditor->Document->Selection->SetRange(0, text->Length());
}

void ClipboardManager::TextEditPage::RichTextEditor_TextChanged(Object^ sender, RoutedEventArgs^ e)
{
ButtonUndo->IsEnabled = RichTextEditor->Document->CanUndo();
ButtonRedo->IsEnabled = RichTextEditor->Document->CanRedo();
RichTextEditor->Document->GetText(TextGetOptions::UseCrlf, &text);
bool IsEmpty = text->IsEmpty();
ButtonAccept->IsEnabled = !IsEmpty;
ButtonSelectAll->IsEnabled = !IsEmpty;
}

void ClipboardManager::TextEditPage::RichTextEditor_SelectionChanged(Object^ sender, RoutedEventArgs^ e)
{
String^ selected_text = RichTextEditor->Document->Selection->Text;
ButtonDelete->IsEnabled = !selected_text->IsEmpty();
ButtonSelectAll->IsEnabled = !(selected_text->Length() >= text->Length());
}

void ClipboardManager::TextEditPage::ButtonUndo_Click(Object^ sender, RoutedEventArgs^ e)
{
RichTextEditor->Document->Undo();
}

void ClipboardManager::TextEditPage::ButtonRedo_Click(Object^ sender, RoutedEventArgs^ e)
{
RichTextEditor->Document->Redo();
}

void ClipboardManager::TextEditPage::ButtonDelete_Click(Object^ sender, RoutedEventArgs^ e)
{
RichTextEditor->Document->Selection->SetText(TextSetOptions::None, "");
}

void ClipboardManager::TextEditPage::ToggleButtonTextWrapping_StateChanged(Object^ sender, RoutedEventArgs^ e)
{
localSettings->Values->Insert("TextWrappingTextEditPage", ToggleButtonTextWrapping->IsChecked->Value);
if(ToggleButtonTextWrapping->IsChecked->Value)
{ 
Scroller->HorizontalScrollBarVisibility = ScrollBarVisibility::Disabled;
RichTextEditor->TextWrapping = TextWrapping::Wrap;
}
else
{
Scroller->HorizontalScrollBarVisibility = ScrollBarVisibility::Auto;
RichTextEditor->TextWrapping = TextWrapping::NoWrap;
}
}