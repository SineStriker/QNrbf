#include "MainWindow.h"

#include <QApplication>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>

#include "Objects/OneOrMoreNullObject.h"
#include "Objects/PrimitiveListObject.h"
#include "Objects/PrimitiveObject.h"
#include "Objects/StringListObject.h"
#include "Objects/StringObject.h"

#include "Utils/NrbfReader.h"

#include "QNrbfStream.h"

Q_DECLARE_METATYPE(QNrbf::ObjectRef);

static const int PointerRole = Qt::UserRole + 1;
static const int VisitedRole = PointerRole + 1;

static const int MaxDisplayCount = 1000;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    lineEdit = new QLineEdit();

    browseButton = new QPushButton("Browse");
    loadButton = new QPushButton("Load");

    buttonLayout = new QHBoxLayout();
    buttonLayout->setMargin(0);
    buttonLayout->setSpacing(0);
    buttonLayout->addWidget(browseButton);
    buttonLayout->addWidget(loadButton);

    treeWidget = new QTreeWidget();
    treeWidget->setColumnCount(3);

    auto header = treeWidget->header();
    header->setHighlightSections(false);
    header->setSectionResizeMode(QHeaderView::Interactive);

    mainLayout = new QVBoxLayout();
    mainLayout->setMargin(20);
    mainLayout->setSpacing(10);
    mainLayout->addWidget(lineEdit);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(treeWidget);

    mainWidget = new QWidget();
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    connect(browseButton, &QPushButton::clicked, this, &MainWindow::_q_browseButtonClicked);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::_q_loadButtonClicked);
    connect(treeWidget, &QTreeWidget::itemExpanded, this, &MainWindow::_q_treeItemExpanded);

    resize(1280, 720);
    treeWidget->setColumnWidth(0, 500);
    treeWidget->setColumnWidth(1, 600);

    {
        const auto &args = qApp->arguments();
        if (args.size() > 1) {
            lineEdit->setText(args.at(1));
            _q_loadButtonClicked();
        }
    }
}

MainWindow::~MainWindow() {
}

void MainWindow::preExpandItem(QTreeWidgetItem *item) {
    auto var = item->data(0, PointerRole);
    if (var.userType() != qMetaTypeId<QNrbf::ObjectRef>()) {
        return;
    }

    auto obj = var.value<QNrbf::ObjectRef>();
    if (obj.isNull()) {
        item->setText(1, "Null");
    } else {
        switch (obj->type()) {
            case QNrbf::AbstractObject::Mapping: {
                if (item->childCount() == 0) {
                    auto mapping = obj.dynamicCast<QNrbf::MappingObject>();
                    int cnt = 0;
                    for (auto it = mapping->members.begin(); it != mapping->members.end(); ++it) {
                        auto childItem = new QTreeWidgetItem();
                        childItem->setText(0, it.key());
                        childItem->setData(
                            0, PointerRole,
                            QVariant::fromValue(registry.resolveReference(it.value())));
                        item->addChild(childItem);
                        if (cnt++ == MaxDisplayCount) {
                            auto lastItem = new QTreeWidgetItem();
                            lastItem->setText(0, "...");
                            lastItem->setText(
                                1,
                                QString("Length %1").arg(QString::number(mapping->members.size())));
                            item->addChild(lastItem);
                            break;
                        }
                    }
                }
                break;
            }
            case QNrbf::AbstractObject::ClassMember: {
                break;
            }
            case QNrbf::AbstractObject::Primitive: {
                auto val = obj.dynamicCast<QNrbf::PrimitiveObject>();
                item->setText(1, val->value.asString());
                item->setText(2, QNrbf::Parser::strPrimitiveTypeEnum(val->value.type()));
                break;
            }
            case QNrbf::AbstractObject::String: {
                auto val = obj.dynamicCast<QNrbf::StringObject>();
                item->setText(1, val->value);
                break;
            }
            case QNrbf::AbstractObject::Null: {
                auto val = obj.dynamicCast<QNrbf::OneOrMoreNullObject>();
                if (val->nullCount == 1) {
                    item->setText(1, "Null");
                } else {
                    for (int i = 0; i < val->nullCount; ++i) {
                        auto childItem = new QTreeWidgetItem();
                        childItem->setText(0, QString::number(i));
                        childItem->setText(1, "Null");
                        item->addChild(childItem);
                        if (i == MaxDisplayCount) {
                            auto lastItem = new QTreeWidgetItem();
                            lastItem->setText(0, "...");
                            lastItem->setText(
                                1, QString("Length %1").arg(QString::number(val->nullCount)));
                            item->addChild(lastItem);
                            break;
                        }
                    }
                }
                break;
            }
            case QNrbf::AbstractObject::PrimitiveList: {
                auto val = obj.dynamicCast<QNrbf::PrimitiveListObject>();
                QStringList strs = val->values.asStringList();
                for (int i = 0; i < strs.size(); ++i) {
                    auto childItem = new QTreeWidgetItem();
                    childItem->setText(0, QString::number(i));
                    childItem->setText(1, strs.at(i));
                    item->addChild(childItem);
                    if (i == MaxDisplayCount) {
                        auto lastItem = new QTreeWidgetItem();
                        lastItem->setText(0, "...");
                        lastItem->setText(1,
                                          QString("Length %1").arg(QString::number(strs.size())));
                        item->addChild(lastItem);
                        break;
                    }
                }
                item->setText(2, QNrbf::Parser::strPrimitiveTypeEnum(val->values.type()));
                break;
            }
            case QNrbf::AbstractObject::StringList: {
                auto val = obj.dynamicCast<QNrbf::StringListObject>();
                QStringList strs = val->values;
                for (int i = 0; i < strs.size(); ++i) {
                    auto childItem = new QTreeWidgetItem();
                    childItem->setText(0, QString::number(i));
                    childItem->setText(1, strs.at(i));
                    item->addChild(childItem);
                    if (i == MaxDisplayCount) {
                        auto lastItem = new QTreeWidgetItem();
                        lastItem->setText(0, "...");
                        lastItem->setText(1,
                                          QString("Length %1").arg(QString::number(strs.size())));
                        item->addChild(lastItem);
                        break;
                    }
                }
                break;
            }
            case QNrbf::AbstractObject::ObjectList: {
                auto list = obj.dynamicCast<QNrbf::ObjectListObject>();
                for (int i = 0; i < list->values.size(); ++i) {
                    auto childItem = new QTreeWidgetItem();
                    childItem->setText(0, QString::number(i));
                    childItem->setData(
                        0, PointerRole,
                        QVariant::fromValue(registry.resolveReference(list->values.at(i))));
                    item->addChild(childItem);
                    if (i == MaxDisplayCount) {
                        auto lastItem = new QTreeWidgetItem();
                        lastItem->setText(0, "...");
                        lastItem->setText(
                            1, QString("Length %1").arg(QString::number(list->values.size())));
                        item->addChild(lastItem);
                        break;
                    }
                }
                break;
            }
            default:
                break;
        }
    }
}

