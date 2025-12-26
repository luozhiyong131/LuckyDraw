#include "prizesettingdialog.h"
#include <QFileDialog>

PrizeSettingDialog::PrizeSettingDialog(const QVector<PrizeConfig> &prizes, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("奖项管理 - 使用按钮调整顺序");
    resize(1100, 750);
    this->setStyleSheet("font-size: 18px;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    table = new QTableWidget(this);

    // 设置表格：禁止拖拽，开启整行选中
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::AnyKeyPressed);

    QStringList headers = {"抽奖顺序", "等级名称", "奖品名称", "每轮人数", "总轮数", "图片路径 (双击选择)", "全员参与"};
    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setDefaultSectionSize(60);

    // 填充数据qiu
    table->setRowCount(prizes.size());
    for(int i = 0; i < prizes.size(); ++i) {
        setRowData(i, prizes[i]);
    }

    // --- 按钮布局 ---
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnAdd = new QPushButton("➕ 添加");
    QPushButton *btnDel = new QPushButton("❌ 删除");
    QPushButton *btnUp = new QPushButton("▲ 上移");
    QPushButton *btnDown = new QPushButton("▼ 下移");
    QPushButton *btnSave = new QPushButton("✅ 保存设置");

    // 统一样式
    QString btnStyle = "QPushButton { height: 50px; min-width: 120px; font-weight: bold; }";
    btnAdd->setStyleSheet(btnStyle);
    btnDel->setStyleSheet(btnStyle);
    btnUp->setStyleSheet(btnStyle + "background-color: #3498db; color: white;");
    btnDown->setStyleSheet(btnStyle + "background-color: #3498db; color: white;");
    btnSave->setStyleSheet(btnStyle + "background-color: #27ae60; color: white;");

    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnDel);
    btnLayout->addSpacing(20); // 间距
    btnLayout->addWidget(btnUp);
    btnLayout->addWidget(btnDown);
    btnLayout->addStretch();
    btnLayout->addWidget(btnSave);

    mainLayout->addWidget(table);
    mainLayout->addLayout(btnLayout);

    // --- 信号槽连接 ---
    connect(btnAdd, &QPushButton::clicked, [this](){
        int row = table->rowCount();
        table->insertRow(row);
        PrizeConfig p; p.id = row + 1; p.displayName = "新奖项"; p.priceName = "奖品";
        p.winnersPerRound = 1; p.totalRounds = 1; p.image = "images/default.png"; p.allowUsedPeople = false;
        setRowData(row, p);
        table->selectRow(row);
    });

    connect(btnDel, &QPushButton::clicked, [this](){
        int cur = table->currentRow();
        if(cur >= 0) table->removeRow(cur);
    });

    connect(btnUp, &QPushButton::clicked, this, &PrizeSettingDialog::moveRowUp);
    connect(btnDown, &QPushButton::clicked, this, &PrizeSettingDialog::moveRowDown);
    connect(btnSave, &QPushButton::clicked, this, &QDialog::accept);

    // 双击选图
    connect(table, &QTableWidget::cellDoubleClicked, [this](int row, int col){
        if(col == 5) {
            QString path = QFileDialog::getOpenFileName(this, "选择图片", "./images", "Images (*.png *.jpg)");
            if(!path.isEmpty()) table->item(row, col)->setText("images/" + QFileInfo(path).fileName());
        }
    });
}

void PrizeSettingDialog::setRowData(int row, const PrizeConfig &p) {
    auto itemID = new QTableWidgetItem(QString::number(p.id));
    itemID->setFlags(itemID->flags() & ~Qt::ItemIsEditable); // ID禁止手动编辑
    itemID->setTextAlignment(Qt::AlignCenter);
    table->setItem(row, 0, itemID);

    table->setItem(row, 1, new QTableWidgetItem(p.displayName));
    table->setItem(row, 2, new QTableWidgetItem(p.priceName));
    table->setItem(row, 3, new QTableWidgetItem(QString::number(p.winnersPerRound)));
    table->setItem(row, 4, new QTableWidgetItem(QString::number(p.totalRounds)));
    table->setItem(row, 5, new QTableWidgetItem(p.image));

    QCheckBox *check = new QCheckBox();
    check->setStyleSheet("QCheckBox::indicator { width: 30px; height: 30px; }");
    check->setChecked(p.allowUsedPeople);
    QWidget *w = new QWidget();
    QHBoxLayout *l = new QHBoxLayout(w);
    l->addWidget(check); l->setAlignment(Qt::AlignCenter); l->setContentsMargins(0,0,0,0);
    table->setCellWidget(row, 6, w);
}

// 核心：交换两行内容
void PrizeSettingDialog::swapRows(int rowA, int rowB) {
    // 交换 1-5 列的文字内容
    for (int col = 1; col <= 5; ++col) {
        QTableWidgetItem *itemA = table->takeItem(rowA, col);
        QTableWidgetItem *itemB = table->takeItem(rowB, col);
        table->setItem(rowA, col, itemB);
        table->setItem(rowB, col, itemA);
    }

    // 交换 CheckBox 状态 (CellWidget不能take，只能重新设置)
    QCheckBox *cbA = table->cellWidget(rowA, 6)->findChild<QCheckBox*>();
    QCheckBox *cbB = table->cellWidget(rowB, 6)->findChild<QCheckBox*>();
    bool checkedA = cbA->isChecked();
    bool checkedB = cbB->isChecked();
    cbA->setChecked(checkedB);
    cbB->setChecked(checkedA);
}

void PrizeSettingDialog::moveRowUp() {
    int cur = table->currentRow();
    if (cur <= 0) return;
    swapRows(cur, cur - 1);
    table->selectRow(cur - 1);
}

void PrizeSettingDialog::moveRowDown() {
    int cur = table->currentRow();
    if (cur < 0 || cur >= table->rowCount() - 1) return;
    swapRows(cur, cur + 1);
    table->selectRow(cur + 1);
}

QVector<PrizeConfig> PrizeSettingDialog::getNewConfigs() const {
    QVector<PrizeConfig> configs;
    for(int i = 0; i < table->rowCount(); ++i) {
        PrizeConfig c;
        c.id = i + 1; // 强制按视觉顺序分配ID
        c.displayName = table->item(i, 1)->text();
        c.priceName = table->item(i, 2)->text();
        c.winnersPerRound = table->item(i, 3)->text().toInt();
        c.totalRounds = table->item(i, 4)->text().toInt();
        c.image = table->item(i, 5)->text();
        QCheckBox *cb = table->cellWidget(i, 6)->findChild<QCheckBox*>();
        c.allowUsedPeople = cb->isChecked();
        configs.append(c);
    }
    return configs;
}
