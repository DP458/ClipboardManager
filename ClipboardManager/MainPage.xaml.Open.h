#pragma once

#include "MainPage.xaml.h"
#include "CommonTools.h"

namespace ClipboardManager
{

class MainPageLoadTextClass
{

private:
	ClipboardManager::MainPage^ main_page;

public:
	explicit MainPageLoadTextClass(ClipboardManager::MainPage^ page);

	~MainPageLoadTextClass();

	void operator()(const concurrency::task<Platform::String^>& t)const;

};

class MainPageLoadFilesClass
{

private:
	ClipboardManager::MainPage^ main_page;

public:
	explicit MainPageLoadFilesClass(ClipboardManager::MainPage^ page);

	~MainPageLoadFilesClass();

	void operator()(const concurrency::task<Windows::Foundation::Collections::IVectorView<Windows::Storage::StorageFile^>^>& t)const;

};

class MainPageLoadImageClass
{

private:
	ClipboardManager::MainPage^ main_page;
	ClipboardManager::CommonTools::LoadImageFileData^ ldata;

public:
	explicit MainPageLoadImageClass(ClipboardManager::MainPage^ page, ClipboardManager::CommonTools::LoadImageFileData^ lif_data);

	~MainPageLoadImageClass();

	void operator()(const concurrency::task<void>& t)const;

};

class MainPageLoadDataClass
{

private:
	ClipboardManager::MainPage^ main_page;
	ClipboardManager::CommonTools::ClipboardDataType data_type;

public:
	explicit MainPageLoadDataClass(ClipboardManager::MainPage^ page, ClipboardManager::CommonTools::ClipboardDataType type);

	~MainPageLoadDataClass();

	void operator()(const concurrency::task<Windows::Storage::StorageFile^>& t)const;

};

}