// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include "SettingsPage.xaml.h"

using namespace ClipboardManager;
using namespace ClipboardManager::CommonTools;

using namespace Platform;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Notifications;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Documents;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Storage;
using namespace Windows::Storage::Search;
using namespace Windows::Globalization;
using namespace Windows::ApplicationModel::Resources;

ClipboardManager::SettingsPage::SettingsPage()
{
InitializeComponent();
app = Application::Current;
appData = ApplicationData::Current;
localSettings = appData->LocalSettings;
if (static_cast<bool>(localSettings->Values->Lookup("DarkTheme")))
{
RequestedTheme = ElementTheme::Dark;
ThemeSwitcher->IsOn = true;
}
ThemeSwitcher->Toggled += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::ThemeSwitcher_Toggled);
//
SetFontSizeSwitcher(static_cast<int>(localSettings->Values->Lookup("FontSize")));
FontSizeSwitcher->SelectionChanged += ref new SelectionChangedEventHandler(this, &ClipboardManager::SettingsPage::OnSelectionChanged);
//
IVectorView<String^>^ langTags = ApplicationLanguages::ManifestLanguages;
String^ langTag;
Windows::Globalization::Language^ lang;
ComboBoxItem^ item;
for (unsigned int i = 0; i < langTags->Size; i++)
{
langTag = langTags->GetAt(i);
lang = ref new Windows::Globalization::Language(langTag);
item = ref new ComboBoxItem();
item->Content = lang->DisplayName;
item->Tag = langTag;
LanguageSwitcher->Items->Append(item);
}
if (ApplicationLanguages::PrimaryLanguageOverride->Length() == 0)
	LanguageSwitcher->SelectedIndex = 0;
else
for (unsigned int i = 0; i < langTags->Size; i++)
{
if (langTags->GetAt(i) == ApplicationLanguages::PrimaryLanguageOverride)
	LanguageSwitcher->SelectedIndex = i + 1;
}
LanguageSwitcher->SelectionChanged += ref new SelectionChangedEventHandler(this, &ClipboardManager::SettingsPage::LanguageSwitcherSelectionChanged);
//
if (static_cast<bool>(localSettings->Values->Lookup("ConfirmClear")))
	CheckBoxClearClipboardConfirm->IsChecked = true;
CheckBoxClearClipboardConfirm->Checked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxClearClipboardConfirm_StateChanged);
CheckBoxClearClipboardConfirm->Unchecked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxClearClipboardConfirm_StateChanged);
//
if (static_cast<bool>(localSettings->Values->Lookup("ConfirmClearFavorites")))
	CheckBoxClearFavoritesConfirm->IsChecked = true;
CheckBoxClearFavoritesConfirm->Checked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxClearFavoritesConfirm_StateChanged);
CheckBoxClearFavoritesConfirm->Unchecked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxClearFavoritesConfirm_StateChanged);
//
if (static_cast<bool>(localSettings->Values->Lookup("ShowSuccessNotify")))
	CheckBoxFileSavingNotify->IsChecked = true;
CheckBoxFileSavingNotify->Checked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxFileSavingNotify_StateChanged);
CheckBoxFileSavingNotify->Unchecked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxFileSavingNotify_StateChanged);
//
if (static_cast<bool>(localSettings->Values->Lookup("ShowSuccessAddContentNotify")))
	CheckBoxSuccessAddContentNotify->IsChecked = true;
CheckBoxSuccessAddContentNotify->Checked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxSuccessAddContentNotify_StateChanged);
CheckBoxSuccessAddContentNotify->Unchecked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxSuccessAddContentNotify_StateChanged);
//
if (static_cast<bool>(localSettings->Values->Lookup("ExitFavoritesPage")))
	ExitSwitcher->IsOn = true;
ExitSwitcher->Toggled += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::ExitSwitcher_Toggled);
//
if (static_cast<bool>(localSettings->Values->Lookup("ShowErrors")))
	CheckBoxErrorMessages->IsChecked = true;
CheckBoxErrorMessages->Checked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxErrorMessages_StateChanged);
CheckBoxErrorMessages->Unchecked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxErrorMessages_StateChanged);
//
if (static_cast<bool>(localSettings->Values->Lookup("ShowSuccessSaveFavoritesNotify")))
	CheckBoxShowSuccessSaveFavoritesNotify->IsChecked = true;
CheckBoxShowSuccessSaveFavoritesNotify->Checked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxShowSuccessSaveFavoritesNotify_StateChanged);
CheckBoxShowSuccessSaveFavoritesNotify->Unchecked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxShowSuccessSaveFavoritesNotify_StateChanged);
//
if (static_cast<bool>(localSettings->Values->Lookup("TextWrapping")))
	CheckBoxTextWrapping->IsChecked = true;
