namespace vars
{
	namespace global
	{
		extern bool enable;
	}
	namespace visuals
	{
		extern bool enable;
		extern int player_type;
		extern int box_type;
		extern bool name;
		extern bool health;
		extern bool radar_3d;
		extern bool offscreen;
		extern float max_distance_offscreen;
		extern bool distance;
		extern float col_enemy_vis[4];
		extern float col_teammate_vis[4];
	}
	namespace font
	{
		extern int style;
		extern float size;
	}
	namespace extra
	{
		extern bool debug;
	}
	void load_default_settings();
}
