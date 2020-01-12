// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include "Flyout.h"

using namespace ClipboardManager::CommonTools;

using namespace Platform;
using namespace Windows::UI;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Documents;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;

Flyout^ ClipboardManager::CommonTools::CreateFlyout(ElementTheme theme, String^ icon, String^ title)
{
Application^ app = Application::Current;
Flyout^ flyout = ref new Flyout();
Style^ style = ref new Style(TypeName(FlyoutPresenter::typeid));
style->Setters->Append(ref new Setter(FlyoutPresenter::BorderBrushProperty, dynamic_cast<Brush^>(app->Resources->Lookup("SystemControlBackgroundAccentBrush"))));
style->Setters->Append(ref new Setter(FlyoutPresenter::PaddingProperty, Thickness(0, 0, 0, 0)));
style->Setters->Append(ref new Setter(FlyoutPresenter::HorizontalContentAlignmentProperty, Windows::UI::Xaml::HorizontalAlignment::Stretch));
style->Setters->Append(ref new Setter(FlyoutPresenter::VerticalContentAlignmentProperty, Windows::UI::Xaml::VerticalAlignment::Top));
style->Setters->Append(ref new Setter(FlyoutPresenter::RequestedThemeProperty, theme));
flyout->FlyoutPresenterStyle = style;
Grid^ grid = ref new Grid();
Grid^ top_bar = ref new Grid();
top_bar->Background = dynamic_cast<Brush^>(app->Resources->Lookup("SystemControlBackgroundAccentBrush"));
top_bar->Height = 30;
top_bar->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Top;
TextBlock^ icon_tb = ref new TextBlock();
icon_tb->Foreground = ref new SolidColorBrush(Colors::White);
icon_tb->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Left;
icon_tb->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
icon_tb->FontSize = 16;
icon_tb->FontFamily = ref new FontFamily("Segoe MDL2 Assets");
icon_tb->IsTextSelectionEnabled = false;
icon_tb->Margin = Thickness(6, 0, 0, 0);
icon_tb->Text = icon;
top_bar->Children->Append(icon_tb);
TextBlock^ title_tb = ref new TextBlock();
title_tb->Foreground = ref new SolidColorBrush(Colors::White);
title_tb->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Left;
title_tb->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
title_tb->FontSize = 16;
title_tb->IsTextSelectionEnabled = false;
title_tb->Margin = Thickness(30, 0, 35, 0);
title_tb->Text = title;
top_bar->Children->Append(title_tb);
Button^ button_close = ref new Button();
button_close->Background = ref new SolidColorBrush(Colors::Transparent);
button_close->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Right;
button_close->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
button_close->BorderThickness = Thickness(0, 0, 0, 0);
button_close->RequestedTheme = ElementTheme::Dark;
TextBlock^ close_tb = ref new TextBlock();
close_tb->Foreground = ref new SolidColorBrush(Colors::White);
close_tb->FontSize = 16;
close_tb->FontFamily = ref new FontFamily("Segoe MDL2 Assets");
close_tb->IsTextSelectionEnabled = false;
close_tb->Text = L"\xE10A";
button_close->Content = close_tb;
button_close->Click += ref new RoutedEventHandler
(
[flyout](Object^ sender, RoutedEventArgs^ e)
{
flyout->Hide();
}
);
top_bar->Children->Append(button_close);
grid->Children->Append(top_bar);
flyout->Content = grid;
return flyout;
}

Flyout^ ClipboardManager::CommonTools::CreateMessageFlyout(ElementTheme theme, String^ icon, String^ Message, String^ title)
{
Application^ app = Application::Current;
Flyout^ flyout = CreateFlyout(theme, icon,title);
Grid^ grid = dynamic_cast<Grid^>(flyout->Content);
grid->Style = dynamic_cast<Style^>(app->Resources->Lookup("FlyoutGridStyle"));
TextBlock^ tb = ref new TextBlock();
tb->Style = dynamic_cast<Style^>(app->Resources->Lookup("FlyoutTextBlockStyle"));
Run^ r1 = ref new Run();
r1->Text = Message;
tb->Inlines->Append(r1);
grid->Children->Append(tb);
Button^ b = ref new Button();
b->Style = dynamic_cast<Style^>(app->Resources->Lookup("FlyoutButtonStyle"));
TextBlock^ tb2 = ref new TextBlock();
tb2->Style = dynamic_cast<Style^>(app->Resources->Lookup("FlyoutButtonTextBlockStyle"));
tb2->Text = "OK";
b->Content = tb2;
b->Click += ref new RoutedEventHandler
(
[flyout](Object^ sender, RoutedEventArgs^ args)
{
flyout->Hide();
}
);
grid->Children->Append(b);
return flyout;
}

