#pragma once

namespace ClipboardManager
{
	public enum class ClipboardManagerPages : int
	{
		AboutPage = 1,
		SettingsPage = 2,
		TextEditPage = 3,
		FavoritesPage = 4,
		MultiloadPage = 5
	};

	public ref class MainStaticObject sealed
	{
	private:
		MainStaticObject();

#pragma region Properties

	public:
		static property Platform::String^ AppVersionString
		{
			Platform::String^ get();
		}

#pragma endregion

#pragma region Commands

#pragma region PageBackwardCommand

	private:
		static void GoBackward(Platform::Object^ parameter);

	public:
		static property Windows::UI::Xaml::Input::ICommand^ PageBackwardCommand
		{
			Windows::UI::Xaml::Input::ICommand^ get();
		}

#pragma endregion

#pragma region GoToPageCommand

	private:
		static void GoToPage(Platform::Object^ parameter);

	public:
		static property Windows::UI::Xaml::Input::ICommand^ GoToPageCommand
		{
			Windows::UI::Xaml::Input::ICommand^ get();
		}

#pragma endregion

#pragma endregion

#pragma region Methods

	public:
		static void GoBack();

#pragma endregion
	};
}