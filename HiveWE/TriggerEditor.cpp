#include "stdafx.h"

TriggerEditor::TriggerEditor(QWidget* parent) : QMainWindow(parent) {
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);

	ui.splitter->setSizes({ 10000, 20000 });
	show();

	QFileIconProvider icons;
	folder_icon = icons.icon(QFileIconProvider::Folder);
	file_icon = icons.icon(QFileIconProvider::File);

	trigger_comment_icon = texture_to_icon(world_edit_data.data("WorldEditArt", "SEIcon_TriggerComment") + ".blp");

	event_icon = texture_to_icon(world_edit_data.data("WorldEditArt", "SEIcon_Event"));
	condition_icon = texture_to_icon(world_edit_data.data("WorldEditArt", "SEIcon_Condition"));
	action_icon = texture_to_icon(world_edit_data.data("WorldEditArt", "SEIcon_Action"));

	for (auto& i : map->triggers.categories) {
		QTreeWidgetItem* item = new QTreeWidgetItem(ui.explorer);
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		item->setData(0, Qt::EditRole, QString::fromLocal8Bit(i.name.c_str()));
		item->setData(0, Qt::UserRole, item->data(0, Qt::EditRole));

		item->setIcon(0, folder_icon);
		
		folders[i.id] = item;

		folder_ids.emplace(item, i.id);

		categorys.emplace(item, i);
	}

	for (auto&& i : map->triggers.triggers) {
		QTreeWidgetItem* item = new QTreeWidgetItem(folders[i.category_id]);
		item->setData(0, Qt::EditRole, QString::fromLocal8Bit(i.name.c_str()));
		item->setData(0, Qt::UserRole, item->data(0, Qt::EditRole));
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		if (i.is_comment) {
			item->setIcon(0, trigger_comment_icon);
		} else {
			item->setIcon(0, file_icon);
		}

		files.emplace(item, i);

		folder_ids.emplace(item, i.category_id);

	}
	trigger_num = map->triggers.triggers.size() + 1;
	category_num = map->triggers.categories.size() + 1;

	connect(ui.explorer, &QTreeWidget::itemClicked, this, &TriggerEditor::item_clicked);

	connect(ui.explorer, &QTreeWidget::itemChanged, this, &TriggerEditor::item_changed);

	connect(ui.explorer, &QTreeWidget::customContextMenuRequested, this, &TriggerEditor::custom_menu_popup);
	

	connect(ui.editor, &QTabWidget::tabCloseRequested, [&](int index) { delete ui.editor->widget(index); });

}

void TriggerEditor::custom_menu_popup(const QPoint& pos) {
	QTreeWidgetItem* item = ui.explorer->itemAt(pos);
	if (item == NULL) {
		return;
	}
	QMenu* menu = new QMenu(this);

	QAction* create_category_action = new QAction(u8"新建别类");
	QAction* create_trigger_action = new QAction(u8"新建触发器");
	QAction* create_trigger_action2 = new QAction(u8"新建注释");
	QAction* rename_item_action = new QAction(u8"重命名");
	QAction* delete_item_action = new QAction(u8"删除");

	connect(create_category_action, &QAction::triggered, [&]() {create_category(item); });
	connect(create_trigger_action, &QAction::triggered, [&](){create_trigger(item,false);});
	connect(create_trigger_action2, &QAction::triggered, [&]() {create_trigger(item, true); });
	connect(rename_item_action, &QAction::triggered, [&]() { item->treeWidget()->editItem(item,0); });
	connect(delete_item_action, &QAction::triggered, [&]() {delete_item(item); });
	
	menu->addAction(create_category_action);
	menu->addAction(create_trigger_action);
	menu->addAction(create_trigger_action2);
	menu->addAction(rename_item_action);
	menu->addAction(delete_item_action);

	menu->exec(QCursor::pos());

}

void TriggerEditor::create_category(QTreeWidgetItem* parent) {

	map->triggers.categories.push_back(TriggerCategory());


	TriggerCategory& category = map->triggers.categories.back();

	category.name = "新建文件夹";

	category.id = ++category_num;


	QTreeWidgetItem* item = new QTreeWidgetItem(ui.explorer);
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	item->setData(0, Qt::EditRole, QString::fromLocal8Bit(category.name.c_str()));
	item->setData(0, Qt::UserRole, item->data(0, Qt::EditRole));

	item->setIcon(0, folder_icon);

	folders[category.id] = item;

	folder_ids.emplace(item, category.id);

	categorys.emplace(item, category);
}

