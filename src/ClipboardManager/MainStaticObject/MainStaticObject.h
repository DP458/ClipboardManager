#pragma once

namespace ClipboardManager
{
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

#pragma region PageBackwardCommand

	public:
		static property Windows::UI::Xaml::Input::ICommand^ PageBackwardCommand
		{
			Windows::UI::Xaml::Input::ICommand^ get();
		}

#pragma endregion

#pragma region Methods

	public:
		static void GoBack();

#pragma endregion
	};
}