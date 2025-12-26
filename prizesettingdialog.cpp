#include "prizesettingdialog.h"
#include <QFileDialog>
#include <QSpinBox>
#include <QCheckBox>
#include <QHeaderView>
#include <QEvent>
#include <QWheelEvent>

PrizeSettingDialog::PrizeSettingDialog(const QVector<PrizeConfig> &prizes, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("奖项管理");
    resize(1200, 750);
    this->setStyleSheet("font-size: 18px;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    table = new QTableWidget(this);

    // 表格基础设置
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::AnyKeyPressed);

    // 8列：抽奖顺序, 等级, 奖品, 每轮人数, 总轮数, 奖品总数, 图片路径, 全员参与
    QStringList headers = {"抽奖顺序", "等级名称", "奖品名称", "每轮人数", "总轮数", "奖品总数", "图片路径 (双击)", "全员参与"};
    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setDefaultSectionSize(60);

    // 填充数据
    table->setRowCount(prizes.size());
    for(int i = 0; i < prizes.size(); ++i) {
        setRowData(i, prizes[i]);
    }

    // --- 按钮与统计布局 ---
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnAdd = new QPushButton("➕ 添加");
    QPushButton *btnDel = new QPushButton("❌ 删除");
    QPushButton *btnUp = new QPushButton("▲ 上移");
    QPushButton *btnDown = new QPushButton("▼ 下移");
    QPushButton *btnSave = new QPushButton("✅ 保存设置");

    // 新增：底部累计总人数标签
    totalSumLabel = new QLabel("累计总人数: 0 人");
  //hai  totalSumLabel->setStyleSheet("font-weight: bold; color: #2c3e50; font-size: 20px; margin-left: 15px;");

    QString btnStyle = "QPushButton { height: 50px; min-width: 110px; font-weight: bold; }";
    btnAdd->setStyleSheet(btnStyle);
    btnDel->setStyleSheet(btnStyle);
    btnUp->setStyleSheet(btnStyle + "background-color: #3498db; color: white;");
    btnDown->setStyleSheet(btnStyle + "background-color: #3498db; color: white;");
    btnSave->setStyleSheet(btnStyle + "background-color: #27ae60; color: white;");

    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnDel);
    btnLayout->addSpacing(15);
    btnLayout->addWidget(btnUp);
    btnLayout->addWidget(btnDown);
    btnLayout->addWidget(totalSumLabel); // 放置统计标签
    btnLayout->addStretch();
    btnLayout->addWidget(btnSave);

    mainLayout->addWidget(table);
    mainLayout->addLayout(btnLayout);

    // --- 信号槽连接 ---
    connect(btnAdd, &QPushButton::clicked, [this](){
        int row = table->rowCount();
        table->insertRow(row);
        PrizeConfig p;
        p.id = row + 1; p.displayName = "新奖项"; p.priceName = "奖品";
        p.winnersPerRound = 1; p.totalRounds = 1; p.image = "images/default.png"; p.allowUsedPeople = false;
        setRowData(row, p);
        table->selectRow(row);
        updateGrandTotal();
    });

    connect(btnDel, &QPushButton::clicked, [this](){
        int cur = table->currentRow();
        if(cur >= 0) {
            table->removeRow(cur);
            updateGrandTotal();
        }
    });

    connect(btnUp, &QPushButton::clicked, this, &PrizeSettingDialog::moveRowUp);
    connect(btnDown, &QPushButton::clicked, this, &PrizeSettingDialog::moveRowDown);
    connect(btnSave, &QPushButton::clicked, this, &QDialog::accept);

    // 双击选图 (第6列)
    connect(table, &QTableWidget::cellDoubleClicked, [this](int row, int col){
        if(col == 6) {
            QString path = QFileDialog::getOpenFileName(this, "选择图片", "./images", "Images (*.png *.jpg)");
            if(!path.isEmpty()) table->item(row, col)->setText("images/" + QFileInfo(path).fileName());
        }
    });

    updateGrandTotal(); // 初始化统计
}

