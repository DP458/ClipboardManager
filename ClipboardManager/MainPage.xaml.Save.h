#pragma once

#include "MainPage.xaml.h"
#include "CommonTools.h"

namespace ClipboardManager
{

class MainPageSaveDataClass
{

private:
	ClipboardManager::MainPage^ main_page;
	Windows::Storage::StorageFile^ storage_file;
	Platform::String^ res_successful_save;
	Platform::String^ res_error_save;
	Platform::String^ res_error_save_description;

public:
	explicit MainPageSaveDataClass(ClipboardManager::MainPage^ page, Windows::Storage::StorageFile^ file, Platform::String^ successful_save, Platform::String^ error_save, Platform::String^ error_save_description);

	~MainPageSaveDataClass();

	Windows::Foundation::IAsyncOperation<Windows::Storage::Provider::FileUpdateStatus>^ operator()(const concurrency::task<void>& t)const;

};

ref class SaveImageData sealed
{

private:
	Windows::UI::Xaml::Media::Imaging::WriteableBitmap^ w_bitmap;

	~SaveImageData();

public:
	SaveImageData();

	property Windows::UI::Xaml::Media::Imaging::WriteableBitmap^ WBitmap
	{

		Windows::UI::Xaml::Media::Imaging::WriteableBitmap^ get()
		{
			return w_bitmap;
		}

		void set(Windows::UI::Xaml::Media::Imaging::WriteableBitmap^ value)
		{
			w_bitmap = value;
		}

	}

};

class MainPageSetWriteableBitmapSourceClass
{
	
private:
	ClipboardManager::SaveImageData^ save_data;
	Windows::UI::Xaml::Media::Imaging::BitmapImage^ bitmap_image;
	Windows::Storage::Streams::IRandomAccessStream^ random_access_stream;

public:
	explicit MainPageSetWriteableBitmapSourceClass(ClipboardManager::SaveImageData^ s_data, Windows::UI::Xaml::Media::Imaging::BitmapImage^ bitmap, Windows::Storage::Streams::IRandomAccessStream^ stream);

	~MainPageSetWriteableBitmapSourceClass();

	Windows::Foundation::IAsyncAction^ operator()(const concurrency::task<void>& t)const;

};

class MainPageOpenFileClass
{

private:
	Windows::Storage::StorageFile^ storage_file;

public:
	explicit MainPageOpenFileClass(Windows::Storage::StorageFile^ file);

	~MainPageOpenFileClass();

	Windows::Foundation::IAsyncOperation<Windows::Storage::Streams::IRandomAccessStream^>^ operator()(const concurrency::task<void>& t)const;

};

class MainPageGetBitmapEncoderClass
{

private:
	Platform::String^ file_type;

public:
	explicit MainPageGetBitmapEncoderClass(Platform::String^ type);

	~MainPageGetBitmapEncoderClass();

	Windows::Foundation::IAsyncOperation<Windows::Graphics::Imaging::BitmapEncoder^>^ operator()(const concurrency::task<Windows::Storage::Streams::IRandomAccessStream^>& t)const;

};

class MainPageSetSoftwareBitmaptoEncoderClass
{

private:
	ClipboardManager::SaveImageData^ save_data;

public:
	explicit MainPageSetSoftwareBitmaptoEncoderClass(ClipboardManager::SaveImageData^ s_data);

	~MainPageSetSoftwareBitmaptoEncoderClass();

	Windows::Foundation::IAsyncAction^ operator()(const concurrency::task<Windows::Graphics::Imaging::BitmapEncoder^>& t);

};

class MainPageFileUpdateStatusClass
{

private:
	ClipboardManager::MainPage^ main_page;

public:
	explicit MainPageFileUpdateStatusClass(ClipboardManager::MainPage^ page);

	~MainPageFileUpdateStatusClass();

	void operator()(const concurrency::task<Windows::Storage::Provider::FileUpdateStatus>& t)const;

};

class MainPagePickSaveFileClass
{

private:
	ClipboardManager::MainPage^ main_page;
	ClipboardManager::CommonTools::ClipboardDataType data_type;
	Platform::Object^ obj;

public:
	explicit MainPagePickSaveFileClass(ClipboardManager::MainPage^ page, ClipboardManager::CommonTools::ClipboardDataType type, Platform::Object^ data);

	~MainPagePickSaveFileClass();

	void operator()(const concurrency::task<Windows::Storage::StorageFile^>& t)const;

};

}