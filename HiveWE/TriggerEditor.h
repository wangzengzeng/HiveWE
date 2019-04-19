#pragma once

#include "ui_TriggerEditor.h"

class TriggerEditor : public QMainWindow {
	Q_OBJECT

public:
	TriggerEditor(QWidget* parent = nullptr);

private:
	int trigger_num;
	int category_num;

	Ui::TriggerEditor ui;

	QIcon folder_icon;
	QIcon file_icon;
	QIcon trigger_comment_icon;

	QIcon event_icon;
	QIcon condition_icon;
	QIcon action_icon;

	std::unordered_map<std::string, QIcon> trigger_icons;

	std::unordered_map<int, QTreeWidgetItem*> folders;

	std::unordered_map<QTreeWidgetItem*, std::reference_wrapper<TriggerCategory>> categorys;

	std::unordered_map<QTreeWidgetItem*, std::reference_wrapper<Trigger>> files;

	std::unordered_map<QTreeWidgetItem*, int> folder_ids;

	void item_clicked(QTreeWidgetItem* item);

	void item_changed(QTreeWidgetItem* item);

	void custom_menu_popup(const QPoint& pos);
	
	void show_gui_trigger(QTreeWidget* edit, Trigger& trigger);

	void create_category(QTreeWidgetItem* parent);

	void create_trigger(QTreeWidgetItem* parent,bool is_comment);

	void remove_trigger(QTreeWidgetItem* item);

	void remove_category(QTreeWidgetItem* item);

	void delete_item(QTreeWidgetItem* item);

	void rename_item(QTreeWidgetItem* item, QString& name);
	
	std::string get_parameters_names(const std::vector<std::string>& string_parameters, const std::vector<TriggerParameter>& parameters) const ;

signals:
	void save_changes();
};