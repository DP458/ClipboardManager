// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "pch.h"
#include "FavoritesPage.xaml.History.h"

using namespace ClipboardManager;
using namespace ClipboardManager::CommonTools;

using namespace Platform;
using namespace concurrency;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Documents;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::Search;
using namespace Windows::Storage::Streams;
using namespace Windows::ApplicationModel::Resources;

ClipboardManager::FavoritesPageShowTextClass::FavoritesPageShowTextClass(FavoritesPage^ page, StorageFile^ file, ClipboardDataType type) : fav_page(page), storage_file(file), data_type(type)
{

}

ClipboardManager::FavoritesPageShowTextClass::~FavoritesPageShowTextClass()
{
fav_page = nullptr;
storage_file = nullptr;
}

bool ClipboardManager::FavoritesPageShowTextClass::operator()(const task<String^>& task_text)const
{

String^ text;

try
{
	text = task_text.get();
}
catch (Exception^)
{
	return false;
}

switch (data_type)
{

case ClipboardDataType::Text:
fav_page->SetText(text, storage_file);
return true;

case ClipboardDataType::Link:
fav_page->SetLink(ref new Uri(text), storage_file);
return true;

default:
return false;

}

}

ClipboardManager::FavoritesPageShowImageClass::FavoritesPageShowImageClass(FavoritesPage^ page, StorageFile^ file) : fav_page(page), storage_file(file)
{

}

ClipboardManager::FavoritesPageShowImageClass::~FavoritesPageShowImageClass()
{
fav_page = nullptr;
storage_file = nullptr;
}

bool ClipboardManager::FavoritesPageShowImageClass::operator()(const task<IRandomAccessStream^>& t)const
{

IRandomAccessStream^ fileStream;

try
{
	fileStream = t.get();
}
catch (Exception^)
{
	return false;
}

if (fileStream->Size == 0ULL)
	return false;

fav_page->SetImage(fileStream, storage_file);

return true;

}

ClipboardManager::FavoritesPageGetHistoryFolderFilesClass::FavoritesPageGetHistoryFolderFilesClass(FavoritesPage^ page) : fav_page(page)
{

}

ClipboardManager::FavoritesPageGetHistoryFolderFilesClass::~FavoritesPageGetHistoryFolderFilesClass()
{
fav_page = nullptr;
}

IAsyncOperation<IVectorView<StorageFile^>^>^ ClipboardManager::FavoritesPageGetHistoryFolderFilesClass::operator()(const task<StorageFolder^>& task_folder)const
{

try
{
	fav_page->HistoryFolder = task_folder.get();
}
catch (Exception^)
{
	return nullptr;
}

return fav_page->HistoryFolder->GetFilesAsync(CommonFileQuery::DefaultQuery);

}

ClipboardManager::FavoritesPageShowHistoryFilesClass::FavoritesPageShowHistoryFilesClass(FavoritesPage^ page) : fav_page(page)
{

}

ClipboardManager::FavoritesPageShowHistoryFilesClass::~FavoritesPageShowHistoryFilesClass()
{
fav_page = nullptr;
}

task<std::vector<bool>> ClipboardManager::FavoritesPageShowHistoryFilesClass::operator()(const task<IVectorView<StorageFile^>^>& task_files)const
{

IVectorView<StorageFile^>^ files;

try
{
	files = task_files.get();
}
catch (Exception^)
{
	std::vector<bool> vector;
	vector.push_back(false);
	return task_from_result(vector);
}

std::vector<task<bool>> taskBool;

{

StorageFile^ file;
String^ type;

for (IIterator<StorageFile^>^ i = files->First(); i->HasCurrent; i->MoveNext())
{
	file = i->Current;
	type = file->FileType;

	if (type == ".txt")
	{
		taskBool.push_back
		(
			create_task(FileIO::ReadTextAsync(file)).then(FavoritesPageShowTextClass(fav_page, file, ClipboardDataType::Text))
		);
		continue;
	}

	if (type == ".tlf")
	{
		taskBool.push_back
		(
			create_task(FileIO::ReadTextAsync(file)).then(FavoritesPageShowTextClass(fav_page, file, ClipboardDataType::Link))
		);
		continue;
	}

	if (type == ".bmp")
	{
		taskBool.push_back
		(
			create_task(file->OpenAsync(FileAccessMode::Read)).then(FavoritesPageShowImageClass(fav_page, file))
		);
		continue;
	}

	taskBool.push_back
	(
		task_from_result(false)
	);

}

}

return when_all(taskBool.begin(), taskBool.end());

}

ClipboardManager::FavoritesPageShowHistoryFilesResultClass::FavoritesPageShowHistoryFilesResultClass(FavoritesPage^ page) : fav_page(page)
{

}

ClipboardManager::FavoritesPageShowHistoryFilesResultClass::~FavoritesPageShowHistoryFilesResultClass()
{
fav_page = nullptr;
}

void ClipboardManager::FavoritesPageShowHistoryFilesResultClass::operator()(const task<std::vector<bool>>& t)const
{

std::vector<bool> vB;

try
{
	vB = t.get();
}
catch (Exception^)
{
	fav_page->WaiterState = false;
	ShowError("Failed to get cached files from local storage", "", fav_page->Properties);
	GoBack();
	return;
}

fav_page->WaiterState = false;

}

task<void> ClipboardManager::FavoritesPageClearFavoritesClass::operator()(const task<IVectorView<StorageFile^>^>& task_files)const
{

IVectorView<StorageFile^>^ files;

try
{
	files = task_files.get();
}
catch (Exception^)
{
	return task<void>();
}

std::vector<task<void>> taskOperation;

for (IIterator<StorageFile^>^ i = files->First(); i->HasCurrent; i->MoveNext())
	taskOperation.push_back
	(
		create_task(i->Current->DeleteAsync(StorageDeleteOption::Default))
	);

return when_all(taskOperation.begin(), taskOperation.end());

}

ClipboardManager::FavoritesPageClearFavoritesResultsClass::FavoritesPageClearFavoritesResultsClass(FavoritesPage^ page) : fav_page(page)
{

}

ClipboardManager::FavoritesPageClearFavoritesResultsClass::~FavoritesPageClearFavoritesResultsClass()
{
fav_page = nullptr;
}

void ClipboardManager::FavoritesPageClearFavoritesResultsClass::operator()(const task<void>& t)
{

fav_page->SetEmptyPageStub();

try
{
	t.get();
}
catch (Exception^)
{
	ShowError("Failed to clear the your favorites", "Please try it again", fav_page->Properties);
	GoBack();
	return;
}

if (static_cast<bool>(fav_page->Properties->Lookup("ShowSuccessClearFavoritesNotify")))
	UserNotify(fav_page->Res->GetString("NotifyClearFavorites"), "", NotificationGroup::SaveData);

if (static_cast<bool>(this->fav_page->Properties->Lookup("ExitFavoritesPage")))
	GoBack();

}