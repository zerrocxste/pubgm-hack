#include "../includes.h"

namespace vars
{
	bool bMenuOpen = false;
	namespace global
	{
		bool enable;
	}
	namespace visuals
	{
		int player_type = 0;
		int box_type = 0;
		bool name = false;
		bool health = false;
		bool radar_3d = false;
		bool offscreen = false;
		float max_distance_offscreen = 0;
		bool distance = false;
		float col_enemy_vis[4];
		float col_teammate_vis[4];
	}
	namespace font
	{
		int style = 0;
		float size = 0;
	}
	namespace extra
	{
		bool debug = false;
	}
	void load_default_settings() {
		global::enable = true;

		visuals::player_type = 0;
		visuals::box_type = 4;
		visuals::name = true;
		visuals::health = true;
		visuals::radar_3d = true;
		visuals::offscreen = true;
		visuals::max_distance_offscreen = 100.f;
		visuals::distance = true;

		visuals::col_enemy_vis[0] = 1.f;
		visuals::col_enemy_vis[1] = 1.f;
		visuals::col_enemy_vis[2] = 0.f;
		visuals::col_enemy_vis[3] = 1.f;

		visuals::col_teammate_vis[0] = 0.f;
		visuals::col_teammate_vis[1] = 1.f;
		visuals::col_teammate_vis[2] = 1.f;
		visuals::col_teammate_vis[3] = 1.f;

		font::style = 1;

		extra::debug = true;
	}
}