#pragma once

#include "MultiloadPage.xaml.h"
#include "CommonTools.h"

namespace ClipboardManager
{

class MultiloadPageLoadTextClass
{

private:
	ClipboardManager::MultiloadPage^ multiload_page;

public:
	explicit MultiloadPageLoadTextClass(ClipboardManager::MultiloadPage^ page);

	~MultiloadPageLoadTextClass();

	void operator()(const concurrency::task<Platform::String^>& t)const;

};

class MultiloadPageLoadFilesClass
{

private:
	ClipboardManager::MultiloadPage^ multiload_page;

public:
	explicit MultiloadPageLoadFilesClass(ClipboardManager::MultiloadPage^ page);

	~MultiloadPageLoadFilesClass();

	void operator()(const concurrency::task<Windows::Foundation::Collections::IVectorView<Windows::Storage::StorageFile^>^>& t)const;

};

class MultiloadPageLoadImageClass
{

private:
	ClipboardManager::MultiloadPage^ multiload_page;
	ClipboardManager::CommonTools::LoadImageFileData^ ldata;

public:
	explicit MultiloadPageLoadImageClass(ClipboardManager::MultiloadPage^ page, ClipboardManager::CommonTools::LoadImageFileData^ lif_data);

	~MultiloadPageLoadImageClass();

	void operator()(const concurrency::task<void>& t)const;

};

class MultiloadPageLoadDataClass
{

private:
	ClipboardManager::MultiloadPage^ multiload_page;
	ClipboardManager::CommonTools::ClipboardDataType data_type;

public:
	explicit MultiloadPageLoadDataClass(ClipboardManager::MultiloadPage^ page, ClipboardManager::CommonTools::ClipboardDataType type);

	~MultiloadPageLoadDataClass();

	void operator()(const concurrency::task<Windows::Storage::StorageFile^>& t)const;

};

}