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
		std::cerr << e.what();
	}

	return pathToDirectory; // Возвращаем путь к выбранной папке
}

int main() {
	system("cls"); // Очистка консоли

	int count = 0;
	// Путь к директории
	fs::path directoryPath;
	// Файлы в указанной директории, выбранной пользователем, для сортировки
	std::vector<std::wstring> filesInFolder;
	// Папки, которые будут созданы для сортировки данных
	std::vector<std::wstring> creatableFolders{
		L"documents",
		L"multimedia",
		L"archives",
		L"executables",
		L"torrents",
		L"other"
	};

	// Расширения, по которым будет проводиться сортировка
	std::vector<std::vector<std::wstring>> filesExtensions{
		// Документы
		{ L".doc", L".docx", L".pdf", L".ppt", L".pptx",
		L".xls", L".xlsx", L".txt", L".rtf", L".csv",
		L".odt", L".ods", L".odp", L".odg", L".odf",
		L".html", L".htm", L".xml" },

		// Мультимедиа
		{ L".mp3", L".wav", L".ogg", L".flac", L".aac",
		L".wma", L".m4a", L".ape", L".alac", L".mp4",
		L".avi", L".mkv", L".mov", L".wmv", L".flv",
		L".webm", L".m4v", L".mpeg", L".mpg", L".3gp",
		L".rm", L".swf", L".vob", L".bmp", L".gif",
		L".jpg", L".jpeg", L".png", L".tiff", L".webp",
		L".svg", L".ico", L".tif", L".jfif", L".jpe",
		L".jif", L".jfi" },

		// Архивы
		{ L".zip", L".rar", L".7z", L".tar", L".gz",
		L".bz2", L".xz", L".z", L".iso", L".img",
		L".dmg", L".tgz", L".tbz2", L".tbz", L".txz",
		L".tz", L".tlz", L".wim", L".swm", L".lz", L".jar"},

		// Исполняемые файлы и приложения
		{ L".exe", L".dll", L".msi", L".bat", L".com", L".cmd" },

		// Торрент-файлы
		{ L".torrent" },
	};

	// Получаем путь к директории от пользователя
	std::cout << "# Select directory\x1b[90m. . . \x1b[0m\n";
	directoryPath = OpenFileDialog();
	std::cout << "  \x1b[90mThe carved path: \x1b[93m" << directoryPath.string() << "\x1b[0m\n\n";

	// Собираем все файлы в выбранной директории
	for (auto& file : fs::directory_iterator(directoryPath)) {
		if (!fs::is_directory(file)) {
			filesInFolder.push_back(file.path().wstring());
		}
		else {
			count += 1;
		}
	}

	// Проверка на пустоту директории
	if (filesInFolder.empty() || filesInFolder.size() == count) {
		std::cout << "# \x1b[91mThe directory is empty\n\x1b[0mPress \x1b[93mANY KEY\x1b[0m to continue, or \x1b[93mESCAPE\x1b[0m to exit";
		switch (_getch()) {
		case 27: exit(0);
		default: main();
		}
	}
	else {
		// Проверка и создание папок для сортировки
		for (const auto& folder : creatableFolders) {
			bool folderFind = false;
			for (const auto& file : filesInFolder) {
				if (folder == fs::path(file).filename()) {
					folderFind = true;
					break;
				}
			}
			try {
				if (!folderFind) {
					fs::create_directories(directoryPath / folder);
					std::wcout << L"\x1b[92mcreate folder\x1b[90m -- \x1b[93m" << folder << "\x1b[0m\n";
				}
			}
			catch (const std::exception& e) {
				std::cerr << e.what();
			}
		}

		std::cout << "\n\x1b[90m################################\x1b[0m\n\n";

		// Сортировка файлов в соответствующие папки
		try {
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
								std::wcout << L"\x1b[90m~ file\x1b[93m "
									<< filePath.filename().wstring()
									<< L" \x1b[90mmoved to\x1b[93m "
									<< creatableFolders[i] << "\x1b[0m\n";
							}
							catch (const std::exception& e) {
								std::cerr << e.what();
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
			std::cout << "\n\x1b[90m################################\x1b[0m\n";
			std::cout << "\n#\x1b[92m Sorting completed successfully\x1b[0m\n";
		}
		catch (const std::exception& e) {
			std::cout << "\n#\x1b[91m Error in sorting process\x1b[0m\n";
			std::cerr << e.what();
		}

	}

	return 0;
}
