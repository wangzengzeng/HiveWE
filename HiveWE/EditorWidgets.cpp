#include "stdafx.h"


WidgetData::WidgetData(std::string id,std::string display_name, std::string editor_suffix, std::string parent_id, std::string icon) {
	raw_id = id;
	parent_raw_id = parent_id;
	suffix = editor_suffix;
	icon_path = icon;
	name = display_name;
	item = new QTreeWidgetItem();
	item->setText(0, QString::fromStdString(name + suffix));

	if (icon_path != "") {
		const auto q_icon = texture_to_icon(icon_path);
		item->setIcon(0, q_icon);
	}

	if (parent_raw_id != "") {
		is_custom = true;
		item->setTextColor(0, QColor(255, 0, 0));
	}
}
void WidgetData::display_raw(bool is_raw) {
	std::string display = (is_raw == true) ? raw_id : name + suffix;
	item->setText(0,QString::fromStdString(display));
}

template <class T>
constexpr bool ObjectEditorExtension::append(T widget) {
	if constexpr (std::is_same<UnitWidget, T>::value) {
		units.push_back(widget); 
		widgets_by_id.emplace(widget.raw_id, std::make_shared<UnitWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<UnitWidget>(widget));
	}else if constexpr (std::is_same<ItemWidget, T>::value) {
		items.push_back(widget);
		widgets_by_id.emplace(widget.raw_id, std::make_shared<ItemWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<ItemWidget>(widget));
	} else if constexpr (std::is_same<DestructibleWidget, T>::value) {
		destructibles.push_back(widget);
		widgets_by_id.emplace(widget.raw_id, std::make_shared<DestructibleWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<DestructibleWidget>(widget));
	} else if constexpr (std::is_same<DoodadWidget, T>::value) {
		doodads.push_back(widget);
		widgets_by_id.emplace(widget.raw_id, std::make_shared<DoodadWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<DoodadWidget>(widget));
	} else if constexpr (std::is_same<AbilityWidget, T>::value) {
		abilities.push_back(widget);
		widgets_by_id.emplace(widget.raw_id, std::make_shared<AbilityWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<AbilityWidget>(widget));
	} else if constexpr (std::is_same<UpgradeWidget, T>::value) {
		upgrades.push_back(widget);
		widgets_by_id.emplace(widget.raw_id, std::make_shared<UpgradeWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<UpgradeWidget>(widget));
	} else if constexpr (std::is_same<EffectWidget, T>::value) {
		effects.push_back(widget);
		widgets_by_id.emplace(widget.raw_id, std::make_shared<EffectWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<EffectWidget>(widget));
	} else {
		return false;
	}
	return true;
}


