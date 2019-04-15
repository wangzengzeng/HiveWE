#include "stdafx.h"

ImportManager::ImportManager(QWidget* parent) : QMainWindow(parent) {
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);

	ui.treeWidget->setFocus();
	ui.treeWidget->installEventFilter(this);
	ui.treeWidget->header()->resizeSection(0, 220);
	connect(ui.treeWidget, &QTreeWidget::customContextMenuRequested, this, &ImportManager::custom_menu_popup);
	connect(ui.treeWidget, &QTreeWidget::itemDoubleClicked, this, &ImportManager::edit_item);

	connect(ui.createDirectory, &QPushButton::clicked, [&]() { create_directory(ui.treeWidget->invisibleRootItem()); });
	connect(ui.importFiles, &QPushButton::clicked, [&]() { import_files(ui.treeWidget->invisibleRootItem()); });
	connect(ui.exportAll, &QPushButton::clicked, [&]() { export_files(ui.treeWidget->invisibleRootItem()); });

	QFileIconProvider icons;
	folder_icon = icons.icon(QFileIconProvider::Folder);
	file_icon = icons.icon(QFileIconProvider::File);

	ui.createDirectory->setIcon(folder_icon);
	ui.importFiles->setIcon(file_icon);
	ui.exportAll->setIcon(file_icon);
	  
	ui.treeWidget->invisibleRootItem()->setText(0, u8"�������ļ�"); // So export all works correctly
	ui.treeWidget->invisibleRootItem()->setText(1, u8"�ļ���"); // So export all works correctly

	load_files(map->imports.imports);

	show();
}

void ImportManager::custom_menu_popup(const QPoint& pos) {
	QTreeWidgetItem* parent = ui.treeWidget->itemAt(pos);

	if (parent == nullptr) {
		parent = ui.treeWidget->invisibleRootItem();
	}

	QMenu* menu = new QMenu(this);
	QAction* import_files_action = new QAction(u8"�����ļ�");
	QAction* remove_files_action = new QAction(u8"ɾ��");
	QAction* export_file_action = new QAction(u8"�����ļ�");
	QAction* export_files_action = new QAction(u8"�����ļ�");
	QAction* add_directory_action = new QAction(u8"����ļ���");
	QAction* rename_action = new QAction(u8"ɾ��");

	connect(import_files_action, &QAction::triggered, [&]() { import_files(parent); });
	connect(add_directory_action, &QAction::triggered, [&]() { create_directory(parent); });
	connect(remove_files_action, &QAction::triggered, [&]() { remove_item(parent);  });
	connect(rename_action, &QAction::triggered, [&]() { rename_directory(parent);  });
	connect(export_files_action, &QAction::triggered, [&]() { export_files(parent); });
	connect(export_file_action, &QAction::triggered, [&]() { export_files(parent); });

	if (parent->text(1).isEmpty()) {
		menu->addAction(add_directory_action);
		menu->addAction(import_files_action);
		menu->addAction(export_files_action);
	} else if (parent->text(1) == u8"�ļ���") {
		menu->addAction(rename_action);
		menu->addAction(remove_files_action);
		menu->addAction(import_files_action);
		menu->addAction(export_file_action);
		menu->addAction(remove_files_action);
	} else {
		menu->addAction(export_file_action);
		menu->addAction(remove_files_action);
	}

	menu->exec(QCursor::pos());
}

void ImportManager::import_files(QTreeWidgetItem* item) {
	QStringList files = QFileDialog::getOpenFileNames(this, "Select one or more files to import",
		".",
		"*.blp *.mdx *.mdl *.tga *.wav *.mp3 *.txt");

	for (auto&& file : files) {
		const fs::path file_name = fs::path(file.toStdString()).filename();
		const fs::path full_path = "war3mapImported\\" / file_name;

		map->imports.import_file(fs::path(file.toLocal8Bit().toStdString()), full_path);

		QTreeWidgetItem* child = new QTreeWidgetItem(item);
		child->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
		child->setText(0, QString::fromStdString(file_name.string()));
		child->setText(1, get_file_type(file.toStdString()));
		child->setText(2, QString::number(map->imports.file_size(full_path)));
		child->setText(3, u8"��");
		child->setText(4, QString::fromStdString(full_path.string()));
		child->setIcon(0, file_icon);

	
	}
	item->setExpanded(true);
	items_changed();
}

void ImportManager::remove_item(QTreeWidgetItem* item) {

	QMessageBox buttonBox(QMessageBox::Warning, "HiveWE", u8"�Ƿ�Ҫɾ�����ļ� " + item->text(0));
	buttonBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	buttonBox.setButtonText(QMessageBox::Ok, u8"ȷ��");
	buttonBox.setButtonText(QMessageBox::Cancel, u8"ȡ��");
	
	const int choice = buttonBox.exec();


	if (choice == QMessageBox::Ok) {
		hierarchy.map.file_remove(item->text(0).toLocal8Bit().toStdString());
		delete item;
		items_changed();
	}
}

void ImportManager::edit_item(QTreeWidgetItem* item) {
	if (item->text(1) == u8"�ļ���") {
		return;
	}

	auto editor = new ImportManagerEdit(this);
	editor->ui.fileName->setText(item->text(0));
	editor->ui.fileType->setText(item->text(1));
	editor->ui.fileSize->setText(item->text(2));
	editor->ui.fileFullPath->setText(item->text(4));

	editor->ui.fileFullPath->setEnabled(item->text(3) == u8"��");
	editor->ui.customPath->setChecked(item->text(3) == u8"��");

	connect(editor, &ImportManagerEdit::accepted, [&](bool custom, QString full_path) {
		SFileRenameFile(hierarchy.map.handle, item->text(4).toStdString().c_str(), full_path.toStdString().c_str());
		item->setText(4, full_path);
		item->setText(3, custom ? u8"��" : u8"��");
		editor->close();
		items_changed();
	});

	editor->exec();
}