CheckBoxTextWrapping->Checked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxTextWrapping_StateChanged);
CheckBoxTextWrapping->Unchecked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxTextWrapping_StateChanged);
//
if (static_cast<bool>(localSettings->Values->Lookup("TextWrappingTextEditPage")))
	CheckBoxTextWrappingTextEdit->IsChecked = true;
CheckBoxTextWrappingTextEdit->Checked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxTextWrappingTextEditPage_StateChanged);
CheckBoxTextWrappingTextEdit->Unchecked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxTextWrappingTextEditPage_StateChanged);
//
if (static_cast<bool>(localSettings->Values->Lookup("TextWrappingShareTargetPage")))
	CheckBoxTextWrappingShareTargetPage->IsChecked = true;
CheckBoxTextWrappingShareTargetPage->Checked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxTextWrappingShareTargetPage_StateChanged);
CheckBoxTextWrappingShareTargetPage->Unchecked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxTextWrappingShareTargetPage_StateChanged);
//
if (static_cast<bool>(localSettings->Values->Lookup("ShowSuccessAddToFavoritesNotify")))
	CheckBoxShowSuccessAddToFavoritesNotify->IsChecked = true;
CheckBoxShowSuccessAddToFavoritesNotify->Checked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxShowSuccessAddToFavoritesNotify_StateChanged);
CheckBoxShowSuccessAddToFavoritesNotify->Unchecked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxShowSuccessAddToFavoritesNotify_StateChanged);
//
if (static_cast<bool>(localSettings->Values->Lookup("ShowSuccessClearFavoritesNotify")))
	CheckBoxShowSuccessClearFavoritesNotify->IsChecked = true;
CheckBoxShowSuccessClearFavoritesNotify->Checked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxShowSuccessClearFavoritesNotify_StateChanged);
CheckBoxShowSuccessClearFavoritesNotify->Unchecked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxShowSuccessClearFavoritesNotify_StateChanged);
//
if (static_cast<bool>(localSettings->Values->Lookup("MultiloadPageConfirmClear")))
	CheckBoxMultiloadPageConfirmClear->IsChecked = true;
CheckBoxMultiloadPageConfirmClear->Checked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxMultiloadPageConfirmClear_StateChanged);
CheckBoxMultiloadPageConfirmClear->Unchecked += ref new RoutedEventHandler(this, &ClipboardManager::SettingsPage::CheckBoxMultiloadPageConfirmClear_StateChanged);
//
resourceLoader = ResourceLoader::GetForViewIndependentUse();
}

void ClipboardManager::SettingsPage::OnNavigatedTo(NavigationEventArgs^ e)
{
SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = AppViewBackButtonVisibility::Visible;
}

void ClipboardManager::SettingsPage::Page_Loaded(Object^ sender, RoutedEventArgs^ e)
{
if (ToastNotificationManager::CreateToastNotifier()->Setting != NotificationSetting::Enabled)
{
Run^ r = ref new Run();
r->Foreground = ref new SolidColorBrush(Colors::Red);
r->Text = resourceLoader->GetString("SettingsPageNotifyDisabledWarning");
TextBlockWindowsNotify->Inlines->InsertAt(0, r);
TextBlockWindowsNotify->Inlines->InsertAt(1, ref new LineBreak());
}
ButtonReset->Flyout = CreateConfirmFlyout
(
RequestedTheme,
resourceLoader->GetString("SettingsPageFlyoutResetMessage"),
resourceLoader->GetString("SettingsPageFlyoutResetButton"),
ref new FlyoutButtonClick(this, &ClipboardManager::SettingsPage::FlyoutResetButton_Click)
);
}

void ClipboardManager::SettingsPage::SetFontSizeSwitcher(int size)
{
switch (size)
{
case 8:
FontSizeSwitcher->SelectedItem = FontSizeEight;
break;
case 9:
FontSizeSwitcher->SelectedItem = FontSizeNine;
break;
case 10:
FontSizeSwitcher->SelectedItem = FontSizeTen;
break;
case 11:
FontSizeSwitcher->SelectedItem = FontSizeEleven;
break;
case 12:
FontSizeSwitcher->SelectedItem = FontSizeTwelve;
break;
case 13:
FontSizeSwitcher->SelectedItem = FontSizeThirteen;
break;
case 14:
FontSizeSwitcher->SelectedItem = FontSizeFourteen;
break;
case 15:
FontSizeSwitcher->SelectedItem = FontSizeFifteen;
break;
case 16:
FontSizeSwitcher->SelectedItem = FontSizeSixteen;
break;
case 17:
FontSizeSwitcher->SelectedItem = FontSizeSeventeen;
break;
case 18:
FontSizeSwitcher->SelectedItem = FontSizeEighteen;
break;
case 19:
FontSizeSwitcher->SelectedItem = FontSizeNineteen;
break;
case 20:
FontSizeSwitcher->SelectedItem = FontSizeTwenty;
break;
}
}