void ObjectEditorExtension::load() {
	for (int row = 1; row < units_slk.rows; row++) {
		std::string unit_id = units_slk.table_data[row][0];
		std::string unit_name = units_slk.data("Name", unit_id);
		std::string suffix = units_slk.data("EditorSuffix", unit_id);
		std::string path = units_slk.data("Art", unit_id);
		std::string parent = "";
		if (auto vec = split(path, '.'); vec.size() > 2 || path == "") {
			path = "ReplaceableTextures\\WorldEditUI\\DoodadPlaceholder.blp";
		}
		UnitWidget u(unit_id, unit_name, suffix, parent, path);
		u.is_building = (units_slk.data<int>("isbldg", unit_id)) ? true : false;
		u.is_special = (units_slk.data<int>("special", unit_id)) ? true : false;
		u.is_melee = (units_slk.data<int>("campaign", unit_id)) ? false : true;
		u.is_hero = (std::isupper(unit_id.at(0))) ? true : false;
		map->objects.append(u);
	
	}

	for (int row = 1; row < items_slk.rows; row++) {
		std::string item_id = items_slk.table_data[row][0];
		std::string item_name = items_slk.data("Name", item_id);
		std::string path = items_slk.data("Art", item_id);
		if (auto vec = split(path, '.'); vec.size() > 2 || path == "") {
			path = "ReplaceableTextures\\WorldEditUI\\DoodadPlaceholder.blp";
		}
		ItemWidget i(item_id, item_name, "", "", path);
		map->objects.append(i);
	}

	for (int row = 1; row < doodads_slk.rows; row++) {
		std::string doodad_id = doodads_slk.table_data[row][0];
		std::string doodad_name = doodads_slk.data("Name", doodad_id);
		std::string suffix = doodads_slk.data("EditorSuffix", doodad_id);
		DoodadWidget d(doodad_id, doodad_name, suffix, "", "");
		map->objects.append(d);
	}

	for (int row = 1; row < destructibles_slk.rows; row++) {
		std::string dest_id = destructibles_slk.table_data[row][0];
		std::string dest_name = destructibles_slk.data("Name", dest_id);
		std::string suffix = destructibles_slk.data("EditorSuffix", dest_id);
		DestructibleWidget d(dest_id, dest_name, suffix, "", "");
		map->objects.append(d);

	}

	for (int row = 1; row < abilities_slk.rows; row++) {
		std::string ability_id = abilities_slk.table_data[row][0];
		std::string ability_name = abilities_slk.data("Name", ability_id);
		std::string suffix = abilities_slk.data("EditorSuffix", ability_id);
		std::string path = abilities_slk.data("Art", ability_id);
		
		if (auto vec = split(path, '.'); vec.size() > 2 || path == "") {
			path = "ReplaceableTextures\\WorldEditUI\\DoodadPlaceholder.blp";
		}
		AbilityWidget a(ability_id, ability_name, suffix, "", path);
		a.is_hero = (abilities_slk.data<int>("hero", ability_id))? true : false;
		a.is_item = (abilities_slk.data<int>("item", ability_id)) ? true : false;
		a.levels = abilities_slk.data<int>("levels", ability_id);
		map->objects.append(a);

	}

	for (int row = 1; row < effects_slk.rows; row++) {
		std::string effect_id = effects_slk.table_data[row][0];
		std::string effect_name = effects_slk.data("Bufftip", effect_id);
		if (effect_name == "") {
			effect_name = effects_slk.data("EditorName", effect_id);
		}
		std::string suffix = effects_slk.data("EditorSuffix", effect_id);
		std::string path = effects_slk.data("Buffart", effect_id);
		if (auto vec = split(path, '.'); vec.size() > 2 || path == "") {
			path = "ReplaceableTextures\\WorldEditUI\\DoodadPlaceholder.blp";
		}
		EffectWidget e(effect_id, effect_name, suffix, "", path);
		e.is_effect = (effects_slk.data<int>("isEffect", effect_id)) ? true : false;
		map->objects.append(e);
	}

	for (int row = 1; row < upgrades_slk.rows; row++) {
		std::string upgrade_id = upgrades_slk.table_data[row][0];
		std::string upgrade_name = upgrades_slk.data("Name", upgrade_id);
		std::string suffix = upgrades_slk.data("EditorSuffix", upgrade_id);
		std::string path = upgrades_slk.data("Art", upgrade_id);
		if (auto vec = split(path, '.'); vec.size() > 2 || path == "") {
			path = "ReplaceableTextures\\WorldEditUI\\DoodadPlaceholder.blp";
		}
		UpgradeWidget u(upgrade_id, upgrade_name, suffix, "", path);
		map->objects.append(u);
	}

}

std::string WidgetData::generate_path() {
	return "";
}

std::string UnitWidget::generate_path() {
	std::string melee = (is_melee) ? "Melee" : "Campaign";
	std::string type;
	std::string std_folder = (!is_custom)? "Standard Units" : "Custom Units";

	if (is_hero && !is_special) {
		type = "Heroes";
	} else if (is_building) {
		type = "Buildings";
	} else if (is_special) {
		type = "Special";
	} else {
		type = "Units";
	}
	return std_folder + "\\" + race() + "\\" + melee + "\\" + type;
}