void MainWindow::_q_browseButtonClicked() {
    QString path = QFileDialog::getOpenFileName(this, "Browse", "", "XStudio files(*.svip)");
    if (path.isEmpty()) {
        return;
    }
    lineEdit->setText(path);
}

void MainWindow::_q_loadButtonClicked() {
    treeWidget->clear();

    QFile file(lineEdit->text());
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Failed to load nrbf file.");
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QNrbfStream in(&data, QIODevice::ReadOnly);

    QString name;
    QString ver;

    // Read name and version
    in >> name;
    in >> ver;
    if (in.status() != QDataStream::Ok) {
        QMessageBox::critical(this, "Error", "Failed to read version information.");
        return;
    }

    QNrbf::NrbfReader reader(&in);
    auto reg = reader.read();
    if (reader.status() != QNrbf::NrbfReader::ReachEnd) {
        QMessageBox::critical(this, "Error", "Failed to parse nrbf content.");
        return;
    }

    auto root = reg.findReference(reg.header->rootId);
    if (root.isNull() || root->type() != QNrbf::AbstractObject::Mapping) {
        QMessageBox::critical(this, "Error", "Root object is not a mapping.");
        return;
    }

    auto versionItem = new QTreeWidgetItem();
    versionItem->setText(0, "version");
    versionItem->setText(1, name + ver);

    auto rootItem = new QTreeWidgetItem();
    rootItem->setText(0, "root");
    rootItem->setData(0, PointerRole, QVariant::fromValue(root));
    rootItem->setData(0, VisitedRole, false);

    treeWidget->addTopLevelItem(versionItem);
    treeWidget->addTopLevelItem(rootItem);

    registry = std::move(reg);

    preExpandItem(rootItem);
}

void MainWindow::_q_treeItemExpanded(QTreeWidgetItem *item) {
    for (int i = 0; i < item->childCount(); ++i) {
        auto childItem = item->child(i);
        if (childItem->data(0, VisitedRole).toBool()) {
            return;
        }
        childItem->setData(0, VisitedRole, true);
        preExpandItem(childItem);
    }
}