Flyout^ ClipboardManager::CommonTools::CreateConfirmFlyout(ElementTheme theme, String^ Message, String^ ButtonCaption, FlyoutButtonClick^ ButtonClick)
{
Application^ app = Application::Current;
Flyout^ flyout = CreateFlyout(theme, L"\xE11B");
Grid^ grid = dynamic_cast<Grid^>(flyout->Content);
grid->Style = dynamic_cast<Style^>(app->Resources->Lookup("FlyoutGridStyle"));
TextBlock^ tb = ref new TextBlock();
tb->Style = dynamic_cast<Style^>(app->Resources->Lookup("FlyoutTextBlockStyle"));
Run^ r1 = ref new Run();
r1->Text = Message;
tb->Inlines->Append(r1);
grid->Children->Append(tb);
Button^ b = ref new Button();
b->Style = dynamic_cast<Style^>(app->Resources->Lookup("FlyoutButtonStyle"));
TextBlock^ tb2 = ref new TextBlock();
tb2->Style = dynamic_cast<Style^>(app->Resources->Lookup("FlyoutButtonTextBlockStyle"));
tb2->Text = ButtonCaption;
b->Content = tb2;
b->Click += ref new RoutedEventHandler
(
[ButtonClick, flyout](Object^ sender, RoutedEventArgs^ args)
{
if (ButtonClick != nullptr)
	ButtonClick->Invoke();
flyout->Hide();
}
);
grid->Children->Append(b);
return flyout;
}

Button^ ClipboardManager::CommonTools::CreateMenuFlyoutButton(Application^ app, String^ icon, String^ text, RoutedEventHandler^ clickHandler)
{
Button^ b = ref new Button();
b->Style = dynamic_cast<Windows::UI::Xaml::Style^>(app->Resources->Lookup("MenuFlyoutButtonStyle"));
if (clickHandler != nullptr)
	b->Click += clickHandler;
StackPanel^ sp = ref new StackPanel();
sp->Orientation = Orientation::Horizontal;
TextBlock^ tb = ref new TextBlock();
tb->Style= dynamic_cast<Windows::UI::Xaml::Style^>(app->Resources->Lookup("MenuFlyoutButtonIconStyle"));
tb->Text = icon;
sp->Children->Append(tb);
TextBlock^ tb2 = ref new TextBlock();
tb2->Style= dynamic_cast<Windows::UI::Xaml::Style^>(app->Resources->Lookup("MenuFlyoutButtonTextStyle"));
tb2->Text = text;
sp->Children->Append(tb2);
b->Content = sp;
return b;
}

CheckBox^ ClipboardManager::CommonTools::CreateMenuFlyoutCheckBox(Application^ app, String^ text, RoutedEventHandler^ checkedHandler, RoutedEventHandler^ uncheckedHandler, bool isChecked)
{
CheckBox^ cb = ref new CheckBox();
cb->Style = dynamic_cast<Windows::UI::Xaml::Style^>(app->Resources->Lookup("MenuFlyoutCheckBoxStyle"));
cb->IsChecked = isChecked;
cb->Checked += checkedHandler;
cb->Unchecked += uncheckedHandler;
TextBlock^ tb = ref new TextBlock();
tb->Style= dynamic_cast<Windows::UI::Xaml::Style^>(app->Resources->Lookup("MenuFlyoutCheckBoxTextStyle"));
tb->Text = text;
cb->Content = tb;
return cb;
}