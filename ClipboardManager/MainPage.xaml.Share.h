#pragma once

#include "MainPage.g.h"

namespace ClipboardManager
{

partial ref class MainPage sealed
{
private:
	void ShareLink(Windows::ApplicationModel::DataTransfer::DataRequest^ request, Windows::Foundation::Uri^ link);
	void ShareText(Windows::ApplicationModel::DataTransfer::DataRequest^ request, Platform::String^ text);
	void ShareFiles(Windows::ApplicationModel::DataTransfer::DataRequest^ request, Windows::Foundation::Collections::IVectorView<Windows::Storage::IStorageItem^>^ items);
	void ShareImage(Windows::ApplicationModel::DataTransfer::DataRequest^ request, Windows::Storage::Streams::IRandomAccessStream^ stream);
	void OnDataRequested(Windows::ApplicationModel::DataTransfer::DataTransferManager ^sender, Windows::ApplicationModel::DataTransfer::DataRequestedEventArgs ^args);
};

}