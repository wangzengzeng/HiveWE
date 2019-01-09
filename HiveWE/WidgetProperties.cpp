#include <stdafx.h>

WidgetProperty::WidgetProperty(std::string id,std::string field_name, std::string display_name, int idx,std::string type){
	raw_id = id;
	name = display_name;
	field = field_name;
	index = idx;
	data_type = type;
	item = new QTreeWidgetItem();
	item->setText(0,QString::fromStdString(name));
}
void WidgetProperty::display_raw(bool is_raw) {
	std::string display = (is_raw) ? field : name;
	item->setText(0, QString::fromStdString(display));
}

void Properties::load() {
	for (int row = 1; row < units_meta_slk.rows; row++) {
		std::string raw_id = units_meta_slk.table_data[row][0];
		std::string field = units_meta_slk.data("field", raw_id);
		std::string name = units_meta_slk.data("displayName", raw_id);
		std::string type = units_meta_slk.data("type", raw_id);
		int index = std::stoi(units_meta_slk.data("index", raw_id));

		WidgetProperty p(raw_id,field, name, index, type);
		if ( raw_id.at(0) == 'u' ) {
			unit_properties.push_back(p);
		} else {
			item_properties.push_back(p);
		}
		fields_by_item.emplace(p.item, std::make_shared<WidgetProperty>(p));
	}

	for (int row = 1; row < doodads_meta_slk.rows; row++) {
		std::string raw_id = doodads_meta_slk.table_data[row][0];
		std::string field = doodads_meta_slk.data("field", raw_id);
		std::string name = doodads_meta_slk.data("displayName", raw_id);
		std::string type = units_meta_slk.data("type", raw_id);
		int index = std::stoi(doodads_meta_slk.data("index", raw_id));

		WidgetProperty p(raw_id, field, name, index, type);
		doodad_properties.push_back(p);
		fields_by_item.emplace(p.item, std::make_shared<WidgetProperty>(p));
	}

	for (int row = 1; row < destructibles_meta_slk.rows; row++) {
		std::string raw_id = destructibles_meta_slk.table_data[row][0];
		std::string field = destructibles_meta_slk.data("field", raw_id);
		std::string name = destructibles_meta_slk.data("displayName", raw_id);
		std::string type = units_meta_slk.data("type", raw_id);
		int index = std::stoi(destructibles_meta_slk.data("index", raw_id));

		WidgetProperty p(raw_id, field, name, index, type);

		destructible_properties.push_back(p);
		fields_by_item.emplace(p.item, std::make_shared<WidgetProperty>(p));
	}

	for (int row = 1; row < abilities_meta_slk.rows; row++) {
		std::string raw_id = abilities_meta_slk.table_data[row][0];
		std::string field = abilities_meta_slk.data("field", raw_id);
		std::string name = abilities_meta_slk.data("displayName", raw_id);
		std::string type = units_meta_slk.data("type", raw_id);
		int index = std::stoi(abilities_meta_slk.data("index", raw_id));
		
		WidgetProperty p(raw_id, field, name, index, type);
		p.used_by = split(abilities_meta_slk.data("useSpecific", raw_id), ',');

		ability_properties.push_back(p);
		fields_by_item.emplace(p.item, std::make_shared<WidgetProperty>(p));
	}
	for (int row = 1; row < effects_meta_slk.rows; row++) {
		std::string raw_id = effects_meta_slk.table_data[row][0];
		std::string field = effects_meta_slk.data("field", raw_id);
		std::string name = effects_meta_slk.data("displayName", raw_id);
		std::string type = units_meta_slk.data("type", raw_id);
		int index = std::stoi(effects_meta_slk.data("index", raw_id));

		WidgetProperty p(raw_id, field, name, index, type);

		effect_properties.push_back(p);
		fields_by_item.emplace(p.item, std::make_shared<WidgetProperty>(p));
	}

	for (int row = 1; row < upgrades_meta_slk.rows; row++) {
		std::string raw_id = upgrades_meta_slk.table_data[row][0];
		std::string field = upgrades_meta_slk.data("field", raw_id);
		std::string name = upgrades_meta_slk.data("displayName", raw_id);
		std::string type = units_meta_slk.data("type", raw_id);
		int index = std::stoi(upgrades_meta_slk.data("index", raw_id));

		WidgetProperty p(raw_id, field, name, index, type);

		upgrade_properties.push_back(p);
		fields_by_item.emplace(p.item, std::make_shared<WidgetProperty>(p));
	}
}
