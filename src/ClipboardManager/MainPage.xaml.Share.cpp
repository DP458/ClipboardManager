// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include "MainPage.xaml.Share.h"
#include "MainPage.xaml.h"

using namespace ClipboardManager;
using namespace ClipboardManager::CommonTools;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;

void ClipboardManager::MainPage::ShareLink(DataRequest^ request, Uri^ link)
{
DataPackage^ dataPackage = ref new DataPackage();
try
{
dataPackage->Properties->Title = resourceLoader->GetString("MainPageShareLinkTitle");
dataPackage->Properties->Description = resourceLoader->GetString("MainPageShareLinkDescription");
dataPackage->SetWebLink(link);
dataPackage->SetText(link->DisplayUri);
request->Data = dataPackage;
}
catch (Exception^)
{
request->FailWithDisplayText(resourceLoader->GetString("MainPageShareLinkError"));
}
}

void ClipboardManager::MainPage::ShareText(DataRequest^ request, String^ text)
{
DataPackage^ dataPackage = ref new DataPackage();
try
{
dataPackage->Properties->Title = resourceLoader->GetString("MainPageShareTextTitle");
dataPackage->Properties->Description = resourceLoader->GetString("MainPageShareTextDescription");
dataPackage->SetText(text);
request->Data = dataPackage;
}
catch (Exception^)
{
request->FailWithDisplayText(resourceLoader->GetString("MainPageShareTextError"));
}
}

void ClipboardManager::MainPage::ShareFiles(DataRequest^ request, IVectorView<IStorageItem^>^ items)
{
if (items == nullptr)
{
request->FailWithDisplayText(resourceLoader->GetString("MainPageShareFilesError"));
return;
}
Vector<IStorageItem^>^ vItems = ref new Vector<IStorageItem^>();
DataPackage^ dataPackage = ref new DataPackage();
try
{
for (unsigned int i = 0; i < items->Size; i++)
	vItems->Append(items->GetAt(i));
dataPackage->Properties->Title = resourceLoader->GetString("MainPageShareFilesTitle");
dataPackage->Properties->Description = resourceLoader->GetString("MainPageShareFilesDescription");
dataPackage->SetStorageItems(vItems->GetView());
request->Data = dataPackage;
}
catch (Exception^)
{
request->FailWithDisplayText(resourceLoader->GetString("MainPageShareFilesError"));
}
}

void ClipboardManager::MainPage::ShareImage(DataRequest^ request, IRandomAccessStream^ stream)
{
if (stream == nullptr)
{
request->FailWithDisplayText(resourceLoader->GetString("MainPageShareImageError"));
return;
}
DataPackage^ dataPackage = ref new DataPackage();
try
{
dataPackage->Properties->Title = resourceLoader->GetString("MainPageShareImageTitle");
dataPackage->Properties->Description = resourceLoader->GetString("MainPageShareImageDescription");
dataPackage->SetBitmap(RandomAccessStreamReference::CreateFromStream(stream->CloneStream()));
request->Data = dataPackage;
}
catch (Exception^)
{
request->FailWithDisplayText(resourceLoader->GetString("MainPageShareImageError"));
}
}

void ClipboardManager::MainPage::OnDataRequested(DataTransferManager^ sender, DataRequestedEventArgs^ args)
{
Object^ obj = data;
ClipboardDataType type = clipboardDataType;
switch (type)
{
case ClipboardDataType::Link:
ShareLink(args->Request, dynamic_cast<Uri^>(obj));
break;
case ClipboardDataType::Text:
ShareText(args->Request, dynamic_cast<String^>(obj));
break;
case ClipboardDataType::Files:
ShareFiles(args->Request, dynamic_cast<IVectorView<IStorageItem^>^>(obj));
break;
case ClipboardDataType::Image:
ShareImage(args->Request, dynamic_cast<IRandomAccessStream^>(obj));
break;
default:
args->Request->FailWithDisplayText("Failed to share data");
break;
}
}