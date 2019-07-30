// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include "MainPage.xaml.Save.h"
#include "MainPage.xaml.h"

using namespace ClipboardManager;
using namespace ClipboardManager::CommonTools;

using namespace Platform;
using namespace Platform::Collections;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Provider;
using namespace Windows::Storage::Streams;

ClipboardManager::MainPageSaveDataClass::MainPageSaveDataClass(MainPage^ page, StorageFile^ file, String^ successful_save, String^ error_save, String^ error_save_description) :
	main_page(page),
	storage_file(file),
	res_successful_save(successful_save),
	res_error_save(error_save),
	res_error_save_description(error_save_description)
{

}

ClipboardManager::MainPageSaveDataClass::~MainPageSaveDataClass()
{
main_page = nullptr;
storage_file = nullptr;
res_successful_save = nullptr;
res_error_save = nullptr;
res_error_save_description = nullptr;
}

IAsyncOperation<FileUpdateStatus>^ ClipboardManager::MainPageSaveDataClass::operator()(const task<void>& t)const
{

try
{

	t.get();

	if (static_cast<bool>(main_page->Properties->Lookup("ShowSuccessNotify")))
		UserNotify(main_page->Res->GetString(res_successful_save), storage_file->Path, NotificationGroup::SaveData);

}
catch (Exception^)
{
	ShowError(main_page->Res->GetString(res_error_save), main_page->Res->GetString(res_error_save_description), main_page->Properties);
}

return CachedFileManager::CompleteUpdatesAsync(storage_file);

}

ClipboardManager::SaveImageData::~SaveImageData()
{
w_bitmap = nullptr;
}

ClipboardManager::SaveImageData::SaveImageData() : w_bitmap(nullptr)
{

}

ClipboardManager::MainPageSetWriteableBitmapSourceClass::MainPageSetWriteableBitmapSourceClass(SaveImageData^ s_data, BitmapImage^ bitmap, IRandomAccessStream^ stream) : save_data(s_data), bitmap_image(bitmap), random_access_stream(stream)
{

}

ClipboardManager::MainPageSetWriteableBitmapSourceClass::~MainPageSetWriteableBitmapSourceClass()
{
save_data = nullptr;
bitmap_image = nullptr;
random_access_stream = nullptr;
}

IAsyncAction^ ClipboardManager::MainPageSetWriteableBitmapSourceClass::operator()(const task<void>& t)const
{

try
{
	t.get();
}
catch (Exception^)
{
	return nullptr;
}

save_data->WBitmap = ref new WriteableBitmap(bitmap_image->PixelWidth, bitmap_image->PixelHeight);

return save_data->WBitmap->SetSourceAsync(random_access_stream->CloneStream());

}

ClipboardManager::MainPageOpenFileClass::MainPageOpenFileClass(StorageFile^ file) : storage_file(file)
{

}

ClipboardManager::MainPageOpenFileClass::~MainPageOpenFileClass()
{
	storage_file = nullptr;
}

IAsyncOperation<IRandomAccessStream^>^ ClipboardManager::MainPageOpenFileClass::operator()(const task<void>& t)const
{

try
{
	t.get();
}
catch (Exception^)
{
	return nullptr;
}

CachedFileManager::DeferUpdates(storage_file);

return storage_file->OpenAsync(FileAccessMode::ReadWrite);

}

ClipboardManager::MainPageGetBitmapEncoderClass::MainPageGetBitmapEncoderClass(String^ type) : file_type(type)
{

}

ClipboardManager::MainPageGetBitmapEncoderClass::~MainPageGetBitmapEncoderClass()
{
file_type = nullptr;
}

IAsyncOperation<BitmapEncoder^>^ ClipboardManager::MainPageGetBitmapEncoderClass::operator()(const task<IRandomAccessStream^>& t)const
{

IRandomAccessStream^ f_stream;

try
{
	f_stream = t.get();
}
catch (Exception^)
{
	return nullptr;
}

if (file_type == ".jpg")
	return BitmapEncoder::CreateAsync(BitmapEncoder::JpegEncoderId, f_stream);

if (file_type == ".bmp")
	return BitmapEncoder::CreateAsync(BitmapEncoder::BmpEncoderId, f_stream);

if (file_type == ".png")
	return BitmapEncoder::CreateAsync(BitmapEncoder::PngEncoderId, f_stream);

if (file_type == ".gif")
	return BitmapEncoder::CreateAsync(BitmapEncoder::GifEncoderId, f_stream);

if (file_type == ".tiff")
	return BitmapEncoder::CreateAsync(BitmapEncoder::TiffEncoderId, f_stream);

return nullptr;

}

ClipboardManager::MainPageSetSoftwareBitmaptoEncoderClass::MainPageSetSoftwareBitmaptoEncoderClass(SaveImageData^ s_data) : save_data(s_data)
{

}

ClipboardManager::MainPageSetSoftwareBitmaptoEncoderClass::~MainPageSetSoftwareBitmaptoEncoderClass()
{
	save_data = nullptr;
}

