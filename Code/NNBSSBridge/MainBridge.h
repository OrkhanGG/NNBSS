#ifndef MAINBRIDGE_H
#define MAINBRIDGE_H

// NNBSS Global
#include <NNBSSGlobal/TypeDef.h>
// NNBSS AI
#include <NNBSSAI/Management/AISectorManager.h>
// NNBSS Engine
#include <NNBSSUI/Background/Management/ClassManager.h>
// NNBSS Network
#include <NNBSSNetwork/Management/NetworkSectorManager.h>
// NNBSS Render
#include <NNBSSRender/Management/RenderSectorManager.h>
// NNBSS System
#include <NNBSSSystem/Management/SystemSectorManager.h>

#if defined(_WIN32) || defined(_WIN64)
#ifndef OS_WINDOWS
#define OS_WINDOWS  // May display redefinition warning because of WinSDK
// Win32 API
#include "shellapi.h"
#endif // ! OS_WINDOWS
#elif defined(__APPLE__) || defined(__MACH__)
#ifndef OS_OSX
#define OS_OSX
#endif
#elif defined(__linux__)
#ifndef OS_LINUX
#define OS_LINUX
#endif
#endif

/* Get current Coordinated Universal Time*/
#define NNBSS_GET_UTC_TIME_MS std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

inline CNNBSSAIObjectDetection* CNNBSSAIObjectDetectionBaseAddressHandle(wxString _CameraName, bool destroy = false)
{
	auto itr = NNBSSDetectionModel::p_CNNBSSAIObjectDetectionBase.find(_CameraName);
	if (itr != NNBSSDetectionModel::p_CNNBSSAIObjectDetectionBase.end())
	{
		if (destroy)
		{
			CNNBSSAIObjectDetection* p_ToDestroy = itr->second;
			NNBSSDetectionModel::p_CNNBSSAIObjectDetectionBase.erase(itr);
			delete p_ToDestroy;
		}
		return itr->second;
	}
	// if destroy and not found
	if (destroy)
	{
		NNBSSErrorShow("Could not find the CNNBSSAIObjectDetection of given ThreadID: %s. Aborting!", 100, std::string(_CameraName.mb_str()));
		return nullptr;
	}

	NNBSSDetectionModel::p_CNNBSSAIObjectDetectionBase[_CameraName] = new CNNBSSAIObjectDetection();

	return NNBSSDetectionModel::p_CNNBSSAIObjectDetectionBase[_CameraName];
}

// Authorization
inline CNNBSSAuthorization* CNNBSSAuthorizationAddressHandle()
{
	static CNNBSSAuthorization* p_CNNBSSAuthorization = nullptr;

	if (p_CNNBSSAuthorization)
	{
		return p_CNNBSSAuthorization;
	}
	else
	{
		p_CNNBSSAuthorization = new CNNBSSAuthorization();

		return p_CNNBSSAuthorization;
	}

	return nullptr;
}

namespace nnbsstd
{
	/* Always use this to delete a pointer if object does not require a special deletion.
	P.S. wxWidgets' GUI classes unless you're 100% sure that nothing unexpected will happen.*/
	template<typename T>
	void NoDangling(T*& ptr)
	{
		delete ptr;
		ptr = nullptr;
	}

	/* Executes lambda function depending on condition*/
	template<typename F>
	void ConditionalCall(bool condition, F& lambda)
	{
		if (condition)
			lambda();
	}

	/* Executes different lambda functions depending on find results*/
	template<typename I, typename L, typename F, typename NF> 
	void FindOrNot( I& element,  L& list, F& found, NF& notFound)
	{
		if (auto it{ list.find(element) }; it != std::end(list))
		{
			found();
		}
		else
		{
			notFound();
		}
	}

	/* Executes command given command

	Example:
	  // codec of choice
		std::string codec = "H264";

	 // if OS is linux or OSX then use FFmpeg with grep command
	#if defined (OS_LINUX) || defined (OS_OSX)
	std::string command = "ffmpeg -codecs | grep -i ";
	// if WINDOWS then FFmpeg with findstr
	#elif defined (OS_WINDOWS)
	std::string command = "ffmpeg -codecs | findstr /i ";
	#endif

	// execute the system command and get the output
	std::string str = exec(command+codec);

	// if the output string contains the letters DEV then FFmpeg supports the codec
	// D: Decoding supported
	// E: Encoding supported
	// V: Video codec
	std::size_t found = str.find("DEV");

	if(found != std::string::npos)
	std::cout<<"Codec Found"<<std::endl;
	*/
	inline std::string ExecuteCommand(const std::string& command) {
		const char* cmd = command.c_str();
		std::array<char, 128> buffer;
		std::string result;

#if defined(OS_WINDOWS)
		std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
#elif defined(OS_LINUX) || defined (OS_OSX)
		std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
#endif

		if (!pipe) throw std::runtime_error("popen() failed!");
		while (!feof(pipe.get())) {
			if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
				result += buffer.data();
		}
		return result;
	}

	/* Move vector element from x index to y.*/
	template <typename t> void move_vector_index_to(std::vector<t>& v, size_t oldIndex, size_t newIndex)
	{
		if (oldIndex > newIndex)
			std::rotate(v.rend() - oldIndex - 1, v.rend() - oldIndex, v.rend() - newIndex);
		else
			std::rotate(v.begin() + oldIndex, v.begin() + oldIndex + 1, v.begin() + newIndex + 1);
	}

	/* Open File Explorer at a directory*/
	inline void FileExplorerAt(const std::string& directory)
	{
#ifdef OS_WINDOWS
		/*System is not preffered because of antivirus warning ->*/ //::system(std::string("start explorer "+NNBSS_DIR__PLAYBACK_DIR.ToStdString()).c_str());
		std::wstring stemp = std::wstring(directory.begin(), directory.end());
		LPCWSTR sw = stemp.c_str();
		ShellExecuteW(nullptr, L"explore", sw, nullptr, nullptr, SW_SHOWNORMAL);
#elif defined(__APPLE__) || defined(__MACH__)
		// TODO: MAC solution is needed
		NNBSSErrorShow("Somethings went wrong!", 100);
		//[[NSWorkspace sharedWorkspace]selectFile:@"/full/path/to/ =folder" inFileViewerRootedAtPath: @""];
#elif defined(__linux__) && defined(OS_LINUX)
		// TODO: Linux solution is needed
		NNBSSErrorShow("Somethings went wrong!", 100);
#else
		// TODO: Add send report about problem
		NNBSSErrorShow("Somethings went wrong!", 100);
#endif
	}
};

#endif // ! MAINBRIDGE_H