#pragma once

namespace ClipboardManager
{

namespace CommonTools
{

public enum class ClipboardDataType :int
{
Nothing,
Text,
Files,
Image,
Unknown,
Link
};

bool LoadData(Windows::ApplicationModel::DataTransfer::DataPackage^ dataPackage);
bool LoadLink(Windows::Foundation::Uri^ link);
bool LoadText(Platform::String^ text);
bool LoadFiles(Windows::Foundation::Collections::IIterable<Windows::Storage::IStorageItem^>^ files);
bool LoadImage(Windows::Storage::Streams::IRandomAccessStream^ bitmap_stream);
bool ClearClipboard();
Windows::ApplicationModel::DataTransfer::DataPackageView^ GetClipboardContent();

}

}