void TriggerEditor::create_trigger(QTreeWidgetItem* parent,bool is_comment) {
	if (folder_ids.find(parent) == folder_ids.end()) {
		return;
	}

	int& folder_id = folder_ids.at(parent);

	map->triggers.triggers.push_back(Trigger());

	Trigger& trigger = map->triggers.triggers.back();

	trigger.id = ++trigger_num;

	trigger.category_id = folder_id;

	trigger.is_comment = is_comment;

	if (is_comment) {
		
		trigger.name = "注释" + std::to_string(trigger.id);
	}
	else {
		trigger.name = "新建触发器" + std::to_string(trigger.id);
	}


	QTreeWidgetItem* item = new QTreeWidgetItem(folders[folder_id]);
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	item->setData(0, Qt::EditRole, QString::fromLocal8Bit(trigger.name.c_str()));
	item->setData(0, Qt::UserRole, item->data(0, Qt::EditRole));

	if (trigger.is_comment) {
		item->setIcon(0, trigger_comment_icon);
	}
	else {
		item->setIcon(0, file_icon);
	}

	files.emplace(item, trigger);

	folder_ids.emplace(item, trigger.category_id);

	parent->treeWidget()->expandItem(parent);

	parent->treeWidget()->setCurrentItem(item);

	item_clicked(item);

	

}

void TriggerEditor::remove_trigger(QTreeWidgetItem* item) {
	if (files.find(item) == files.end()) 
		return;

	Trigger& trigger = files.at(item).get();

	for (int i = 0; i < ui.editor->count(); i++) {
		QWidget* tbl = ui.editor->widget(i);
		if (tbl->property("TriggerID").toInt() == trigger.id) {
			ui.editor->removeTab(i);
			delete tbl;
			break;
		}
	}

	for (auto it = map->triggers.triggers.begin(); it != map->triggers.triggers.end(); it++) {
		if (it->id == trigger.id) {
			map->triggers.triggers.erase(it);
			break;
		}
	}

	files.erase(item);

	folder_ids.erase(item);

	delete item;
	
}

void TriggerEditor::remove_category(QTreeWidgetItem* item) {
	if (categorys.find(item) == categorys.end())
		return;

	TriggerCategory& category = categorys.at(item).get();

	//删除该文件夹下的所有触发器
	std::map<int, bool> ids;

	for (auto it = map->triggers.triggers.begin(); it != map->triggers.triggers.end(); ) {
		if (it->category_id == category.id) {
			ids.emplace(it->id, true);
			map->triggers.triggers.erase(it++);
		}
		else {
			it++;
		}
	}

	for (int i = 0;i< item->childCount();){
		QTreeWidgetItem* child = item->child(i);
		auto it = files.find(child);
		if (it != files.end()) {

			files.erase(child);

			folder_ids.erase(child);

			delete child;
		}
		else {
			i++;
		}
	}

	for (int i = 0; i < ui.editor->count(); ) {
		QWidget* tbl = ui.editor->widget(i);
		int id = tbl->property("TriggerID").toInt();
		if (ids.find(id) != ids.end()) {
			ui.editor->removeTab(i);
			delete tbl;
		} else {
			i++;
		}
	}
	
	folders.erase(category.id);

	for (auto it = map->triggers.categories.begin(); it != map->triggers.categories.end(); it++) {
		if (it->id == category.id) {
			map->triggers.categories.erase(it);
			break;
		}
	}

	categorys.erase(item);

	folder_ids.erase(item);

	delete item;
}

void TriggerEditor::delete_item(QTreeWidgetItem* item) {

	if (files.find(item) != files.end()) {
		remove_trigger(item);
		
	} else if (categorys.find(item) != categorys.end()) {
		remove_category(item);
	}
}