template<class T> bool ClipboardManager::SettingsPage::SetSetting(String^ name, T value)
{
return localSettings->Values->Insert(name, value);
}

void ClipboardManager::SettingsPage::ButtonBack_Click(Platform::Object^ sender, RoutedEventArgs^ e)
{
GoBack();
}

void ClipboardManager::SettingsPage::ThemeSwitcher_Toggled(Object^ sender, RoutedEventArgs^ e)
{
SetSetting("DarkTheme", ThemeSwitcher->IsOn);
ReloadCurrentPage(Frame);
}

void ClipboardManager::SettingsPage::OnSelectionChanged(Object^ sender, SelectionChangedEventArgs^ e)
{
ComboBoxItem^ item = dynamic_cast<ComboBoxItem^>(FontSizeSwitcher->SelectedItem);
if (item->Name == "FontSizeEight")
{
SetSetting("FontSize", 8);
return;
}
if (item->Name == "FontSizeNine")
{
SetSetting("FontSize", 9);
return;
}
if (item->Name == "FontSizeTen")
{
SetSetting("FontSize", 10);
return;
}
if (item->Name == "FontSizeEleven")
{
SetSetting("FontSize", 11);
return;
}
if (item->Name == "FontSizeTwelve")
{
SetSetting("FontSize", 12);
return;
}
if (item->Name == "FontSizeThirteen")
{
SetSetting("FontSize", 13);
return;
}
if (item->Name == "FontSizeFourteen")
{
SetSetting("FontSize", 14);
return;
}
if (item->Name == "FontSizeFifteen")
{
SetSetting("FontSize", 15);
return;
}
if (item->Name == "FontSizeSixteen")
{
SetSetting("FontSize", 16);
return;
}
if (item->Name == "FontSizeSeventeen")
{
SetSetting("FontSize", 17);
return;
}
if (item->Name == "FontSizeEighteen")
{
SetSetting("FontSize", 18);
return;
}
if (item->Name == "FontSizeNineteen")
{
SetSetting("FontSize", 19);
return;
}
if (item->Name == "FontSizeTwenty")
{
SetSetting("FontSize", 20);
return;
}
}

void ClipboardManager::SettingsPage::LanguageSwitcherSelectionChanged(Object^ sender, SelectionChangedEventArgs^ e)
{
if (LanguageSwitcher->SelectedIndex == 0)
{
ApplicationLanguages::PrimaryLanguageOverride = nullptr;
ReloadCurrentPage(Frame);
return;
}
if (LanguageSwitcher->SelectedIndex > 0)
{
ComboBoxItem^ item = dynamic_cast<ComboBoxItem^>(LanguageSwitcher->SelectedItem);
String^ langTag = dynamic_cast<String^>(item->Tag);
ApplicationLanguages::PrimaryLanguageOverride = langTag;
ReloadCurrentPage(Frame);
}
}

void ClipboardManager::SettingsPage::CheckBoxFileSavingNotify_StateChanged(Object^ sender, RoutedEventArgs^ e)
{
SetSetting("ShowSuccessNotify", CheckBoxFileSavingNotify->IsChecked->Value);
}

void ClipboardManager::SettingsPage::CheckBoxClearClipboardConfirm_StateChanged(Object^ sender, RoutedEventArgs^ e)
{
SetSetting("ConfirmClear", CheckBoxClearClipboardConfirm->IsChecked->Value);
}

void ClipboardManager::SettingsPage::CheckBoxClearFavoritesConfirm_StateChanged(Object^ sender, RoutedEventArgs^ e)
{
SetSetting("ConfirmClearFavorites", CheckBoxClearFavoritesConfirm->IsChecked->Value);
}

void ClipboardManager::SettingsPage::CheckBoxSuccessAddContentNotify_StateChanged(Object^ sender, RoutedEventArgs^ e)
{
SetSetting("ShowSuccessAddContentNotify", CheckBoxSuccessAddContentNotify->IsChecked->Value);
}

void ClipboardManager::SettingsPage::ExitSwitcher_Toggled(Object^ sender, RoutedEventArgs^ e)
{
SetSetting("ExitFavoritesPage", ExitSwitcher->IsOn);
}

