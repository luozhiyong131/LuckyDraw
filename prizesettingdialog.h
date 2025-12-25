#ifndef PRIZESETTINGDIALOG_H
#define PRIZESETTINGDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QCheckBox>
#include <QFileInfo>
#include "widget.h"

class PrizeSettingDialog : public QDialog {
    Q_OBJECT
public:
    explicit PrizeSettingDialog(const QVector<PrizeConfig> &prizes, QWidget *parent = nullptr);
    QVector<PrizeConfig> getNewConfigs() const;

private slots:
    void moveRowUp();   // 上移逻辑
    void moveRowDown(); // 下移逻辑

private:
    QTableWidget *table;
    void setRowData(int row, const PrizeConfig &p);
    void swapRows(int rowA, int rowB); // 交换两行数据的核心函数
};

#endif
