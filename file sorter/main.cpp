#include <iostream>
#include <filesystem>
#include <string>
#include <windows.h>
#include <shlobj.h>
#include <vector>

namespace fs = std::filesystem;

std::wstring OpenFileDialog() {
	std::wstring pathToDirectory;

	BROWSEINFO bi = {};
	bi.lpszTitle = L"Select a folder";
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl != nullptr) {
		wchar_t path[MAX_PATH];
		if (SHGetPathFromIDList(pidl, path)) {
			pathToDirectory = path;
		}
		CoTaskMemFree(pidl);
	}
	return pathToDirectory;
}

int main() {
	std::wstring pathToDirectory;
	std::vector<std::wstring> filesInFolder;
	std::vector<std::wstring> creatableFolders{ L"Image", L"Other" };
	std::vector<std::vector<std::wstring>> allExtensions{
		{ L"png", L"jpg" }
	};

	pathToDirectory = OpenFileDialog();
	fs::path directoryPath = pathToDirectory;

	try {
		for (const auto& folder : creatableFolders) {
			fs::create_directory(directoryPath / folder);
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Filesystem error: " << e.what() << '\n';
	}

	for (auto& file : fs::directory_iterator(directoryPath)) {
		filesInFolder.push_back(file.path().wstring());
	}

	if (filesInFolder.empty()) {
		std::cout << "The directory is empty\n";
		return 0;
	}
	else {
		for (const auto& folder : creatableFolders) {
			bool folderFind = false;
			for (const auto& file : filesInFolder) {
				if (folder == fs::path(file).filename()) {
					folderFind = true;
					break;
				}
			}
			if (!folderFind) {
				fs::create_directories(directoryPath / folder);
			}
		}
		for (const auto& filePath : filesInFolder) {
			fs::path file(filePath);
			std::wstring extension = file.extension().wstring();


		}

		return 0;
	}
