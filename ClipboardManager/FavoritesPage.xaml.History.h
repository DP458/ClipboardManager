#pragma once

#include "FavoritesPage.xaml.h"

namespace ClipboardManager
{

class FavoritesPageShowTextClass
{

private:
	ClipboardManager::FavoritesPage^ fav_page;
	Windows::Storage::StorageFile^ storage_file;
	ClipboardManager::CommonTools::ClipboardDataType data_type;

public:
	explicit FavoritesPageShowTextClass(ClipboardManager::FavoritesPage^ page, Windows::Storage::StorageFile^ file, ClipboardManager::CommonTools::ClipboardDataType type);

	~FavoritesPageShowTextClass();

	bool operator()(const concurrency::task<Platform::String^>& task_text)const;

};

class FavoritesPageShowImageClass
{

private:
	ClipboardManager::FavoritesPage^ fav_page;
	Windows::Storage::StorageFile^ storage_file;

public:
	explicit FavoritesPageShowImageClass(ClipboardManager::FavoritesPage^ page, Windows::Storage::StorageFile^ file);

	~FavoritesPageShowImageClass();

	bool operator()(const concurrency::task<Windows::Storage::Streams::IRandomAccessStream^>& t)const;

};

class FavoritesPageGetHistoryFolderFilesClass
{

private:
	ClipboardManager::FavoritesPage^ fav_page;

public:
	explicit FavoritesPageGetHistoryFolderFilesClass(ClipboardManager::FavoritesPage^ page);

	~FavoritesPageGetHistoryFolderFilesClass();

	Windows::Foundation::IAsyncOperation<Windows::Foundation::Collections::IVectorView<Windows::Storage::StorageFile^>^>^ operator()(const concurrency::task<Windows::Storage::StorageFolder^>& task_folder)const;

};

class FavoritesPageShowHistoryFilesClass
{

private:
	ClipboardManager::FavoritesPage^ fav_page;

public:
	explicit FavoritesPageShowHistoryFilesClass(ClipboardManager::FavoritesPage^ page);

	~FavoritesPageShowHistoryFilesClass();

	concurrency::task<std::vector<bool>> operator()(const concurrency::task<Windows::Foundation::Collections::IVectorView<Windows::Storage::StorageFile^>^>& task_files)const;

};

class FavoritesPageShowHistoryFilesResultClass
{

private:
	ClipboardManager::FavoritesPage^ fav_page;

public:
	explicit FavoritesPageShowHistoryFilesResultClass(ClipboardManager::FavoritesPage^ page);

	~FavoritesPageShowHistoryFilesResultClass();

	void operator()(const concurrency::task<std::vector<bool>>& t)const;

};

class FavoritesPageClearFavoritesClass
{

public:
	concurrency::task<void> operator()(const concurrency::task<Windows::Foundation::Collections::IVectorView<Windows::Storage::StorageFile^>^>& task_files)const;

};

class FavoritesPageClearFavoritesResultsClass
{

private:
	ClipboardManager::FavoritesPage^ fav_page;

public:
	explicit FavoritesPageClearFavoritesResultsClass(ClipboardManager::FavoritesPage^ page);

	~FavoritesPageClearFavoritesResultsClass();

	void operator()(const concurrency::task<void>& t);

};

}