void TriggerEditor::rename_item(QTreeWidgetItem* item,QString& name) {
	QString oldName = item->data(0, Qt::UserRole).toString();
	if (oldName == name) {
		return;
	}

	std::string strName = name.toLocal8Bit().toStdString();

	if (files.find(item) != files.end()) {
		Trigger& trigger = files.at(item).get();
		int num = 0;
		for (auto&& i : map->triggers.triggers) {
			if (strName.compare(i.name) == 0) {
				QMessageBox::information(this, u8"触发编辑器", u8"重复的名字");

				item->setData(0, Qt::EditRole, QString::fromLocal8Bit(trigger.name.c_str()));
				return;
			}
		}

		trigger.name = strName;
		item->setData(0, Qt::EditRole,name);
		item->setData(0, Qt::UserRole, item->data(0, Qt::EditRole));

		for (int i = 0; i < ui.editor->count(); i++) {
			QWidget* tbl = ui.editor->widget(i);
			if (tbl->property("TriggerID").toInt() == trigger.id) {
				ui.editor->tabBar()->setTabText(i, name);
			}
		}

	}
	else if (categorys.find(item) != categorys.end()) {
		TriggerCategory& category = categorys.at(item).get();

		for (auto&& i : map->triggers.categories) {
			if (strName.compare(i.name) == 0) {
				item->setData(0, Qt::EditRole, QString::fromLocal8Bit(category.name.c_str()));
				QMessageBox::information(this, u8"触发编辑器", u8"重复的名字");
				return;
			}
		}

		category.name = strName;
		item->setData(0, Qt::EditRole, name);
		item->setData(0, Qt::UserRole, item->data(0, Qt::EditRole));
	}
}
void TriggerEditor::item_clicked(QTreeWidgetItem* item) {
	if (files.find(item) == files.end()) {
		return;
	}
	Trigger& trigger = files.at(item).get();

	if (trigger.is_comment) {
		return;
	}

	// Check if trigger is already open and if so focus it
	for (int i = 0; i < ui.editor->count(); i++) {
		if (ui.editor->widget(i)->property("TriggerID").toInt() == trigger.id) {
			ui.editor->setCurrentIndex(i);
			return;
		}
	}

	QWidget* tab = new QWidget;
	tab->setProperty("TriggerID", trigger.id);

	QVBoxLayout* layout = new QVBoxLayout(tab);

	if (!trigger.custom_text.empty()) {
		JassEditor* edit = new JassEditor;
		layout->addWidget(edit);
		edit->setText(QString::fromStdString(trigger.custom_text));
		connect(this, &TriggerEditor::save_changes, [=]() {
			files.at(item).get().custom_text = edit->text().toStdString();
		});
	} else {
		QTreeWidget* edit = new QTreeWidget;
		edit->setHeaderHidden(true);
		edit->setUniformRowHeights(true);
		layout->addWidget(edit);
		show_gui_trigger(edit, trigger);
		edit->expandAll();
	}

	ui.editor->addTab(tab, QString::fromLocal8Bit(trigger.name.c_str()));
	ui.editor->setCurrentWidget(tab);
}

void TriggerEditor::item_changed(QTreeWidgetItem* item) {
	QString name = item->data(0, Qt::EditRole).toString();
	rename_item(item, name);
}

void TriggerEditor::show_gui_trigger(QTreeWidget* edit, Trigger& trigger) {
	QTreeWidgetItem* events = new QTreeWidgetItem(edit);
	events->setText(0, u8"事件");
	events->setIcon(0, event_icon);

	QTreeWidgetItem* conditions = new QTreeWidgetItem(edit);
	conditions->setText(0, u8"条件");
	conditions->setIcon(0, condition_icon);

	QTreeWidgetItem* actions = new QTreeWidgetItem(edit);
	actions->setText(0, u8"动作");
	actions->setIcon(0, action_icon);

	std::function<void(QTreeWidgetItem*, ECA&)> recurse = [&](QTreeWidgetItem* parent, ECA& i) {
		QTreeWidgetItem* eca = new QTreeWidgetItem(parent);
		std::string category;

		std::vector<std::string> string_parameters;

		switch (i.type) {
			case ECA::Type::event:
				string_parameters = map->triggers.trigger_strings.whole_data("TriggerEventStrings", i.name);
				category = map->triggers.trigger_data.data("TriggerEvents", "_" + i.name + "_Category");
				break;
			case ECA::Type::condition:
				string_parameters = map->triggers.trigger_strings.whole_data("TriggerConditionStrings", i.name);
				category = map->triggers.trigger_data.data("TriggerConditions", "_" + i.name + "_Category");
				break;
			case ECA::Type::action:
				string_parameters = map->triggers.trigger_strings.whole_data("TriggerActionStrings", i.name);
				category = map->triggers.trigger_data.data("TriggerActions", "_" + i.name + "_Category");
				break;
		}

		eca->setText(0, QString::fromStdString(get_parameters_names(string_parameters, i.parameters)));

		if (auto found = trigger_icons.find(category); found == trigger_icons.end()) {
			std::string icon_path = map->triggers.trigger_data.data("TriggerCategories", category, 1);
			std::string final_path = icon_path;
			fs::path type = fs::path(final_path).extension();
			if (type != ".blp")
			{
				final_path += ".blp";
			}
			QIcon icon = texture_to_icon(final_path);
			trigger_icons[category] = icon;
			eca->setIcon(0, icon);
		} else {
			eca->setIcon(0, found->second);
		}

		QTreeWidgetItem* sub1 = eca;
		QTreeWidgetItem* sub2 = eca;
		QTreeWidgetItem* sub3 = eca;

		if (i.name == "AndMultiple" || i.name == "OrMultiple") {
			sub1 = new QTreeWidgetItem(eca, { "Conditions" });
			sub1->setIcon(0, condition_icon);
		} else if (i.name == "IfThenElseMultiple") {
			sub1 = new QTreeWidgetItem(eca, { "If - Conditions" });
			sub1->setIcon(0, condition_icon);
			sub2 = new QTreeWidgetItem(eca, { "Then - Actions" });
			sub2->setIcon(0, action_icon);
			sub3 = new QTreeWidgetItem(eca, { "Else - Actions" });
			sub3->setIcon(0, action_icon);
		} else if (i.name == "ForLoopAMultiple"
			|| i.name == "ForLoopBMultiple"
			|| i.name == "ForLoopVarMultiple"
			|| i.name == "ForLoopA"
			|| i.name == "ForLoopB"
			|| i.name == "ForLoopVar"
			|| i.name == "ForForceMultiple"
			|| i.name == "ForGroupMultiple") {
			sub1 = new QTreeWidgetItem(eca, { "Loop - Actions" });
			sub1->setIcon(0, action_icon);
		}

		for (auto&& j : i.ecas) {
			if (j.group == 0) {
				recurse(sub1, j);
			} else if (j.group == 1) {
				recurse(sub2, j);
			} else if (j.group == 2) {
				recurse(sub3, j);
			}
		}
	};

	for (auto&& i : trigger.lines) {
		if (i.type == ECA::Type::event) {
			recurse(events, i);
		} else if (i.type == ECA::Type::condition) {
			recurse(conditions, i);
		} else if (i.type == ECA::Type::action) {
			recurse(actions, i);
		}
	}
}

