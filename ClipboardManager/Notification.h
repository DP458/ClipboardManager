#pragma once

namespace ClipboardManager
{

namespace CommonTools
{

enum NotificationGroup
{
None,
CopyToClipboard,
SaveData,
Error
};

Windows::UI::Notifications::ToastNotification^ CreateToastNotification(Platform::String^ header, Platform::String^ message);
bool UserNotify(Platform::String^ header, Platform::String^ message, ClipboardManager::CommonTools::NotificationGroup group);
bool ShowError(Platform::String^ header, Platform::String^ message, Windows::Foundation::Collections::IPropertySet^ values);

}

}