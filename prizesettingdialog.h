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
#include <QLabel>      // 必须包含这个
#include <QSpinBox>    // 必须包含这个
#include "widget.h"

class PrizeSettingDialog : public QDialog {
    Q_OBJECT
public:
    explicit PrizeSettingDialog(const QVector<PrizeConfig> &prizes, QWidget *parent = nullptr);
    QVector<PrizeConfig> getNewConfigs() const;

protected:
    // 处理拦截滚轮事件，防止误触
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void moveRowUp();
    void moveRowDown();

private:
    QTableWidget *table;
    QLabel *totalSumLabel;

    void setRowData(int row, const PrizeConfig &p);
    void swapRows(int rowA, int rowB);
    void updateTotalCount(int row);
    void updateGrandTotal();
};

#endif
