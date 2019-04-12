#include "stdafx.h"

MainRibbon::MainRibbon(QWidget* parent) : QRibbon(parent) {
	// Home
	QRibbonTab* home_tab = new QRibbonTab;

	// Undo/Redo History
	QRibbonSection* history_section = new QRibbonSection;
	history_section->setText(u8"��ʷ");

	undo->setIcon(QIcon("Data/Icons/Ribbon/undo32x32.png"));
	undo->setText(u8"����");
	history_section->addWidget(undo);

	redo->setIcon(QIcon("Data/Icons/Ribbon/redo32x32.png"));
	redo->setText(u8"����");
	history_section->addWidget(redo);

	//view_history->setIcon(QIcon("Data/Icons/Ribbon/description32x32.png"));
	//view_history->setText("View\nHistory");
	//view_history->setEnabled(false);
	//history_section->addWidget(view_history);

	/*copy->setIcon(QIcon("Data/Icons/Ribbon/copy32x32.ico"));
	copy->setText("Copy");
	home_section->addWidget(copy);

	paste->setIcon(QIcon("Data/Icons/Ribbon/paste32x32.ico"));
	paste->setText("Paste");
	home_section->addWidget(paste);

	QVBoxLayout* lay = new QVBoxLayout;
	QToolButton* but = new QToolButton;
	QToolButton* butt = new QToolButton;
	QToolButton* buttt = new QToolButton;

	but->setIcon(QIcon("Data/Icons/Ribbon/paste32x32.ico"));
	but->setIconSize({ 16, 16 });
	but->setText("Cut");
	but->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);*/

	//lay->addWidget(but);
	//lay->addWidget(butt);
	//lay->addWidget(buttt);

	//history_section->addLayout(lay);
	home_tab->addSection(history_section);

	// View
	QRibbonTab* view_tab = new QRibbonTab;

	// Visible section
	QRibbonSection* visible_section = new QRibbonSection;
	visible_section->setText(u8"��ʾ");
	view_tab->addSection(visible_section);
	
	units_visible->setIcon(QIcon("Data/Icons/Ribbon/units32x32.png"));
	units_visible->setText(u8"��λ");
	units_visible->setCheckable(true);
	units_visible->setChecked(true);
	visible_section->addWidget(units_visible);

	doodads_visible->setIcon(QIcon("Data/Icons/Ribbon/doodads32x32.png"));
	doodads_visible->setText(u8"װ����");
	doodads_visible->setCheckable(true);
	doodads_visible->setChecked(true);
	visible_section->addWidget(doodads_visible);

	pathing_visible->setIcon(QIcon("Data/Icons/Ribbon/pathing32x32.png"));
	pathing_visible->setText(u8"ͨ��");
	pathing_visible->setCheckable(true);
	pathing_visible->setChecked(true);
	visible_section->addWidget(pathing_visible);
	
	brush_visible->setIcon(QIcon("Data/Icons/Ribbon/brush32x32.png"));
	brush_visible->setText(u8"ˢ��");
	brush_visible->setCheckable(true);
	brush_visible->setChecked(true);
	visible_section->addWidget(brush_visible);

	lighting_visible->setIcon(QIcon("Data/Icons/Ribbon/lighting32x32.png"));
	lighting_visible->setText(u8"����");
	lighting_visible->setCheckable(true);
	lighting_visible->setChecked(true);
	visible_section->addWidget(lighting_visible);

	wireframe_visible->setIcon(QIcon("Data/Icons/Ribbon/wireframe32x32.png"));
	wireframe_visible->setText(u8"�߿�");
	wireframe_visible->setCheckable(true);
	visible_section->addWidget(wireframe_visible);

	debug_visible->setIcon(QIcon("Data/Icons/Ribbon/debug32x32.png"));
	debug_visible->setText(u8"����");
	debug_visible->setCheckable(true);
	visible_section->addWidget(debug_visible);

	minimap_visible->setIcon(QIcon("Data/Icons/Ribbon/minimap32x32.png"));
	minimap_visible->setText(u8"С��ͼ");
	minimap_visible->setCheckable(true);
	minimap_visible->setChecked(true);
	visible_section->addWidget(minimap_visible);
	// Camera section
	QRibbonSection* camera_section = new QRibbonSection;
	camera_section->setText(u8"��ͷ");
	view_tab->addSection(camera_section);

	switch_camera->setIcon(QIcon("Data/Icons/Ribbon/switch32x32.png"));
	switch_camera->setText(u8"����");
	camera_section->addWidget(switch_camera);

	reset_camera->setIcon(QIcon("Data/Icons/Ribbon/reset32x32.png"));
	reset_camera->setText(u8"����");
	camera_section->addWidget(reset_camera);

	// Menu actions
	new_map->setText(u8"�½���ͼ");
	new_map->setIcon(QIcon("Data/Icons/Ribbon/new32x32.ico"));
	new_map->setIconSize({ 32, 32 });
	new_map->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	new_map->setDisabled(true);
	addMenuItem(new_map);

	open_map->setText(u8"�򿪵�ͼ");
	open_map->setIcon(QIcon("Data/Icons/Ribbon/open32x32.png"));
	open_map->setIconSize({ 32, 32 });
	open_map->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	addMenuItem(open_map);

	save_map->setText(u8"�����ͼ");
	save_map->setIcon(QIcon("Data/Icons/Ribbon/save32x32.png"));
	save_map->setIconSize({ 32, 32 });
	save_map->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	addMenuItem(save_map);

	save_map_as->setText(u8"��ͼ���Ϊ");
	save_map_as->setIcon(QIcon("Data/Icons/Ribbon/saveas32x32.png"));
	save_map_as->setIconSize({ 32, 32 });
	save_map_as->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	addMenuItem(save_map_as);

	test_map->setText(u8"���Ե�ͼ");
	test_map->setIcon(QIcon("Data/Icons/Ribbon/test32x32.ico"));
	test_map->setIconSize({ 32, 32 });
	test_map->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	addMenuItem(test_map);

	settings->setText(u8"����");
	settings->setIcon(QIcon("Data/Icons/Ribbon/options32x32.png"));
	settings->setIconSize({ 32, 32 });
	settings->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	addMenuItem(settings);

	addMenuSeperator();

	exit->setText(u8"�˳�");
	exit->setIcon(QIcon("Data/Icons/Ribbon/exit32x32.ico"));
	exit->setIconSize({ 32, 32 });
	exit->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	addMenuItem(exit);

	// Map tab
	QRibbonTab* map_tab = new QRibbonTab;

	QRibbonSection* map_section = new QRibbonSection;

	map_description->setIcon(QIcon("Data/Icons/Ribbon/description32x32.png"));
	map_description->setText(u8"����˵��");
	map_section->addWidget(map_description);

	map_loading_screen->setIcon(QIcon("Data/Icons/Ribbon/loading32x32.png"));
	map_loading_screen->setText("Loading\nScreen");
	map_section->addWidget(map_loading_screen);

	map_options->setIcon(QIcon("Data/Icons/Ribbon/options32x32.png"));
	map_options->setText(u8"����");
	map_section->addWidget(map_options);

	//map_size_camera_bounds->setIcon(QIcon("Data/Icons/Ribbon/sizebounds32x32.png"));
	//map_size_camera_bounds->setText("Size&&Camera\nBounds");
	//map_section->addWidget(map_size_camera_bounds);

	map_tab->addSection(map_section);

	// Tools tab
	QRibbonTab* tools_tab = new QRibbonTab;

	// Import
	QRibbonSection* import_section = new QRibbonSection;
	import_section->setText(u8"�����ļ�");

	import_heightmap->setIcon(QIcon("Data/Icons/Ribbon/heightmap32x32.png"));
	import_heightmap->setText(u8"����\nHeightmap");
	import_section->addWidget(import_heightmap);

	tools_tab->addSection(import_section);

	// Tileset
	QRibbonSection* tileset_section = new QRibbonSection;
	tileset_section->setText(u8"����");

	change_tileset->setIcon(QIcon("Data/Icons/Ribbon/tileset32x32.png"));
	change_tileset->setText(u8"����\n����");
	tileset_section->addWidget(change_tileset);

	change_tile_pathing->setIcon(QIcon("Data/Icons/Ribbon/tileset32x32.png"));
	change_tile_pathing->setText(u8"����\n��������");
	tileset_section->addWidget(change_tile_pathing);

	tools_tab->addSection(tileset_section);

	QRibbonSection* game_section = new QRibbonSection;
	game_section->setText(u8"��Ϸ");

	switch_warcraft->setIcon(QIcon("Data/Icons/Ribbon/WarIII32x32.ico"));
	switch_warcraft->setText(u8"����\n ��ϷĿ¼");
	game_section->addWidget(switch_warcraft);

	tools_tab->addSection(game_section);	

	// Window Tab
	QRibbonTab* window_tab = new QRibbonTab;

	QRibbonSection* editor_section = new QRibbonSection;
	editor_section->setText(u8"�༭��/��ͼ");

	import_manager->setIcon(QIcon("Data/Icons/Ribbon/importmanager32x32.png"));
	import_manager->setText(u8"��Դ������");
	editor_section->addWidget(import_manager);

	trigger_editor->setIcon(QIcon("Data/Icons/Ribbon/triggereditor32x32.png"));
	trigger_editor->setText(u8"�����༭��");
	editor_section->addWidget(trigger_editor);

	QRibbonSection* palette_section = new QRibbonSection;
	palette_section->setText(u8"���"); 
	
	terrain_palette->setIcon(QIcon("Data/Icons/Ribbon/heightmap32x32.png"));
	terrain_palette->setText(u8"����");
	palette_section->addWidget(terrain_palette);

	doodad_palette->setIcon(QIcon("Data/Icons/Ribbon/doodads32x32.png"));
	doodad_palette->setText(u8"װ����");
	palette_section->addWidget(doodad_palette);

	pathing_palette->setIcon(QIcon("Data/Icons/Ribbon/pathing32x32.png"));
	pathing_palette->setText(u8"ͨ��");
	palette_section->addWidget(pathing_palette);

	window_tab->addSection(editor_section);
	window_tab->addSection(palette_section);


	addTab(home_tab, u8"��ҳ");
	addTab(view_tab, u8"�鿴");
	addTab(map_tab, u8"��ͼ");
	addTab(tools_tab, u8"����");
	addTab(window_tab, u8"����");

}

MainRibbon::~MainRibbon() {
}