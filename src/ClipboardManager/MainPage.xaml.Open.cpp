// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include "MainPage.xaml.Open.h"

using namespace ClipboardManager;
using namespace ClipboardManager::CommonTools;

using namespace Platform;
using namespace Platform::Collections;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Storage::Pickers;
using namespace Windows::Graphics::Imaging;

ClipboardManager::MainPageLoadTextClass::MainPageLoadTextClass(MainPage^ page) : main_page(page)
{

}

ClipboardManager::MainPageLoadTextClass::~MainPageLoadTextClass()
{
main_page = nullptr;
}

void ClipboardManager::MainPageLoadTextClass::operator()(const task<String^>& t)const
{

String^ text;

try
{
	text = t.get();
}
catch (Exception^)
{
	main_page->WaiterState = false;
	ShowError("Failed to load text into the clipboard", "", main_page->Properties);
	return;
}

if (LoadText(text))
{

	main_page->WaiterState = false;

	if (static_cast<bool>(main_page->Properties->Lookup("ShowSuccessAddContentNotify")))
		UserNotify(main_page->Res->GetString("NotifyCopyTextToClipboard"), "", NotificationGroup::CopyToClipboard);

}
else
{

	main_page->WaiterState = false;


	ShowError("Failed to load text into the clipboard", "", main_page->Properties);

}

};

ClipboardManager::MainPageLoadFilesClass::MainPageLoadFilesClass(ClipboardManager::MainPage ^ page) : main_page(page)
{

}

ClipboardManager::MainPageLoadFilesClass::~MainPageLoadFilesClass()
{
main_page = nullptr;
}

void ClipboardManager::MainPageLoadFilesClass::operator()(const task<IVectorView<StorageFile^>^>& t)const
{

IVectorView<StorageFile^>^ files;

try
{
	files = t.get();
}
catch (Exception^)
{
	main_page->WaiterState = false;
	ShowError("Failed to load files into the clipboard", "", main_page->Properties);
	return;
}

if (files->Size == 0U)
{

	main_page->WaiterState = false;

	return;

}

Vector<IStorageItem^>^ items = ref new Vector<IStorageItem^>();

for (unsigned int i = 0; i < files->Size; i++)
	items->Append(files->GetAt(i));

if (LoadFiles(items->GetView()))
{

	main_page->WaiterState = false;

	if (static_cast<bool>(main_page->Properties->Lookup("ShowSuccessAddContentNotify")))
		UserNotify(main_page->Res->GetString("NotifyCopyFilesToClipboard"), "", NotificationGroup::CopyToClipboard);

}
else
{

	main_page->WaiterState = false;

	ShowError("Failed to load files into the clipboard", "", main_page->Properties);

}

}

ClipboardManager::MainPageLoadImageClass::MainPageLoadImageClass(MainPage^ page, LoadImageFileData^ lif_data) : main_page(page), ldata(lif_data)
{

}

ClipboardManager::MainPageLoadImageClass::~MainPageLoadImageClass()
{
main_page = nullptr;
ldata = nullptr;
}

void ClipboardManager::MainPageLoadImageClass::operator()(const task<void>& t)const
{

try
{
	t.get();
}
catch (Exception^)
{
	main_page->WaiterState = false;
	ShowError("Failed to load image into the clipboard", "", main_page->Properties);
	return;
}

if (LoadImage(ldata->MemoryStream))
{

	main_page->WaiterState = false;

	if (static_cast<bool>(main_page->Properties->Lookup("ShowSuccessAddContentNotify")))
		UserNotify(main_page->Res->GetString("NotifyCopyImageToClipboard"), "", NotificationGroup::CopyToClipboard);

}
else
{

	main_page->WaiterState = false;

	ShowError("Failed to load image into the clipboard", "", main_page->Properties);

}

}

ClipboardManager::MainPageLoadDataClass::MainPageLoadDataClass(MainPage^ page, ClipboardDataType type) : main_page(page), data_type(type)
{

}

ClipboardManager::MainPageLoadDataClass::~MainPageLoadDataClass()
{
main_page = nullptr;
}

void ClipboardManager::MainPageLoadDataClass::operator()(const task<StorageFile^>& t)const
{

StorageFile^ file;

try
{
	file = t.get();
}
catch (Exception^)
{
	main_page->WaiterState = false;
	ShowError("Failed to open file", "Possibly the file is not available", main_page->Properties);
	return;
}

if (file == nullptr)
{
	main_page->WaiterState = false;
	return;
}

if (data_type == ClipboardDataType::Text)
{

	create_task(FileIO::ReadTextAsync(file)).then(MainPageLoadTextClass(main_page));

	return;

}

if (data_type == ClipboardDataType::Image)
{

	auto t = create_task(file->OpenAsync(FileAccessMode::Read, StorageOpenOptions::None)).then(GetImageDecoderClass());

	auto t2 = t.then(GetSoftwareBitmapClass());

	LoadImageFileData^ lif_data = ref new LoadImageFileData();

	auto t3 = t2.then(GetImageEncoderClass(lif_data));

	auto t4 = t3.then(SetSoftwareBitmapToBitmapEncoderClass(lif_data));

	t4.then(MainPageLoadImageClass(main_page, lif_data));

}

}