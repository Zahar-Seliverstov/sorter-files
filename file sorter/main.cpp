#include <iostream>
#include <filesystem>
#include <string>
#include <windows.h>
#include <shlobj.h>
#include <vector>
#include <conio.h>

namespace fs = std::filesystem;

// Функция для открытия диалогового окна выбора папки
std::wstring OpenFileDialog() {
	std::wstring pathToDirectory;
	BROWSEINFO bi = {};
	bi.lpszTitle = L"Select a folder"; // Заголовок окна выбора папки
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE; // Флаги для определения поведения окна
	try {
		// Открытие диалогового окна выбора папки
		LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
		if (pidl != nullptr) {
			wchar_t path[MAX_PATH];
			// Получение пути выбранной папки
			if (SHGetPathFromIDList(pidl, path)) {
				pathToDirectory = path;
			}
			// Освобождение памяти, занятой pidl
			CoTaskMemFree(pidl);
		}
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return pathToDirectory; // Возвращаем путь к выбранной папке
}

int main() {
	system("chcp 1251");
	system("cls");

	int count = 0;
	// Путь к директории
	fs::path directoryPath;
	// Файлы в указанной директории, выбранной пользователем, для сортировки
	std::vector<std::wstring> filesInFolder;
	// Папки, которые будут созданы для сортировки данных
	std::vector<std::wstring> creatableFolders{
	   L"video",
	   L"audio",
	   L"image",
	   L"text",
	   L"code_data",
	   L"archive",
	   L"installer",
	   L"other"
	};

	// Расширения, по которым будет проводиться сортировка
	std::vector<std::vector<std::wstring>> filesExtensions{
		// Видео
		{
			L".mp4", L".mov", L".avi", L".mkv", L".wmv", L".3gp", L".3g2",
			L".mpg", L".mpeg", L".m4v", L".h264", L".flv", L".rm", L".swf", L".vob",
			L".mpe", L".mpv", L".mp2", L".m2v", L".ogv", L".qt", L".asf", L".avchd"
		},

		// Аудио
		{
			L".mp3", L".wav", L".ogg", L".flac", L".aif", L".mid", L".midi",
			L".mpa", L".wma", L".wpl", L".cda", L".aac", L".m4a", L".m3u",
			L".pls", L".wv", L".aiff", L".ra", L".ram", L".mka", L".opus"
		},

		// Изображения
		{
			L".jpg", L".png", L".bmp", L".ai", L".psd", L".ico", L".jpeg",
			L".ps", L".svg", L".tif", L".tiff", L".gif", L".eps",
			L".raw", L".cr2", L".nef", L".orf", L".sr2", L".webp", L".heic",
			L".dng", L".arw", L".rw2", L".rwl", L".raf"
		},

		// Текстовые документы
		{
			L".pdf", L".txt", L".doc", L".docx", L".rtf", L".tex", L".wpd",
			L".odt", L".md", L".wps", L".dot", L".dotx", L".pptx", L".ppt",
			L".pps", L".key", L".odp", L".ppsx", L".potx", L".pot", L".pptm", L".ppsm"
		},

		// Код и данные
		{
			L".py", L".js", L".html", L".json", L".java", L".cpp", L".css",
			L".php", L".cs", L".rb", L".pl", L".sh", L".swift", L".vb", L".r", L".go",
			L".sql", L".sqlite", L".sqlite3", L".csv", L".dat", L".db", L".log",
			L".mdb", L".sav", L".tar", L".xml", L".xlsx", L".xls", L".xlsm", L".ods",
			L".json", L".dbf", L".tab", L".tsv", L".parquet", L".h5", L".hdf5",
			L".feather", L".dta", L".sav", L".zsav", L".por"
		},

		// Архивы
		{
			L".zip", L".rar", L".7z", L".z", L".gz", L".rpm", L".arj", L".pkg",L".jar",
			L".deb", L".tar.gz", L".tar.bz2", L".iso", L".img", L".dmg", L".xz",
			L".tgz", L".tbz2", L".txz", L".lz", L".tlz", L".s7z", L".cab", L".lha"
		},

		// Установочные файлы
		{
			L".torrent", L".msi", L".exe", L".deb", L".rpm", L".dmg", L".pkg", L".bin", L".apk", L".ipa"
		},
	};

	// Получаем путь к директории от пользователя
	std::cout << "# \x1b[97mSelect directory\x1b[90m. . . \x1b[0m\n";
	directoryPath = OpenFileDialog();
	std::cout << "\n~ \x1b[92mDirectory selected\n  \x1b[90mcurrent path: \x1b[93m" << directoryPath.string() << "\x1b[0m\n\n";

	// Собираем все файлы в выбранной директории
	for (auto& file : fs::directory_iterator(directoryPath)) {
		if (!fs::is_directory(file)) {
			filesInFolder.push_back(file.path().wstring());
		}
		else { count += 1; }
	}

	// Проверка на пустоту директории
	if (filesInFolder.empty() || filesInFolder.size() == count) {
		std::cout << "# \x1b[91mThe directory is empty\n\n\x1b[0mPress \x1b[93mANY KEY\x1b[0m to continue, or \x1b[93mESCAPE\x1b[0m to exit";
		switch (_getch()) {
		case 27: exit(0);
		default: main();
		}
	}
	else {
		for (int i = 0; i < filesExtensions.size(); i++) {
			bool createDerictoryFlag = false;
			for (int j = 0; j < filesExtensions[i].size(); j++) {
				auto extention = filesExtensions[i][j];
				for (const auto& file : filesInFolder) {
					if (fs::path(file).extension() == extention) {
						bool directoryFind = false;
						for (auto& folder : fs::directory_iterator(directoryPath)) {
							if (fs::path(folder).filename() == creatableFolders[i]) {
								directoryFind = true;
								break;
							}
						}
						if (!directoryFind) {
							fs::create_directories(directoryPath / creatableFolders[i]);
							std::wcout << L"\x1b[92mcreate folder\x1b[90m -- \x1b[93m" << creatableFolders[i] << "\x1b[0m\n";
						}
						createDerictoryFlag = true;
						break;
					}
				}
				if (createDerictoryFlag) {
					break;
				}
			}
		}

		// Сортировка файлов в соответствующие папки
		try {
			std::cout << "\n\x1b[90m_____________ \x1b[97mDistribution List\x1b[90m ____________\x1b[0m\n\n";
			for (const auto& file : filesInFolder) {
				fs::path filePath(file);
				std::wstring extension = filePath.extension().wstring();
				bool fileExtensionFind = false;
				for (int i = 0; i < filesExtensions.size(); i++) {
					for (int j = 0; j < filesExtensions[i].size(); j++) {

						if (extension == filesExtensions[i][j]) {
							fileExtensionFind = true;
							try {
								fs::rename(filePath, directoryPath / creatableFolders[i] / filePath.filename());
								std::wcout << L"\x1b[90m ~ file\x1b[93m "
									<< filePath.filename().wstring()
									<< L" \x1b[90mmoved to\x1b[93m "
									<< creatableFolders[i] << "\x1b[0m\n";
							}
							catch (const std::exception& e) {
								system("cls");
								std::cout << "#\x1b[91m Error in renaming files\x1b[0m\n";
								std::cerr << e.what() << std::endl;
							}
							break;
						}
					}
					if (fileExtensionFind) break;
				}
				// Если расширение файла не было найдено, переместить файл в папку "other"
				if (!fileExtensionFind && !fs::is_directory(file)) {
					fs::rename(filePath, directoryPath / creatableFolders.back() / filePath.filename());
				}
			}
			std::cout << "\n\x1b[90m____________________________________________\x1b[0m\n";
			std::cout << "\n#\x1b[92m Sorting completed successfully\x1b[0m\n";
		}
		catch (const std::exception& e) {
			system("cls");
			std::cout << "#\x1b[91m Error in sorting process\x1b[0m\n";
			std::cerr << e.what() << std::endl;
		}
	}
	return 0;
}
