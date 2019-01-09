class WidgetProperty {
public:
	std::string raw_id;
	std::string field;
	std::string name;
	std::string data_type;
	int index;
	std::vector<std::string> used_by;

	QTreeWidgetItem* item;

	WidgetProperty(std::string id,std::string field_name, std::string display_name,int idx,std::string type);

	void display_raw(bool is_raw);
};

class Properties {
public:
	std::vector<WidgetProperty> unit_properties;
	std::vector<WidgetProperty> item_properties;
	std::vector<WidgetProperty> doodad_properties;
	std::vector<WidgetProperty> destructible_properties;
	std::vector<WidgetProperty> ability_properties;
	std::vector<WidgetProperty> effect_properties;
	std::vector<WidgetProperty> upgrade_properties;

	std::unordered_map<QTreeWidgetItem*, std::shared_ptr<WidgetProperty>> fields_by_item;

	void load();
};