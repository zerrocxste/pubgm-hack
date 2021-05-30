#include "includes.h"

//entity sig: 78 72 00 00 00 02 00 00 14 22 00 00 00 00 00 00 00 00 00 00 00 00 00 00
//\x78\x72\x00\x00\x00\x02\x00\x00\x14\x22\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00 xxxxxxxxxxxxxxxxxxxxxxxxxxx

//viewmatrix sig: 18 D5 31 3F E2 ? D3 3E 06 D9 16 3F 00 00 00 00 B0 ? 38 3F 41 00 ? BE ? ? ? ?
//\x18\xD5\x31\x3F\xE2\xFF\xD3\x3E\x06\xD9\x16\x3F\x00\x00\x00\x00\xB0\xFF\x38\x3F\x41\x00\xFF\xBE\xFF\xFF\xFF\xFF xxxxx?xxxxxxxxxxx?xxxx?x????

//"CD ? CC 3D ? ? ? ? 00 00 18 41" -= 0x74 (recoil), result -= 0x54 == 0.65
//"\xCD\x00\xCC\x3D\x00\x00\x00\x00\x00\x00\x18\x41", "x?xx????xxxx"

//01 00 00 00 ? ? ? ? 01 00 00 00 CD CC 4C 3D
//"\x01\x00\x00\x00\xFF\xFF\xFF\xFF\x01\x00\x00\x00\xCD\xCC\x4C\x3D
//xxxx????xxxxxxxx

//21 0C 04 0A 00 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 FF FF FF FF 00
//\x21\x0C\x04\x0A\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xFF\xFF\xFF\x00
//xxxxxxxxxxxxxxxxxxxxxxxxx

//01 00 00 00 ? ? ? ? 68 A9 A7 A5 00 00 00 00

//01 00 00 00 ? ? ? ? 68 A9 A7 A5 00 00 00 00

//00 00 00 00 3D F4 E4 C0 7E C5 0F 40 7E C5 0F 40 01 00 00 00 00 00 00 00 01 00 00 00 CD CC 4C 3D 00 00 00

//00 00 00 00 3D F4 E4 C0 7E C5 0F 40 7E C5 0F 40 01 00 00 00 A0 AC AA 3D 01 00 00 00 CD CC 4C 3D 00 00 00

//00 00 00 00 ? ? ? ? ? ? ? ? ? ? ? ? 01 00 00 00 ? ? ? ? ? ? ? ? ? ? ? ? ? 00 00

namespace console
{
	DWORD console_error = NULL;

	FILE* output_stream = nullptr;

	void attach(const char* name)
	{
		if (AllocConsole())
		{
			freopen_s(&output_stream, "conout$", "w", stdout);
			SetConsoleTitle(name);
		}
		else
		{
			console_error = GetLastError();
		}
	}

	void detach()
	{
		if (output_stream)
		{
			fclose(output_stream);
		}
		FreeConsole();
	}
}

struct search_info_s
{
	DWORD_OF_BITNESS start;
	DWORD_OF_BITNESS end;
};

void collect_info_thread(search_info_s info)
{
retry:

	std::cout << "start find matrix\n";

	auto offset_array = memory_utils::find_pattern_in_heap_array(
		"\x00\x00\x80\x3F\x00\x00\x80\x3F\x00\x00\x80\x3F\x00\x00\x80\x3F\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80\x3F\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x80\x3F\x00\x00\x80\x3F", 
		"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", 0x8);

	if (offset_array.empty())
	{
		std::cout << "matrix not found, retry...\n";
		goto retry;
	}

	for (auto address : offset_array)
	{
		auto matrix_first_value = *(float*)(address + 0xD0);
		if ((matrix_first_value < 3.f || matrix_first_value > -3.f) 
			&& (*(int*)(address + 0xD4) != NULL)
			&& (*(int*)(address + 0xDC) != NULL))
		{
			features::game_data::matrix_offset = address + 0xD0;
		}
	}

	if (features::game_data::matrix_offset == NULL)
	{
		std::cout << "something wrong, retry...\n";
		goto retry;
	}

	std::cout << "matrix found 0x" << std::hex << features::game_data::matrix_offset << std::endl;

	while (true)
	{
		auto result = memory_utils::find_pattern_in_memory_array(
			"\x78\x72\x00\x00\x00\x02\x00\x00\x14\x22\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxxxxx", 
			info.start, info.end, 0x8);

		features::game_data::founded_player_entity.clear();
		features::game_data::founded_player_entity = result;

		auto matrix_first_value = *(float*)features::game_data::matrix_offset;
		if ((matrix_first_value > 3.f || matrix_first_value < -3.f) 
			&& (*(int*)(features::game_data::matrix_offset + 0x4) == NULL) 
			&& (*(int*)(features::game_data::matrix_offset + 0xC) == NULL))
		{
			std::cout << "matrix offset is not valid, retry!\n";
			goto retry;
		}
	}
}