QString ImportManager::generate_full_path(QString file_name) {
	if (file_name.startsWith("BTN")) {
		return "ReplaceableTextures\\CommandButtons\\" + file_name;
	} else if (file_name.startsWith("PAS")) {
		return "ReplaceableTextures\\PassiveButtons\\" + file_name;
	} else if (file_name.startsWith("DISBTN") || file_name.startsWith("DISPAS")) {
		return "ReplaceableTextures\\CommandButtonsDisabled\\" + file_name;
	}

	return file_name;
}

QString ImportManager::get_file_type(const fs::path& path) {
	const std::string extension = path.extension().string();

	if (extension == ".blp" || extension == ".tga" ) {
		return u8"ͼ��";
	} else if (extension == ".mdx") {
		return u8"ģ��";
	} else if (extension == ".txt") {
		return u8"�ı�";
	} else if (extension == ".mp3" || extension == ".wav") {
		return u8"����";
	}

	return "Other";
}

void ImportManager::create_directory(QTreeWidgetItem* parent) {
	QTreeWidgetItem* item = new QTreeWidgetItem(parent);
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
	item->setText(0, u8"�½��ļ���");
	item->setText(1, u8"�ļ���");
	item->setIcon(0, folder_icon);
	items_changed();
}

void ImportManager::rename_directory(QTreeWidgetItem* item) {
	QDialog* diag = new QDialog(this);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	connect(buttons, &QDialogButtonBox::rejected, diag, &QDialog::reject);

	QLineEdit* input = new QLineEdit;
	input->setPlaceholderText("Directory name...");
	input->setMaxLength(32);
	input->setText(item->text(0));

	connect(buttons, &QDialogButtonBox::accepted, [&]() {
		item->setText(0, input->text());
		diag->close();
	});
	connect(input, &QLineEdit::textChanged, [&](const QString &text) {
		buttons->button(QDialogButtonBox::Ok)->setDisabled(text.isEmpty());
	});

	mainLayout->addWidget(input);
	mainLayout->addWidget(buttons);

	diag->setLayout(mainLayout);
	diag->setWindowTitle(u8"ɾ���ļ���");
	diag->exec();
	items_changed();
}

void ImportManager::load_files(const std::vector<ImportItem>& imports) const {
	const std::function<void(const std::vector<ImportItem>&, QTreeWidgetItem*)> create_tree = [&](const std::vector<ImportItem>& items, QTreeWidgetItem* parent) {
		for (auto&& i : items) {
			QTreeWidgetItem* item = new QTreeWidgetItem(parent);
			item->setText(0, QString::fromStdString(i.name.string()));
			if (i.directory) {
				item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
				item->setText(1, u8"�ļ���");
				item->setIcon(0, folder_icon);
				create_tree(i.children, item);
			} else {
				item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
				item->setText(1, get_file_type(i.name));
				item->setText(2, QString::number(i.size));
				item->setText(3, i.custom ? u8"��" : u8"��");
				item->setText(4, QString::fromStdString(i.full_path.string()));
				item->setIcon(0, file_icon);
				//map->imports.import_file((i.full_path)
			}
		}
	};

	create_tree(imports, ui.treeWidget->invisibleRootItem());
}

void ImportManager::export_files(QTreeWidgetItem* item) {
	const fs::path path = QFileDialog::getExistingDirectory(this, u8"ѡ���ļ���", ".").toStdString();

	const std::function<void(fs::path, QTreeWidgetItem*)> export_files = [&](fs::path target, QTreeWidgetItem* parent) {
		if (parent->text(1) == u8"�ļ���") {
			target /= parent->text(0).toLocal8Bit().toStdString();
			fs::create_directory(target);
			const int count = parent->childCount();
			for (int i = 0; i < count; i++) {
				QTreeWidgetItem* child = parent->child(i);
				export_files(target, child);
			}
		} else {
			auto a = parent->text(4).toStdString();
			map->imports.export_file(target, parent->text(4).toStdString());
		}
	};

	export_files(path, item);
}

bool ImportManager::eventFilter(QObject*, QEvent* event) {
	if (event->type() == QKeyEvent::KeyPress) {
		QKeyEvent* k_event = dynamic_cast<QKeyEvent *>(event);
		if (k_event->key() == Qt::Key_Delete) {
			remove_item(ui.treeWidget->currentItem());
			return true;
		}
	}
	if (event->type() == QEvent::ChildRemoved) {
		items_changed();
	}
	return false;
}

void ImportManager::items_changed() {
	map->imports.imports.clear();

	const std::function<void(std::vector<ImportItem>&, QTreeWidgetItem*)> recreate_imports = [&](std::vector<ImportItem>& items, QTreeWidgetItem* parent) {
		const int count = parent->childCount();

		QTextCodec* gbk = QTextCodec::codecForName("gbk");

		for (int i = 0; i < count; i++) {
			QTreeWidgetItem* item = parent->child(i);
			ImportItem import_item;
			import_item.directory = item->text(1) == u8"�½��ļ���";
			
			import_item.name = item->text(0).toLocal8Bit().toStdString();
			import_item.size = item->text(2).toInt();
			import_item.custom = item->text(3) == u8"��";
			import_item.full_path = item->text(4).toLocal8Bit().toStdString();

			if (import_item.directory) {
				recreate_imports(import_item.children, item);
			}
			items.push_back(import_item);
		}
	};

	recreate_imports(map->imports.imports, ui.treeWidget->invisibleRootItem());
}