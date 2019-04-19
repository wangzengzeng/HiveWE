#pragma once

class Hierarchy {
public:
	//char tileset = 'L';
	mpq::MPQ map;
	mpq::MPQ tileset;
	mpq::MPQ war3Patch;
	mpq::MPQ war3xLocal;
	mpq::MPQ war3x;
	mpq::MPQ war3Local;
	mpq::MPQ war3;
	mpq::MPQ deprecated;


	casc::CASC game_data;
	json::JSON aliases;

	fs::path warcraft_directory;

	void init();
	void load_tileset(char tileset_code);

	BinaryReader open_file(const fs::path& path) const;
	bool file_exists(const fs::path& path) const;
};
 
extern Hierarchy hierarchy;