bool compare(std::vector<DWORD_OF_BITNESS>array, DWORD_OF_BITNESS element)
{
	return std::find(array.begin(), array.end(), element) != array.end();
}

void collect_weapon_entity_thread(search_info_s info)
{
	bool is_active = false;

	while (true)
	{
		if (GetAsyncKeyState(VK_DELETE))
			is_active = true;

		if (is_active)
		{
			std::cout << "start find recoil offsets\n";
			auto result = memory_utils::find_pattern_in_memory_array("\x01\x00\x00\x00\xFF\xFF\xFF\xFF\x01\x00\x00\x00\xCD\xCC\x4C\x3D", "xxxx????xxxxxxxx", info.start, info.end);
			features::game_data::weapon_entity_struct.clear();
			for (const auto& address : result)
			{
				features::game_data::weapon_entity_struct.push_back((DWORD_OF_BITNESS)(address - 0x60));
				std::cout << "founded weapon from scan 1 entity 0x" << std::hex << address << std::endl;
			}
			auto result2 = memory_utils::find_pattern_in_memory_array("\xCD\x00\xCC\x3D\x00\x00\x00\x00\x00\x00\x18\x41", "x?xx????xxxx", info.start, info.end);
			for (const auto& address : result2)
			{
				if (!compare(result, (DWORD_OF_BITNESS)(address - 0x14)))
				{
					features::game_data::weapon_entity_struct.push_back((DWORD_OF_BITNESS)(address - 0x74));
					std::cout << "founded weapon from scan 2 entity 0x" << std::hex << address << std::endl;
				}
			}
			auto result3 = memory_utils::find_pattern_in_memory_array("\x21\x0C\x04\x0A\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xFF\xFF\xFF\x00", "xxxxxxxxxxxxxxxxxxxxxxxxx", info.start, info.end);
			for (const auto& address : result3)
			{
				if (!compare(result, (DWORD_OF_BITNESS)(address + 0xB4) && !compare(result2, (DWORD_OF_BITNESS)(address + 0xB4))))
				{
					features::game_data::weapon_entity_struct.push_back((DWORD_OF_BITNESS)(address + 0x54));
					std::cout << "founded weapon from scan 3 entity 0x" << std::hex << address << std::endl;
				}
			}
			std::cout << "end find recoil offsets\n";
			is_active = false;
		}
		Sleep(1);
	}
}

void overlay_thread()
{
	if (overlay::initialize() == FALSE)
	{
		std::cout << "overlay initialize error\n";
		FreeLibraryAndExitThread(global_vars::dll_module, 1);
	}

	SYSTEM_INFO info{ NULL };
	GetSystemInfo(&info);

	auto proc_heap = GetProcessHeap();
	DWORD_OF_BITNESS start = (DWORD_OF_BITNESS)proc_heap;
	DWORD_OF_BITNESS end = (DWORD_OF_BITNESS)info.lpMaximumApplicationAddress;

	search_info_s search_info;
	search_info.start = start;
	search_info.end = end;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)collect_info_thread, &search_info, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)collect_weapon_entity_thread, &search_info, NULL, NULL);

	overlay::render_loop();

	overlay::terminate();

	FreeLibraryAndExitThread(global_vars::dll_module, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{

	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		global_vars::dll_module = hModule;

		console::detach();
		console::attach("testin");

		vars::load_default_settings();

		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)overlay_thread, NULL, NULL, NULL);
	}
    return TRUE;
}

