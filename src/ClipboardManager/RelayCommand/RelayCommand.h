#pragma once

namespace ClipboardManager
{
    public delegate bool CanExecuteHandler(Platform::Object^ parameter);

    public delegate void ExecuteHandler(Platform::Object^ parameter);

    //Implementation of the ICommand interface to create user commands
	[Windows::Foundation::Metadata::WebHostHiddenAttribute]
	public ref class RelayCommand sealed : public Windows::UI::Xaml::Input::ICommand
    {
    private:
        ClipboardManager::CanExecuteHandler^ _canExecuteHandler;
        ClipboardManager::ExecuteHandler^ _executeHandler;

    public:
        RelayCommand(ClipboardManager::ExecuteHandler^ executeHandler, ClipboardManager::CanExecuteHandler^ canExecuteHandler);

#pragma region Properties

#pragma region CanExecuteHandler

    public:
        property ClipboardManager::CanExecuteHandler^ CanExecuteHandler
        {
            ClipboardManager::CanExecuteHandler^ get();
            void set(ClipboardManager::CanExecuteHandler^ value);
        }

#pragma endregion

#pragma region ExecuteHandler

    public:
        property ClipboardManager::ExecuteHandler^ ExecuteHandler
        {
            ClipboardManager::ExecuteHandler^ get();
            void set(ClipboardManager::ExecuteHandler^ value);
        }

#pragma endregion

#pragma endregion

#pragma region ICommand Implementations

    public:
        virtual bool CanExecute(Platform::Object^ parameter);
        virtual void Execute(Platform::Object^ parameter);
        virtual event Windows::Foundation::EventHandler<Platform::Object^>^ CanExecuteChanged;

#pragma endregion
    };
}