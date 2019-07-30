#pragma once

namespace ClipboardManager
{

namespace CommonTools
{

class GetImageDecoderClass
{

public:
	Windows::Foundation::IAsyncOperation<Windows::Graphics::Imaging::BitmapDecoder^>^ operator()(const concurrency::task<Windows::Storage::Streams::IRandomAccessStream^>& t)const;

};

class GetSoftwareBitmapClass
{

public:
	Windows::Foundation::IAsyncOperation<Windows::Graphics::Imaging::SoftwareBitmap^>^ operator()(const concurrency::task<Windows::Graphics::Imaging::BitmapDecoder^>& t)const;

};

ref class LoadImageFileData sealed
{

private:
	Windows::Storage::Streams::InMemoryRandomAccessStream^ stream;
	Windows::Graphics::Imaging::SoftwareBitmap^ bitmap;

	~LoadImageFileData();

public:

	LoadImageFileData();

	property Windows::Storage::Streams::InMemoryRandomAccessStream^ MemoryStream
	{

		Windows::Storage::Streams::InMemoryRandomAccessStream^ get()
		{
			return stream;
		}

	}

	property Windows::Graphics::Imaging::SoftwareBitmap^ Bitmap
	{

		Windows::Graphics::Imaging::SoftwareBitmap^ get()
		{
			return bitmap;
		}

		void set(Windows::Graphics::Imaging::SoftwareBitmap^ value)
		{
			bitmap = value;
		}

	}

};

class GetImageEncoderClass
{

private:
	LoadImageFileData^ ldata;

public:
	explicit GetImageEncoderClass(LoadImageFileData^ lif_data);

	~GetImageEncoderClass();

	Windows::Foundation::IAsyncOperation<Windows::Graphics::Imaging::BitmapEncoder^>^ operator()(const concurrency::task<Windows::Graphics::Imaging::SoftwareBitmap^>& t)const;

};

class SetSoftwareBitmapToBitmapEncoderClass
{

private:
	LoadImageFileData^ ldata;

public:
	explicit SetSoftwareBitmapToBitmapEncoderClass(LoadImageFileData^ lif_data);

	~SetSoftwareBitmapToBitmapEncoderClass();

	Windows::Foundation::IAsyncAction^ operator()(const concurrency::task<Windows::Graphics::Imaging::BitmapEncoder^>& t)const;

};

}

}