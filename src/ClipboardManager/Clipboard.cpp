// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include "Clipboard.h"

using namespace ClipboardManager::CommonTools;

using namespace concurrency;
using namespace Platform;
using namespace Windows::ApplicationModel::DataTransfer;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;

bool ClipboardManager::CommonTools::LoadData(DataPackage^ dataPackage)
{
try
{
Clipboard::SetContent(dataPackage);
Clipboard::Flush();
}
catch (Exception^)
{
return false;
}
return true;
}

bool ClipboardManager::CommonTools::LoadLink(Uri^ link)
{
DataPackage^ data_package = ref new DataPackage();
try
{
data_package->SetWebLink(link);
data_package->SetText(link->DisplayUri);
}
catch (Exception^)
{
return false;
}
return LoadData(data_package);
}

bool ClipboardManager::CommonTools::LoadText(String^ inputText)
{
DataPackage^ data_package = ref new DataPackage();
try
{
data_package->SetText(inputText);
}
catch (Exception^)
{
return false;
}
return LoadData(data_package);
}

bool ClipboardManager::CommonTools::LoadFiles(IIterable<IStorageItem^>^ files)
{
DataPackage^ data_package = ref new DataPackage();
try
{
data_package->RequestedOperation = DataPackageOperation::Copy;
data_package->SetStorageItems(files, true);
}
catch (Exception^)
{
return false;
}
return LoadData(data_package);
}

bool ClipboardManager::CommonTools::LoadImage(IRandomAccessStream^ bitmap_stream)
{ 
DataPackage^ data_package = ref new DataPackage();
try
{
data_package->SetBitmap(RandomAccessStreamReference::CreateFromStream(bitmap_stream));
}
catch (Exception^)
{
return false;
}
return LoadData(data_package);
}

bool ClipboardManager::CommonTools::ClearClipboard()
{
try
{
Clipboard::Clear();
}
catch (Exception^)
{
return false;
}
return true;
}

DataPackageView^ ClipboardManager::CommonTools::GetClipboardContent()
{
try
{
return Clipboard::GetContent();
}
catch (Exception^)
{
return nullptr;
}
}