std::string TriggerEditor::get_parameters_names(const std::vector<std::string>& string_parameters, const std::vector<TriggerParameter>& parameters) const {
	std::string result;

	int current_parameter = 0;
	for (auto&& i : string_parameters) {
		if (i.empty() || i.front() != '~') {
			result += i;
			continue;
		}
		const TriggerParameter& j = parameters.at(current_parameter);

		std::vector<std::string> sub_string_parameters;
		if (j.has_sub_parameter) {
			switch (j.sub_parameter.type) {
				case TriggerSubParameter::Type::events:
					sub_string_parameters = map->triggers.trigger_strings.whole_data("TriggerEventStrings", j.sub_parameter.name);
					break;
				case TriggerSubParameter::Type::conditions:
					sub_string_parameters = map->triggers.trigger_strings.whole_data("TriggerConditionStrings", j.sub_parameter.name);
					break;
				case TriggerSubParameter::Type::actions:
					sub_string_parameters = map->triggers.trigger_strings.whole_data("TriggerActionStrings", j.sub_parameter.name);
					break;
				case TriggerSubParameter::Type::calls:
					sub_string_parameters = map->triggers.trigger_strings.whole_data("TriggerCallStrings", j.sub_parameter.name);
					break;
			}
			result += "(" + get_parameters_names(sub_string_parameters, j.sub_parameter.parameters) + ")";
		} else {
			switch (j.type) {
				case TriggerParameter::Type::preset:
					result += map->triggers.trigger_data.data("TriggerParams", j.value, 3);
					break;
				case TriggerParameter::Type::string: {
					std::string pre_result;
					if (j.value.size() == 4) {
						if (units_slk.row_header_exists(j.value)) {
							pre_result = units_slk.data("Name", j.value);
						} else if (items_slk.row_header_exists(j.value)) {
							pre_result = items_slk.data("Name", j.value);
						} else {
							pre_result = j.value;
						}
					}

					if (pre_result.size() > 8 && pre_result.substr(0, 7) == "TRIGSTR") {
						result += map->trigger_strings.string(pre_result);
					} else if (!pre_result.empty()) {
						result += pre_result;
					} else if (j.value.size() > 8 && j.value.substr(0, 7) == "TRIGSTR") {
						result += map->trigger_strings.string(j.value);
					} else {
						result += j.value;
					}
					break;
				}
				case TriggerParameter::Type::variable: {
					if (j.value.size() > 7 && j.value.substr(0, 7) == "gg_unit") {
						std::string type = j.value.substr(8, 4);
						std::string instance = j.value.substr(13);
						result += units_slk.data("Name", type);
						result += " " + instance;
					} else {
						std::string type = map->triggers.variables[j.value].type;
						if (type == "unit") {
							//std::cout << "test\n";
						} else {
							result += j.value;
						}
					}
					break;
				}
				default:
					result += j.value;
			}
		}
		current_parameter++;
	}

	return result;
}