// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include "Notification.h"

using namespace ClipboardManager::CommonTools;

using namespace Platform;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Notifications;
using namespace Windows::Data::Xml::Dom;

ToastNotification^ ClipboardManager::CommonTools::CreateToastNotification(String^ header, String^ message)
{
XmlDocument^ xml_doc = ToastNotificationManager::GetTemplateContent(ToastTemplateType::ToastText02);
XmlNodeList^ textNodes = xml_doc->GetElementsByTagName("text");
textNodes->GetAt(0)->InnerText = header;
textNodes->GetAt(1)->InnerText = message;
ToastNotification^ notification = ref new ToastNotification(xml_doc);
return notification;
}

bool ClipboardManager::CommonTools::UserNotify(String^ header, String^ message, NotificationGroup group)
{
ToastNotifier^ notifier = ToastNotificationManager::CreateToastNotifier();
if (notifier->Setting != NotificationSetting::Enabled)
	return false;
if (message->Length() > 140)
{
wchar_t s_arr[141];
wcsncpy_s(s_arr, message->Data(), 140);
s_arr[140] = L'\0';
String^ str = ref new String(s_arr);
notifier->Show(CreateToastNotification(header, str));
}
else
	notifier->Show(CreateToastNotification(header, message));
return true;
}

bool ClipboardManager::CommonTools::ShowError(String^ header, String^ message, IPropertySet^ values)
{
if (values->HasKey("ShowErrors"))
	return static_cast<bool>(values->Lookup("ShowErrors")) ? UserNotify(header, message, NotificationGroup::Error) : false;
return false;
}