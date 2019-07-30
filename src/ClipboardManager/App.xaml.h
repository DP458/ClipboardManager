#pragma once

#include "App.g.h"

namespace ClipboardManager
{
	
ref class App sealed
{
protected:
	virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;
	virtual void OnShareTargetActivated(Windows::ApplicationModel::Activation::ShareTargetActivatedEventArgs^ args) override;

internal:
	App();

private:
	template<class T> void InitSetting(Windows::Foundation::Collections::IPropertySet^ values, Platform::String^ parameter, T value);
	void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
	void OnResuming(Platform::Object ^sender, Platform::Object ^args);
	void OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e);
	void OnBackRequested(Platform::Object ^sender, Windows::UI::Core::BackRequestedEventArgs ^args);
};

}