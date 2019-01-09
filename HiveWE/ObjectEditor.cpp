#include "stdafx.h"

bool ObjectEditor::show_as_raw = false;

ObjectEditor::ObjectEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ObjectEditor)
{
    ui->setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	this->installEventFilter(this);
	ui->tabWidget->setCurrentIndex(0);
	
    QFileIconProvider icons;
    folder_icon = icons.icon(QFileIconProvider::Folder);

	load();
	load_properties();
	setup();

	show();
}
void ObjectEditor::closeEvent(QCloseEvent *event) {
	//hide();
	//event->ignore();
}

ObjectEditor::~ObjectEditor()
{
    delete ui;
}

void ObjectEditor::setup(){
    //==========================//
    // Units Tab Initialization //
    //==========================// 
	//map->objects.units.at(0).item->setSelected(true);
	//ui->units->topLevelItem(0)->setExpanded(true);
	ui->tabWidget->setTabIcon(0,QIcon(texture_to_icon("ReplaceableTextures\\WorldEditUI\\ObjectEditor-CustomUnit.blp")));
	ui->unit_search->installEventFilter(this);
    ui->unit_stats_search->installEventFilter(this);
    ui->unit_stats->header()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->unit_stats->header()->setSectionResizeMode(1, QHeaderView::Stretch);
	ui->units->setContextMenuPolicy(Qt::CustomContextMenu);
	//ui->units->layout()->setContentsMargins(0, 0, 0, 0);
	
	connect(ui->unit_search, &QLineEdit::textChanged, [&](const QString &text){  });
	connect(ui->unit_stats_search, &QLineEdit::textChanged, [&](const QString &text) { });

	connect(ui->units, &QTreeWidget::customContextMenuRequested, this, &ObjectEditor::custom_menu_popup);
	connect(ui->units, &QTreeWidget::itemClicked, [&](QTreeWidgetItem* item, int column) { load_widget_data(item); });
	connect(ui->unit_stats, &QTreeWidget::itemDoubleClicked, [&](QTreeWidgetItem* item, int column) { });
    //==========================//
    // Items Tab Initialization //
    //==========================//
	ui->tabWidget->setTabIcon(1,QIcon(texture_to_icon("ReplaceableTextures\\WorldEditUI\\ObjectEditor-CustomItem.blp")));
    ui->item_search->installEventFilter(this);
    ui->item_stats_search->installEventFilter(this);
    ui->item_stats->header()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->item_stats->header()->setSectionResizeMode(1, QHeaderView::Stretch);


	connect(ui->items, &QTreeWidget::itemClicked, [&](QTreeWidgetItem* item, int column) { load_widget_data(item); });
	connect(ui->item_stats, &QTreeWidget::itemDoubleClicked, [&](QTreeWidgetItem* item, int column) {});
	//==================================//
    // Destructibles Tab Initialization //
    //==================================//
	ui->tabWidget->setTabIcon(2,QIcon(texture_to_icon("ReplaceableTextures\\WorldEditUI\\ObjectEditor-CustomDestructible.blp")));
    ui->destructibles_search->installEventFilter(this);
    ui->destructibles_stats_search->installEventFilter(this);
    ui->destructibles_stats->header()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->destructibles_stats->header()->setSectionResizeMode(1, QHeaderView::Stretch);

	connect(ui->destructibles, &QTreeWidget::itemClicked, [&](QTreeWidgetItem* item, int column) { load_widget_data(item); });
	connect(ui->destructibles_stats, &QTreeWidget::itemDoubleClicked, [&](QTreeWidgetItem* item, int column) {});
	//==================================//
	//    Doodads Tab Initialization    //
	//==================================//
	ui->tabWidget->setTabIcon(3,QIcon(texture_to_icon("ReplaceableTextures\\WorldEditUI\\ObjectEditor-CustomDoodad.blp")));
	ui->doodads_search->installEventFilter(this);
	ui->doodads_stats_search->installEventFilter(this);
	ui->doodads_stats->header()->setSectionResizeMode(0, QHeaderView::Fixed);
	ui->doodads_stats->header()->setSectionResizeMode(1, QHeaderView::Stretch);

	connect(ui->doodads, &QTreeWidget::itemDoubleClicked, [&](QTreeWidgetItem* item, int column) { load_widget_data(item); });
	connect(ui->doodads_stats, &QTreeWidget::itemDoubleClicked, [&](QTreeWidgetItem* item, int column) {});
	//==================================//
	//   Abilities Tab Initialization   //
	//==================================//
	ui->tabWidget->setTabIcon(4,QIcon(texture_to_icon("ReplaceableTextures\\WorldEditUI\\ObjectEditor-CustomAbility.blp")));
	ui->abilities_search->installEventFilter(this);
	ui->abilities_stats_search->installEventFilter(this);
	ui->abilities_stats->header()->setSectionResizeMode(0, QHeaderView::Fixed);
	ui->abilities_stats->header()->setSectionResizeMode(1, QHeaderView::Stretch);

	connect(ui->abilities, &QTreeWidget::itemClicked, [&](QTreeWidgetItem* item, int column) { load_widget_data(item); });
	connect(ui->abilities_stats, &QTreeWidget::itemDoubleClicked, [&](QTreeWidgetItem* item, int column) {  });
	//==================================//
	//   Effects Tab Initialization     //
	//==================================//
	ui->tabWidget->setTabIcon(5,QIcon(texture_to_icon("ReplaceableTextures\\WorldEditUI\\ObjectEditor-CustomAbility.blp")));
	ui->effects_search->installEventFilter(this);
	ui->effects_stats_search->installEventFilter(this);
	ui->effects_stats->header()->setSectionResizeMode(0, QHeaderView::Fixed);
	ui->effects_stats->header()->setSectionResizeMode(1, QHeaderView::Stretch);

	connect(ui->effects, &QTreeWidget::itemClicked, [&](QTreeWidgetItem* item, int column) { load_widget_data(item); });
	connect(ui->effects_stats, &QTreeWidget::itemDoubleClicked, [&](QTreeWidgetItem* item, int column) { });
	//==================================//
	//   Upgrades Tab Initialization    //
	//==================================//
	ui->tabWidget->setTabIcon(6,QIcon(texture_to_icon("ReplaceableTextures\\WorldEditUI\\ObjectEditor-CustomUpgrade.blp")));
	ui->upgrades_search->installEventFilter(this);
	ui->upgrades_stats_search->installEventFilter(this);
	ui->upgrades_stats->header()->setSectionResizeMode(0, QHeaderView::Fixed);
	ui->upgrades_stats->header()->setSectionResizeMode(1, QHeaderView::Stretch);

	connect(ui->upgrades, &QTreeWidget::itemClicked, [&](QTreeWidgetItem* item, int column) { load_widget_data(item); });
	connect(ui->upgrades_stats, &QTreeWidget::itemDoubleClicked, [&](QTreeWidgetItem* item, int column) {  });

}

