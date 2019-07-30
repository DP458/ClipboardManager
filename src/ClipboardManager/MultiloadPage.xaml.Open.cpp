// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include <MultiloadPage.xaml.Open.h>

using namespace ClipboardManager;
using namespace ClipboardManager::CommonTools;

using namespace Platform;
using namespace concurrency;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;


ClipboardManager::MultiloadPageLoadTextClass::MultiloadPageLoadTextClass(MultiloadPage^ page) : multiload_page(page)
{

}

ClipboardManager::MultiloadPageLoadTextClass::~MultiloadPageLoadTextClass()
{
multiload_page = nullptr;
}

void ClipboardManager::MultiloadPageLoadTextClass::operator()(const task<String^>& t)const
{

String^ text;

try
{
	text = t.get();
}
catch (Exception^)
{
	multiload_page->WaiterState = false;
	ShowError("Failed to load text", "", multiload_page->Properties);
	return;
}

multiload_page->SetText(text);

multiload_page->PostLoadingProc(ClipboardDataType::Text);

}

ClipboardManager::MultiloadPageLoadFilesClass::MultiloadPageLoadFilesClass(MultiloadPage^ page) : multiload_page(page)
{

}

ClipboardManager::MultiloadPageLoadFilesClass::~MultiloadPageLoadFilesClass()
{
multiload_page = nullptr;
}

void ClipboardManager::MultiloadPageLoadFilesClass::operator()(const task<IVectorView<StorageFile^>^>& t)const
{

IVectorView<StorageFile^>^ files;

try
{
	files = t.get();
}
catch (Exception^)
{
	multiload_page->WaiterState = false;
	ShowError("Failed to load files", "", multiload_page->Properties);
	return;
}

if (files->Size == 0U)
{

	multiload_page->WaiterState = false;

	return;

}

multiload_page->SetFiles(files);

multiload_page->PostLoadingProc(ClipboardDataType::Files);

}

ClipboardManager::MultiloadPageLoadImageClass::MultiloadPageLoadImageClass(MultiloadPage^ page, LoadImageFileData^ lif_data) : multiload_page(page), ldata(lif_data)
{

}

ClipboardManager::MultiloadPageLoadImageClass::~MultiloadPageLoadImageClass()
{
multiload_page = nullptr;
ldata = nullptr;
}

void ClipboardManager::MultiloadPageLoadImageClass::operator()(const task<void>& t)const
{

try
{
	t.get();
}
catch (Exception^)
{
	multiload_page->WaiterState = false;
	ShowError("Failed to load image", "", multiload_page->Properties);
	return;
}

multiload_page->SetImage(ldata->MemoryStream);

multiload_page->PostLoadingProc(ClipboardDataType::Image);

}

ClipboardManager::MultiloadPageLoadDataClass::MultiloadPageLoadDataClass(MultiloadPage^ page, ClipboardDataType type) : multiload_page(page), data_type(type)
{

}

ClipboardManager::MultiloadPageLoadDataClass::~MultiloadPageLoadDataClass()
{
multiload_page = nullptr;
}

void ClipboardManager::MultiloadPageLoadDataClass::operator()(const task<StorageFile^>& t)const
{

StorageFile^ file;

try
{
	file = t.get();
}
catch (Exception^)
{
	multiload_page->WaiterState = false;
	ShowError("Failed to open file", "", multiload_page->Properties);
	return;
}

if (file == nullptr)
{

	multiload_page->WaiterState = false;

	return;

}

if (data_type == ClipboardDataType::Text)
{

	create_task(FileIO::ReadTextAsync(file)).then(MultiloadPageLoadTextClass(multiload_page));

	return;

}

if (data_type == ClipboardDataType::Image)
{

	auto t = create_task(file->OpenAsync(FileAccessMode::Read, StorageOpenOptions::None)).then(GetImageDecoderClass());

	auto t2 = t.then(GetSoftwareBitmapClass());

	LoadImageFileData^ lif_data = ref new LoadImageFileData();

	auto t3 = t2.then(GetImageEncoderClass(lif_data));

	auto t4 = t3.then(SetSoftwareBitmapToBitmapEncoderClass(lif_data));

	t4.then(MultiloadPageLoadImageClass(multiload_page, lif_data));

}

}