#include "../includes.h"

namespace features
{
	
	
	namespace game_data
	{
		std::vector<DWORD_OF_BITNESS>weapon_entity_struct;
		std::vector<DWORD_OF_BITNESS>founded_player_entity;
		DWORD_OF_BITNESS matrix_offset;
	}

	void esp()
	{
		auto matrix = *(Matrix4x4*)(features::game_data::matrix_offset);

		for (const auto& entity : game_data::founded_player_entity)
		{
			if (IsBadCodePtr((FARPROC)entity) != NULL)
				continue;
		
			bool its_me = memory_utils::read<int>({ entity, 0xA3 }) == 364612;

			if (its_me)
				continue;

			Vector origin = memory_utils::read<Vector>({ entity, (DWORD_OF_BITNESS)-0x88 });

			if (origin.IsAnyZero())
				continue;

			Vector min = Vector(origin.x, origin.y, origin.z - 85.f);
			Vector max = Vector(origin.x, origin.y, origin.z + 85.f);

			BYTE pos_state = memory_utils::read<BYTE>({ entity, (DWORD_OF_BITNESS)-0xC2 });

			std::string pos_type("stand");

			if (pos_state == 0x70)
			{
				max.z -= 40.f;
				pos_type = "sit";
			}
			else if (pos_state == 0x48)
			{
				max.z -= 80.f;
				pos_type = "lying";
			}

			float out_min[2], out_max[2];
			if (utils::world_to_screen(matrix, min, out_min) && utils::world_to_screen(matrix, max, out_max))
			{
				float h = out_min[1] - out_max[1];
				float w = h / 2.f;
				float x = out_min[0] - w / 2.f;
				float y = out_max[1];

				m_pDrawing->DrawEspBox(vars::visuals::box_type, x, y, w, h, vars::visuals::col_enemy_vis[0], vars::visuals::col_enemy_vis[1], vars::visuals::col_enemy_vis[2], vars::visuals::col_enemy_vis[3]);

				m_pDrawing->AddText(x + w / 2.f, y + h + 4.9f, ImColor(1.f, 1.f, 1.f), vars::font::size, FL_CENTER_X, "%s", pos_type.c_str());
			}
		}
	}

	void norecoil()
	{
		if (features::game_data::weapon_entity_struct.empty())
			return;

		for (const auto& recoil_address : features::game_data::weapon_entity_struct)
		{
			if (IsBadCodePtr((FARPROC)recoil_address) == NULL)
			{
				if (*(float*)(recoil_address + 0x38) > 1.f && *(float*)(recoil_address + 0x38) < 20.f)
				{
					*(float*)recoil_address = -99999999999999999999999999999999999999.f;
				}
			}
		}
	}

	void run()
	{
		if (features::game_data::matrix_offset == NULL || game_data::founded_player_entity.empty())
		{
			features::game_data::weapon_entity_struct.clear();
			return;
		}
		
		esp();
		norecoil();
	}
}