void PrizeSettingDialog::setRowData(int row, const PrizeConfig &p) {
    // 0. 顺序 (只读)
    auto itemID = new QTableWidgetItem(QString::number(p.id));
    itemID->setFlags(itemID->flags() & ~Qt::ItemIsEditable);
    itemID->setTextAlignment(Qt::AlignCenter);
    table->setItem(row, 0, itemID);

    // 1. 等级 & 2. 奖品
    table->setItem(row, 1, new QTableWidgetItem(p.displayName));
    table->setItem(row, 2, new QTableWidgetItem(p.priceName));

    // --- 3 & 4. 纯手动数字控件 ---
    auto createSpinBox = [this, row](int value) {
        QSpinBox *sb = new QSpinBox();
        sb->setRange(1, 9999);
        sb->setValue(value);
        sb->setAlignment(Qt::AlignCenter);

        // 【关键】：隐藏上下按钮，只能手动填
        sb->setButtonSymbols(QAbstractSpinBox::NoButtons);
        // 【关键】：安装事件过滤器，用于禁用鼠标滚轮
        sb->installEventFilter(this);

        connect(sb, QOverload<int>::of(&QSpinBox::valueChanged), [this, row](){
            updateTotalCount(row);
        });
        return sb;
    };

    table->setCellWidget(row, 3, createSpinBox(p.winnersPerRound));
    table->setCellWidget(row, 4, createSpinBox(p.totalRounds));

    // 5. 奖品总数 (计算列，底色区分)
    int total = p.winnersPerRound * p.totalRounds;
    auto itemTotal = new QTableWidgetItem(QString::number(total));
    itemTotal->setFlags(itemTotal->flags() & ~Qt::ItemIsEditable);
   // itemTotal->setBackground(QBrush(QColor(245, 245, 245)));
    itemTotal->setTextAlignment(Qt::AlignCenter);
    table->setItem(row, 5, itemTotal);

    // 6. 图片路径
    table->setItem(row, 6, new QTableWidgetItem(p.image));

    // 7. 全员参与
    QCheckBox *check = new QCheckBox();
    check->setChecked(p.allowUsedPeople);
    check->setStyleSheet("QCheckBox::indicator { width: 30px; height: 30px; }");
    QWidget *w = new QWidget();
    QHBoxLayout *l = new QHBoxLayout(w);
    l->addWidget(check); l->setAlignment(Qt::AlignCenter); l->setContentsMargins(0,0,0,0);
    table->setCellWidget(row, 7, w);
}

// 禁用滚轮的核心实现
bool PrizeSettingDialog::eventFilter(QObject *watched, QEvent *event) {
    if (qobject_cast<QSpinBox*>(watched) && event->type() == QEvent::Wheel) {
        return true; // 拦截滚轮事件
    }
    return QDialog::eventFilter(watched, event);
}

void PrizeSettingDialog::updateTotalCount(int row) {
    QSpinBox *sbPer = qobject_cast<QSpinBox*>(table->cellWidget(row, 3));
    QSpinBox *sbRound = qobject_cast<QSpinBox*>(table->cellWidget(row, 4));
    if(sbPer && sbRound) {
        int total = sbPer->value() * sbRound->value();
        if(table->item(row, 5)) table->item(row, 5)->setText(QString::number(total));
    }
    updateGrandTotal();
}

void PrizeSettingDialog::updateGrandTotal() {
    int total = 0;
    for(int i = 0; i < table->rowCount(); ++i) {
        if(table->item(i, 5)) total += table->item(i, 5)->text().toInt();
    }
    totalSumLabel->setText(QString("所有奖项累计总人数: %1 人").arg(total));
}

void PrizeSettingDialog::swapRows(int rowA, int rowB) {
    // 1. 交换文本列 (1, 2, 5, 6)
    QList<int> textCols = {1, 2, 5, 6};
    for (int col : textCols) {
        QTableWidgetItem *itemA = table->takeItem(rowA, col);
        QTableWidgetItem *itemB = table->takeItem(rowB, col);
        table->setItem(rowA, col, itemB);
        table->setItem(rowB, col, itemA);
    }
    // 2. 交换 SpinBox 里的数值 (3, 4)
    QSpinBox *sb3A = qobject_cast<QSpinBox*>(table->cellWidget(rowA, 3));
    QSpinBox *sb3B = qobject_cast<QSpinBox*>(table->cellWidget(rowB, 3));
    int v3 = sb3A->value(); sb3A->setValue(sb3B->value()); sb3B->setValue(v3);

    QSpinBox *sb4A = qobject_cast<QSpinBox*>(table->cellWidget(rowA, 4));
    QSpinBox *sb4B = qobject_cast<QSpinBox*>(table->cellWidget(rowB, 4));
    int v4 = sb4A->value(); sb4A->setValue(sb4B->value()); sb4B->setValue(v4);

    // 3. 交换 CheckBox (7)
    QCheckBox *cbA = table->cellWidget(rowA, 7)->findChild<QCheckBox*>();
    QCheckBox *cbB = table->cellWidget(rowB, 7)->findChild<QCheckBox*>();
    bool c = cbA->isChecked(); cbA->setChecked(cbB->isChecked()); cbB->setChecked(c);
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
        c.id = i + 1;
        c.displayName = table->item(i, 1)->text();
        c.priceName = table->item(i, 2)->text();
        c.winnersPerRound = qobject_cast<QSpinBox*>(table->cellWidget(i, 3))->value();
        c.totalRounds = qobject_cast<QSpinBox*>(table->cellWidget(i, 4))->value();
        c.image = table->item(i, 6)->text();
        QCheckBox *cb = table->cellWidget(i, 7)->findChild<QCheckBox*>();
        c.allowUsedPeople = cb->isChecked();
        configs.append(c);
    }
    return configs;
}