IAsyncAction^ ClipboardManager::MainPageSetSoftwareBitmaptoEncoderClass::operator()(const task<BitmapEncoder^>& t)
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

encoder->SetSoftwareBitmap(SoftwareBitmap::CreateCopyFromBuffer(save_data->WBitmap->PixelBuffer, BitmapPixelFormat::Bgra8, save_data->WBitmap->PixelWidth, save_data->WBitmap->PixelHeight));

return encoder->FlushAsync();

}

ClipboardManager::MainPageFileUpdateStatusClass::MainPageFileUpdateStatusClass(MainPage^ page) : main_page(page)
{

}

ClipboardManager::MainPageFileUpdateStatusClass::~MainPageFileUpdateStatusClass()
{
main_page = nullptr;
}

void ClipboardManager::MainPageFileUpdateStatusClass::operator()(const task<FileUpdateStatus>& t)const
{

main_page->WaiterState = false;

try
{
	t.get();
}
catch (Exception^)
{
	return;
}

}

ClipboardManager::MainPagePickSaveFileClass::MainPagePickSaveFileClass(MainPage^ page, ClipboardDataType type, Object^ data) : main_page(page), data_type(type), obj(data)
{

}

ClipboardManager::MainPagePickSaveFileClass::~MainPagePickSaveFileClass()
{
main_page = nullptr;
obj = nullptr;
}

void ClipboardManager::MainPagePickSaveFileClass::operator()(const task<StorageFile^>& t)const
{

StorageFile^ file;

try
{
	file = t.get();
}
catch (Exception^ ex)
{
	main_page->WaiterState = false;
	ShowError("Failed to save the file", "Possibly the file can not be written", main_page->Properties);
	return;
}

if (file == nullptr)
{
	main_page->WaiterState = false;
	return;
}

switch (data_type)
{

case ClipboardDataType::Link:
{

Uri^ link = dynamic_cast<Uri^>(obj);

if (link == nullptr)
{
	main_page->WaiterState = false;
	ShowError("Failed to save the link", "Possibly the link can not be saved", main_page->Properties);
	break;
}

CachedFileManager::DeferUpdates(file);

auto t = create_task(FileIO::WriteTextAsync(file, link->DisplayUri)).then(MainPageSaveDataClass(main_page, file, "NotifySaveLink", "MainPageSaveLinkError", "MainPageSaveLinkErrorDescription"));

t.then(MainPageFileUpdateStatusClass(main_page));

}
break;

case ClipboardDataType::Text:
{

String^ text = dynamic_cast<String^>(obj);

if (text == nullptr)
{
	main_page->WaiterState = false;
	ShowError("Failed to save the text", "Possibly the text can not be saved", main_page->Properties);
	break;
}

CachedFileManager::DeferUpdates(file);

auto t = create_task(FileIO::WriteTextAsync(file, text)).then(MainPageSaveDataClass(main_page, file, "NotifySaveText", "MainPageSaveTextError", "MainPageSaveTextErrorDescription"));

t.then(MainPageFileUpdateStatusClass(main_page));

}
break;

case ClipboardDataType::Files:
{

IVectorView<IStorageItem^>^ items = dynamic_cast<IVectorView<IStorageItem^>^>(obj);

if (items == nullptr)
{
	main_page->WaiterState = false;
	ShowError("Failed to save list of files", "Possibly the list can not be saved", main_page->Properties);
	break;
}

String^ paths = ref new String();

for (unsigned int i = 0; i < items->Size; i++)
	paths += items->GetAt(i)->Path + "\r\n";

CachedFileManager::DeferUpdates(file);

auto t = create_task(FileIO::WriteTextAsync(file, paths)).then(MainPageSaveDataClass(main_page, file, "NotifySaveFiles", "MainPageSaveFileListError", "MainPageSaveFileListErrorDescription"));

t.then(MainPageFileUpdateStatusClass(main_page));

}
break;

case ClipboardDataType::Image:
{

IRandomAccessStream^ BitmapStream = dynamic_cast<IRandomAccessStream^>(obj);

if (BitmapStream == nullptr)
{
	main_page->WaiterState = false;
	ShowError("Failed to save the image", "Possibly the image can not be saved", main_page->Properties);
	return;
}

task<void> t;

SaveImageData^ s_data = ref new SaveImageData();

{
	BitmapImage^ bitmap = ref new BitmapImage();
	t = create_task(bitmap->SetSourceAsync(BitmapStream->CloneStream())).then(MainPageSetWriteableBitmapSourceClass(s_data, bitmap, BitmapStream));
}

auto t2 = t.then(MainPageOpenFileClass(file));

auto t3 = t2.then(MainPageGetBitmapEncoderClass(file->FileType));

auto t4 = t3.then(MainPageSetSoftwareBitmaptoEncoderClass(s_data));

auto t5 = t4.then(MainPageSaveDataClass(main_page, file, "NotifySaveImage", "MainPageSaveImageError", "MainPageSaveImageErrorDescription"));

t5.then(MainPageFileUpdateStatusClass(main_page));

}
break;

default:
main_page->WaiterState = false;
break;

}

}