std::string UnitWidget::race() const {
	std::string race = units_slk.data("race", raw_id);
	
	if (race == "orc") {
		return "Orc";
	} else if (race == "human") {
		return "Human";
	} else if (race == "undead") {
		return "Undead";
	} else if (race == "nightelf") {
		return "Night Elf";
	} else if (race == "creeps") {
		return "Neutral Hostile";
	} else if (race == "naga") {
		return "Naga";
	}
	return "Neutral Passive";
}

std::string ItemWidget::generate_path() {
	std::string std_folder = (!is_custom) ? "Standard Items" : "Custom Items";
	return std_folder + "\\" + items_slk.data("class", raw_id);
}
std::string DestructibleWidget::generate_path() {
	std::string type = destructibles_slk.data("category", raw_id);
	std::vector<std::string> pair = world_edit_data.whole_data("DestructibleCategories", type);
	std::string std_folder = (!is_custom) ? "Standard Destructibles" : "Custom Destructibles";
	auto icon = texture_to_icon(pair.at(1) + ".blp");
	item->setIcon(0, icon);
	return std_folder + "\\" + pair.at(0);
}
std::string DoodadWidget::generate_path() {
	std::string type = doodads_slk.data("category", raw_id);
	std::vector<std::string> pair = world_edit_data.whole_data("DoodadCategories", type);
	std::string std_folder = (!is_custom) ? "Standard Doodads" : "Custom Doodads";
	auto icon = texture_to_icon(pair.at(1) + ".blp");
	item->setIcon(0, icon);
	return std_folder + "\\" + pair.at(0);
}

std::string AbilityWidget::generate_path() {
	std::string race = abilities_slk.data("race", raw_id);
	std::string std_folder = (!is_custom) ? "Standard Abilities" : "Custom Abilities";
	if (race == "orc") {
		race = "Orc";
	} else if (race == "human") {
		race = "Human";
	} else if (race == "undead") {
		race = "Undead";
	} else if (race == "nightelf") {
		race = "Night Elf";
	} else if (race == "creeps") {
		race = "Neutral Hostile";
	} else if (race == "naga") {
		race = "Neutral Passive";
	} else if (race == "critter" || race == "other") {
		race = "Special";
	} else {
		race = "Neutral Passive";
	}
	
	if (is_hero) {
		return std_folder + "\\" + race + "\\" + "Heroes";
	} else if (is_item) {
		return std_folder + "\\" + race + "\\" + "Items";
	}
	return std_folder + "\\" + race + "\\" + "Units";
}

std::string EffectWidget::generate_path() {
	std::string race = effects_slk.data("race", raw_id);
	std::string std_folder = (!is_custom) ? "Standard Effects" : "Custom Effects";
	if (race == "orc") {
		race = "Orc";
	} else if (race == "human") {
		race = "Human";
	} else if (race == "undead") {
		race = "Undead";
	} else if (race == "nightelf") {
		race = "Night Elf";
	} else {
		race = "Special";
	}
	std::string effect = (is_effect) ? "Effect" : "Buff";

	return std_folder + "\\" + race + "\\" + effect;
}


std::string UpgradeWidget::generate_path() {
	std::string race = upgrades_slk.data("race", raw_id);
	std::string std_folder = (!is_custom) ? "Standard Upgrades" : "Custom Upgrades";
	if (race == "orc") {
		race = "Orc";
	} else if (race == "human") {
		race = "Human";
	} else if (race == "undead") {
		race = "Undead";
	} else if (race == "nightelf") {
		race = "Night Elf";
	} else {
		race = "Special";
	}
	return std_folder + "\\" + race;
}

void ObjectEditorExtension::clear() { 
	widgets_by_id.clear();
	widgets_by_item.clear();
	units.clear();
	items.clear();
	doodads.clear();
	destructibles.clear();
	upgrades.clear();
	abilities.clear();
	effects.clear();
}