void ObjectEditor::create_folder(std::shared_ptr<WidgetData> widget) {
	QTreeWidgetItem* root_item;
	std::string path = widget->generate_path();
	
	if (auto u = std::dynamic_pointer_cast<UnitWidget>(widget); u) {
		root_item = ui->units->invisibleRootItem();
	} else if (auto i = std::dynamic_pointer_cast<ItemWidget>(widget); i) {
		root_item = ui->items->invisibleRootItem();
	} else if (auto d = std::dynamic_pointer_cast<DoodadWidget>(widget); d) {
		root_item = ui->doodads->invisibleRootItem();
	} else if (auto d = std::dynamic_pointer_cast<DestructibleWidget>(widget); d) {
		root_item = ui->destructibles->invisibleRootItem();
	} else if (auto a = std::dynamic_pointer_cast<AbilityWidget>(widget); a) {
		root_item = ui->abilities->invisibleRootItem();
	} else if (auto e = std::dynamic_pointer_cast<EffectWidget>(widget); e) {
		root_item = ui->effects->invisibleRootItem();
	} else if (auto u = std::dynamic_pointer_cast<UpgradeWidget>(widget); u) {
		root_item = ui->upgrades->invisibleRootItem();
	}

	if (folder_hierarchy.find(path) == folder_hierarchy.end()) {
		QTreeWidgetItem* last_item = root_item;
		for (auto&& folder : split(path, '\\')) {
			for (int index = 0; index < last_item->childCount(); index++) {
				if (auto c = last_item->child(index); c->text(0).toStdString() == folder) {
					last_item = c;
					goto outer_loop;
				}
			}
			root_item = new QTreeWidgetItem(last_item);
			root_item->setText(0, QString::fromStdString(folder));
			root_item->setText(1, QString::fromStdString("D"));
			root_item->setIcon(0, folder_icon);
			last_item = root_item;
		outer_loop: {};
		}
		last_item->addChild(widget->item);
		folder_hierarchy.emplace(path, last_item);
	} else {
		folder_hierarchy.at(path)->addChild(widget->item);
	}
}


void ObjectEditor::custom_menu_popup(const QPoint& pos) {
	QTreeWidgetItem* item = ui->units->itemAt(pos);

	QMenu* menu = new QMenu(this);

	QAction* create_as_custom = new QAction("Copy as a Custom Widget...");
	QAction* new_custom_unit = new QAction("New Custom Widget...");
	QAction* delete_custom_unit = new QAction("Delete Custom Widget...");
	QAction* create_custom_race = new QAction("Create Custom Race...");
	if (item->text(1) == "D") {
		menu->addAction(create_custom_race);
	} else {
		menu->addAction(create_as_custom);
		menu->addAction(new_custom_unit);
		menu->addAction(delete_custom_unit);
	}

	menu->exec(QCursor::pos());
}