void ClipboardManager::SettingsPage::CheckBoxErrorMessages_StateChanged(Object^ sender, RoutedEventArgs^ e)
{
SetSetting("ShowErrors", CheckBoxErrorMessages->IsChecked->Value);
}

void ClipboardManager::SettingsPage::CheckBoxShowSuccessSaveFavoritesNotify_StateChanged(Object^ sender, RoutedEventArgs^ e)
{
SetSetting("ShowSuccessSaveFavoritesNotify", CheckBoxShowSuccessSaveFavoritesNotify->IsChecked->Value);
}

void ClipboardManager::SettingsPage::FlyoutResetButton_Click()
{
ThemeSwitcher->IsOn = true;
SetFontSizeSwitcher(16);
CheckBoxFileSavingNotify->IsChecked = true;
CheckBoxClearClipboardConfirm->IsChecked = true;
CheckBoxClearFavoritesConfirm->IsChecked = true;
CheckBoxSuccessAddContentNotify->IsChecked = true;
ExitSwitcher->IsOn = true;
CheckBoxErrorMessages->IsChecked = true;
CheckBoxShowSuccessSaveFavoritesNotify->IsChecked = true;
CheckBoxTextWrapping->IsChecked = true;
CheckBoxTextWrappingTextEdit->IsChecked = true;
CheckBoxTextWrappingShareTargetPage->IsChecked = true;
CheckBoxShowSuccessAddToFavoritesNotify->IsChecked = true;
CheckBoxShowSuccessClearFavoritesNotify->IsChecked = true;
CheckBoxMultiloadPageConfirmClear->IsChecked = true;
auto t = create_task(appData->LocalFolder->CreateFolderAsync("History", CreationCollisionOption::OpenIfExists)).then
(
[](const task<StorageFolder^>& task_folder)->task<IVectorView<StorageFile^>^>
{
StorageFolder^ historyFolder;
try
{
historyFolder = task_folder.get();
}
catch (Exception^)
{
return task<IVectorView<StorageFile^>^>();
}
return create_task(historyFolder->GetFilesAsync(CommonFileQuery::DefaultQuery));
}
);
auto t2 = t.then
(
[](const task<IVectorView<StorageFile^>^>& task_files)->task<void>
{
IVectorView<StorageFile^>^ files;
try
{
files = task_files.get();
}
catch (Exception^)
{
return task<void>();
}
std::vector<task<void>> taskOperation;
for (unsigned int i = 0; i < files->Size; i++)
	taskOperation.push_back(create_task(files->GetAt(i)->DeleteAsync(StorageDeleteOption::Default)));
return when_all(taskOperation.begin(), taskOperation.end());
}
);
t2.then
(
[this](const task<void>& t)
{
try
{
t.get();
}
catch (Exception^)
{
ShowError("Failed to clear all your favorites", "", localSettings->Values);
}
LanguageSwitcher->SelectedIndex = 0;
}
);
}

void ClipboardManager::SettingsPage::CheckBoxTextWrapping_StateChanged(Object^ sender, RoutedEventArgs^ e)
{
SetSetting("TextWrapping", CheckBoxTextWrapping->IsChecked->Value);
}

void ClipboardManager::SettingsPage::CheckBoxTextWrappingTextEditPage_StateChanged(Object^ sender, RoutedEventArgs^ e)
{
SetSetting("TextWrappingTextEditPage", CheckBoxTextWrappingTextEdit->IsChecked->Value);
}

void ClipboardManager::SettingsPage::CheckBoxTextWrappingShareTargetPage_StateChanged(Object^ sender, RoutedEventArgs^ e)
{
SetSetting("TextWrappingShareTargetPage", CheckBoxTextWrappingShareTargetPage->IsChecked->Value);
}

void ClipboardManager::SettingsPage::CheckBoxShowSuccessAddToFavoritesNotify_StateChanged(Object^ sender, RoutedEventArgs^ e)
{
SetSetting("ShowSuccessAddToFavoritesNotify", CheckBoxShowSuccessAddToFavoritesNotify->IsChecked->Value);
}

void ClipboardManager::SettingsPage::CheckBoxShowSuccessClearFavoritesNotify_StateChanged(Object^ sender, RoutedEventArgs^ e)
{
SetSetting("ShowSuccessClearFavoritesNotify", CheckBoxShowSuccessClearFavoritesNotify->IsChecked->Value);
}

void ClipboardManager::SettingsPage::CheckBoxMultiloadPageConfirmClear_StateChanged(Object^ sender, RoutedEventArgs^ e)
{
SetSetting("MultiloadPageConfirmClear", CheckBoxMultiloadPageConfirmClear->IsChecked->Value);
}