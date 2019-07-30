#pragma once

namespace ClipboardManager
{

namespace CommonTools
{

delegate void FlyoutButtonClick();

Windows::UI::Xaml::Controls::Flyout^ CreateFlyout(Windows::UI::Xaml::ElementTheme theme, Platform::String^ icon, Platform::String^ title = nullptr);
Windows::UI::Xaml::Controls::Flyout^ CreateMessageFlyout(Windows::UI::Xaml::ElementTheme theme, Platform::String^ icon, Platform::String^ Message, Platform::String^ title = nullptr);
Windows::UI::Xaml::Controls::Flyout^ CreateConfirmFlyout(Windows::UI::Xaml::ElementTheme theme, Platform::String^ Message, Platform::String^ ButtonCaption, ClipboardManager::CommonTools::FlyoutButtonClick^ ButtonClick = nullptr);
Windows::UI::Xaml::Controls::Button^ CreateMenuFlyoutButton(Windows::UI::Xaml::Application^ app, Platform::String^ icon, Platform::String^ text, Windows::UI::Xaml::RoutedEventHandler^ clickHandler);
Windows::UI::Xaml::Controls::CheckBox^ CreateMenuFlyoutCheckBox(Windows::UI::Xaml::Application^ app, Platform::String^ text, Windows::UI::Xaml::RoutedEventHandler^ checkedHandler, Windows::UI::Xaml::RoutedEventHandler^ uncheckedHandler, bool isChecked);

}

}