bool ObjectEditor::eventFilter(QObject* obj, QEvent* event){
	if (event->type() == QKeyEvent::KeyPress) {
		QKeyEvent* k_event = dynamic_cast<QKeyEvent*>(event);
		QLineEdit * search_bar = qobject_cast<QLineEdit*>(obj);
		pressed_keys += k_event->key();
		if (pressed_keys.contains(Qt::Key_Escape) && search_bar != NULL) {
			search_bar->clear();
		} else if ( pressed_keys.contains(Qt::Key_Control) && pressed_keys.contains(Qt::Key_D) ) {
			show_as_raw = !show_as_raw;
			display_transition();
		}
	} else if (event->type() == QKeyEvent::KeyRelease) {
		QKeyEvent* k_event = dynamic_cast<QKeyEvent*>(event);
		pressed_keys -= k_event->key();
	}

    return false;
}

void ObjectEditor::display_transition() {
	for (auto&& element : map->objects.widgets_by_item) {
		element.second->display_raw(show_as_raw);
	}
	for (auto&& element : map->objects.properties.unit_properties) {
		element.display_raw(show_as_raw);
	}
	for (auto&& element : map->objects.properties.ability_properties) {
		element.display_raw(show_as_raw);
	}
	for (auto&& element : map->objects.properties.item_properties) {
		element.display_raw(show_as_raw);
	}
	for (auto&& element : map->objects.properties.doodad_properties) {
		element.display_raw(show_as_raw);
	}
	for (auto&& element : map->objects.properties.destructible_properties) {
		element.display_raw(show_as_raw);
	}
	for (auto&& element : map->objects.properties.effect_properties) {
		element.display_raw(show_as_raw);
	}
	for (auto&& element : map->objects.properties.upgrade_properties) {
		element.display_raw(show_as_raw);
	}
}

void ObjectEditor::load() {
	for (auto&& widget : map->objects.widgets_by_id) {
		create_folder(widget.second);
	}
}

void ObjectEditor::load_widget_data(QTreeWidgetItem* item) {
	if (item->text(1) != "") {
		return;
	} 
	auto widget = map->objects.widgets_by_item.at(item);
	
	if (auto u = std::dynamic_pointer_cast<UnitWidget>(widget); u) {
		for (auto&& p: map->objects.properties.unit_properties) {
			std::string value = units_slk.data(p.field, u->raw_id);
			p.item->setText(1, QString::fromStdString(value));
		}
	} else if (auto i = std::dynamic_pointer_cast<ItemWidget>(widget); i) {
		for (auto&& p : map->objects.properties.item_properties) {
			std::string value = items_slk.data(p.field, i->raw_id);
			p.item->setText(1, QString::fromStdString(value));
		}
	} else if (auto d = std::dynamic_pointer_cast<DoodadWidget>(widget); d) {
		for (auto&& p :  map->objects.properties.doodad_properties) {
			std::string value = doodads_slk.data(p.field, d->raw_id);
			p.item->setText(1, QString::fromStdString(value));
		}
	} else if (auto d = std::dynamic_pointer_cast<DestructibleWidget>(widget); d) {
		for (auto&& p : map->objects.properties.destructible_properties) {
			std::string value = destructibles_slk.data(p.field, d->raw_id);
			p.item->setText(1, QString::fromStdString(value));
		}
	} else if (auto a = std::dynamic_pointer_cast<AbilityWidget>(widget); a) {
		for (auto&& p : map->objects.properties.ability_properties) {
			p.item->setHidden(true);
			if (p.used_by.size() == 0 || std::find(p.used_by.begin(), p.used_by.end(), a->raw_id) != p.used_by.end() ) {
				std::string value = abilities_slk.data(p.field, a->raw_id);
				p.item->setText(1, QString::fromStdString(value));
				p.item->setHidden(false);
			}			 
		}
	} else if (auto e = std::dynamic_pointer_cast<EffectWidget>(widget); e) {
		for (auto&& p : map->objects.properties.effect_properties) {
			std::string value = effects_slk.data(p.field, e->raw_id);
			p.item->setText(1, QString::fromStdString(value));
		}
	} else if (auto u = std::dynamic_pointer_cast<UpgradeWidget>(widget); u) {
		std::cout << u->name << std::endl;
		for (auto&& p : map->objects.properties.upgrade_properties) {
			std::string value = upgrades_slk.data(p.field, u->raw_id);
			p.item->setText(1, QString::fromStdString(value));
		}
	}
}

void ObjectEditor::load_properties() {
	for (auto&& p : map->objects.properties.unit_properties) {
		ui->unit_stats->addTopLevelItem(p.item);
	}
	for (auto&& p : map->objects.properties.item_properties) {
		ui->item_stats->addTopLevelItem(p.item);
	}
	for (auto&& p : map->objects.properties.doodad_properties) {
		ui->doodads_stats->addTopLevelItem(p.item);
	}
	for (auto&& p : map->objects.properties.destructible_properties) {
		ui->destructibles_stats->addTopLevelItem(p.item);
	}
	for (auto&& p : map->objects.properties.ability_properties) {
		ui->abilities_stats->addTopLevelItem(p.item);
	}
	for (auto&& p : map->objects.properties.effect_properties) {
		ui->effects_stats->addTopLevelItem(p.item);
	}
	for (auto&& p : map->objects.properties.upgrade_properties) {
		ui->upgrades_stats->addTopLevelItem(p.item);
	}
}
