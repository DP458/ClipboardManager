// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include "Functors.h"

using namespace ClipboardManager::CommonTools;

using namespace Platform;
using namespace Platform::Collections;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Graphics::Imaging;

IAsyncOperation<BitmapDecoder^>^ GetImageDecoderClass::operator()(const task<IRandomAccessStream^>& t)const
{

IRandomAccessStream^ stream;

try
{
	stream = t.get();
}
catch (Exception^)
{
	return nullptr;
}

return BitmapDecoder::CreateAsync(stream);

};

IAsyncOperation<SoftwareBitmap^>^ GetSoftwareBitmapClass::operator()(const task<BitmapDecoder^>& t)const
{

BitmapDecoder^ decoder;

try
{
	decoder = t.get();
}
catch (Exception^)
{
	return nullptr;
}

return decoder->GetSoftwareBitmapAsync();

}

LoadImageFileData::~LoadImageFileData()
{
stream = nullptr;
bitmap = nullptr;
}

LoadImageFileData::LoadImageFileData()
{
stream = ref new Windows::Storage::Streams::InMemoryRandomAccessStream();
bitmap = nullptr;
}

GetImageEncoderClass::GetImageEncoderClass(LoadImageFileData^ lif_data) : ldata(lif_data)
{

}

GetImageEncoderClass::~GetImageEncoderClass()
{
ldata = nullptr;
}

IAsyncOperation<BitmapEncoder^>^ GetImageEncoderClass::operator()(const task<SoftwareBitmap^>& t)const
{

try
{
	ldata->Bitmap = t.get();
}
catch (Exception^)
{
	return nullptr;
}

return BitmapEncoder::CreateAsync(BitmapEncoder::BmpEncoderId, ldata->MemoryStream);

}

SetSoftwareBitmapToBitmapEncoderClass::SetSoftwareBitmapToBitmapEncoderClass(LoadImageFileData^ lif_data) : ldata(lif_data)
{

}

SetSoftwareBitmapToBitmapEncoderClass::~SetSoftwareBitmapToBitmapEncoderClass()
{
ldata = nullptr;
}

IAsyncAction^ SetSoftwareBitmapToBitmapEncoderClass::operator()(const task<BitmapEncoder^>& t)const
{

BitmapEncoder^ encoder;

try
{
	encoder = t.get();
}
catch (Exception^)
{
	return nullptr;
}

encoder->SetSoftwareBitmap(ldata->Bitmap);

return encoder->FlushAsync();

}