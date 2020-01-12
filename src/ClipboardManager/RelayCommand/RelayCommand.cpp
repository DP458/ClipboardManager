// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"

using namespace ClipboardManager;

ClipboardManager::RelayCommand::RelayCommand(ClipboardManager::ExecuteHandler^ executeHandler, ClipboardManager::CanExecuteHandler^ canExecuteHandler) : _canExecuteHandler(canExecuteHandler), _executeHandler(executeHandler)
{
};

#pragma region Properties

#pragma region CanExecuteHandler

ClipboardManager::CanExecuteHandler^ ClipboardManager::RelayCommand::CanExecuteHandler::get()
{
	return _canExecuteHandler;
}

void ClipboardManager::RelayCommand::CanExecuteHandler::set(ClipboardManager::CanExecuteHandler^ value)
{
	_canExecuteHandler = value;
}

#pragma endregion

#pragma region ExecuteHandler

ClipboardManager::ExecuteHandler^ ClipboardManager::RelayCommand::ExecuteHandler::get()
{
	return _executeHandler;
}

void ClipboardManager::RelayCommand::ExecuteHandler::set(ClipboardManager::ExecuteHandler^ value)
{
	_executeHandler = value;
}

#pragma endregion

#pragma endregion

#pragma region ICommand Implementations

bool ClipboardManager::RelayCommand::CanExecute(Platform::Object^ parameter)
{
	return _canExecuteHandler == nullptr ? false : _canExecuteHandler(parameter);
}

void ClipboardManager::RelayCommand::Execute(Platform::Object^ parameter)
{
	if (_executeHandler != nullptr)
		_executeHandler(parameter);
}

#pragma endregion